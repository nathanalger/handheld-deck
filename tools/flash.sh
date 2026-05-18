#!/usr/bin/env bash
set -e

ROOT_DIR="$(cd "$(dirname "$0")/.." && pwd)"
BUILD_DIR="$ROOT_DIR/RP2350/build"

echo "[flash] Searching for UF2..."

UF2_FILE=$(find "$BUILD_DIR" -maxdepth 1 -name "*.uf2" | head -n 1)

if [ -z "$UF2_FILE" ]; then
    echo "[error] No UF2 found. Build first."
    exit 1
fi

echo "[flash] Found: $UF2_FILE"

# BOOTSEL mount point (WSL2 uses /mnt/..., Linux uses /media/)
PICO_MOUNT=""

if [ -d "/mnt" ]; then
    # WSL2 common mount behavior
    PICO_MOUNT=$(ls /mnt | while read d; do
        if [ -d "/mnt/$d/RPI-RP2" ]; then
            echo "/mnt/$d/RPI-RP2"
            break
        fi
    done)
fi

# Fallback Linux mount path
if [ -z "$PICO_MOUNT" ]; then
    if [ -d "/media" ]; then
        PICO_MOUNT=$(find /media -type d -name "RPI-RP2" 2>/dev/null | head -n 1)
    fi
fi

if [ -z "$PICO_MOUNT" ]; then
    echo "[error] Pico not found in BOOTSEL mode."
    echo "        Plug in while holding BOOTSEL (or press BOOTSEL + reset)"
    exit 1
fi

echo "[flash] Pico mounted at: $PICO_MOUNT"

cp "$UF2_FILE" "$PICO_MOUNT/"

echo "[flash] Flash complete"