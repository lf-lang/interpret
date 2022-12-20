#include <stdio.h>
#include <fstream>
#include <stdlib.h>

#include "uartsim.h"


using namespace std;

#define DBG
#ifdef DBG
#define DBG_PRINT(x) do {printf(x); printf("\n");} while(false)
#else
#define DBG_PRINT(x) do {} while(false)
#endif

typedef enum {
    UART_RX_IDLE = 0,
    UART_RX,
    UART_RX_DONE
} uart_rx_state_t;

typedef enum {
    UART_TX_START = 0,
    UART_TX,
    UART_TX_STOP,
    UART_TX_DONE,
    UART_TX_GET_NEW_BYTE
} uart_tx_state_t;

// Hard-coded to 100MHz clock and 115200
// FIXME: Calculate ticks_per_bit
int ticks_per_bit=868;


int rx_tick_count=0;
int rx_bit_count=0;
unsigned char rx=0;
uart_rx_state_t rx_state;

int tx_tick_count=ticks_per_bit;
int tx_bit_count=0;
int tx_write_idx=0;
uart_tx_state_t tx_state;





fstream app_file;   
void uartsim_init(const char * app_mem_file) {

    rx_state = UART_RX_IDLE;
    // For now, just hardcoded. In the future. read from file
    if (app_mem_file != "") {
        DBG_PRINT("Opening file");
        app_file.open(app_mem_file, ios::in | ios::binary);
        tx_state = UART_TX_GET_NEW_BYTE;
    } else {
        tx_state = UART_TX_DONE;

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

char tx_byte;
void uartsim_write(unsigned char* tx) {    
    switch(tx_state) {
        case UART_TX_GET_NEW_BYTE: {
            if (!app_file.eof()) {
                app_file.read(&tx_byte,1);
                tx_state = UART_TX_START;
                printf("Writing %u\n", (unsigned char) tx_byte);
            } else {
                tx_state = UART_TX_DONE;
            }

        }

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
            *tx = (tx_byte >> tx_bit_count) & 0x1;
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
                tx_state = UART_TX_GET_NEW_BYTE;
            }
        break;
        }

        case UART_TX_DONE:{
            // Keep line high when done/inactive
            *tx=1;
        }
        break;
    }
}
