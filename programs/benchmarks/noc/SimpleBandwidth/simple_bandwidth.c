#include <stdint.h>
#include <stdlib.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>

#define N 200

#define SENDER_CORE_ID 0
#define RECEIVER_CORE_ID 1

uint32_t to_transmit[N];
uint32_t to_receive[N];

static void fill_to_transmit() {
    uint32_t x = 1;
    for (int i = 0; i < N; i++) {
        x = (to_transmit[i] = x * 31);
    }
}

static void transmit(uint32_t receiver, uint32_t* a) {
    for (int i = 0; i < N; i++) {
        // _fp_print(a[i]);
        noc_send(receiver, a[i]);
    }
}

static void receive() {
    for (int i = 0; i < N; i++) {
        to_receive[i] = noc_receive();
        // _fp_print(to_receive[i]);
    }
}

static void check_transmitted_equals_received() {
    for (int i = 0; i < N; i++) {
        if (to_transmit[i] != to_receive[i]) {
            _fp_print(i);
            _fp_print(666);
        }
    }
}

static void do_send() {
    fill_to_transmit();
    uint32_t t0 = rdcycle();
    transmit(RECEIVER_CORE_ID, &to_transmit[0]);
    uint32_t t1 = rdcycle();
    receive();
    check_transmitted_equals_received();
    _fp_print(77777);
    _fp_print(t1 - t0);
}

static void do_receive() {
    uint32_t t0 = rdcycle();
    receive();
    uint32_t t1 = rdcycle();
    transmit(SENDER_CORE_ID, &to_receive[0]);
}

int main()
{
    int core_id = read_csr(CSR_COREID);
    if (core_id == SENDER_CORE_ID) do_send();
    if (core_id == RECEIVER_CORE_ID) do_receive();
    return 0;
}
