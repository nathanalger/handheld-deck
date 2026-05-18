#!/usr/bin/env bash
set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
FW_DIR="$ROOT_DIR/RP2040"
BUILD_DIR="$FW_DIR/build"

echo "[build] Project root: $ROOT_DIR"

# Check SDK
if [ -z "$PICO_SDK_PATH" ]; then
    echo "[error] PICO_SDK_PATH is not set"
    exit 1
fi

echo "[build] Using Pico SDK at: $PICO_SDK_PATH"

# Clean build
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

cd "$BUILD_DIR"

echo "[build] Running CMake..."
cmake -G Ninja "$FW_DIR"

echo "[build] Compiling..."
ninja

echo "[build] Done"

# Find UF2 output
UF2_FILE=$(find "$BUILD_DIR" -maxdepth 1 -name "*.uf2" | head -n 1)

if [ -z "$UF2_FILE" ]; then
    echo "[error] No UF2 file produced"
    exit 1
fi

echo "[build] Output: $UF2_FILE"