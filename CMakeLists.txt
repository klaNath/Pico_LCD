cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(my_project)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

add_executable(test_pio)

pico_generate_pio_header(test_pio ${CMAKE_CURRENT_LIST_DIR}/test_pio.pio)

target_sources(test_pio PRIVATE test_pio.c)

target_link_libraries(test_pio PRIVATE
    pico_stdlib
    hardware_pio
    )

pico_enable_stdio_usb(test_pio 1)
pico_enable_stdio_uart(test_pio 0)

pico_add_extra_outputs(test_pio)
