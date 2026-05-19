#include "uart.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"

static uint8_t seq = 0;

void uart_setup()
{
   uart_init(UART_ID, BAUD_RATE);

   gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
   gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

   uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);

   uart_set_fifo_enabled(UART_ID, true);

   while (uart_is_readable(UART_ID))
   {
      uart_getc(UART_ID);
   }
}

uint8_t next_seq()
{
   uint32_t save = save_and_disable_interrupts();
   uint8_t s = seq++;
   restore_interrupts(save);
   return s;
}

int uart_putb(uint8_t byte)
{
   uart_putc_raw(UART_ID, byte);

   return 0;
}

int uart_put32(uint32_t value)
{
   uint8_t bytes[4];

   bytes[0] = (value >> 0) & 0xFF;
   bytes[1] = (value >> 8) & 0xFF;
   bytes[2] = (value >> 16) & 0xFF;
   bytes[3] = (value >> 24) & 0xFF;

   for (uint8_t i = 0; i < 4; i++)
   {
      uart_putb(bytes[i]);
   }

   return 0;
}

int uart_put16(uint16_t value)
{
   uint8_t bytes[2];

   bytes[0] = (value >> 0) & 0xFF;
   bytes[1] = (value >> 8) & 0xFF;

   for (uint8_t i = 0; i < 2; i++)
   {
      uart_putb(bytes[i]);
   }

   return 0;
}

uint8_t crc8_i(uint8_t crc, uint8_t byte)
{
   crc ^= byte;

   for (uint8_t j = 0; j < 8; j++)
   {
      if (crc & 0x80)
         crc = (crc << 1) ^ 0x07;
      else
         crc <<= 1;
   }

   return crc;
}

int uart_packet(
    uart_packet_t type,
    uint8_t *payload,
    uint16_t size)
{
   if (size > UART_MAX_PAYLOAD)
      return -1;

   uint8_t crc = 0x00;

   uint8_t size_lo = size & 0xFF;
   uint8_t size_hi = (size >> 8) & 0xFF;

   // Sync bytes
   uart_putb(UART_SYNC_0);
   uart_putb(UART_SYNC_1);

   // Write sequence
   uart_putb(seq);
   crc = crc8_i(crc, seq);

   // Type
   uart_putb(type);
   crc = crc8_i(crc, type);

   // Size
   uart_putb(size_lo);
   crc = crc8_i(crc, size_lo);

   uart_putb(size_hi);
   crc = crc8_i(crc, size_hi);

   // Payload
   for (uint16_t i = 0; i < size; i++)
   {
      uart_putb(payload[i]);
      crc = crc8_i(crc, payload[i]);
   }

   // CRC
   uart_putb(crc);

   // Incrememnt sequence
   seq = (uint8_t)(seq + 1);
   return 0;
}