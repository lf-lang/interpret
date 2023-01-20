/**
 * This program explores the absolute minimum amount of time that it can take to send one word and
 * write it into a register on another core, when under the most favorable circumstances,
 * and when controlling relative timing and optimizing the assembly.
 */
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
    if (coreid == 0) for (int i = 0; i < 10; i++) send_main(1);
    if (coreid == 1) for (int i = 0; i < 10; i++) receive_main(0);
}

static int send_main(uint32_t receiver) {
    asm volatile (
        "li t4, 0x40000000\n\t"
        WAIT_FOR_NEXT_ZERO_MOD_1024(send)  // clobber "a" registers, as well as t0, t1, t6
        // like noc_send, but without blocking
        "li t5, 0x1\n\t"  // noc destination
        "sw t5, 8(t4)\n\t"
        "li t5, 0x08\n\t"
        "sw t5, 4(t4)\n\t"
        "nop\n\t"
        "nop\n\t"
        "li t5, 42\n\t"  // Set noc data to 42
        "sw t5, 8(t4)\n\t" // NOTE: Data must be written first. This is by design.
        "li t5, 0x04\n\t"
        "sw t5, 4(t4)\n\t"
    );
}

static int receive_main(uint32_t sender) {
    asm volatile (
        WAIT_FOR_NEXT_ZERO_MOD_1024(receive)
        // "nop\n\t"  // The 9-cycle read loop is aligned optimally when the number of nops here is zero mod 9
        // "nop\n\t"
        // "nop\n\t"
        // "nop\n\t"
        // "nop\n\t"
        // "nop\n\t"
        // "nop\n\t"
        // "nop\n\t"
        // "nop\n\t"
        "li t4, 0x40000000\n\t"  // wishbone base address
        // FIXME: Why does this loop have to go through one iteration extra the first time around, compared to the number of iterations that it makes thereafter?
        "CHECK_IF_RECEIVED_YET:\n\t"
        // Sadly, this whole sequence -- store, wait, read, mask, beq -- must be in the loop. In particular, if the store is factored out, the read doesn't work, even though we are storing the same thing each time.
        "sw x0, 0(t4)\n\t"  // Write the address of NoC CSR to Wishbone read address
        "nop\n\t"
        "nop\n\t"
        "lw t5, 12(t4)\n\t"  // Read NoC CSR
        "andi t5, t5, 2\n\t"
        "beq x0, t5, CHECK_IF_RECEIVED_YET\n\t"
        "li t5, 4\n\t"  // Write the address of NoC data to Wishbone read address
        "sw t5, 0(t4)\n\t"
        "nop\n\t"
        "nop\n\t"
        "lw t5, 12(t4)\n\t"  // Read NoC data
        "rdcycle t3\n\t"
        "andi t3, t3, 1023\n\t"
        "li t0, 0xbaaabaaa\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t3\n\t"
        "csrw 0x51e, t0\n\t"
        "csrw 0x51e, t5\n\t"
    );
}
