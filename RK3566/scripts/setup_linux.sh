#!/bin/bash

set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
VCPKG="$ROOT/vcpkg"

if [ ! -d "$VCPKG" ]; then
    git clone https://github.com/microsoft/vcpkg.git "$VCPKG"
fi

cd "$VCPKG"

if [ ! -f "./vcpkg" ]; then
    ./bootstrap-vcpkg.sh
fi

./vcpkg install sdl3:x64-linux