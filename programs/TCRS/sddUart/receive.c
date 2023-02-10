
#include <flexpret.h>
#include <sdd_uart.h>
#include <flexpret_stdio.h>

#define RX_PIN 0
#define RX_BAUD 115200
#define RX_PORT 1

static sdd_uart_config_t uart ={.initialized = false};
void * t1(void * arg) {
    uart.pin = RX_PIN;
    uart.baud = RX_BAUD;
    uart.port = RX_PORT;
    uart.buf_size = 8;
    sdd_uart_rx_run(&uart);
}

void main(void) {
    if(read_coreid() == 0) {
        thread_t tid1 = 1;
        int errno = thread_map(true, &tid1, t1, 0);
        assert(errno == 0);
        while(!uart.initialized) {};
        char byte;
        while(true) {
            if (sdd_uart_rx_receive(&uart, &byte) == FP_SUCCESS) {
                print_int(byte);
            }
        }
    }
}