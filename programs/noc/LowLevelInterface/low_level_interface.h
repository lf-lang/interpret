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
 */
#define BLOCKING_READ(nonce, noc_base_address, read_to_reg, sending_core_reg) \
    LOAD_NOC_BASE_ADDRESS(noc_base_address)                                                        \
    "slli " #read_to_reg ", " #sending_core_reg ", 2\n\t"                                          \
    "add " #noc_base_address ", " #read_to_reg ", " #noc_base_address "\n\t"                       \
    "POLL" #nonce ": lw " #read_to_reg ", 16(" #noc_base_address ")\n\t"                           \
    "beq x0, " #read_to_reg ", POLL" #nonce "\n\t"                                                 \
    "lw " #read_to_reg ", 0(" #noc_base_address ")\n\t"

#define FP_PRINT_ASM(reg, clobber0) \
    "li " #clobber0 ", 0xbaaabaaa\n\t"                                                             \
    "csrw 0x51e, " #clobber0 "\n\t"                                                                \
    "csrw 0x51e, " #reg "\n\t"                                                                     \

