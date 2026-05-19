#pragma once

#include <stdint.h>
#include <stdbool.h>

#define GPIO_MAX_PINS 32
#define GPIO_EVENT_QUEUE_SIZE 32
#define LED_STATUS_GP 25

typedef enum
{
   GPIO_EVENT_RISING = 1 << 0,
   GPIO_EVENT_FALLING = 1 << 1,
} gpio_event_t;

typedef void (*gpio_callback_t)(
    uint8_t gpio,
    gpio_event_t event,
    void *user_data);

typedef struct
{
   gpio_callback_t callback;
   void *user_data;
   bool enabled;
   gpio_event_t subscribed_events;
} gpio_handler_t;

typedef struct
{
   uint8_t gpio;
   gpio_event_t event;
} gpio_event_entry_t;

/**
 * Initializes internal GPIO event system and IRQ routing.
 */
void gpio_init_driver(void);

/**
 * Registers a callback for a GPIO pin.
 */
void gpio_register_callback(
    uint8_t gpio,
    gpio_event_t events,
    gpio_callback_t cb,
    void *user_data);

/**
 * Enables or disables a GPIO interrupt without removing handler.
 */
void gpio_set_enabled(uint8_t gpio, bool enabled);

gpio_handler_t *gpio_get_handler(uint8_t gpio);