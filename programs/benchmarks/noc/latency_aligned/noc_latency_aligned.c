
#include <stdint.h>
#include <flexpret_io.h>
#include <flexpret_noc.h>
#include <stdlib.h>

#include "align.h"

#define N 100

static int main_of(uint32_t core);

static int send_main(uint32_t receiver);
static int receive_main(uint32_t sender);

int main() {
    unsigned long coreid = read_csr(CSR_COREID);
    srand(coreid);
    if (coreid == 0) send_main(1);
    if (coreid == 1) receive_main(0);
}

static int send_main(uint32_t receiver) {
    asm volatile (
        // "li t1, 0x40000004\n\t"  // wishbone address mmio address
        // "li t2, 0x40000008\n\t"  // wishbone data mmio address
        // "li t3, 0x08\n\t"  // noc destination wishbone address
        // "li t4, 0x04\n\t"  // noc data wishbone address
        "li t4, 0x40000000\n\t"
        // Set noc destination to 1
        "li t5, 0x08\n\t"  // noc destination wishbone address
        "sw t5, 4(t4)\n\t"
        "li t5, 0x1\n\t"  // noc destination
        "sw t5, 8(t4)\n\t"
        // Check that the wishbone has accepted the write
        "nop\n\t"
        "nop\n\t"
        "lw t5, 16(t4)\n\t"
        "li t4, 0xbaaabaaa\n\t"
        "csrw 0x51e, t4\n\t"
        "csrw 0x51e, t5\n\t"  // should be nonzero
        "li t4, 0x40000000\n\t"
        WAIT_FOR_NEXT_ZERO_MOD_1024(send)  // clobber "a" registers, as well as t0, t1, t6
        // like noc_send, but without blocking
        "li t5, 0x04\n\t"  // Set noc data to 42
        "sw t5, 4(t4)\n\t"
        "li t5, 42\n\t"
        "sw t5, 8(t4)\n\t"
        "li t5, 0x08\n\t"  // noc destination
        "sw t5, 4(t4)\n\t"
        "li t5, 0x1\n\t"
        "sw t5, 8(t4)\n\t"
        "nop\n\t"
        "nop\n\t"
        // Is it necessary to wait here to make sure the destination was passed down to the NoC?
        // "li t5, 42\n\t"  // noc value
        // "sw t5, 4(x0)\n\t"  // noc data
        // "rdcycle t1\n\t"
        // "andi t1, t1, 1023\n\t"
        // "li t0, 0xbaaabaaa\n\t"
        // "csrw 0x51e, t0\n\t"
        // "csrw 0x51e, t1\n\t"
    );
}

static int receive_main(uint32_t sender) {
    asm volatile (
        WAIT_FOR_NEXT_ZERO_MOD_1024(receive)
        "li t4, 0x40000000\n\t"  // wishbone base address
        "CHECK_IF_RECEIVED_YET:\n\t"
        "sw x0, 0(t4)\n\t"  // Write the address of NoC CSR to Wishbone read address
        "lw t5, 12(t4)\n\t"  // Read NoC CSR
        "beq x0, t5, CHECK_IF_RECEIVED_YET\n\t"
        "rdcycle t3\n\t"
        "andi t3, t3, 1023\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t3\n\t"
    );
}
