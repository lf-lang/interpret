#ifndef ASM_UTILS_H
#define ASM_UTILS_H

/**
 * @brief Pure assembly version of _fp_print. Executes in 4 cycles.
 */
#define FP_PRINT_ASM(reg, clobber0) \
    "li " #clobber0 ", 0xbaaabaaa\n\t"                                                             \
    "csrw 0x51e, " #clobber0 "\n\t"                                                                \
    "csrw 0x51e, " #reg "\n\t"

#define TRUE_MACRO(case_true, case_false) case_true
#define FALSE_MACRO(case_true, case_false) case_false

#define REPEAT1(x) x
#define REPEAT2(x) x x
#define REPEAT3(x) x x x
#define REPEAT5(x) REPEAT2(x) REPEAT3(x)

#define REPEAT4(x) REPEAT2(REPEAT2(x))

#endif // ASM_UTILS_H
