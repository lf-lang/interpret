#include <stdio.h>
#include <stdlib.h>

#include "uartsim.h"

typedef enum {
    UART_RX_IDLE = 0,
    UART_RX,
    UART_RX_DONE
} uart_rx_state_t;

// Hard-coded to 100MHz clock and 115200
// FIXME: Calculate ticks_per_bit
int ticks_per_bit=868;


int tick_count=0;
int bit_count=0;
unsigned char rx=0;

uart_rx_state_t rx_state = UART_RX_IDLE;


// A simple UART receiver which prints out the received data to the terminal
void uartsim_print_rx(unsigned char* rxd) {
    if (rx_state == UART_RX_IDLE) {
        if (*rxd == 0) {
            rx_state = UART_RX;
            bit_count = 0;
            tick_count = ticks_per_bit + ticks_per_bit/2;
        }
    } else if (rx_state == UART_RX) {
        if (--tick_count == 0) {
            // Sample
            rx = rx | (*rxd << bit_count);
            
            bit_count++;
            tick_count = ticks_per_bit;

            if (bit_count == 8) {
                bit_count=0;
                printf("Received: %d\n", rx);
                rx_state = UART_RX_DONE;
                tick_count = ticks_per_bit + ticks_per_bit/2;
            }
        }
    } else if(rx_state == UART_RX_DONE) {
        if (--tick_count == 0) {
            rx_state = UART_RX_IDLE;
        }
    }

}