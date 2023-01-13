#ifndef FP_UART_H
#define FP_UART_H
#include <stdint.h>

typedef struct {
    int pin;
    int baud;
    int _ns_per_bit;
    uint32_t _mask;
} uart_config_t;

void uart_tx_init(uart_config_t *uart);
void uart_tx_send(uart_config_t *uart, char byte);

void uart_rx_init(uart_config_t *uart);
void uart_rx_send(uart_config_t *uart, char byte);

#endif