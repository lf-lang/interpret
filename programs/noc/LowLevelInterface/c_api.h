#ifndef C_API_H
#define C_API_H

#include <flexpret_io.h>

#include "low_level_interface.h"

void we_are_bedeviled() {
    _fp_print(666);
}

/** code related to READ_N_WORDS_AND_PRINT ********************************************************/

#define LOAD_AND_PRINT_RECEIVER_BODY(noc_base_address, clobber0, clobber1) REPEAT64(               \
    "lw " #clobber0 ", 0(" #noc_base_address ")\n\t" /* t4 has the base address for the noc */     \
    "nop\n\t"                                                                                      \
    "csrw 0x51e, " #clobber1 "\n\t"                                                                \
    "csrw 0x51e, " #clobber0 "\n\t"                                                                \
) "nop\n\t" /* Do not let the code self-modification kill a line that we actually need in the special case that the packet length is exactly 64. */

/**
 * @brief Block and print up to 64 words that were sent by another core using the protocol of
 * SEND_N_WORDS.
 * @param DIRECTION_QUINTET_MACRO The macro corresponding to the direction of the sender from the
 * receiver.
 * @param sender_reg A register containing the core ID of the sender.
 * The remaining registers are all clobbers.
 */
#define READ_N_WORDS_AND_PRINT(nonce, DIRECTION_QUINTET_MACRO, sender_reg, clobber0, clobber1, clobber2, clobber3, clobber4, clobber5) \
    READ_N_WORDS(nonce, DIRECTION_QUINTET_MACRO, MUL4, 48, 438, sender_reg, LOAD_AND_PRINT_RECEIVER_BODY(clobber0, clobber4, clobber5), clobber0, clobber1, clobber2, clobber3, clobber4)

#define read_n_words_and_print_HELPER(DIRECTION_QUINTET_MACRO)                                     \
    asm volatile(                                                                                  \
        "li a4, 0xbaaabaaa\n\t"                                                                    \
        READ_N_WORDS_AND_PRINT(__LINE__, DIRECTION_QUINTET_MACRO, %[sender_reg], t4, t5, a5, a2, a3, a4) \
        : /* no outputs */                                                                         \
        : [sender_reg] "r" (sending_core)                                                          \
        : "t4", "t5", "a5", "a2", "a3", "a4"                                                       \
    )

/** @brief See READ_N_WORDS_AND_PRINT for documentation. */
void read_n_words_and_print(int sending_core, int direction) {
    switch (direction) {
        case NORTHEAST_INT: read_n_words_and_print_HELPER(NORTHEAST_QUINTET); break;
        case NORTH_INT: read_n_words_and_print_HELPER(NORTH_QUINTET); break;
        case EAST_INT: read_n_words_and_print_HELPER(EAST_QUINTET); break;
        default: we_are_bedeviled();
    }
}

/** code related to BROADCAST_COUNT ***************************************************************/

#define BROADCAST_COUNT_SEND_ASM(nonce, n_words_reg, noc_base_address, clobber0) REPEAT64( \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "addi " #clobber0 ", " #n_words_reg ", -1\n\t"                                                 \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "sw " #n_words_reg ", 0(" #noc_base_address ")\n\t"                                            \
    "bge x0, " #clobber0 ", END_BROADCAST_COUNT_SEND_ASM" #nonce "\n\t"                                     \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "addi " #n_words_reg ", " #clobber0 ", -1\n\t"                                                 \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "sw " #clobber0 ", 0(" #noc_base_address ")\n\t"                                               \
    "bge x0, " #n_words_reg ", END_BROADCAST_COUNT_SEND_ASM" #nonce "\n\t"                                  \
) "END_BROADCAST_COUNT_SEND_ASM" #nonce ": nop\n\t" "nop\n\t" "nop\n\t"
#define BROADCAST_COUNT_LOAD_ASM(n_words_reg) "andi " #n_words_reg ", " #n_words_reg ", 1023\n\t"

