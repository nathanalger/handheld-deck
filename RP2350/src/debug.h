#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

#define DEBUG_UART_ID uart1

#define DEBUG_BAUD_RATE 115200

#define DEBUG_TX_PIN 4
#define DEBUG_RX_PIN 5

void debug_init(void);

int debug_putb(uint8_t byte);
int debug_puts(const char *str);

void debug_u8(uint8_t value);
void debug_u16(uint16_t value);
void debug_u32(uint32_t value);
void debug_hex8(uint8_t value);

#endif