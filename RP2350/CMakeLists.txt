cmake_minimum_required(VERSION 3.13)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

project(handheld_rp2350 C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

find_program(PICOTOOL picotool)

if(PICOTOOL)
    message(STATUS "Using installed picotool: ${PICOTOOL}")
    set(PICO_PICOTOOL_PATH ${PICOTOOL})
endif()

set(SOURCES
    src/main.c
    src/stringf.c
    src/event_queue.c
    src/event_loop.c
    
    drivers/buttons.c
    drivers/uart.c
    drivers/gpio.c
)

add_executable(handheld_rp2350
    ${SOURCES}
)

target_include_directories(handheld_rp2350 PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src
    ${CMAKE_CURRENT_LIST_DIR}/drivers
    ${CMAKE_CURRENT_LIST_DIR}/lib
)

target_link_libraries(handheld_rp2350
    pico_stdlib
    hardware_gpio
    hardware_irq
)

pico_enable_stdio_uart(handheld_rp2350 0)
pico_enable_stdio_usb(handheld_rp2350 1)

pico_add_extra_outputs(handheld_rp2350)