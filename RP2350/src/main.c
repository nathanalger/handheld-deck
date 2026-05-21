#include "pico/stdlib.h"
#include "hardware/sync.h"

#include "event_queue.h"
#include "event_loop.h"
#include "gpio.h"
#include "uart.h"
#include "buttons.h"

event_queue_t system_events;
event_router_t router;

int main()
{
   // System Init
   event_queue_init(&system_events);
   event_router_init(&router);
   gpio_init_driver();
   uart_setup();

   // Output Init
   gpio_init(LED_STATUS_GP);
   gpio_set_dir(LED_STATUS_GP, GPIO_OUT);
   gpio_put(LED_STATUS_GP, 1);

   // Input Init
   button_init(2, &router, default_button_handler);

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