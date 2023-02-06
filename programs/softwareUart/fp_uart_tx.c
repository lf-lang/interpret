#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_time.h>
#include "sdd_uart.h"

int main() {
    
    int core_id = read_csr(CSR_COREID);
    if (core_id == 0) {
        sdd_uart_config_t uart = {
            0, 57600,0,0
        };
        sdd_uart_tx_init(&uart);
        delay_for(50000);

        sdd_uart_tx_send(&uart, 0x55);
        delay_for(50000);
    }
}