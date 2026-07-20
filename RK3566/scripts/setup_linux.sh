#!/bin/bash

set -euo pipefail

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
VCPKG="$ROOT/vcpkg"

if [ ! -d "$VCPKG" ]; then
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG"
fi

cd "$VCPKG"

if [ ! -f "./vcpkg" ]; then
    ./bootstrap-vcpkg.sh
fi

if command -v dnf >/dev/null 2>&1; then
    echo "Detected Fedora/RHEL-based system; installing build prerequisites..."
    SUDO=""
    if [ "$(id -u)" -ne 0 ]; then
        if command -v sudo >/dev/null 2>&1; then
            SUDO="sudo"
        else
            echo "sudo is required to install build dependencies. Please run this script as root or install sudo." >&2
            exit 1
        fi
    fi

    "$SUDO" dnf install -y --skip-unavailable \
        @development-tools \
        autoconf autoconf-archive automake libtool \
        pkgconf-pkg-config cmake ninja-build gperf \
        git curl zip unzip tar \
        perl-open perl-FindBin \
        libX11-devel libXext-devel libXcursor-devel libXi-devel libXrandr-devel \
        libXrender-devel libXfixes-devel libXScrnSaver-devel libxkbcommon-devel \
        wayland-devel wayland-protocols-devel libdrm-devel mesa-libEGL-devel mesa-libGL-devel \
        dbus-devel ibus-devel alsa-lib-devel libudev-devel pulseaudio-libs-devel
elif command -v apt-get >/dev/null 2>&1; then
    echo "Detected Debian/Ubuntu-based system; installing build prerequisites..."
    SUDO=""
    if [ "$(id -u)" -ne 0 ]; then
        if command -v sudo >/dev/null 2>&1; then
            SUDO="sudo"
        else
            echo "sudo is required to install build dependencies. Please run this script as root or install sudo." >&2
            exit 1
        fi
    fi

    "$SUDO" apt-get update
    "$SUDO" apt-get install -y \
        build-essential autoconf autoconf-archive automake libtool pkg-config \
        cmake ninja-build gperf git curl zip unzip tar \
        libx11-dev libxext-dev libxcursor-dev libxi-dev libxrandr-dev \
        libxrender-dev libxfixes-dev libxss-dev libxkbcommon-dev \
        libwayland-dev libwayland-egl-backend-dev libegl1-mesa-dev libgl1-mesa-dev \
        libdbus-1-dev libibus-1.0-dev libasound2-dev libudev-dev libpulse-dev
else
    echo "Unsupported Linux distribution; install the required build dependencies manually before continuing." >&2
    exit 1
fi

./vcpkg install sdl3:x64-linux