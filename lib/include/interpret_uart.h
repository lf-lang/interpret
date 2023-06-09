
#ifndef INTERPRET_UART_H
#define INTERPRET_UART_H

#include <stdint.h>
#include "interpret_wb.h"


// Blocking send word
void uart_send(uint8_t data);

// Blocking read word
uint8_t uart_receive();

#endif
