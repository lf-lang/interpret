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

#define BROADCAST_COUNT_SEND_ASM(nonce, n_words_reg, noc_base_address, clobber0) REPEAT64( \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "addi " #clobber0 ", " #n_words_reg ", -1\n\t"                                                 \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "bge x0, " #clobber0 ", END_BROADCAST_COUNT_FROM_CORE_ZERO\n\t"                                \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "addi " #n_words_reg ", " #clobber0 ", -1\n\t"                                                 \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "bge x0, " #n_words_reg ", END_BROADCAST_COUNT_FROM_CORE_ZERO\n\t"                             \
)
#define BROADCAST_COUNT_LOAD_ASM(n_words_reg) "andi " #n_words_reg ", " #n_words_reg ", 1023\n\t"

/* n_words_reg assumed no greater than 1023. The only part here that is specific to core 0 is where you listen for responses. */
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
        "li t0, 64\n\t"
        // Let t1 be result reg
        // Let t2 be noc_base_address
        BROADCAST_COUNT_FROM_CORE_ZERO(__LINE__, t0, t1, t2, t3, t4, t5, t6)
    );
}

int main1() {
    read_n_words_and_print(0, EAST_INT);
}

int main2() {
    read_n_words_and_print(0, NORTH_INT);
}

int main3() {
    read_n_words_and_print(0, NORTHEAST_INT);
}
