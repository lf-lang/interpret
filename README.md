# InterPRET

## Getting started
```
git clone git@github.com:lf-lang/interpret.git --recursive
cd interpret
make emulator
```

#### Hello World Noc
```
cd programs/noc/HelloWorld
make
fp-emu
```

#### UART TX
The verilator emulator also contains C++ code which listens on the top-level uart tx pin and prints all bytes in receives.
Verify that this works by running the following program.

```
cd programs/uart/tx
make
fp-emu
```

#### UART RX
The default verilator emulator also contains C++ code that writes a sequence of bytes to the top-level uart rx pin. 
Verify that all the cores are able to receive this by:

```
cd programs/uart/tx
make
fp-emu
```



## Project plan for CPSWeek submission

### Tooling
- [x] Setup interpret repository
- [ ] Make Bootloader
	- Could be the same on all FPs and connect to SAME GPIO pin
	- Use ip-contribution UART and wrap it into a WB device.

### Precise timing (serial device) benchmark
- [ ] Prototype a precision timed serial device for RX/TX
- [ ] (optional) Add a UART device (e.g. Martins) to the Top.scala and see that it works
- [ ] Evaluate on FPGA

###  Throughput/latency benchmark
- [ ] Make throughput test program
- [ ] Evaluate on FPGA

### Multithreaded benchmark
- [ ] Get HW locks tested (and merged)
- [ ] Figure out what benchmark to run...
- [ ] Run in emulation
- [ ] Run on FPGA

### Area/Resources evaluation
- [ ] Synthesize to FPGA
