.global transmit, receive, transmit_arr, receive_arr, do_test_nops0, do_test_nops1, do_test_nops2, do_test_nops3, do_test_nops4, do_test_nops5, do_test_nops6, do_test_nops7, do_test_nops8, do_test_nops9, 
.section .text
transmit:
    lui t0, 262144
    addi t0, t0, 32
LOOP_tJ6slU7Skn:
    lw t1, 0(t0)
    andi t1, t1, 1
    beq t1, zero, LOOP_tJ6slU7Skn
    sw a0, 8(t0)
    sw a1, 4(t0)
    jalr zero, ra, 0
receive:
    lui t0, 262144
    addi t0, t0, 32
LOOP_vWRvZ2Pn4y:
    lw t1, 0(t0)
    andi t1, t1, 2
    beq t1, zero, LOOP_vWRvZ2Pn4y
    lw a0, 4(t0)
    jalr zero, ra, 0
    addi zero, zero, 0
do_test_nops9:
    addi zero, zero, 0
do_test_nops8:
    addi zero, zero, 0
do_test_nops7:
    addi zero, zero, 0
do_test_nops6:
    addi zero, zero, 0
do_test_nops5:
    addi zero, zero, 0
do_test_nops4:
    addi zero, zero, 0
do_test_nops3:
    addi zero, zero, 0
do_test_nops2:
    addi zero, zero, 0
do_test_nops1:
    addi zero, zero, 0
do_test_nops0:
    jalr zero, ra, 0

.end
