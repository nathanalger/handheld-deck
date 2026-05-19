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
   uart_packet(PKT_GPIO_EVENT, (uint8_t *)"A", 1);
}

int main()
{
   event_queue_init(&system_events);
   event_router_init(&router);
   gpio_init_driver();
   uart_setup();

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