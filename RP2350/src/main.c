#include "pico/stdlib.h"
#include "uart.h"

int main()
{
   uart_setup();

   const uint LED_PIN = 25;

   gpio_init(LED_PIN);
   gpio_set_dir(LED_PIN, GPIO_OUT);

   while (1)
   {
      gpio_put(LED_PIN, 1);
      uart_putb('A');
      sleep_ms(500);

      gpio_put(LED_PIN, 0);
      sleep_ms(500);
   }
}