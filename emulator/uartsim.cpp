#include <stdio.h>
#include <stdlib.h>

#include "uartsim.h"

typedef enum {
    UART_RX_IDLE = 0,
    UART_RX,
    UART_RX_DONE
} uart_rx_state_t;

typedef enum {
    UART_TX_START = 0,
    UART_TX,
    UART_TX_STOP,
    UART_TX_DONE
} uart_tx_state_t;

// Hard-coded to 100MHz clock and 115200
// FIXME: Calculate ticks_per_bit
int ticks_per_bit=868;


int rx_tick_count=0;
int rx_bit_count=0;
unsigned char rx=0;
uart_rx_state_t rx_state = UART_RX_IDLE;

int tx_tick_count=ticks_per_bit;
int tx_bit_count=0;
int tx_write_idx=0;
uart_tx_state_t tx_state = UART_TX_START;



unsigned char * write_data;
int write_len;
void uartsim_init() {
    // For now, just hardcoded. In the future. read from file
    write_data = (unsigned char *) malloc(sizeof(unsigned char) * 10);
    write_len = 10;
    for (int i=0; i<10; i++) {
        write_data[i] = i*2;
    }
}


// A simple UART receiver which prints out the received data to the terminal
void uartsim_print_rx(unsigned char* rxd) {
    if (rx_state == UART_RX_IDLE) {
        if (*rxd == 0) {
            rx_state = UART_RX;
            rx_bit_count = 0;
            rx_tick_count = ticks_per_bit + ticks_per_bit/2;
        }
    } else if (rx_state == UART_RX) {
        if (--rx_tick_count == 0) {
            // Sample
            rx = rx | (*rxd << rx_bit_count);
            
            rx_bit_count++;
            rx_tick_count = ticks_per_bit;

            if (rx_bit_count == 8) {
                rx_bit_count=0;
                printf("Received: %d\n", rx);
                rx_state = UART_RX_DONE;
                rx_tick_count = ticks_per_bit + ticks_per_bit/2;
            }
        }
    } else if(rx_state == UART_RX_DONE) {
        if (--rx_tick_count == 0) {
            rx_state = UART_RX_IDLE;
        }
    }
}

void uartsim_write(unsigned char* tx) {
    
    switch(tx_state) {
        case UART_TX_START:
        {
            
            *tx=0;
            if (--tx_tick_count == 0) {
                tx_state = UART_TX;
                tx_tick_count = ticks_per_bit;
                tx_bit_count = 0;
            }
            break;
        }
        case UART_TX:
        {
            *tx = (write_data[tx_write_idx] >> tx_bit_count) & 0x1;
            if (--tx_tick_count == 0) {
                tx_tick_count = ticks_per_bit;
                tx_bit_count++;

                if (tx_bit_count == 8) {
                    tx_state = UART_TX_STOP;
                }
            }
            break;
        }
        case UART_TX_STOP:
        {
            *tx = 1;
            if (--tx_tick_count == 0) {
                tx_tick_count=ticks_per_bit;
                tx_bit_count=0;

                tx_write_idx++;
                if (tx_write_idx == write_len) {
                    tx_state = UART_TX_DONE;
                } else {
                    tx_state = UART_TX_START;
                }
            }
        break;
        }

        case UART_TX_DONE:
        break;
    }
}