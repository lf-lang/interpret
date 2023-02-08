build: gen compile

compile:
	riscv-compile.sh 1 ispm complicated_bandwidth.c asm-gen/transmit_receive.s

clean:
	riscv-clean.sh

gen:
	mkdir -p asm-gen
	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg asm/transmit_receive.rvg > asm-gen/transmit_receive.s
	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg asm/transmit_receive_h.rvg > asm-gen/transmit_receive.h

rebuild: clean build

PHONY: build clean rebuild
