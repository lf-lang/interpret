
#include <stdint.h>
void transmit(uint32_t receiver, uint32_t word);
uint32_t receive();
void transmit_arr(uint32_t receiver, uint32_t* a, uint32_t nwords);
void receive_arr(uint32_t* a, uint32_t nwords);
void do_test_nops0();
void do_test_nops1();
void do_test_nops2();
void do_test_nops3();
void do_test_nops4();
void do_test_nops5();
void do_test_nops6();
void do_test_nops7();
void do_test_nops8();
void do_test_nops9();

#define N 100
