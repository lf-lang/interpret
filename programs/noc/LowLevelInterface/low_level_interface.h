#include "asm_utils.h"

#define LOAD_NOC_BASE_ADDRESS(reg) "li " #reg ", 0x80000000\n\t"

/**
 * @brief The instruction immediately following SYNC5 is able to store a word into the zeroth TDM
 * slot (northwest) in a single-threaded setting. Clobber the given registers.
 * It 9-13 cycles to synchronize. The 0-4 is fundamental and the remaining 9 cycles are overhead.
 */
#define SYNC5(nonce, noc_base_address, reg0, reg1, reg2, reg3)                                     \
    "li " #reg0 ", 1\n\t"                                                                          \
    "li " #reg1 ", 2\n\t"                                                                          \
    "li " #reg2 ", 3\n\t"                                                                          \
    "li " #noc_base_address ", 0x80000000\n\t"                                                     \
    "lw " #reg3 ", 32(" #noc_base_address ")\n\t"  /* Get elapsed cycles mod period */             \
    "beq " #reg3 ", " #reg0 ", DONE_SYNCHRONIZING" #nonce "\n\t"                                   \
    "beq " #reg3 ", x0, DONE_SYNCHRONIZING" #nonce "\n\t"                                          \
    "nop\n\t"                                                                                      \
    "beq " #reg3 ", " #reg2 ", DONE_SYNCHRONIZING" #nonce "\n\t"                                   \
    "beq " #reg3 ", " #reg1 ", DONE_SYNCHRONIZING" #nonce "\n\t"                                   \
    "DONE_SYNCHRONIZING" #nonce ":\n\t"

/**
 * @brief Broadcast the value stored in reg to all other cores. The instruction immediately
 * following BROADCAST_SYNCED is able to store into the zeroth TDM slot.
 * This assumes that the current thread is synchronized to the TDM schedule.
 */
#define BROADCAST_SYNCED_WITH_INSTRUCTIONS(nonce, noc_base_address, reg, instr0, instr1)           \
    "sw " #reg ", 0(" #noc_base_address ")\n\t"                                                    \
    instr0                                                                                         \
    "sw " #reg ", 0(" #noc_base_address ")\n\t"                                                    \
    "sw " #reg ", 0(" #noc_base_address ")\n\t"                                                    \
    instr1

#define BROADCAST_SYNCED(nonce, noc_base_address, reg)                                                               \
    BROADCAST_SYNCED_WITH_INSTRUCTIONS(nonce, noc_base_address, reg, "nop\n\t", "nop\n\t")

#define SEND_NORTHEAST_SYNCED_WITH_INSTRUCTIONS(nonce, noc_base_address, reg, instr0, instr1, instr2, instr3) \
    "sw " #reg ", 0(" #noc_base_address ")\n\t"                                                    \
    instr0                                                                                         \
    instr1                                                                                         \
    instr2                                                                                         \
    instr3

#define SEND_NORTHEAST_SYNCED(nonce, noc_base_address, reg) \
    SEND_NORTHEAST_SYNCED_WITH_INSTRUCTIONS(nonce, noc_base_address, reg, "nop\n\t", "nop\n\t", "nop\n\t", "nop\n\t")

/**
 * @brief Do a blocking read of the message from the core at sending_core_reg.
 * Set noc_base_address to the base address corresponding to sending_core_reg.
 * Preserve the value of sending_core_reg.
 * When run on a FlexPRET core with a single thread, this is guaranteed to take three cycles
 * (mod 5).
 */
#define BLOCKING_READ(nonce, noc_base_address, read_to_reg, sending_core_reg) \
    LOAD_NOC_BASE_ADDRESS(noc_base_address)                                                        \
    "slli " #read_to_reg ", " #sending_core_reg ", 2\n\t"                                          \
    "add " #noc_base_address ", " #read_to_reg ", " #noc_base_address "\n\t"                       \
    "POLL" #nonce ": lw " #read_to_reg ", 16(" #noc_base_address ")\n\t"                           \
    "beq x0, " #read_to_reg ", POLL" #nonce "\n\t"                                                 \
    "lw " #read_to_reg ", 0(" #noc_base_address ")\n\t"

/* Helper to SEND_N_WORDS. Accumulates valid bits. */
#define OR_VALIDITY_OF_NOC_DATA(noc_base_address_reg, accumulator_reg, offset_literal, clobber0) \
    "lw " #clobber0 ", " offset_literal "(" #noc_base_address_reg ")\n\t" \
    "or " #accumulator_reg ", " #accumulator_reg ", " #clobber0 "\n\t" \

#define READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce, noc_base_address_reg, read_to_reg, sending_core_reg, result_reg, tag_bit_mask) \
    BLOCKING_READ(nonce, clobber0, read_to_reg, x0) \
    "andi " #result_reg ", " #read_to_reg ", " #tag_bit_mask "\n\t" /* check tag bit */ \
    "bnez " #result_reg ", END" #nonce "\n\t" \

