#include "asm_utils.h"

#define NORTHEAST_QUINTET(asm2cycles, asm1cycle0, asm1cycle1, to_send_reg, noc_base_address_reg) \
    "sw " #to_send_reg ", 0(" #noc_base_address_reg ")\n\t" \
    asm2cycles \
    asm1cycle0 \
    asm1cycle1

#define NORTH_QUINTET(asm2cycles, asm1cycle0, asm1cycle1, to_send_reg, noc_base_address_reg) \
    asm2cycles \
    "sw " #to_send_reg ", 0(" #noc_base_address_reg ")\n\t" \
    asm1cycle0 \
    asm1cycle1

#define EAST_QUINTET(asm2cycles, asm1cycle0, asm1cycle1, to_send_reg, noc_base_address_reg) \
    asm2cycles \
    asm1cycle0 \
    "sw " #to_send_reg ", 0(" #noc_base_address_reg ")\n\t" \
    asm1cycle1

#define LOAD_NOC_BASE_ADDRESS(reg) "li " #reg ", 0x80000000\n\t"

/**
 * @brief The same as SYNC5, except with the opportunity to pack one extra single-cycle instruction
 * in.
 * @param instr An instruction (presumably unrelated to the behavior of SYNC5) that does not clobber
 * clobber1, clobber2, or clobber3, but that is permitted to clobber clobber0. This instruction must
 * execute in exactly one cycle.
 */
#define SYNC5_PACKED(nonce, noc_base_address, clobber0, clobber1, clobber2, clobber3, extra_instr) \
    "li " #clobber0 ", 1\n\t"                                                                      \
    "li " #clobber1 ", 2\n\t"                                                                      \
    "li " #clobber2 ", 3\n\t"                                                                      \
    "li " #noc_base_address ", 0x80000000\n\t"                                                     \
    "lw " #clobber3 ", 32(" #noc_base_address ")\n\t"  /* Get elapsed cycles mod period */         \
    "beq " #clobber3 ", " #clobber0 ", DONE_SYNCHRONIZING" #nonce "\n\t"                           \
    "beq " #clobber3 ", x0, DONE_SYNCHRONIZING" #nonce "\n\t"                                      \
    extra_instr                                                                                    \
    "beq " #clobber3 ", " #clobber2 ", DONE_SYNCHRONIZING" #nonce "\n\t"                           \
    "beq " #clobber3 ", " #clobber1 ", DONE_SYNCHRONIZING" #nonce "\n\t"                           \
    "DONE_SYNCHRONIZING" #nonce ":\n\t"

/**
 * @brief The instruction immediately following SYNC5 is able to store a word into the zeroth TDM
 * slot (northwest) in a single-threaded setting. Clobber the given registers.
 * It 9-13 cycles to synchronize. The 0-4 is fundamental and the remaining 9 cycles are overhead.
 */
#define SYNC5(nonce, noc_base_address, clobber0, clobber1, clobber2, clobber3)                     \
    SYNC5_PACKED(nonce, noc_base_address, clobber0, clobber1, clobber2, clobber3, "nop\n\t")

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
 * @brief Load the NoC base address corresponding to the value of sending_core_reg into
 * noc_core_base_address.
 * @param noc_core_base_address Output: The address of data sent from the given core.
 * @param sending_core_reg Input (preserved): The number of the sending core. This register is not
 * clobbered so that the optimization of passing in x0 for it can be used.
 */
#define LOAD_NOC_CORE_BASE_ADDRESS(noc_core_base_address, sending_core_reg, clobber0) \
    LOAD_NOC_BASE_ADDRESS(noc_core_base_address)                                                        \
    "slli " #clobber0 ", " #sending_core_reg ", 2\n\t"                                             \
    "add " #noc_core_base_address ", " #clobber0 ", " #noc_core_base_address "\n\t"

#define BLOCK_ON_FLIT_FROM_CORE(nonce, noc_core_base_address, clobber0) BLOCK_ON_FLIT_FROM_CORE__(nonce, noc_core_base_address, clobber0) // This indirection is necessary for the preprocessor to expand the macro arg __LINE__. :eye_roll:
#define BLOCK_ON_FLIT_FROM_CORE__(nonce, noc_core_base_address, clobber0) \
    "BLOCKING_READ_POLL" #nonce ": lw " #clobber0 ", 16(" #noc_core_base_address ")\n\t"           \
    "beq x0, " #clobber0 ", BLOCKING_READ_POLL" #nonce "\n\t"                                      \

