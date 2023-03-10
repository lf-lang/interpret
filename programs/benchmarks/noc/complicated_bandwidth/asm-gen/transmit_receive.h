
#include <stdint.h>
void transmit(uint32_t receiver, uint32_t word);
uint32_t receive();
void transmit_arr(uint32_t receiver, uint32_t* a, uint32_t nwords);
void receive_arr(uint32_t* a, uint32_t nwords);
void do_test_nops();

#define N 500
