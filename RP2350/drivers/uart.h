#pragma once

#include <stdint.h>
#include "hardware/uart.h"

#define UART_ID uart0
#define BAUD_RATE 115200

#define UART_TX_PIN 0
#define UART_RX_PIN 1

/**
 * Initializes UART for peripheral communication. Prepares GPIO function and format. Always uses UART0.
 */
void uart_setup();

/**
 * Adds a byte to the UART FIFO
 */
void uart_putb(uint8_t byte);