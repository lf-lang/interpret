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

# Compile
$CC -I$LIB_DIR/include -T $LINKER_SCRIPT -Xlinker -Map=output.map -DNUM_THREADS=$1 -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles --specs=nosys.specs -o $2.riscv $LIB_DIR/start.S $LIB_DIR/syscalls.c $LIB_DIR/tinyalloc/tinyalloc.c $LIB_DIR/startup.c $LIB_DIR/flexpret_thread.c $LIB_DIR/flexpret_lock.c $LIB_DIR/flexpret_exceptions.c "${@:3}"

# Generate dump file.
$OBJDUMP -S -d $2.riscv > $2.dump

# Extract a temporary binary file from the ELF file.
$OBJCOPY -O binary $2.riscv $2.binary.txt

# Generate a hex file (with a .mem extension) from the temporary binary file.
xxd -c 4 -e $2.binary.txt | cut -c11-18 > $2.mem
xxd -c 4 -e $2.binary.txt > $2.mem.orig

# Delete the temporary binary file.
rm $2.binary.txt

# Serialize
serialize_app.py $2.mem $2.app