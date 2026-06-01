#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "hardware/irq.h"

#include "event_queue.h"
#include "event_loop.h"
#include "gpio.h"
#include "debug.h"
#include "uart.h"
#include "buttons.h"

event_queue_t system_events;
event_router_t router;

volatile bool uart_woke = false;

int main(void)
{
   event_queue_init(&system_events);
   event_router_init(&router);

   gpio_init_driver();

   uart_setup();
   uart_rx_init();

   debug_init();

   debug_puts("Initialized System Drivers.\n");

   gpio_init(LED_STATUS_GP);
   gpio_set_dir(LED_STATUS_GP, GPIO_OUT);
   gpio_put(LED_STATUS_GP, 1);

   debug_puts("Initialized Output Parameters.\n");

   button_init(2, &router, default_button_handler);

   event_router_register(&router, EVENT_UART_RX, 0, uart_rx_handler, NULL);

   debug_puts("Entering Event Loop.\n");

   while (1)
   {
      uart_rx_process();
      uart_rx_poll(&system_events);
      event_loop(&system_events, &router);

      bool can_sleep =
          event_empty(&system_events) &&
          !uart_rx_pending() &&
          !uart_woke;

      if (can_sleep)
      {
         uart_woke = false;
         __wfi();
      }
   }
}