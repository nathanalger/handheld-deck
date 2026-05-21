#include "gpio.h"
#include "event_queue.h"

#include "pico/time.h"
#include "hardware/gpio.h"
#include "hardware/irq.h"

#define MAX_GPIO_PINS 32
#define GPIO_DEBOUNCE_MS 15

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

   static uint32_t last_rise_time[MAX_GPIO_PINS] = {0};
   static uint32_t last_fall_time[MAX_GPIO_PINS] = {0};

   gpio_handler_t *h = &handlers[gpio];

   if (!h->enabled)
      return;

   uint32_t now = to_ms_since_boot(get_absolute_time());

   gpio_event_t events = translate_events(hardware_events);

   // filter by subscription
   events &= h->subscribed_events;

   if (events == 0)
      return;

   if (events & GPIO_EVENT_RISING)
   {
      if (now - last_rise_time[gpio] < GPIO_DEBOUNCE_MS)
         events &= ~GPIO_EVENT_RISING;
      else
         last_rise_time[gpio] = now;
   }

   if (events & GPIO_EVENT_FALLING)
   {
      if (now - last_fall_time[gpio] < GPIO_DEBOUNCE_MS)
         events &= ~GPIO_EVENT_FALLING;
      else
         last_fall_time[gpio] = now;
   }

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

   gpio_set_irq_enabled_with_callback(gpio, hw_events, true, gpio_irq_handler);
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

void gpio_enable_events(uint8_t gpio, gpio_event_t events)
{
   if (gpio >= MAX_GPIO_PINS)
      return;

   handlers[gpio].enabled = true;
   handlers[gpio].subscribed_events = events;

   uint32_t hw_events = 0;

   if (events & GPIO_EVENT_RISING)
      hw_events |= GPIO_IRQ_EDGE_RISE;

   if (events & GPIO_EVENT_FALLING)
      hw_events |= GPIO_IRQ_EDGE_FALL;

   gpio_set_irq_enabled_with_callback(gpio, hw_events, true, gpio_irq_handler);
}