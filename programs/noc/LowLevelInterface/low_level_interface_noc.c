#include <stdint.h>
#include <flexpret_io.h>

#include "low_level_interface.h"

#define NORTHEAST 12
#define NORTH 0
#define EAST 4
#define PERIOD 20

int main0();
int main1();
int main2();
int main3();

int main() {
    int core_id = read_csr(CSR_COREID);
    switch(core_id) {
        case 0: main0(NORTH); break;
        case 1: main1(); break;
        case 2: main1(); break;
        case 3: main1(); break;
        default: _fp_print(666); //ERROR
    }
}

int main0(uint32_t direction) {
    // Goal: write to slot
    asm volatile(
        "li t1, 42\n\t"
        LOAD_NOC_BASE_ADDRESS(t4)
        SYNC5(main0, t4, a0, a1, a2, t2)
        BROADCAST_SYNCED(main0, t4, t1)
    );
}

int main1() {
    asm volatile(
        BLOCKING_READ(__LINE__, t4, t2, x0)
        FP_PRINT_ASM(t2, a0)
    );
}

int main2() {

}

int main3() {

}
