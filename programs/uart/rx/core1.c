#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_uart.h>

// Core1 -> Core3
int main() {
    _fp_print(40);
    uint8_t recv;
    for (int i = 0; i<10; i++) {
        recv=uart_receive();
        _fp_print(recv);
    }
}