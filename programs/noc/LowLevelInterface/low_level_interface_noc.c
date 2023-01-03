#include <stdint.h>
#include <flexpret_io.h>

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
        // case 1: main1(); break;
        // case 2: main1(); break;
        case 3: main1(); break;
        // default: _fp_print(666); //ERROR
    }
}

int main0(uint32_t direction) {
    // Goal: write to slot
    asm volatile(
        "li t4, 0x80000000\n\t"
        "li t5, 0x007ea023\n\t"  // sw t2, 0(t4)
        "lw t2, 32(t4)\n\t"  // Get elapsed cycles mod period
        "slli t2, t2, 2\n\t" // log2 instruction byte width
        "addi t2, t2, 12\n\t" //  = NORTHEAST
        "addi t3, t2, -20\n\t"  // -20 = -4 times period
        "blt t3, x0, DONE_MODDING_BY_PERIOD\n\t"
        "add t2, t3, x0\n\t"
        "nop\n\t" // Ensure 3 cycles are required regardless of whether branch is taken
        "DONE_MODDING_BY_PERIOD:"
        // "li t2, 42\n\t"
        "auipc t1, 0\n\t"
        "add t1, t1, t2\n\t" // PC + offset
        "sw t5, 28(t1)\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t2\n\t"
    );
}

int main1() {
    asm volatile(
        "li t4, 0x80000000\n\t"
        "POLL: lw t3, 16(t4)\n\t"
        "beq x0, t3, POLL\n\t"
        "rdcycle t3\n\t"
        "lw t2, 0(t4)\n\t"
        "sub t3, t3, t2\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t2\n\t"
    );
}

int main2() {

}

int main3() {

}
