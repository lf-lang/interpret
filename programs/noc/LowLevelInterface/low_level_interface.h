/**
 * @brief The instruction immediately following SYNC5 is able to store a word into the zeroth TDM
 * slot (northwest) in a single-threaded setting.
 * It 9-13 cycles to synchronize. The 0-4 is fundamental and the remaining 9 cycles are overhead.
 */
#define SYNC5(nonce, reg0, reg1, reg2, reg3, reg4)                                                 \
    "li " #reg0 ", 1\n\t"                                                                          \
    "li " #reg1 ", 2\n\t"                                                                          \
    "li " #reg2 ", 3\n\t"                                                                          \
    "li " #reg3 ", 0x80000000\n\t"                                                                 \
    "lw " #reg4 ", 32(" #reg3 ")\n\t"  /* Get elapsed cycles mod period */                         \
    "beq " #reg4 ", " #reg0 ", DONE_SYNCHRONIZING" #nonce "\n\t"                                   \
    "beq " #reg4 ", x0, DONE_SYNCHRONIZING" #nonce "\n\t"                                          \
    "nop\n\t"                                                                                      \
    "beq " #reg4 ", " #reg2 ", DONE_SYNCHRONIZING" #nonce "\n\t"                                   \
    "beq " #reg4 ", " #reg1 ", DONE_SYNCHRONIZING" #nonce "\n\t"                                   \
    "DONE_SYNCHRONIZING" #nonce ":\n\t"

/**
 * @brief Broadcast the value stored in reg to all other cores. The instruction immediately
 * following BROADCAST_SYNCED is able to store into the zeroth TDM slot.
 */
#define BROADCAST_SYNCED_WITH_INSTRUCTIONS(nonce, reg, instr0, instr1)                             \
    "sw " #reg ", 0(t4)\n\t"                                                                       \
    instr0                                                                                         \
    "sw " #reg ", 0(t4)\n\t"                                                                       \
    "sw " #reg ", 0(t4)\n\t"                                                                       \
    instr1                                                                                         \

#define BROADCAST_SYNCED(nonce, reg)                                                               \
    BROADCAST_SYNCED_WITH_INSTRUCTIONS(nonce, reg, "nop\n\t", "nop\n\t")
