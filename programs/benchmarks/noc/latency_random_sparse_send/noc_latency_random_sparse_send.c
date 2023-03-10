/**
 * This benchmark aims to capture the full range of variability in the latency of sending a single
 * word. It does this by randomizing the time at which the word is sent so that at the moment of
 * message arrival, the position of the receiver in its busy-wait loop should be variable.
 *
 * It also uses a large minimum spacing between sending messages to ensure that the receiver is
 * completely finished handling the previous message before the next message arrives.
 */

#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>
#include <stdlib.h>

#define N 1000
// 1 << LOG2_OF_A_LONG_TIME should be much greater than the number of cycles required to run
// one iteration of the benchmark. I think it takes less than 512 cycles to run one iteration
// of the benchmark.
#define LOG2_OF_A_LONG_TIME 8

static int main_of(uint32_t core);

int main() {
    unsigned long coreid = read_csr(CSR_COREID);
    srand(coreid);
    main_of(coreid);
}

static int send_main(uint32_t receiver) {
    for (uint32_t i = 0; i < N; i++) {
        uint32_t min_delay = 1 << LOG2_OF_A_LONG_TIME;
        uint32_t additional_delay = rand() & ((1 << LOG2_OF_A_LONG_TIME) - 1);
        unsigned long end_time = rdcycle() + min_delay + additional_delay;
        while (rdcycle() < end_time) {}
        unsigned long t0 = rdcycle(); // benchmark start
        noc_send(receiver, t0, TIMEOUT_FOREVER);
    }
}

static int receive_main(uint32_t sender) {
    for (uint32_t i = 0; i < N; i++) {
        uint32_t t0;
        noc_receive(&t0, TIMEOUT_FOREVER);
        uint32_t t1 = rdcycle(); // benchmark end
        _fp_print((sender + 1) * 1000000 + t1 - t0);
    }
}

static int send_receive(uint32_t partner, int first) {
    first ? send_main(partner) : receive_main(partner);
    !first ? send_main(partner) : receive_main(partner);
}

static int main_of(uint32_t core) {
    int big = core & 2;
    int odd  = core & 1;
    send_receive((core + 1) & 3, !odd);
    send_receive((core + 2) & 3, !big);
    send_receive((core + 3) & 3, !odd);
}
