#ifndef UARTSIM_H
#define UARTSIM_H

void uartsim_init(const char * app_mem_file);

void uartsim_print_rx(unsigned char* rxd);

void uartsim_write(unsigned char* tx);

#endif