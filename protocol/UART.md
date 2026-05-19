# UART Protocol 

This document describes the current UART packet protocol used in the system. It is a lightweight binary framing protocol designed for embedded communication over UART (RP2350 / Pico SDK).

## 1. Physical Layer

- UART peripheral: uart0
- Baud rate: 115200
- Frame format: 8N1 (8 data bits, no parity, 1 stop bit)
- Pins:
  - TX: GPIO 0
  - RX: GPIO 1

## 2. General Design Goals

This protocol is designed for:

- Fast binary communication
- Minimal CPU overhead
- Simple parsing state machine
- Robust recovery using sync bytes
- Error detection via CRC8
- Extensibility via packet types

## 3. Packet Structure

Each packet has the following structure:

[SYNC0: 1][SYNC1: 1][SEQ: 1][TYPE: 1][SIZE: 1][PAYLOAD: SIZE][CRC: 1]

## 4. Field Definitions

### 4.1 Sync Bytes

0xAA 0x55

Purpose:
- Mark the start of a packet
- Allow receiver to re-synchronize if byte stream is corrupted or lost

### 4.2 Sequence Number (SEQ)

- uint8_t
- increments per packet
- wraps at 255

### 4.3 Packet Type (TYPE)

PKT_PING = 0x01

### 4.4 Payload Size (SIZE)

uint16_t little endian

Max payload: 256 bytes

### 4.5 Payload

Raw binary data depending on packet type

### 4.6 CRC8

Polynomial 0x07, init 0x00

Covers:
SEQ + TYPE + SIZE + PAYLOAD