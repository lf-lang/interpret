#include "fp_uart.h"
#include <flexpret_io.h>

#include <stdbool.h>

#define BILLION 1000000000UL

void uart_rx_init(uart_config_t *uart) {
    // Calculate nsec per bit
    uart->_ns_per_bit = BILLION/uart->baud;

    // Create mask
    uart->_mask = (1 << uart->pin);
}

typedef enum {
    WAIT_FOR_START_BIT,
    RECEIVE,
    WAIT_FOR_STOP_BIT
} uart_state_t;

int uart_rx_receive(uart_config_t *uart, char *byte) {

    uart_state_t state = WAIT_FOR_START_BIT;
    uint32_t next_event;
    bool done=false;
    int return_code;
    char rx_tmp;

    while(!done) {

        switch(state) {
            case WAIT_FOR_START_BIT: {
                if ((gpi_read_0() & uart->_mask) == 0) {
                    state = RECEIVE;
                    next_event = rdtime() + uart->_ns_per_bit + uart->_ns_per_bit/2;
                }
                break;   
            }

            case RECEIVE: {
                delay_until(next_event);
                // Read byte
                for (int i = 0; i<8; i++) {
                    rx_tmp |= (gpi_read_0() & uart->_mask) << i;
                    next_event += uart->_ns_per_bit;
                }
                state = WAIT_FOR_STOP_BIT;
            }

            case WAIT_FOR_STOP_BIT {
                delay_until(next_event);
                if (gpi_read_0() & uart->_mask) {
                    *byte = rx_tmp;
                    return_code=0;
                } else {
                    return_code=1;
                }
                done=true;
                break;
            }
        }
    }
    return return_code;
}

int main() {
    char rx;
    int res;
    uart_config_t uart = {
        1, 115200,0,0
    };
    uart_rx_init(&uart);
    while(1) {
        res = uart_rx_receive(&uart, &rx);
        if (res == 0) {
            // Display?
        } else {
            // Display something else
        }
    }
}