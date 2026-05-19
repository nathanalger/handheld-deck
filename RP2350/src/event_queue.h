#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <stdint.h>
#include <stdbool.h>

#define EVENT_QUEUE_SIZE 64

typedef enum
{
   EVENT_GPIO = 0,
   EVENT_UART_RX,
   EVENT_UART_TX,
   EVENT_SYSTEM
} event_type_t;

typedef struct
{
   event_type_t type;
   uint8_t source;
   uint16_t value;
} event_t;

typedef struct
{
   volatile uint8_t head;
   volatile uint8_t tail;
   event_t data[EVENT_QUEUE_SIZE];
} event_queue_t;

void event_queue_init(event_queue_t *q);

bool event_push(event_queue_t *q, event_t e);
bool event_pop(event_queue_t *q, event_t *out);
bool event_empty(event_queue_t *q);

#endif