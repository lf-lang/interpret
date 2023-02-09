#include <flexpret_stdio.h>
#include <flexpret_io.h>
#include <flexpret_lock.h>
#include <flexpret_thread.h>
#include <flexpret_printer.h>

#define UART_PIN 0
#define UART_BAUD 270000
#define UART_PORT 1


void* t1(void * arg) {
    fp_printer_config_t cfg = {
        .port=UART_PORT, 
        .pin=UART_PIN,
        .baudrate=UART_BAUD
    };
    fp_printer_run(&cfg);
}

void main(void) {
    if(read_coreid() == 0) {
        thread_t tid = 1;

        int errno = thread_map(true, &tid, t1, 0);
        assert(errno == 0);

        fp_printer_str("Hello World!\n");
    }
}