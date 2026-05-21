#include "buttons.h"
#include "uart.h"
#include "pico/stdlib.h"

void button_init(uint8_t pin, event_router_t *router, event_handler_t handler)
{
   gpio_init(pin);
   gpio_set_dir(pin, GPIO_IN);
   gpio_pull_up(pin);

   gpio_enable_events(pin, GPIO_EVENT_FALLING | GPIO_EVENT_RISING);

   event_router_register(router, EVENT_GPIO, pin, handler, NULL);
}

bool button_is_pressed(event_t *e)
{
   return (e->value & GPIO_EVENT_FALLING) != 0;
}

bool button_is_released(event_t *e)
{
   return (e->value & GPIO_EVENT_RISING) != 0;
}

void default_button_handler(event_t *e, void *user)
{
   if (button_is_pressed(e))
   {
      gpio_put(LED_STATUS_GP, 0);
      uart_packet16(BUTTON_DOWN, e->source);
   }
   else if (button_is_released(e))
   {
      gpio_put(LED_STATUS_GP, 1);
      uart_packet16(BUTTON_UP, e->source);
   }
}