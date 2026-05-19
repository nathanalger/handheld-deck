#include "pico/stdlib.h"
#include "hardware/sync.h"

#include "event_queue.h"
#include "event_loop.h"
#include "gpio.h"
#include "uart.h"

event_queue_t system_events;
event_router_t router;

void button_handler(event_t *e, void *user)
{
   // Turn off the LED when button is pressed for debugging
   gpio_put(LED_STATUS_GP, 0);
   uart_packet(PKT_GPIO_EVENT, (uint8_t *)"A", 1);
   // Turn LED back on after sending packet
   sleep_ms(500);
   gpio_put(LED_STATUS_GP, 1);
   // Add debug output to verify function is called
   uart_packet(PKT_PING, (uint8_t *)"B", 1); // This will help confirm if handler is called
}

int main()
{
   event_queue_init(&system_events);
   event_router_init(&router);

   // Prepare LED Status Pin
   gpio_init(LED_STATUS_GP);
   gpio_init(2);

   gpio_set_dir(LED_STATUS_GP, GPIO_OUT);
   gpio_put(LED_STATUS_GP, 1);

   gpio_init_driver();
   uart_setup();

   gpio_set_dir(2, GPIO_IN);
   gpio_pull_up(2);
   gpio_register_callback(2, GPIO_EVENT_FALLING, NULL, NULL);
   event_router_register(&router, EVENT_GPIO, 2, button_handler, NULL);

   while (1)
   {
      event_loop(&system_events, &router);

      uint32_t state = save_and_disable_interrupts();
      if (event_empty(&system_events))
      {
         __wfi();
      }
      restore_interrupts(state);
   }
}