#include "event_loop.h"
#include <stddef.h>
#include "debug.h"
#include "uart.h"

void event_router_init(event_router_t *r)
{
   for (int t = 0; t < MAX_EVENT_TYPES; t++)
   {
      for (int i = 0; i < MAX_EVENT_SOURCES; i++)
      {
         r->table[t][i].handler = NULL;
         r->table[t][i].user_data = NULL;
         r->table[t][i].used = false;
      }
   }
}

void event_loop(event_queue_t *q, event_router_t *router)
{
   event_t e;

   while (event_pop(q, &e))
   {
      int8_t result = event_router_dispatch(router, &e);
      switch (result)
      {
      case -1:
         /* type or source exceeds maximum in memory */
         debug_puts("Failure upon event dispatch: type or source exceeds maximum size in memory.\n");
         break;
      case 0:
         /* failed to locate a handler */
         debug_puts("Failure upon event dispatch: no suitable handler found. Searching for: table[");
         debug_hex8(e.type);
         debug_puts("][");
         debug_hex8("].");
         break;
      case 1:
         /* successfully executed handler */
         break;
      }
   }
}

void event_router_register(
    event_router_t *r,
    event_type_t type,
    uint8_t id,
    event_handler_t handler,
    void *user_data)
{
   if (type >= MAX_EVENT_TYPES || id >= MAX_EVENT_SOURCES)
      return;

   r->table[type][id].handler = handler;
   r->table[type][id].user_data = user_data;
   r->table[type][id].used = true;
}

int8_t event_router_dispatch(event_router_t *r, event_t *e)
{
   if (e->type >= MAX_EVENT_TYPES || e->source >= MAX_EVENT_SOURCES)
      return -1;

   route_entry_t *entry = &r->table[e->type][e->source];

   if (!entry->used || !entry->handler)
      return 0;

   entry->handler(e, entry->user_data);
   return 1;
}

void uart_rx_handler(event_t *e, void *user_data)
{
   // This handler processes UART RX events
   // The event type is EVENT_UART_RX and the value contains the packet data

   debug_puts("Recieved UART Ping.\n");
}