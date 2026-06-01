#include "uart.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "hardware/irq.h"
#include "event_loop.h"
#include "debug.h"

#define UART_RX_RING_SIZE 512

static volatile uint8_t seq = 0;

static volatile uint8_t rx_ring[UART_RX_RING_SIZE];
static volatile uint16_t rx_head = 0;
static volatile uint16_t rx_tail = 0;
static volatile uint32_t rx_dropped = 0;

extern volatile bool uart_woke;

static enum {
    RX_STATE_SYNC_0,
    RX_STATE_SYNC_1,
    RX_STATE_SEQ,
    RX_STATE_TYPE,
    RX_STATE_SIZE_LO,
    RX_STATE_SIZE_HI,
    RX_STATE_PAYLOAD,
    RX_STATE_CRC
} rx_state = RX_STATE_SYNC_0;

static uint8_t rx_buffer[UART_MAX_PAYLOAD];
static uint16_t rx_payload_size = 0;
static uint16_t rx_payload_index = 0;
static uint8_t rx_actual_crc = 0;
static uint8_t rx_seq = 0;
static uint8_t rx_type = 0;

static rx_packet_t rx_packet;
static volatile bool rx_packet_ready = false;
static volatile bool rx_packet_error = false;

static void uart_irq_handler(void);

void uart_setup(void)
{
    uart_init(UART_ID, BAUD_RATE);

    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);

    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_fifo_enabled(UART_ID, true);

    while (uart_is_readable(UART_ID))
    {
        uart_getc(UART_ID);
    }

    irq_set_exclusive_handler(UART0_IRQ, uart_irq_handler);
    irq_set_priority(UART0_IRQ, 1);
    irq_set_enabled(UART0_IRQ, true);

    uart_set_irq_enables(UART_ID, true, false);
}

void uart_rx_init(void)
{
    rx_state = RX_STATE_SYNC_0;
    rx_payload_size = 0;
    rx_payload_index = 0;
    rx_actual_crc = 0;
    rx_packet_ready = false;
    rx_packet_error = false;
}

uint8_t next_seq(void)
{
    uint32_t save = save_and_disable_interrupts();
    uint8_t s = seq++;
    restore_interrupts(save);
    return s;
}

int uart_putb(uint8_t byte)
{
    while (!uart_is_writable(UART_ID))
        tight_loop_contents();

    uart_putc_raw(UART_ID, byte);
    return 0;
}

uint8_t crc8_i(uint8_t crc, uint8_t byte)
{
    crc ^= byte;

    for (uint8_t i = 0; i < 8; i++)
    {
        if (crc & 0x80)
            crc = (crc << 1) ^ 0x07;
        else
            crc <<= 1;
    }

    return crc;
}

int uart_packet(uart_packet_t type, uint8_t *payload, uint16_t size)
{
    if (size > UART_MAX_PAYLOAD)
        return -1;

    uint8_t crc = 0;

    uint8_t size_lo = size & 0xFF;
    uint8_t size_hi = (size >> 8) & 0xFF;

    uart_putb(UART_SYNC_0);
    uart_putb(UART_SYNC_1);

    uint8_t s = next_seq();
    uart_putb(s);
    crc = crc8_i(crc, s);

    uart_putb(type);
    crc = crc8_i(crc, type);

    uart_putb(size_lo);
    crc = crc8_i(crc, size_lo);

    uart_putb(size_hi);
    crc = crc8_i(crc, size_hi);

    for (uint16_t i = 0; i < size; i++)
    {
        uart_putb(payload[i]);
        crc = crc8_i(crc, payload[i]);
    }

    uart_putb(crc);
    return 0;
}

int uart_packet16(uart_packet_t type, uint16_t value)
{
    return uart_packet(type, (uint8_t *)&value, sizeof(value));
}

