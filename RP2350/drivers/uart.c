#include "uart.h"
#include "pico/stdlib.h"

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

void uart_putb(uint8_t byte)
{
   uart_hw_t *hw = uart_get_hw(uart0);

   while (hw->fr & UART_UARTFR_TXFF_BITS)
   {
      tight_loop_contents();
   }

   hw->dr = byte;
}