/**
 * @brief Do a blocking read of the message from the core at sending_core_reg.
 * @param noc_core_base_address Output: The base address corresponding to sending_core_reg.
 * @param read_to_reg Output: The result of the blocking read.
 * @param sending_core_reg Input (preserved): The number of the sending core.
 */
#define BLOCKING_READ(nonce, noc_core_base_address, read_to_reg, sending_core_reg) \
    LOAD_NOC_CORE_BASE_ADDRESS(noc_core_base_address, sending_core_reg, read_to_reg)                    \
    BLOCK_ON_FLIT_FROM_CORE(nonce, noc_core_base_address, read_to_reg) \
    "lw " #read_to_reg ", 0(" #noc_core_base_address ")\n\t"

/** Helper to SEND_N_WORDS. Accumulates valid bits. */
#define OR_VALIDITY_OF_NOC_DATA(noc_base_address_reg, accumulator_reg, offset_literal, clobber0) \
    "lw " #clobber0 ", " #offset_literal "(" #noc_base_address_reg ")\n\t" \
    "or " #accumulator_reg ", " #accumulator_reg ", " #clobber0 "\n\t"

/** Helper to SEND_N_WORDS. */
#define READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce, noc_base_address_reg, read_to_reg, sending_core_reg, result_reg, tag_bit_mask, fail_label) \
    BLOCKING_READ(nonce, noc_base_address_reg, read_to_reg, sending_core_reg) \
    "and " #result_reg ", " #read_to_reg ", " #tag_bit_mask "\n\t" /* check tag bit */ \
    "bnez " #result_reg ", " #fail_label "\n\t"

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
 * @param noc_base_address Register that will hold the NoC base address. No assumptions are made
 * about the original value held in this register (the NoC base address will be written into it
 * regardless).
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
    noc_base_address, clobber1, clobber2, clobber3, clobber4 \
)                                      \
    LOAD_NOC_BASE_ADDRESS(noc_base_address) \
    "add " #result_reg ", x0, x0\n\t"  \
    SENDING_TO_ZERO_MACRO(OR_VALIDITY_OF_NOC_DATA(noc_base_address, result_reg, 16, clobber1), "") \
    SENDING_TO_ONE_MACRO(OR_VALIDITY_OF_NOC_DATA(noc_base_address, result_reg, 20, clobber1), "") \
    SENDING_TO_TWO_MACRO(OR_VALIDITY_OF_NOC_DATA(noc_base_address, result_reg, 24, clobber1), "") \
    SENDING_TO_THREE_MACRO(OR_VALIDITY_OF_NOC_DATA(noc_base_address, result_reg, 28, clobber1), "") \
    "bnez " #result_reg ", END_SEND_N_WORDS" #nonce "\n\t" /** Fail with error code 1 */ \
    SYNC5(nonce ## 0, noc_base_address, clobber1, clobber2, clobber3, clobber4) \
    "li " #clobber1 ", 0x80000000\n\t" /* Set the top bit as a tag bit. If u wanna send so many words that this creates amiguity, then u have a bigger problem on your hands */ \
    "or " #n_words_reg ", " #n_words_reg ", " #clobber1 "\n\t" \
    SENDING_NORTH_MACRO("sw " #n_words_reg ", 0(" #noc_base_address ")\n\t", "nop\n\t") \
    SENDING_EAST_MACRO("sw " #n_words_reg ", 0(" #noc_base_address ")\n\t", "nop\n\t") \
    "li " #clobber3 ", 1\n\t" \
    SENDING_NORTHEAST_MACRO("sw " #n_words_reg ", 0(" #noc_base_address ")\n\t", "nop\n\t") \
    SENDING_TO_ZERO_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce ## 0, noc_base_address, clobber2, x0, result_reg, clobber1, END_SEND_N_WORDS ## nonce), "") \
    SENDING_TO_ONE_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce ## 1, noc_base_address, clobber2, clobber3, result_reg, clobber1, END_SEND_N_WORDS ## nonce), "") \
    "li " #clobber3 ", 2\n\t" \
    SENDING_TO_TWO_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce ## 2, noc_base_address, clobber2, clobber3, result_reg, clobber1, END_SEND_N_WORDS ## nonce), "") \
    "li " #clobber3 ", 3\n\t" \
    SENDING_TO_THREE_MACRO(READ_AND_FAIL_IF_TAG_BIT_IS_1(nonce ## 3, noc_base_address, clobber2, clobber3, result_reg, clobber1, END_SEND_N_WORDS ## nonce), "") \
    /* At this point, all prospective message receivers have agreed that they are ready to receive the given number of words by replying using responses that have zero as their tag bit. By my count the TDM slot of the next instruction will be -2 mod 5, but for now I won't use that fact, preferring instead to re-synchronize, just to make the assembly easier to write (less brittle, less performant). */ \
    load_words_asm \
    SYNC5(nonce ## 1, noc_base_address, clobber1, clobber2, clobber3, clobber4) \
    send_words_asm \
    "END_SEND_N_WORDS" #nonce ": nop\n\t"

#define MUL4_2CYCLES_2INSTRS(in_reg, out_reg) \
    MUL4(in_reg, out_reg) "nop\n\t"

#define MUL5_2CYCLES_2INSTRS(in_reg, out_reg) MUL5(in_reg, out_reg)

/**
 * @brief Read the number of words specified by the sender.
 * @param DIRECTION_QUINTET_MACRO The macro corresponding to the direction of the sender from the
 * receiver.
 * @param MUL_BY_RECEIVE_WORDS_PERIOD_2CYCLES_2INSTRS Assembly consisting of two instructions that
 * takes an in_reg and an out_reg and sets the out_reg to the number of instructions in each 5-cycle
 * subsequence of receive_words_asm. See MUL4_2CYCLES_2INSTRS, MUL5_2CYCLES_2INSTRS for examples.
 * @param preparatory_asm_4cycles Assembly taking 4 cycles that may prepare for the
 * receive_words_asm that follows. The number of instructions used (not just cycles) matters!
 * @param offset_numeric_literal 28 plus (4 times the number of instructions in
 * preparatory_asm_4cycles). This will be 44 unless the 4-cycle sequence includes instructions that
 * cause stalls, such as loads, branches, or jumps.
 * @param sending_core_reg Input: A register specifying the sending core.
 * @param receive_words_asm Assembly that receives the sent words. Must read in its first cycle, and
 * exactly every 5 cycles thereafter!
 * @param noc_base_address Output: A register that will be set to the base address of the NoC
 * corresponding to the given core.
 */
#define READ_N_WORDS(                                                                              \
    nonce,                                                                                         \
    DIRECTION_QUINTET_MACRO,                                                                       \
    MUL_BY_RECEIVE_WORDS_PERIOD_2CYCLES_2INSTRS,                                                   \
    preparatory_asm_4cycles,                                                                       \
    offset_numeric_literal,                                                                        \
    sending_core_reg,                                                                              \
    receive_words_asm,                                                                             \
    noc_base_address, clobber0, clobber1, clobber2, clobber3, clobber4                             \
)                                                                                                  \
    BLOCKING_READ(nonce ## 1, noc_base_address, clobber0, sending_core_reg)                        \
    /* "andi " #clobber0 ", " #clobber0 ", 1023\n\t" /* 1023 is the largest 12-bit number that doesn't sign-extend to have a 1 in its top bit */ \
    MUL_BY_RECEIVE_WORDS_PERIOD_2CYCLES_2INSTRS(clobber0, clobber0) \
    SYNC5_PACKED(nonce ## 2, noc_base_address, clobber1, clobber2, clobber3, clobber4, "slli " #clobber0 ", " #clobber0 ", 2\n\t") \
    "jal " #clobber1 ", CONTINUING_READ_N_WORDS" #nonce "\n\t"                                     \
    "jal x0, READ_N_WORDS_END" #nonce "\n\t" /* This instruction will not be executed in its present position. It's just here so we can copy it somewhere else. */                                             \
    "CONTINUING_READ_N_WORDS" #nonce ":\n\t"                                                       \
    "lw " #clobber2 ", 0(" #clobber1 ")\n\t" /* clobber2 := "jump to end" instruction */           \
    "add " #clobber1 ", " #clobber1 ", " #clobber0 "\n\t"                                          \
    preparatory_asm_4cycles                                                                        \
    DIRECTION_QUINTET_MACRO(                                                                       \
        "lw " #clobber3 ", " #offset_numeric_literal "(" #clobber1 ")\n\t", /* clobber3 := instruction to be replaced (receive_words_asm must not clobber clobber3!) */ \
        "sw " #clobber2 ", " #offset_numeric_literal "(" #clobber1 ")\n\t",                        \
        "nop\n\t",                                                                                 \
        clobber0,                                                                                  \
        noc_base_address,                                                                          \
    )                                                                                              \
    receive_words_asm                                                                              \
    "READ_N_WORDS_END" #nonce ":\n\t"                                                              \
    "sw " #clobber3 ", " #offset_numeric_literal "(" #clobber1 ")\n\t"  /* restore the modified imem entry */

