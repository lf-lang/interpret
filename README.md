# InterPRET

## Getting started
```
git clone git@github.com:lf-lang/interpret.git --recursive
cd interpret
source env.bash
make emulator
```

#### Hello World NoC
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
cd programs/uart/rx
make
fp-emu
```
