#include "interpret.h"

int main() {
    int core_id = read_csr(CSR_COREID);
    _fp_print(core_id);
    if (core_id == 0) {
        uart_send(42);
    }
    delay_for(MSEC(2));
}

