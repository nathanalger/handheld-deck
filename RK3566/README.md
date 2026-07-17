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

## Appendix

Yes, the name SaDOS is a play on the name GLaDOS from the Portal Series.