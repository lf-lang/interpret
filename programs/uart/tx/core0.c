#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_uart.h>

// Core0 -> Core2
int main() {
    _fp_print(40);
    uart_send(42);
    while(1) {}
}