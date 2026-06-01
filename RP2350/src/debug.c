#include "debug.h"

#include "pico/stdlib.h"

void debug_init(void)
{
   uart_init(DEBUG_UART_ID, DEBUG_BAUD_RATE);

   gpio_set_function(DEBUG_TX_PIN, GPIO_FUNC_UART);
   gpio_set_function(DEBUG_RX_PIN, GPIO_FUNC_UART);

   uart_set_format(DEBUG_UART_ID, 8, 1, UART_PARITY_NONE);

   uart_set_fifo_enabled(DEBUG_UART_ID, true);
}

int debug_putb(uint8_t byte)
{
   while (!uart_is_writable(DEBUG_UART_ID))
      tight_loop_contents();

   if (byte == '\n')
      uart_putc_raw(DEBUG_UART_ID, '\r');

   uart_putc_raw(DEBUG_UART_ID, byte);

   return 0;
}

int debug_puts(const char *str)
{
   while (*str)
   {
      debug_putb((uint8_t)*str++);
   }

   return 0;
}

static void debug_print_unsigned(uint32_t value)
{
   char buffer[11];
   int i = 0;

   if (value == 0)
   {
      debug_putb('0');
      return;
   }

   while (value > 0)
   {
      buffer[i++] = '0' + (value % 10);
      value /= 10;
   }

   while (i--)
   {
      debug_putb(buffer[i]);
   }
}

void debug_u8(uint8_t value)
{
   debug_print_unsigned(value);
}

void debug_u16(uint16_t value)
{
   debug_print_unsigned(value);
}

void debug_u32(uint32_t value)
{
   debug_print_unsigned(value);
}