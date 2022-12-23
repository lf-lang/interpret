#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_uart.h>

int main() {
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);
    uint8_t recv;
    for (int i = 0; i<10; i++) {
        recv=uart_receive();
        _fp_print(recv);
    }
}