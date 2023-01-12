#!/bin/bash

# Script for compiling app to be loaded by bootloader
# This basically means linking with a different linker script.
# It also means that we cannot load this program into the I-SPM
# through verilator the "normal" way

set -ex
set -euo pipefail

LIB_DIR=$FP_ROOT/flexpret/programs/lib
LINKER_SCRIPT=$FP_ROOT/programs/linker/flexpret_app.ld
CC=riscv32-unknown-elf-gcc
OBJDUMP=riscv32-unknown-elf-objdump
OBJCOPY=riscv32-unknown-elf-objcopy
EMU=fp-emu

# Compile a C program into a riscv ELF file.
$CC -I$LIB_DIR/include -T $LINKER_SCRIPT -Xlinker -Map=output.map -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles --specs=nosys.specs -o $1.riscv $LIB_DIR/start.S $LIB_DIR/syscalls.c $LIB_DIR/startup.c $LIB_DIR/tinyalloc/tinyalloc.c $LIB_DIR/flexpret_exceptions.c "${@:2}"
# Generate dump file.
$OBJDUMP -S -d $1.riscv > $1.dump

# Extract a binary file from the ELF file.
$OBJCOPY -O binary $1.riscv $1.binary.txt

# Generate a hex file (with a .mem extension) from the binary file.
xxd -c 4 -e $1.binary.txt | cut -c11-18 > $1.mem
xxd -c 4 -e $1.binary.txt > $1.mem.orig

# Delete the binary file.
rm $1.binary.txt
