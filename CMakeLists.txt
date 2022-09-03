cmake_minimum_required(VERSION 3.12)

set(MAKE_FS_DATA_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/external/makefsdata)

if (NOT EXISTS ${MAKE_FS_DATA_SCRIPT})
        file(DOWNLOAD
                https://raw.githubusercontent.com/krzmaz/lwip/e15654409d14a238aec5ed4bd5516063938c9345/src/apps/http/makefsdata/makefsdata
                ${MAKE_FS_DATA_SCRIPT}
                )
endif()

execute_process(COMMAND
        perl ${MAKE_FS_DATA_SCRIPT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )

execute_process(COMMAND
        mv fsdata.c my_fsdata.c
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
        )

set(PICO_BOARD pico_w)
set(FREERTOS_KERNEL_PATH $ENV{FREERTOS_PATH})

set(PICO_PLATFORM rp2040)
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)

include(credentials.cmake)
project(main_project)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(
  blink
  reboot_post.c
  blink.c
)

target_include_directories(blink PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
)
add_subdirectory(picowota)
target_compile_definitions( blink PRIVATE
        PICOWOTA_WIFI_SSID=\"${PICOWOTA_WIFI_SSID}\"
        PICOWOTA_WIFI_PASS=\"${PICOWOTA_WIFI_PASS}\"
        REBOOT_USER=\"${REBOOT_USER}\"
        REBOOT_PASS=\"${REBOOT_PASS}\"
        )

pico_enable_stdio_usb(blink 1)

pico_add_extra_outputs(blink)

target_link_libraries(
  blink
  pico_lwip_http
  pico_stdlib
  hardware_adc
  pico_cyw43_arch_lwip_sys_freertos
  FreeRTOS-Kernel-Heap4
  picowota_reboot
)


#picowota_build_standalone(blink)
picowota_build_combined(blink)