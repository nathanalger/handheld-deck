# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a handheld computing system project combining:
- RP2350 Microcontroller (for peripheral control and communication)
- RK3566 SoC (for system processing, networking, and display output)

The project focuses on creating a custom operating system to run specific applications on this hardware combination.

## Key Components

### RP2350 Microcontroller
- Uses Raspberry Pi Pico SDK for development
- Implements a custom event-driven architecture
- Handles GPIO and UART communication
- Source files are in the RP2350 directory

### Event System Architecture
The system uses a custom event-driven architecture:
- `event_queue.h` and `event_queue.c` - Circular buffer queue for events
- `event_loop.h` and `event_loop.c` - Event routing and dispatch system
- `gpio.h` and `gpio.c` - GPIO handling
- `uart.h` and `uart.c` - UART communication

## Development Setup

### Prerequisites
- PICO_SDK_PATH environment variable must be set to the Raspberry Pi Pico SDK path
- arm-none-eabi-gcc toolchain must be installed and in PATH

### Build Process
```bash
./tools/build.sh
```

### Flashing
```bash
./tools/flash.sh
```

### Build and Flash in One Command
```bash
./tools/build.sh && ./tools/flash.sh
```

## Key Files and Directories

### Main Source Files
- `RP2350/src/main.c` - Entry point and main event loop
- `RP2350/src/event_queue.c` - Event queue implementation
- `RP2350/src/event_loop.c` - Event routing and dispatch
- `RP2350/drivers/gpio.c` - GPIO driver implementation
- `RP2350/drivers/uart.c` - UART driver implementation

### Build System
- `tools/build.sh` - Build script for RP2350 firmware
- `tools/flash.sh` - Flash script for RP2350
- `RP2350/CMakeLists.txt` - CMake build configuration

## Development Workflow

1. Make changes to source files in RP2350 directory
2. Run `./tools/build.sh` to compile the firmware
3. Run `./tools/flash.sh` to flash to the RP2350 device
4. Connect the RP2350 via USB or UART for debugging output

## Testing

The project uses a simple event-driven testing approach where:
- GPIO events are handled via the event system
- UART communication is implemented for debugging and communication
- Events are queued and processed in the main loop

## Debugging

Debugging is done through:
- USB serial output (enabled with `pico_enable_stdio_usb`)
- UART communication (via `uart.c` driver)
- GPIO events (for button presses and other inputs)