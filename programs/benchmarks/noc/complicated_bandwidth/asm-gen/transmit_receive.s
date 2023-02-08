.global transmit, receive, transmit_arr, receive_arr
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
transmit_arr:
    lui t0, 262144
    addi t0, t0, 32
LOOP_N840jLsMYh:
    lw t1, 0(t0)
    andi t1, t1, 1
    beq t1, zero, LOOP_N840jLsMYh
    slli t1, a2, 2
    add t1, t1, a1
    sw a0, 8(t0)
LOOP_tZvLUwyjwy:
    lw t2, 0(a1)
    sw t2, 4(t0)
    addi a1, a1, 4
    blt a1, t1, LOOP_tZvLUwyjwy
    jalr zero, ra, 0
receive_arr:
    lui t0, 262144
    addi t0, t0, 32
LOOP_xx2KBeM1TM:
    lw t3, 0(t0)
    andi t3, t3, 2
    beq t3, zero, LOOP_xx2KBeM1TM
    slli t1, a1, 2
    add t1, t1, a0
LOOP_dCcIYk9YE4:
    lw t2, 4(t0)
    sw t2, 0(a0)
    addi a0, a0, 4
    blt a0, t1, LOOP_dCcIYk9YE4
    jalr zero, ra, 0

.end
