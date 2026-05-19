#include "pico/stdlib.h"
#include "uart.h"

int main()
{
   uart_setup();
   while (1)
   {
      uart_putb('A');
   }
}