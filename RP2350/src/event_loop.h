#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H
#define MAX_EVENT_TYPES 4
#define MAX_EVENT_SOURCES 32
#define MAX_ROUTES 32

#include "event_queue.h"

typedef void (*event_handler_t)(event_t *e, void *user_data);

typedef struct
{
   event_handler_t handler;
   void *user_data;
   bool used;
} route_entry_t;

typedef struct
{
   route_entry_t table[MAX_EVENT_TYPES][MAX_EVENT_SOURCES];
} event_router_t;

typedef struct
{
   event_type_t type;
   uint8_t id; // gpio number, uart channel, etc

   event_handler_t handler;
   void *user_data;
} event_route_t;

void event_router_init(event_router_t *r);
void event_loop(event_queue_t *q, event_router_t *router);
void event_router_register(
    event_router_t *r,
    event_type_t type,
    uint8_t id,
    event_handler_t handler,
    void *user_data);
int8_t event_router_dispatch(event_router_t *r, event_t *e);

/**
 * Handler for UART RX events
 */
void uart_rx_handler(event_t *e, void *user_data);

#endif