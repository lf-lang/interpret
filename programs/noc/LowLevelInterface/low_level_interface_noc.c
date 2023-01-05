#include <stdint.h>
#include <flexpret_io.h>

#include "asm_utils.h"
#include "low_level_interface.h"

/* | NE | _ | N | E | _ | */

/***********************
 * core 0 / N \ core 1 *
 *        W + E        *
 * core 2 \ S / core 3 *
 ***********************/

int main0();
int main1();
int main2();
int main3();

#define REPEAT64(x) REPEAT4(REPEAT4(REPEAT4(x)))
#define BROADCAST_COUNT_SEND_ASM(nonce, n_words_reg, noc_base_address, clobber0) REPEAT64( \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "addi " #clobber0 ", " #n_words_reg ", -1\n\t"                                                 \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "blt " #clobber0 ", x0, END_BROADCAST_COUNT_FROM_CORE_ZERO\n\t"                                \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "addi " #n_words_reg ", " #clobber0 ", -1\n\t"                                                 \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "blt " #n_words_reg ", x0, END_BROADCAST_COUNT_FROM_CORE_ZERO\n\t"                             \
)
#define BROADCAST_COUNT_LOAD_ASM(n_words_reg) "andi " #n_words_reg ", " #n_words_reg ", 15\n\t"

/* n_words_reg assumed no greater than 15. The only part here that is specific to core 0 is where you listen for responses. */
#define BROADCAST_COUNT_FROM_CORE_ZERO(nonce, n_words_reg, result_reg, noc_base_address, clobber1, clobber2, clobber3, clobber4) \
    SEND_N_WORDS(                                                                                  \
        nonce,                                                                                     \
        n_words_reg,                                                                               \
        result_reg,                                                                                \
        TRUE_MACRO, TRUE_MACRO, TRUE_MACRO,                                                        \
        FALSE_MACRO, TRUE_MACRO, TRUE_MACRO, TRUE_MACRO,                                           \
        BROADCAST_COUNT_LOAD_ASM(n_words_reg),                                                     \
        BROADCAST_COUNT_SEND_ASM(nonce, n_words_reg, noc_base_address, clobber1),                  \
        noc_base_address, clobber1, clobber2, clobber3, clobber4                                   \
    )                                                                                              \
    "END_BROADCAST_COUNT_FROM_CORE_ZERO:"

#define RECEIVER_BODY(endlabel) REPEAT7( \
    "lw t2, 0(t4)\n\t" /* t4 has the base address for the noc */                                   \
    "rdcycle t0\n\t"                                                                               \
    "bge t0, t3, " #endlabel "\n\t"                                                                \
    "csrw 0x51e, a0\n\t"                                                                           \
    "lw t2, 0(t4)\n\t" /* t4 has the base address for the noc */                                   \
    "rdcycle t0\n\t"                                                                               \
    "bge t0, t3, " #endlabel "\n\t"                                                                \
    "csrw 0x51e, t2\n\t"                                                                           \
)

int main() {
    int core_id = read_csr(CSR_COREID);
    switch(core_id) {
        case 0: main0(); break;
        case 1: main1(); break;
        case 2: main2(); break;
        case 3: main3(); break;
        default: _fp_print(666); //ERROR
    }
}

int main0() {
    asm volatile(

        "li t0, 7\n\t"
        // Let t1 be result reg
        // Let t2 be noc_base_address
        BROADCAST_COUNT_FROM_CORE_ZERO(__LINE__, t0, t1, t2, t3, t4, t5, t6)
    );
}

int main1() {
    asm volatile(
        BLOCKING_READ(0, t4, t2, x0)
        "andi t2, t2, 1023\n\t"
        "li a0, 0xbaaabaaa\n\t"
        SYNC5(67, t4, a1, a2, a3, a4)
        EAST_QUINTET(
            "slli t3, t2, 2\n\t" "add t3, t3, t2\n\t", /* multiplying by 5 */
            "nop\n\t",
            "nop\n\t",
            t2,
            t4
        )
        BLOCK_ON_FLIT_FROM_CORE(__LINE__, t4, t2)
        "rdcycle t2\n\t"
        "add t3, t2, t3\n\t" // end cycle
        RECEIVER_BODY(END_MAIN1)
        "END_MAIN1:"
    );
}

int main2() {
    asm volatile(
        BLOCKING_READ(00, t4, t2, x0)
        "andi t2, t2, 1023\n\t"
        "li a0, 0xbaaabaaa\n\t"
        SYNC5(106, t4, a1, a2, a3, a4)
        "nop\n\t"
        "nop\n\t"
        "sw t2, 0(t4)\n\t"
        "nop\n\t"
        "nop\n\t"
        BLOCK_ON_FLIT_FROM_CORE(__LINE__, t4, t2)
        "nop\n\t"
        "nop\n\t"
        RECEIVER_BODY(END_MAIN2)
        "END_MAIN2:"
    );
}

int main3() {
    asm volatile(
        BLOCKING_READ(000, t4, t2, x0)
        "andi t2, t2, 1023\n\t"
        "li a0, 0xbaaabaaa\n\t"
        SYNC5(146, t4, a1, a2, a3, a4)
        "sw t2, 0(t4)\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        "nop\n\t"
        BLOCK_ON_FLIT_FROM_CORE(__LINE__, t4, t2)
        "nop\n\t"
        "nop\n\t"
        RECEIVER_BODY(END_MAIN3)
        "END_MAIN3:"
    );

}
