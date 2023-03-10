#include <stdint.h>
#include <stdlib.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>
#include <flexpret_time.h>

#include "asm-gen/transmit_receive.h"
#include "align.h"

#define UNROLLNOPS(MACRO) { MACRO(0) } { MACRO(1) } { MACRO(2) } { MACRO(3) } { MACRO(4) } { MACRO(5) } { MACRO(6) } { MACRO(7) } { MACRO(8) } { MACRO(9) } // { MACRO(10) } { MACRO(11) } { MACRO(12) } { MACRO(13) } { MACRO(14) }

#define SENDER_CORE_ID 3
#define RECEIVER_CORE_ID 1

uint32_t to_transmit[N];
uint32_t to_receive[N];

static inline int sync() {
    asm volatile (WAIT_FOR_NEXT_ZERO_MOD_1024(__LINE__));
}

static void do_send() {
    // fill_to_transmit takes a long time, which ensures that the receiver is ready
    // when it comes time to take a measurement
    #define SENDBODY(N) \
        _fp_print(N); \
        sync(); \
        do_test_nops ## N(); \
        uint32_t t0 = rdcycle(); \
        transmit(RECEIVER_CORE_ID, t0);
    for (int i = 0; i < 10; i++) {
        UNROLLNOPS(SENDBODY)
    }
}

static void do_receive() {
    #define RECEIVEBODY(N) \
        for (int i = 0; i < 10; i++) { \
            _fp_print(N); \
            sync(); \
            do_test_nops ## N(); \
            uint32_t t0 = receive(); \
            uint32_t t1 = rdcycle(); \
            _fp_print(t1 - t0); \
        }
    UNROLLNOPS(RECEIVEBODY)
}

int main()
{
    int core_id = read_csr(CSR_COREID);
    if (core_id == SENDER_CORE_ID) do_send();
    if (core_id == RECEIVER_CORE_ID) do_receive();
    return 0;
}
