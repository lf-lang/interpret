#ifndef INTERPRET_WB_H
#define INTERPRET_WB_H

#include <stdint.h>

void wb_write(uint32_t addr, uint32_t data);

uint32_t wb_read(uint32_t addr);

#endif