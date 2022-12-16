#ifndef UARTSIM_H
#define UARTSIM_H

void uartsim_init(int freq, int baud);

void uartsim_print_rx(unsigned char* rxd);

#endif