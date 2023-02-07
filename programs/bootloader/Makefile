SRCS=bootloader.c
LINKER_SCRIPT=$(FP_ROOT)/programs/linker/flexpret_btl.ld
CC=riscv32-unknown-elf-gcc
OBJDUMP=riscv32-unknown-elf-objdump
OBJCOPY=riscv32-unknown-elf-objcopy
LIB_DIR=$(FP_ROOT)/flexpret/programs/lib


build: btl.mem

btl.mem: btl.binary.txt
	xxd -c 4 -e btl.binary.txt | cut -c11-18 > btl.mem
	xxd -c 4 -e btl.binary.txt > btl.mem.orig
	cp btl.mem ispm.mem


btl.binary.txt: btl.riscv
	$(OBJCOPY) -O binary btl.riscv btl.binary.txt
	$(OBJDUMP) -S -d btl.riscv > btl.dump


btl.riscv: bootloader.c
	$(CC) -I$(LIB_DIR)/include -T $(LINKER_SCRIPT) -DBOOTLOADER=1 -DNUM_THREADS=4 -DCPU_FREQ=50000000 -Xlinker -Map=output.map -g -static -O0 -march=rv32i -mabi=ilp32 -nostartfiles --specs=nosys.specs -o btl.riscv $(LIB_DIR)/start.S $(LIB_DIR)/syscalls.c $(LIB_DIR)/startup.c bootloader.c


clean:
	riscv_clean.sh


rebuild: clean build

PHONY: build clean rebuild