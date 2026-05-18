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

echo "[flash] Searching for Pico in BOOTSEL mode..."

PICO_MOUNT=$(find /media /run/media \
    -type f \
    -name "INFO_UF2.TXT" \
    2>/dev/null | head -n 1 | xargs -r dirname)

if [ -z "$PICO_MOUNT" ]; then
    echo "[error] Pico not found in BOOTSEL mode."
    echo "        Hold BOOTSEL while plugging in the Pico."
    exit 1
fi

echo "[flash] Pico mounted at: $PICO_MOUNT"

cp "$UF2_FILE" "$PICO_MOUNT/"

sync

echo "[flash] Flash complete"