/**
 * @brief Broadcast a countdown from n_words_reg to 1 to all other cores.
 * @param SENDING_TO_ZERO_MACRO, ..., SENDING_TO_THREE_MACRO: All of these should be TRUE_MACRO
 * except the one corresponding to the current core (which does not broadcast to itself).
 * @param n_words_reg Input: The number of words to send; also, the first number in the countdown.
 * Must be less than 128! May need to be even smaller depending on capability of receiver.
 * @param result_reg Output: Indicates whether operation succeeded. See SEND_N_WORDS for details.
 * @param noc_base_address Clobber.
 */
#define BROADCAST_COUNT(                                                                           \
    nonce,                                                                                         \
    SENDING_TO_ZERO_MACRO,                                                                         \
    SENDING_TO_ONE_MACRO,                                                                          \
    SENDING_TO_TWO_MACRO,                                                                          \
    SENDING_TO_THREE_MACRO,                                                                        \
    n_words_reg,                                                                                   \
    result_reg,                                                                                    \
    noc_base_address,                                                                              \
    clobber1,                                                                                      \
    clobber2,                                                                                      \
    clobber3,                                                                                      \
    clobber4                                                                                       \
)                                                                                                  \
    SEND_N_WORDS(                                                                                  \
        nonce,                                                                                     \
        n_words_reg,                                                                               \
        result_reg,                                                                                \
        TRUE_MACRO, TRUE_MACRO, TRUE_MACRO,                                                        \
        SENDING_TO_ZERO_MACRO, SENDING_TO_ONE_MACRO, SENDING_TO_TWO_MACRO, SENDING_TO_THREE_MACRO, \
        BROADCAST_COUNT_LOAD_ASM(n_words_reg),                                                     \
        BROADCAST_COUNT_SEND_ASM(nonce, n_words_reg, noc_base_address, clobber1),                  \
        noc_base_address, clobber1, clobber2, clobber3, clobber4                                   \
    )

#define broadcast_count_HELPER(                                                                    \
    nonce,                                                                                         \
    SENDING_TO_ZERO_MACRO,                                                                         \
    SENDING_TO_ONE_MACRO,                                                                          \
    SENDING_TO_TWO_MACRO,                                                                          \
    SENDING_TO_THREE_MACRO                                                                         \
)                                                                                                  \
    asm volatile(                                                                                  \
        BROADCAST_COUNT(                                                                           \
            nonce,                                                                                 \
            SENDING_TO_ZERO_MACRO,                                                                 \
            SENDING_TO_ONE_MACRO,                                                                  \
            SENDING_TO_TWO_MACRO,                                                                  \
            SENDING_TO_THREE_MACRO,                                                                \
            %[n_words_reg],                                                                        \
            t1, t2, t3, t4, t5, t6                                                                 \
        )                                                                                          \
        : /* no outputs */                                                                         \
        : [n_words_reg] "r" (start_count_at)                                                       \
        : "t1", "t2", "t3", "t4", "t5", "t6"                                                       \
    )

/** @brief See BROADCAST_COUNT for documentation. start_count_at corresponds to n_words_reg. */
void broadcast_count(int current_core, int start_count_at) {
    switch(current_core) {
        case 0: broadcast_count_HELPER(__LINE__, FALSE_MACRO, TRUE_MACRO, TRUE_MACRO, TRUE_MACRO); break;
        case 1: broadcast_count_HELPER(__LINE__, TRUE_MACRO, FALSE_MACRO, TRUE_MACRO, TRUE_MACRO); break;
        case 2: broadcast_count_HELPER(__LINE__, TRUE_MACRO, TRUE_MACRO, FALSE_MACRO, TRUE_MACRO); break;
        case 3: broadcast_count_HELPER(__LINE__, TRUE_MACRO, TRUE_MACRO, TRUE_MACRO, FALSE_MACRO); break;
        default: we_are_bedeviled();
    }
}

#endif // C_API_H
