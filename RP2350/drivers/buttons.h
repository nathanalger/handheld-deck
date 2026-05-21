#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>
#include <stdbool.h>
#include "event_loop.h"
#include "gpio.h"

void button_init(uint8_t pin, event_router_t *router, event_handler_t handler);

/**
 * Checks if the button event was a press (down)
 */
bool button_is_pressed(event_t *e);

/**
 * Checks if the button event was a release (up)
 */
bool button_is_released(event_t *e);

/**
 * Sends a UART message with the data being the source pin for both up and down events.
 */
void default_button_handler(event_t *e, void *user);

#endif