# Getting Started

This assumes you are using a debian build of linux. If on Windows, use WSL.

## WSL Users

For IntelliSense to recognize the Pico SDK, you must ensure VSCode is working out of the WSL backend. 

First, ensure you have the WSL and CMake Tools extention installed.
Then, press `Ctrl + Shift + P` -> `WSL: Open folder in WSL`.

If it asks for your CMake, make sure to select the CMake file in RP2350, and scan for toolkits recursively.

> - Keep project inside WSL filesystem (not /mnt/c or /mnt/d) for faster builds
> - Example: ~/handheld-deck instead of /mnt/d/Development/handheld-deck



## 1. Pico 2 SDK

Once the repository is cloned, run this in a terminal to prepare the Pico SDK.

a. Clone the repository:
```bash
cd ~
git clone https://github.com/raspberrypi/pico-sdk
cd pico-sdk
git submodule update --init --recursive
```

b. Save in environment variables
```bash
echo 'export PICO_SDK_PATH=$HOME/pico-sdk' >> ~/.bashrc
source ~/.bashrc
```

c. Verify it saved
```bash
echo $PICO_SDK_PATH
```

## 2. Installing build tools

a. Install the standard build tools in the terminal
```bash
sudo apt update
sudo apt install -y \
  git \
  cmake \
  ninja-build \
  gcc-arm-none-eabi \
  libnewlib-arm-none-eabi \
  build-essential \
  python3

sudo apt install -y g++-arm-none-eabi
sudo apt install -y usbutils
sudo apt install -y minicom
sudo apt install -y clang-format
```