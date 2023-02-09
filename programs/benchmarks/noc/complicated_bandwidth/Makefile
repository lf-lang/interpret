build: gen compile

compile:
	riscv-compile.sh 1 ispm complicated_bandwidth.c asm-gen/transmit_receive.s

clean:
	riscv-clean.sh

TDM_N_CYCLES ?= 5
TX_RX_N_UNROLL ?= 16

gen:
	mkdir -p asm-gen
	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg "fpconfig.rvg=[mu [(program)] [def (tdm-n-cycles) {$(TDM_N_CYCLES)}] [def (tx-rx-n-unroll num!) {$(TX_RX_N_UNROLL)}] [program]]" asm/transmit_receive.rvg > asm-gen/transmit_receive.s
	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg asm/transmit_receive_h.rvg > asm-gen/transmit_receive.h

rebuild: clean build

PHONY: build clean rebuild
