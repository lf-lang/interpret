build: gen compile

gen:
	mkdir -p asm-gen

	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg asm/flexpret-noc-low-level-interface.rvg asm/flexpret-noc-c-api.rvg "flexpret-noc-c-api.rvg=[flexpret-noc-c-api [mu [] [print-flexpret-noc-c-api-h]]]" > asm-gen/flexpret-noc-c-api.h

	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg asm/flexpret-noc-low-level-interface.rvg asm/flexpret-noc-c-api.rvg "flexpret-noc-c-api.rvg=[flexpret-noc-c-api [mu [] [print-read-n-words-and-print]]]" > asm-gen/read-n-words-and-print.s

	rvg asm/stdlib.rvg asm/ctrl.rvg asm/flexpret.rvg asm/flexpret-noc-low-level-interface.rvg asm/flexpret-noc-c-api.rvg "flexpret-noc-c-api.rvg=[flexpret-noc-c-api [mu [] [print-broadcast-count]]]" > asm-gen/broadcast-count.s

	rvg asm/stdlib.rvg asm/ctrl.rvg asm/hello.rvg > asm-gen/hello.s
	rvg asm/stdlib.rvg asm/ctrl.rvg asm/hello_h.rvg > asm-gen/hello.h

compile:
	riscv_compile.sh ispm asm-gen/hello.s asm-gen/read-n-words-and-print.s asm-gen/broadcast-count.s broadcast_memory_noc.c

clean:
	rm -r asm-gen
	riscv_clean.sh

rebuild: clean build

PHONY: build clean rebuild
