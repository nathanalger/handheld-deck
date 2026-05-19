#include "event_queue.h"
#include "hardware/sync.h"

void event_queue_init(event_queue_t *q)
{
   q->head = 0;
   q->tail = 0;
}

bool event_push(event_queue_t *q, event_t e)
{
   uint32_t state = save_and_disable_interrupts();

   uint8_t next = (uint8_t)((q->head + 1) % EVENT_QUEUE_SIZE);

   // full
   if (next == q->tail)
   {
      restore_interrupts(state);
      return false;
   }

   q->data[q->head] = e;
   q->head = next;

   restore_interrupts(state);
   return true;
}

bool event_pop(event_queue_t *q, event_t *out)
{
   uint32_t state = save_and_disable_interrupts();

   if (q->head == q->tail)
   {
      restore_interrupts(state);
      return false;
   }

   *out = q->data[q->tail];
   q->tail = (uint8_t)((q->tail + 1) % EVENT_QUEUE_SIZE);

   restore_interrupts(state);
   return true;
}

bool event_empty(event_queue_t *q)
{
   return q->head == q->tail;
}