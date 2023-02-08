#include <stdint.h>
#include <stdlib.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>

#include "asm-gen/transmit_receive.h"

#define N 200

#define SENDER_CORE_ID 0
#define RECEIVER_CORE_ID 1

uint32_t to_transmit[N];
uint32_t to_receive[N];

static void fill_to_transmit() {
    uint32_t x = 1;
    for (int i = 0; i < N; i++) {
        to_transmit[i] = i;
        // x = (to_transmit[i] = x * 31);
    }
}

static void print_to_receive() {
    for (int i = 0; i < N; i++) {
        _fp_print(to_receive[i]);
    }
}

static void do_send() {
    fill_to_transmit();
    int t0 = rdcycle();
    transmit_arr(RECEIVER_CORE_ID, &to_transmit[0], N);
    int t1 = rdcycle();
    _fp_print(t1 - t0);
}

static void do_receive() {
    for (int i = 0; i < N; i++) to_receive[i] = -1;
    receive_arr(&to_receive[0], N);
    _fp_print(1111111);
    print_to_receive();
}

int main()
{
    int core_id = read_csr(CSR_COREID);
    if (core_id == SENDER_CORE_ID) do_send();
    if (core_id == RECEIVER_CORE_ID) do_receive();
    return 0;
}
