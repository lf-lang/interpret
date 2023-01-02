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
        default: _fp_print(66); //ERROR
    }
}

int main0() {
    asm volatile(
        "li t4, 0x80000000\n\t"
        "li t2, 42\n\t"
        "li t3, 43\n\t"
        "li a1, 44\n\t"
        "li t5, 45\n\t"
        "li t6, 46\n\t"
        "sw t2, 0(t4)\n\t"
        "sw t3, 0(t4)\n\t"
        "sw a1, 0(t4)\n\t"
        "sw t5, 0(t4)\n\t"
        "sw t6, 0(t4)\n\t"
        "sw t2, 0(t4)\n\t"
        "sw t3, 0(t4)\n\t"
        "sw a1, 0(t4)\n\t"
        "sw t5, 0(t4)\n\t"
        "sw t6, 0(t4)\n\t"
    );
}

int main1() {
    asm volatile(
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
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "li t4, 0x80000000\n\t"
        "lw t3, 0(t4)\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t3\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "li t4, 0x80000000\n\t"
        "lw t3, 0(t4)\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t3\n\t"
    );
}

int main2() {

}

int main3() {

}
