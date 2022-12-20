build:
	riscv_compile.sh core0 hello.c
	cp core0.mem core1.mem
	cp core0.mem core2.mem
	cp core0.mem core3.mem

clean:
	riscv_clean.sh


rebuild: clean build

PHONY: build clean rebuild