#pragma once

#include <stdint.h>
#include "hardware/uart.h"
#include "hardware/structs/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

#define UART_SYNC_0 0xAA
#define UART_SYNC_1 0x55

#define UART_MAX_PAYLOAD 256

typedef uint8_t uart_packet_t;
enum
{
   PKT_PING = 0x01
};

/**
 * Initializes UART for peripheral communication. Prepares GPIO function and format. Always uses UART0.
 */
void uart_setup();

/**
 * Adds a byte to the UART FIFO.
 */
int uart_putb(uint8_t byte);

/**
 * Sends a uint32_t as 4 little-endian bytes.
 */
void uart_put32(uint32_t value);

/**
 * Sends a uint16_t as 2 little-endian bytes.
 */
void uart_put16(uint16_t value);

/**
 * Incremental crc8 builder for error detection.
 */
uint8_t crc8_i(uint8_t crc, uint8_t byte);

/**
 * Sends a structured packet through the UART0 BUS, little endian.
 */
int uart_packet(
    uart_packet_t type,
    uint8_t *payload,
    uint16_t size);
