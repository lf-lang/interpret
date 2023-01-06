# InterPRET

## Getting started

#### Building a quad-core InterPRET emulator
```
git clone git@github.com:lf-lang/interpret.git --recursive
cd interpret
source env.bash
make emulator N_CORES=4
```
#### Emulating programs on the InterPRET
For simplicity, each core in the emulated InterPRET will execute the same program. It gets loaded into the instruction scratch-pad memory by Verilator. Verilator expects a file named `ispm.mem` to be located in the folder from which `fp-emu` is invoked. This file will be loaded into all the ISPMs of all the cores. To differentiate between the cores, each has a unique core id.


#### Hello World
In HelloWorld we show how we can have each core execute different parts of the whole program by reading its core-id.
```
cd programs/HelloWorld
make
fp-emu
```

#### UART TX
The verilator emulator also contains C++ code which listens on the top-level uart tx pin and prints all bytes in receives.
The following program has one of the core write a byte through its uart device. Verilator should detect and print out this byte.
```
cd programs/uart/tx
make
fp-emu
```

#### UART RX
Verilator can also pass data to the InterPRET uart_rx pin. This examples writes a couple of bytes from Verilator which should be detected by all cores.

```
cd programs/uart/rx
make
fp-emu test.bin # test.bin will be written to the UART RX pin
```

#### Hello World NoC
The HelloWorld NoC program shows how different cores can communicate, concurrently, through the NoC. 
```
cd programs/noc/HelloWorld
make
fp-emu
```


#### Simple SW UART
```
make remulator N_CORES=1
cd programs/software-uart
riscv_compile.sh ispm uart_tx.c
fp-emu
```

Then open the generated `trace.vcd` in GTKwave and you should see the pin1 on io_gpio_out_0 transmitting the byte 0x55;
0x55 = Start bit (low)-1-0-1-0-1-0-1-0-stop bit (high) 


#### Bootloader
A simple serial bootloader using the UART peripheral is also provided. This program will receive the application as a sequence of bytes through the UART peripheral. It expects a simple protocol as follows:
- [SYNC_ID LEN DATA SYNC_ID]
- SYNC_ID = 0xC0 0xDE
- LEN = uint16 representing the length, MSB first.
- DATA is the program should be of length LEN
- SYNC_ID is as above.

The application most be linked with a special linker script which places the application at a 4KB offset from bottom of ISPM. The script in `programs/scripts/compile_app.sh` does this. The script `programs/scripts/serialize_app.py` takes the .mem file produced and wraps it in the simple protocol explained above.

To verify that the bootloader works we will try to load a simple HelloWorld onto a single core InterPRET through the serial bootloader

1. Build a single-core InterPRET with tracing disabled
```
make clean && make emulator N_CORES=1 TRACE=0
```

2. Build a HelloWorld application linked to start at 0x1000 (4KB) 
```
cd programs/HelloWorld
../scripts/compile_app.sh hello hello.c
```
3. Wrap program in simple serial protocol
```
python3 ../scripts/serialize_app.py hello.mem hello.app
```
4. Build bootloader
```
cd ../bootloader
make
fp-emu ../HelloWorld/hello.app
```

This will take quite some time and you should see Core-0 printing a bunch of debug info for each byte it receives. After a couple of minutes you should see:
```
Core-0:          3
Writing 222
Core-0:        222
Core-0:          3
Writing 192
Core-0:        192
Core-0:        100
Writing 192
Core-0:         42
Core-0 is done
All cores are done terminating
Assertion failed: Program terminated sucessfully
    at Top.scala:97 assert(false.B, "Program terminated sucessfully")
[43463332] %Error: Top.sim.v:13817: Assertion failed in TOP.Top
%Error: Top.sim.v:13817: Verilog $stop
Aborting...
```

I.e. Core-0 receives the 222 and 192 (0xDE and 0xC0 the SYNC_ID) before printing 100. Then finally printing 42 which is done from the application. 