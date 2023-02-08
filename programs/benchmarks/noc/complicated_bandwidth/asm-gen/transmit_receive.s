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
    addi t3, t1, -256
    addi t3, t3, 1
    blt t3, zero, REMAINDER_xx2KBeM1TM
LOOP_tZvLUwyjwy:
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
    lw t2, 64(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 68(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 72(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 76(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 80(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 84(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 88(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 92(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 96(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 100(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 104(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 108(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 112(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 116(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 120(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 124(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 128(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 132(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 136(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 140(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 144(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 148(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 152(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 156(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 160(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 164(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 168(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 172(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 176(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 180(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 184(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 188(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 192(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 196(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 200(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 204(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 208(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 212(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 216(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 220(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 224(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 228(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 232(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 236(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 240(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 244(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 248(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 252(a1)
    sw t2, 4(t0)
    addi zero, zero, 0
    addi zero, zero, 0
    addi a1, a1, 256
    blt a1, t3, LOOP_tZvLUwyjwy
REMAINDER_xx2KBeM1TM:
LOOP_dCcIYk9YE4:
    lw t2, 0(a1)
    sw t2, 4(t0)
    addi a1, a1, 4
    blt a1, t1, LOOP_dCcIYk9YE4
    jalr zero, ra, 0
receive_arr:
    lui t0, 262144
    addi t0, t0, 32
    slli t1, a1, 2
    add t1, t1, a0
LOOP_oSVW8oFZ1y:
    lw t3, 0(t0)
    andi t3, t3, 2
    beq t3, zero, LOOP_oSVW8oFZ1y
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    addi zero, zero, 0
    addi t3, t1, -256
    addi t3, t3, 1
    blt t3, zero, REMAINDER_dyQgJOEnQ0
LOOP_qkSDaAOYNe:
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
    lw t2, 4(t0)
    sw t2, 64(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 68(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 72(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 76(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 80(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 84(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 88(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 92(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 96(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 100(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 104(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 108(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 112(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 116(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 120(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 124(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 128(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 132(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 136(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 140(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 144(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 148(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 152(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 156(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 160(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 164(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 168(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 172(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 176(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 180(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 184(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 188(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 192(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 196(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 200(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 204(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 208(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 212(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 216(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 220(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 224(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 228(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 232(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 236(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 240(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 244(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 248(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    lw t2, 4(t0)
    sw t2, 252(a0)
    addi zero, zero, 0
    addi zero, zero, 0
    addi a0, a0, 256
    blt a0, t3, LOOP_qkSDaAOYNe
REMAINDER_dyQgJOEnQ0:
LOOP_syOGwjVTp7:
    lw t2, 4(t0)
    sw t2, 0(a0)
    addi a0, a0, 4
    blt a0, t1, LOOP_syOGwjVTp7
    jalr zero, ra, 0

.end
