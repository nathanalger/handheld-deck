#include "gpio.h"
#include "event_queue.h"

#include "hardware/gpio.h"
#include "hardware/irq.h"

#define MAX_GPIO_PINS 32

extern event_queue_t system_events;
static gpio_handler_t handlers[MAX_GPIO_PINS];

static gpio_event_t translate_events(uint32_t hardware_events)
{
   gpio_event_t e = 0;

   if (hardware_events & GPIO_IRQ_EDGE_RISE)
      e |= GPIO_EVENT_RISING;

   if (hardware_events & GPIO_IRQ_EDGE_FALL)
      e |= GPIO_EVENT_FALLING;

   return e;
}

static void gpio_irq_dispatch(uint8_t gpio, uint32_t hardware_events)
{
   if (gpio >= MAX_GPIO_PINS)
      return;

   gpio_handler_t *h = &handlers[gpio];

   if (!h->enabled)
      return;

   gpio_event_t events = translate_events(hardware_events);

   // filter by subscription
   events &= h->subscribed_events;

   if (events == 0)
      return;

   event_t e = {
       .type = EVENT_GPIO,
       .source = gpio,
       .value = (uint16_t)events};

   event_push(&system_events, e);
}

static void gpio_irq_handler(uint gpio, uint32_t events)
{
   gpio_irq_dispatch((uint8_t)gpio, events);
}

void gpio_init_driver(void)
{
   for (int i = 0; i < MAX_GPIO_PINS; i++)
   {
      handlers[i].callback = NULL;
      handlers[i].user_data = NULL;
      handlers[i].enabled = false;
      handlers[i].subscribed_events = 0;
   }

   gpio_set_irq_callback(gpio_irq_handler);
   irq_set_enabled(IO_IRQ_BANK0, true);
}

void gpio_register_callback(
    uint8_t gpio,
    gpio_event_t events,
    gpio_callback_t cb,
    void *user_data)
{
   if (gpio >= MAX_GPIO_PINS)
      return;

   handlers[gpio].callback = cb;
   handlers[gpio].user_data = user_data;
   handlers[gpio].enabled = true;
   handlers[gpio].subscribed_events = events;

   uint32_t hw_events = 0;

   if (events & GPIO_EVENT_RISING)
      hw_events |= GPIO_IRQ_EDGE_RISE;

   if (events & GPIO_EVENT_FALLING)
      hw_events |= GPIO_IRQ_EDGE_FALL;

   gpio_set_irq_enabled(gpio, hw_events, true);
}

void gpio_set_enabled(uint8_t gpio, bool enabled)
{
   if (gpio >= MAX_GPIO_PINS)
      return;

   handlers[gpio].enabled = enabled;

   if (!enabled)
   {
      gpio_set_irq_enabled(
          gpio,
          GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL,
          false);
   }
}

gpio_handler_t *gpio_get_handler(uint8_t gpio)
{
   if (gpio >= MAX_GPIO_PINS)
      return NULL;

   return &handlers[gpio];
}