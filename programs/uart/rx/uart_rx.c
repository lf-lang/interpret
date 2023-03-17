#include "interpret.h"


int main() {
    int core_id = read_csr(CSR_COREID);

    uint8_t recv;
    while (1) {
        recv=uart_receive();
        print_int(recv);
    }
}