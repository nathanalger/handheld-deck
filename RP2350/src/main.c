#include "pico/stdlib.h"
#include "hardware/sync.h"

#include "event_queue.h"
#include "event_loop.h"
#include "gpio.h"
#include "uart.h"
#include "buttons.h"

event_queue_t system_events;
event_router_t router;

void button_a_handler(event_t *e, void *user)
{
   if (button_is_pressed(e))
   {
      gpio_put(LED_STATUS_GP, 0);
      uart_packet(BUTTON_DOWN, (uint8_t *)"DOWN", 4);
   }
   else if (button_is_released(e))
   {
      gpio_put(LED_STATUS_GP, 1);
      uart_packet(BUTTON_UP, (uint8_t *)"UP", 2);
   }
}

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
   button_init(2, &router, button_a_handler);

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