#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>

#define N 3

static int main_of(uint32_t core);

int main() {
    main_of(read_csr(CSR_COREID));
}

static int send_main(uint32_t receiver) {
    _fp_print(111111111);
    noc_send(receiver, 0);  // sync for reproducibility
    int cycles_of_latency = 0;
    for (uint32_t i = 0; i < N; i++) {
        unsigned long t0 = rdcycle(); // benchmark start
        noc_send(receiver, i);
        uint32_t reply = noc_receive();
        unsigned long t1 = rdcycle(); // benchmark end
        _fp_print(receiver * 1000000 + t1 - t0);
        cycles_of_latency = t1 - t0;
    }
    for (uint32_t i = 0; i < N; i++) {
        unsigned long t0 = rdcycle(); // benchmark start
        noc_send(receiver, t0);
    }
}

static int receive_main(uint32_t sender) {
    noc_receive();  // sync for reproducibility
    for (uint32_t i = 0; i < N; i++) {
        noc_send(sender, noc_receive());
    }
    for (uint32_t i = 0; i < N; i++) {
        uint32_t t0 = noc_receive();
        uint32_t t1 = rdcycle(); // benchmark end
        _fp_print(sender * 1000000 + t1 - t0);
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
