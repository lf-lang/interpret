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
#define REPEAT64(x) REPEAT4(REPEAT4(REPEAT4(x)))

#define COUNT_TO_20(MACRO, param0, param1, param2) \
    MACRO(param0, param1, param2, 0) \
    MACRO(param0, param1, param2, 1) \
    MACRO(param0, param1, param2, 2) \
    MACRO(param0, param1, param2, 3) \
    MACRO(param0, param1, param2, 4) \
    MACRO(param0, param1, param2, 5) \
    MACRO(param0, param1, param2, 6) \
    MACRO(param0, param1, param2, 7) \
    MACRO(param0, param1, param2, 8) \
    MACRO(param0, param1, param2, 9) \
    MACRO(param0, param1, param2, 10) \
    MACRO(param0, param1, param2, 11) \
    MACRO(param0, param1, param2, 12) \
    MACRO(param0, param1, param2, 13) \
    MACRO(param0, param1, param2, 14) \
    MACRO(param0, param1, param2, 15) \
    MACRO(param0, param1, param2, 16) \
    MACRO(param0, param1, param2, 17) \
    MACRO(param0, param1, param2, 18) \
    MACRO(param0, param1, param2, 19)

#define MUL4(in_reg, out_reg)                                                                      \
    "slli " #out_reg ", " #in_reg ", 2\n\t"

#define MUL5(in_reg, out_reg)                                                                      \
    MUL4(in_reg, out_reg)                                                                          \
    "add " #out_reg ", " #out_reg ", " #in_reg "\n\t"


#endif // ASM_UTILS_H
