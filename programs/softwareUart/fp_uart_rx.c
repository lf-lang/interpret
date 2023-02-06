#include <flexpret_io.h>
#include <flexpret_time.h>
#include <sdd_uart.h>

#include <stdbool.h>

int main() {

    int core_id = read_csr(CSR_COREID);
    if (core_id == 0) {
        gpo_set(0, 1);
        gpo_clear(0, 1);
        char rx;
        int res;
        sdd_uart_config_t uart = {
            0, 57600,0,0
        };
        sdd_uart_rx_init(&uart);
        while(1) {
            res = sdd_uart_rx_receive(&uart, &rx);
            if (res == 0) {
                _fp_print(rx);
                gpo_write(0,rx);
                // Display?
            } else {
                // Display something else
            }
        }
    }
}