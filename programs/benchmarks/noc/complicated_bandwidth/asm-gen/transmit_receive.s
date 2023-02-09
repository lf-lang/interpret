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
    slli t1, a2, 2
    add t1, t1, a1
LOOP_N840jLsMYh:
    lw t2, 0(t0)
    andi t2, t2, 1
    beq t2, zero, LOOP_N840jLsMYh
    sw a0, 8(t0)
    csrrs t3, 0x510, zero
    sw t3, 4(t0)
    lui t2, 764588
    addi t2, t2, -1366
    csrrw zero, 0x51e, t2
    csrrw zero, 0x51e, t3
LOOP_tZvLUwyjwy:
    lw t2, 0(t0)
    andi t2, t2, 2
    beq t2, zero, LOOP_tZvLUwyjwy
    lw t2, 4(t0)
    addi t3, t1, -64
    addi t3, t3, 1
    blt t3, zero, REMAINDER_dCcIYk9YE4
    sw zero, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
LOOP_xx2KBeM1TM:
    lw t2, 0(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 8(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 12(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 16(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 20(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 24(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 28(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 32(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 36(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 40(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 44(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 48(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 52(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 56(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 60(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    addi a1, a1, 64
    blt a1, t3, LOOP_xx2KBeM1TM
REMAINDER_dCcIYk9YE4:
LOOP_oSVW8oFZ1y:
    lw t2, 0(a1)
    sw t2, 4(t0)
    addi a1, a1, 4
    blt a1, t1, LOOP_oSVW8oFZ1y
    jalr zero, ra, 0
FAIL_qkSDaAOYNe:
    lui t3, 764588
    addi t3, t3, -1366
    csrrw zero, 0x51e, t3
    csrrw zero, 0x51e, t2
    lui t2, 764588
    addi t2, t2, -1366
    addi t3, zero, 666
    csrrw zero, 0x51e, t2
    csrrw zero, 0x51e, t3
receive_arr:
    lui t0, 262144
    addi t0, t0, 32
LOOP_dyQgJOEnQ0:
    lw t2, 0(t0)
    andi t2, t2, 2
    beq t2, zero, LOOP_dyQgJOEnQ0
    lw t2, 4(t0)
    sw t2, 8(t0)
    sw t2, 4(t0)
    slli t1, a1, 2
    add t1, t1, a0
    addi t3, t1, -64
    addi t3, t3, 1
LOOP_syOGwjVTp7:
    lw t2, 0(t0)
    andi t2, t2, 2
    beq t2, zero, LOOP_syOGwjVTp7
    lw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    blt t3, zero, REMAINDER_RI1ag3enLg
LOOP_3gNwMs2sHd:
    lw t2, 4(t0)
    sw t2, 0(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 4(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 8(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 12(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 16(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 20(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 24(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 28(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 32(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 36(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 40(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 44(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 48(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 52(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 56(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 60(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    addi a0, a0, 64
    blt a0, t3, LOOP_3gNwMs2sHd
REMAINDER_RI1ag3enLg:
LOOP_ABkSUJRNgK:
    lw t2, 4(t0)
    sw t2, 0(a0)
    addi a0, a0, 4
    blt a0, t1, LOOP_ABkSUJRNgK
    jalr zero, ra, 0

.end