int uart_rx_process(void)
{
    while (rx_tail != rx_head)
    {
        uint8_t byte = rx_ring[rx_tail];
        rx_tail = (rx_tail + 1) % UART_RX_RING_SIZE;

        switch (rx_state)
        {
        case RX_STATE_SYNC_0:
            if (byte == UART_SYNC_0)
            {
                rx_actual_crc = 0;
                rx_payload_index = 0;
                rx_payload_size = 0;
                rx_state = RX_STATE_SYNC_1;
            }
            break;

        case RX_STATE_SYNC_1:
            if (byte == UART_SYNC_1)
                rx_state = RX_STATE_SEQ;
            else
                rx_state = RX_STATE_SYNC_0;
            break;

        case RX_STATE_SEQ:
            rx_seq = byte;
            rx_actual_crc = crc8_i(rx_actual_crc, byte);
            rx_state = RX_STATE_TYPE;
            break;

        case RX_STATE_TYPE:
            rx_type = byte;
            rx_actual_crc = crc8_i(rx_actual_crc, byte);
            rx_state = RX_STATE_SIZE_LO;
            break;

        case RX_STATE_SIZE_LO:
            rx_payload_size = byte;
            rx_actual_crc = crc8_i(rx_actual_crc, byte);
            rx_state = RX_STATE_SIZE_HI;
            break;

        case RX_STATE_SIZE_HI:
            rx_payload_size |= (byte << 8);
            rx_actual_crc = crc8_i(rx_actual_crc, byte);

            if (rx_payload_size > UART_MAX_PAYLOAD)
            {
                rx_packet_error = true;
                rx_state = RX_STATE_SYNC_0;
                rx_actual_crc = 0;
            }
            else if (rx_payload_size == 0)
            {
                rx_state = RX_STATE_CRC;
            }
            else
            {
                rx_payload_index = 0;
                rx_state = RX_STATE_PAYLOAD;
            }
            break;

        case RX_STATE_PAYLOAD:
            if (rx_payload_index < UART_MAX_PAYLOAD)
            {
                rx_buffer[rx_payload_index++] = byte;
                rx_actual_crc = crc8_i(rx_actual_crc, byte);
            }
            else
            {
                rx_state = RX_STATE_SYNC_0;
                rx_actual_crc = 0;
            }
            break;

        case RX_STATE_CRC:
            if (rx_actual_crc == byte)
            {
                rx_packet.type = (uart_packet_t)rx_type;
                rx_packet.size = rx_payload_size;
                rx_packet.seq = rx_seq;

                rx_packet.value = 0;
                if (rx_payload_size >= 2)
                    rx_packet.value = (rx_buffer[1] << 8) | rx_buffer[0];
                else if (rx_payload_size == 1)
                    rx_packet.value = rx_buffer[0];

                rx_packet_ready = true;
                rx_packet_error = false;
            }
            else
            {
                rx_packet_error = true;
                rx_actual_crc = 0;
            }

            rx_state = RX_STATE_SYNC_0;
            break;
        }
    }

    if (rx_packet_ready)
        return 1;
    if (rx_packet_error)
        return -1;
    return 0;
}

int uart_rx_get_packet(rx_packet_t *packet)
{
    if (rx_packet_ready && packet)
    {
        *packet = rx_packet;
        rx_packet_ready = false;
        rx_packet_error = false;
        return 1;
    }
    return 0;
}

int uart_rx_poll(event_queue_t *q)
{
    uart_rx_process();

    rx_packet_t packet;

    while (uart_rx_get_packet(&packet))
    {
        event_t e = {
            .type = EVENT_UART_RX,
            .source = packet.type,
            .value = packet.value};

        debug_puts("UART RX: Type 0x");
        debug_hex8(packet.type);
        debug_puts("\n");

        event_push(q, e);
    }

    return 0;
}

static void uart_irq_handler(void)
{
    uart_woke = true;

    while (uart_is_readable(UART_ID))
    {
        uint8_t byte = uart_getc(UART_ID);

        uint16_t next = (rx_head + 1) % UART_RX_RING_SIZE;

        if (next != rx_tail)
        {
            rx_ring[rx_head] = byte;
            rx_head = next;
        }
        else
        {
            rx_dropped++;
        }
    }
}

bool uart_rx_pending(void)
{
    return rx_head != rx_tail;
}