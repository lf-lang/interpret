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
    // if (core_id != 0) return;
    // main0(NORTH);
    // asm volatile("nop\n\t");
    // main0(NORTH);
    // asm volatile("nop\n\tnop\n\t");
    // main0(NORTH);
    // asm volatile("nop\n\tnop\n\tnop\n\t");
    // main0(NORTH);
    // asm volatile("nop\n\tnop\n\tnop\n\tnop\n\t");
    // main0(NORTH);
    switch(core_id) {
        case 0: main0(NORTH); break;
        case 1: main1(); break;
        case 2: main1(); break;
        case 3: main1(); break;
        // default: _fp_print(666); //ERROR
    }
}

int main0(uint32_t direction) {
    // Goal: write to slot
    asm volatile(
        "li t1, 42\n\t"
        SYNC5(main0, a0, a1, a2, t4, t2)
        BROADCAST_SYNCED(main0, t1)
    );
}

int main1() {
    asm volatile(
        "li t4, 0x80000000\n\t"
        "POLL: lw t3, 16(t4)\n\t"
        "beq x0, t3, POLL\n\t"
        "lw t2, 0(t4)\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t2\n\t"
    );
}

int main2() {

}

int main3() {

}
