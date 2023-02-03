build: gen compile

gen:
	mkdir -p asm-gen
	rvg asm/stdlib.rvg asm/ctrl.rvg asm/hello.rvg > asm-gen/hello2.s

compile:
	riscv_compile.sh ispm asm-gen/hello2.s broadcast_memory_noc.c

clean:
	rm -r asm-gen
	riscv_clean.sh

rebuild: clean build

PHONY: build clean rebuild
