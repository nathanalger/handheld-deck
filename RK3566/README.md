# SaDOS (Simple Application-Driven Operating System)

Linux-based operating system designed to run on a RK3566-based device. Linux kernel is built using buildroot with the necessary drivers to drive display and UART communication. Linux provides the hardware abstraction and display drivers, while the UI application is responsible for rendering and updating the display contents.

Display uses the SDL3 API for better debugging on Linux and Windows.

## Table of Contents

- [Prototyping](#prototyping)
- [Getting Started](#getting-started)
- [Building](#building)
- [Project Status](#project-status)
- [Appendix](#appendix)

## Prototyping

- The compute module is a 2GB OrangePi with the RK3566 chip.
- The display is a 960x552 e-ink HAT using the UltraChip UC8179 display controller.

## Getting Started

In a terminal/PowerShell window, navigate to the [Scripts Folder](/RK3566/scripts/) and run your respective operating system's install script from that folder.

Windows: 
```bash
setup_win.ps1
``` 

Linux: 
```bash
chmod +x setup_linux.sh
./setup_linux.sh
```

## Building

In order to build and compile, run these commands from the [UI Folder](/RK3566/ui/):
```bash
  cmake -S . -B build                   # Generates Build Files
  cmake --build build --config Debug    # Compiles Application
```

Output will be in the [Bin Folder](RK3566/ui/bin/).

## Project Status

Current:
- SDL3 UI development environment functional

Planned:
- Application manager
- Buildroot image for RK3566
- Display driver integration
- Battery management
- UART communication drivers

## Project Structure

### Directory Layout

The `src` and `include` folders are structured the same. 

The structure is as shown below:
src/include - root folder and contains entry points
↳ display - contains display drivers and other related files
↳ root - contains app core files

### Noteable files

This will list notable files and their basic descriptions. Combines source and header files unless specified. Some files will not be mentioned.

display
↳ framebuffer: storage object for a single frame before being pushed to the display.
↳ renderer: provides standard functions for drawing to the framebuffer
↳ display_driver.hpp: abstract interface for display drivers. In particular:
   ↳ uc8179_display: unimplemented
   ↳ sdl3_display: prints to SDL3 display for debugging on windows/linux

root
↳ app: has the `run()` function for the app.

## Appendix

Yes, the name SaDOS is a play on the name GLaDOS from the Portal Series.