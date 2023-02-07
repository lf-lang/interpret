#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_uart.h>

int main() {
    int core_id = read_csr(CSR_COREID);

    uint8_t recv;
    while (1) {
        recv=uart_receive();
        gpo_write(0, recv);
    }
}