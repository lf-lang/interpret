#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>
#include <stdlib.h>

int main() {
    unsigned long coreid = read_csr(CSR_COREID);
    srand(coreid);
    switch (coreid) {
        case 0: main0(); break;
        case 1: main1(); break;
        case 2: main0(); break;
    }
}

int main0() {
    for (int i = 0; i < 10; i++){
        _fp_print(27);
        noc_send(1, 42 + i);
    }
    _fp_print(rdcycle());
}

int main1() {
    while(rdcycle() < 25053) {}
    for (int i = 0; i < 10; i++) {
        _fp_print(noc_receive());
    }
}
