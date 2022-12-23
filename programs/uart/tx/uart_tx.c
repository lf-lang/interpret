#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_uart.h>

int main() {
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);
    if (core_id == 0) {
        uart_send(42);
    }
    while(1) {}
}

