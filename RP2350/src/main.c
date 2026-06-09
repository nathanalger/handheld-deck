#include "pico/stdlib.h"
#include "hardware/sync.h"

#include "event_queue.h"
#include "event_loop.h"
#include "gpio.h"
#include "debug.h"
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
   debug_init();
   uart_rx_init(); // Initialize RX state machine

   debug_puts("Initialized System Drivers.\n");

   // Output Init
   gpio_init(LED_STATUS_GP);
   gpio_set_dir(LED_STATUS_GP, GPIO_OUT);
   gpio_put(LED_STATUS_GP, 1);

   debug_puts("Initialized Output Parameters.\n");

   // Input Init
   button_init(2, &router, default_button_handler);

   // Register UART RX handler
   event_router_register(&router, EVENT_UART_RX, 0, uart_rx_handler, NULL);

   debug_puts("Entering Event Loop.\n");

   while (1)
   {
      // Process incoming UART packets
      uart_rx_process();
      uart_rx_poll(&system_events);

      event_loop(&system_events, &router);

      uint32_t state = save_and_disable_interrupts();
      if (event_empty(&system_events))
      {
         debug_puts("Event loop empty. Waiting for interrupt.\n");
         __wfi();
      }
      restore_interrupts(state);
   }
}