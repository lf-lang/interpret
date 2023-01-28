#include <flexpret_io.h>
#include <flexpret_time.h>
#include <ip_uart.h>

#include <stdbool.h>

int main() {

    int core_id = read_csr(CSR_COREID);
    if (core_id == 0) {
        gpo_set_0(1);
        gpo_clear_0(1);
        char rx;
        int res;
        ip_uart_config_t uart = {
            0, 57600,0,0
        };
        ip_uart_rx_init(&uart);
        while(1) {
            res = ip_uart_rx_receive(&uart, &rx);
            if (res == 0) {
                _fp_print(rx);
                // Display?
            } else {
                // Display something else
            }
        }
    }
}