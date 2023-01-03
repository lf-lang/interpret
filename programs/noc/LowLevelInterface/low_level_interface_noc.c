#include <stdint.h>
#include <flexpret_io.h>

int main0();
int main1();
int main2();
int main3();

int main() {
    int core_id = read_csr(CSR_COREID);
    switch(core_id) {
        case 0: main0(); break;
        case 1: main1(); break;
        case 2: main1(); break;
        case 3: main1(); break;
        default: _fp_print(666); //ERROR
    }
}

int main0() {
    asm volatile(
        "li t4, 0x80000000\n\t"
        "rdcycle t2\n\t"
        "sw t2, 0(t4)\n\t"
        "sw t2, 0(t4)\n\t"
        "sw t2, 0(t4)\n\t"
        "sw t2, 0(t4)\n\t"
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
        "csrw 0x51e, t3\n\t"
    );
}

int main2() {

}

int main3() {

}