/**
 * @brief Try to send the number of words specified by n_words_reg.
 * @param n_words_reg The number of words to send.
 * @param result_reg 0 if successful, 1 if one of the potential message receivers has already tried
 * to send a message before this even tries to send a message, 0x80000000 if one of the potential
 * message receivers tries to send a message after this tries to send a message (so awkward! who
 * gets to talk first?)
 * @param load_words_asm Assembly code for preparing to send words, e.g. by loading the words into
 * the register file.
 * @param send_words_asm Assembly code for sending the words rapidly. Must never miss a TDM slot!
 * May assume that the preceding code has already taken care of synchronization. Must preserve
 * synchronization.
 * @param TIMES_TO_REPEAT_SEND_WORDS_ASM Macro that repeats send_words_asm an appropriate number of
 * times.
 * The remaining parameters (e.g., SENDING_NORTHEAST_MACRO) must be either TRUE_MACRO or
 * FALSE_MACRO.
 */
#define SEND_N_WORDS( \
    nonce, \
    n_words_reg,                                                                                   \
    result_reg,                                                                                    \
    SENDING_NORTHEAST_MACRO,                                                                       \
    SENDING_NORTH_MACRO,                                                                           \
    SENDING_EAST_MACRO,                                                                            \
    SENDING_TO_ZERO_MACRO,                                                                         \
    SENDING_TO_ONE_MACRO,                                                                          \
    SENDING_TO_TWO_MACRO,                                                                          \
    SENDING_TO_THREE_MACRO,                                                                        \
    load_words_asm,                                                                                \
    send_words_asm,                                                                                \
    TIMES_TO_REPEAT_SEND_WORDS_ASM,                                                                \
    clobber0, clobber1, clobber2, clobber3, clobber4)                                              \
    LOAD_NOC_BASE_ADDRESS(clobber0) \
    "add " #result_reg ", x0, x0\n\t"  \
    SENDING_TO_ZERO_MACRO(OR_VALIDITY_OF_NOC_DATA(clobber0, result_reg, 16, clobber1), "") \
    SENDING_TO_ONE_MACRO(OR_VALIDITY_OF_NOC_DATA(clobber0, result_reg, 20, clobber1), "") \
    SENDING_TO_TWO_MACRO(OR_VALIDITY_OF_NOC_DATA(clobber0, result_reg, 24, clobber1), "") \
    SENDING_TO_THREE_MACRO(OR_VALIDITY_OF_NOC_DATA(clobber0, result_reg, 28, clobber1), "") \
    "bnez " #result_reg ", END" #nonce "\n\t" /** Fail with error code 1 */ \
    SYNC5(nonce, clobber0, clobber1, clobber2, clobber3, clobber4) \
    "li " #clobber1 ", 0x80000000\n\t" /* Set the top bit as a tag bit. If u wanna send so many words that this creates amiguity, then u have a bigger problem on your hands */ \
    "ori " #n_words_reg ", " #n_words_reg ", " #clobber1 "\n\t" \
    SENDING_NORTH_MACRO("sw " #n_words_reg ", 0(" #clobber0 ")\n\t", "nop\n\t") \
    SENDING_EAST_MACRO("sw " #n_words_reg ", 0(" #clobber0 ")\n\t", "nop\n\t") \
    "li " #clobber3 ", 1\n\t" \
    SENDING_NORTHEAST_MACRO("sw " #n_words_reg ", 0(" #clobber0 ")\n\t", "nop\n\t") \
    SENDING_TO_ZERO_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce, clobber0, clobber2, x0, result_reg, clobber1), "") \
    SENDING_TO_ONE_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce, clobber0, clobber2, clobber3, result_reg, clobber1), "") \
    "li " #clobber3 ", 2\n\t" \
    SENDING_TO_TWO_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce, clobber0, clobber2, clobber3, result_reg, clobber1), "") \
    "li " #clobber3 ", 3\n\t" \
    SENDING_TO_THREE_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce, clobber0, clobber2, clobber3, result_reg, clobber1), "") \
    /* At this point, all prospective message receivers have agreed that they are ready to receive the given number of words by replying using responses that have zero as their tag bit. By my count the TDM slot of the next instruction will be -2 mod 5, but for now I won't use that fact, preferring instead to re-synchronize, just to make the assembly easier to write (less brittle, less performant). */ \
    #load_words_asm \
    SYNC5(nonce, clobber0, clobber1, clobber2, clobber3, clobber4) \
    #TIMES_TO_REPEAT_SEND_WORDS_ASM(send_words_asm) \
    "END" #nonce ":\n\t"
