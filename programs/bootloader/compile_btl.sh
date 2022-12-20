#!/bin/bash
set -ex
set -euo pipefail

SCRIPT_DIR=$(dirname "$0")
LIB_DIR=$SCRIPT_DIR/../../flexpret/programs/lib
LINKER_SCRIPT=$SCRIPT_DIR/../../flexpret/programs/lib/linker/flexpret_btl.ld
CC=riscv32-unknown-elf-gcc
OBJDUMP=riscv32-unknown-elf-objdump
OBJCOPY=riscv32-unknown-elf-objcopy
EMU=fp-emu

# Compile a C program into a riscv ELF file.
$CC -I$LIB_DIR/include -T $LINKER_SCRIPT -DBOOTLOADER=1 -Xlinker -Map=output.map -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles --specs=nosys.specs -o btl.riscv $LIB_DIR/start.S $LIB_DIR/syscalls.c $LIB_DIR/startup.c $LIB_DIR/bootloader.c

# Generate dump file.
$OBJDUMP -S -d btl.riscv > btl.dump

# Extract a binary file from the ELF file.
$OBJCOPY -O binary btl.riscv btl.binary.txt

# Generate a hex file (with a .mem extension) from the binary file.
xxd -c 4 -e btl.binary.txt | cut -c11-18 > btl.mem
xxd -c 4 -e btl.binary.txt > btl.mem.orig

# Delete the binary file.
rm btl.binary.txt
