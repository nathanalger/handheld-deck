#include "uart.h"
#include "pico/stdlib.h"
#include "hardware/sync.h"
#include "event_loop.h"
#include "debug.h"

static volatile uint8_t seq = 0;

// RX state machine variables
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
static uint8_t rx_expected_crc = 0;
static uint8_t rx_actual_crc = 0;
static uint8_t rx_seq = 0;
static uint8_t rx_type = 0;
static rx_packet_t rx_packet;
static volatile bool rx_packet_ready = false;
static volatile bool rx_packet_error = false;

void uart_setup()
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
}

void uart_rx_init(void)
{
    rx_state = RX_STATE_SYNC_0;
    rx_payload_size = 0;
    rx_payload_index = 0;
    rx_expected_crc = 0;
    rx_actual_crc = 0;
    rx_packet_ready = false;
    rx_packet_error = false;
}

uint8_t next_seq()
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

    for (uint8_t j = 0; j < 8; j++)
    {
        if (crc & 0x80)
            crc = (crc << 1) ^ 0x07;
        else
            crc <<= 1;
    }

    return crc;
}

int uart_packet(
    uart_packet_t type,
    uint8_t *payload,
    uint16_t size)
{
    if (size > UART_MAX_PAYLOAD)
        return -1;

    uint8_t crc = 0x00;

    uint8_t size_lo = size & 0xFF;
    uint8_t size_hi = (size >> 8) & 0xFF;

    // Sync bytes
    uart_putb(UART_SYNC_0);
    uart_putb(UART_SYNC_1);

    // Write sequence
    uint8_t s = next_seq();
    uart_putb(s);
    crc = crc8_i(crc, s);

    // Type
    uart_putb(type);
    crc = crc8_i(crc, type);

    // Size
    uart_putb(size_lo);
    crc = crc8_i(crc, size_lo);

    uart_putb(size_hi);
    crc = crc8_i(crc, size_hi);

    // Payload
    for (uint16_t i = 0; i < size; i++)
    {
        uart_putb(payload[i]);
        crc = crc8_i(crc, payload[i]);
    }

    // CRC
    uart_putb(crc);

    return 0;
}

int uart_packet16(uart_packet_t type, uint16_t value)
{
    return uart_packet(type, (uint8_t *)&value, sizeof(value));
}

int uart_rx_process(void)
{
    // Check if data is available
    while (uart_is_readable(UART_ID))
    {
        debug_puts("UART BYTE\n");

        uint8_t byte = uart_getc(UART_ID);

        switch (rx_state)
        {
        case RX_STATE_SYNC_0:
            if (byte == UART_SYNC_0)
            {
                rx_actual_crc = 0;
                rx_state = RX_STATE_SYNC_1;
            }
            break;

        case RX_STATE_SYNC_1:
            if (byte == UART_SYNC_1)
            {
                rx_state = RX_STATE_SEQ;
            }
            else
            {
                // Reset if we don't get the expected sync byte
                rx_state = RX_STATE_SYNC_0;
            }
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
                // Error: payload too large
                rx_packet_error = true;
                rx_state = RX_STATE_SYNC_0;
            }
            else if (rx_payload_size == 0)
            {
                // No payload, go directly to CRC
                rx_state = RX_STATE_CRC;
            }
            else
            {
                // Expect payload
                rx_payload_index = 0;
                rx_state = RX_STATE_PAYLOAD;
            }
            break;

        case RX_STATE_PAYLOAD:
            rx_buffer[rx_payload_index++] = byte;
            rx_actual_crc = crc8_i(rx_actual_crc, byte);

            if (rx_payload_index >= rx_payload_size)
            {
                rx_state = RX_STATE_CRC;
            }
            break;

        case RX_STATE_CRC:
            rx_expected_crc = byte;

            // Verify CRC
            if (rx_actual_crc == rx_expected_crc)
            {
                debug_puts("VALID PACKET\n");

                debug_puts("SEQ=");
                debug_u8(rx_seq);

                debug_puts(" TYPE=");
                debug_u8(rx_type);

                debug_puts(" SIZE=");
                debug_u16(rx_payload_size);

                debug_puts("\n");

                // Valid packet
                rx_packet.type = (uart_packet_t)rx_type;
                rx_packet.value = 0;

                // Copy payload to value if it's a 16-bit value
                if (rx_payload_size >= 2)
                {
                    rx_packet.value = (rx_buffer[1] << 8) | rx_buffer[0];
                }
                else if (rx_payload_size == 1)
                {
                    rx_packet.value = rx_buffer[0];
                }

                rx_packet.size = rx_payload_size;
                rx_packet.seq = rx_seq;
                rx_packet_ready = true;
            }
            else
            {
                // CRC error
                rx_packet_error = true;
            }

            rx_state = RX_STATE_SYNC_0;
            break;
        }
    }

    // Return status
    if (rx_packet_ready)
    {
        return 1; // Packet ready
    }
    else if (rx_packet_error)
    {
        return -1; // Error
    }
    else
    {
        return 0; // No packet yet
    }
}

int uart_rx_get_packet(rx_packet_t *packet)
{
    if (rx_packet_ready && packet != NULL)
    {
        *packet = rx_packet;
        rx_packet_ready = false;
        return 1; // Packet available
    }
    return 0; // No packet available
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