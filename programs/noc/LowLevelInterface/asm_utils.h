#ifndef ASM_UTILS_H
#define ASM_UTILS_H

#define TOKENPASTE2__(x, y) x ## y
#define TOKENPASTE1__(x, y) TOKENPASTE2__(x, y)
#define TOKENPASTE(x, y) TOKENPASTE1(x, y)

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
#define REPEAT4(x) REPEAT2(REPEAT2(x))
#define REPEAT5(x) REPEAT2(x) REPEAT3(x)
#define REPEAT7(x) REPEAT2(x) REPEAT5(x)
#define REPEAT11(x) REPEAT7(x) REPEAT4(x)

#define MUL4(in_reg, out_reg)                                                                      \
    "slli " #out_reg ", " #in_reg ", 2\n\t"

#define MUL5(in_reg, out_reg)                                                                      \
    MUL4(in_reg, out_reg)                                                                          \
    "add " #out_reg ", " #out_reg ", " #in_reg "\n\t"


#endif // ASM_UTILS_H
