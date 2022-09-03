/*
 * LED blink with FreeRTOS
 */
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"

#include "lwipopts.h"

#include "ssi.h"

#ifndef RUN_FREERTOS_ON_CORE
#define RUN_FREERTOS_ON_CORE 0
#endif


struct led_task_arg
{
    int gpio;
    int delay;
};

void pico_w_led_task()
{
    while (true)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(255));
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(255));
        printf("%s: core%u\n", pcTaskGetName(NULL), get_core_num());
    }
}

void pico_w_http_task(__unused void *params)
{

    if (cyw43_arch_init())
    {
        printf("WiFi init failed");
        return;
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("Enable sta mode...\n");
    cyw43_arch_enable_sta_mode();
    // this seems to be the best be can do using the predefined `cyw43_pm_value` macro:
    // cyw43_wifi_pm(&cyw43_state, CYW43_PERFORMANCE_PM);
    // however it doesn't use the `CYW43_NO_POWERSAVE_MODE` value, so we do this instead:
    cyw43_wifi_pm(&cyw43_state, cyw43_pm_value(CYW43_NO_POWERSAVE_MODE, 20, 1, 1, 1));

    printf("Connecting to WiFi...\n");
    while (cyw43_arch_wifi_connect_timeout_ms(PICOWOTA_WIFI_SSID, PICOWOTA_WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 30000))
    {
        printf("failed to connect.\n");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    printf("Connected.\n");
    printf("\nReady, running httpd at %s\n", ip4addr_ntoa(netif_ip4_addr(netif_list)));
    xTaskCreate(pico_w_led_task, "LED_Task 2", 256, NULL, 1, NULL);
    httpd_init();
    ssi_init();
    printf("Http server initialized.\n");

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void led_task(void *p)
{
    struct led_task_arg *a = (struct led_task_arg *)p;

    gpio_init(a->gpio);
    gpio_set_dir(a->gpio, GPIO_OUT);
    while (true)
    {
        gpio_put(a->gpio, 1);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        gpio_put(a->gpio, 0);
        vTaskDelay(pdMS_TO_TICKS(a->delay));
        printf("%s: core%u\n", pcTaskGetName(NULL), get_core_num());
    }
}

int main()
{
    stdio_init_all();

    struct led_task_arg arg1 = {25, 100};
    xTaskCreate(led_task, "LED_Task 1", 256, &arg1, 1, NULL);

    xTaskCreate(pico_w_http_task, "HTTP_Task 1", 6000, NULL, 1, NULL);
    // Create a 3rd task
    // Force it to run on core1
    TaskHandle_t task3_handle;
    UBaseType_t uxCoreAffinityMask;
    struct led_task_arg arg3 = {8, 300};
    xTaskCreate(led_task, "LED_Task 3", 256, &arg3, 1, &(task3_handle));
    // To force to run on core0:
    // uxCoreAffinityMask = ( ( 1 << 0 ));

    uxCoreAffinityMask = ((1 << 1));
    vTaskCoreAffinitySet(task3_handle, uxCoreAffinityMask);

    vTaskStartScheduler();

    while (true)
        ;
}
