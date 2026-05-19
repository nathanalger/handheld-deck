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
   gpio_put(LED_STATUS_GP, 1);
   sleep_ms(500);
   // Add debug output to verify function is called
   uart_packet(PKT_PING, (uint8_t *)"B", 1); // This will help confirm if handler is called
}

int main()
{
   // Prepare LED Status Pin
   gpio_init(LED_STATUS_GP);
   gpio_set_dir(LED_STATUS_GP, GPIO_OUT);
   gpio_put(LED_STATUS_GP, 1);

   event_queue_init(&system_events);
   event_router_init(&router);
   gpio_init_driver();
   uart_setup();

   gpio_register_callback(2, GPIO_EVENT_RISING, NULL, NULL);
   event_router_register(&router, EVENT_GPIO, 2, button_handler, NULL);

   gpio_set_dir(2, GPIO_IN);
   gpio_pull_up(2);

   while (1)
   {
      event_loop(&system_events, &router);

      if (event_empty(&system_events))
         __wfi();
   }
}