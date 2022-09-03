
# Pico-w password protected wireless updates

Layout:

```sh
~/pico/pico-sdk/
~/pico/FreeRTOS-Kernel/
~/pico/pico-w-wireless-updates/
```

To download the FreeRTOS-Kernel do the following:

```sh
cd ~/pico/
git clone https://github.com/FreeRTOS/FreeRTOS-Kernel.git
cd FreeRTOS-Kernel/
git checkout smp
```

# Password file

Create the following file `credentials.cmake`

Contents should be as follows:

```sh
set(PICOWOTA_WIFI_SSID "YOUR_WIRELESS_SSID" CACHE INTERNAL "WiFi SSID for examples")
set(PICOWOTA_WIFI_PASS "YOUR_WIRELESS_PASSWORD" CACHE INTERNAL "WiFi password for examples")
set(REBOOT_USER "A_REBOOT_USERNAME" CACHE INTERNAL "Reboot post user")
set(REBOOT_PASS "A_REBOOT_PASSWORD" CACHE INTERNAL "Reboot post password")
```

# Building

```sh
cd ~/pico/
git clone https://github.com/ghubcoder/pico-w-wireless-updates.git
cd pico-w-wireless-updates/
git submodule update --init
mkdir build && cd build/
export PICO_SDK_PATH=../../pico-sdk
export FREERTOS_PATH=../../FreeRTOS-Kernel
cmake ..
make -j4
```

Blink code is a modified version of the code available in [this](https://github.com/yunkya2/pico-freertos-sample) repository.

Embedded webserver based on repo [here](https://github.com/ghubcoder/pico-w-wireless-updates)

Picowota source repo can be found [here](https://github.com/usedbytes/picowota)

Full details can be found [here](https://ghubcoder.github.io/posts/pico-w-password-protected-wireless-updates/)
