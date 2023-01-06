#include <stdint.h>
#include <flexpret_io.h>

#define BILLION 1000000000UL

// FIXME: What is a more elegant way of storing meta-data (_ns_per_bit and _mask) "hidden" from the user?
typedef struct {
    int pin;
    int baud;
    int _ns_per_bit;
    uint32_t _mask;
} uart_tx_t;

void uart_tx_init(uart_tx_t *uart) {
    // Calculate nsec per bit
    uart->_ns_per_bit = BILLION/uart->baud;
    _fp_print(uart->_ns_per_bit);

    // Create mask
    uart->_mask = (1 << uart->pin);

    // Initialize pin to high
    gpo_set_0(uart->_mask);
}

void uart_tx_send(uart_tx_t *uart, char byte) {
    // Start bit
    unsigned int next_event = rdtime();
    gpo_clear_0(uart->_mask);
    next_event += uart->_ns_per_bit;
    // Write byte
    for (int i = 0; i<8; i++) {
        delay_until(next_event);
        if ((byte >> i) & 0x01) {
            gpo_set_0(uart->_mask);
        } else {
            gpo_clear_0(uart->_mask);
        }
        next_event += uart->_ns_per_bit;
    }
    // Stop bit
    gpo_set_0(uart->_mask);
}

int main() {
    _fp_print(0);
    uart_tx_t uart = {
        1, 115200,0,0
    };
    uart_tx_init(&uart);
    uart_tx_send(&uart, 0x55);
    _fp_print(1);
}