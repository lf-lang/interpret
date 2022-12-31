#define WAIT_FOR_NEXT_ZERO_MOD_1024(id) \
        "li t0, 1014\n\t"                                                                          \
        "li a0, 1\n\t"                                                                             \
        "li a1, 2\n\t"                                                                             \
        "li a2, 3\n\t"                                                                             \
        "li a3, 4\n\t"                                                                             \
        "li a4, 5\n\t"                                                                             \
        "li a5, 6\n\t"                                                                             \
        "li t6, 7\n\t"                                                                             \
        "rdcycle t1\n\t"                                                                           \
        "andi t1, t1, 7\n\t"                                                                       \
        "beq t1, t6, LOOP" #id "\n\t"                                                              \
        "beq t1, a5, LOOP" #id "\n\t"                                                              \
        "beq t1, a4, LOOP" #id "\n\t"                                                              \
        "beq t1, a3, LOOP" #id "\n\t"                                                              \
        "beq t1, a2, LOOP" #id "\n\t"                                                              \
        "beq t1, a1, LOOP" #id "\n\t"                                                              \
        "beq t1, a0, LOOP" #id "\n\t"                                                              \
        "beq t1, x0, LOOP" #id "\n\t"                                                              \
        /* This entire loop is 8 cycles long, so the value of t1 upon exiting is t0 plus a      */ \
        /* number in the range [0, 7]                                                           */ \
        "LOOP" #id ":\n\t"                                                                         \
        "nop\n\t"  /* Delay so that loop length is a power of 2 */                                 \
        "nop\n\t"                                                                                  \
        "nop\n\t"                                                                                  \
        "rdcycle t1\n\t"                                                                           \
        "andi t1, t1, 1023\n\t"                                                                    \
        "blt t1, t0, LOOP" #id "\n\t" /* Cost of 3 cycles when taken, 1 otherwise; see page 37 https://www2.eecs.berkeley.edu/Pubs/TechRpts/2015/EECS-2015-181.pdf */ \
        "nop\n\t"                                                                                  \
        "nop\n\t"                                                                                  \
        "nop\n\t"                                                                                  \
        "nop\n\t"                                                                                  \
        "nop\n\t"                                                                                  \
        "nop\n\t"
