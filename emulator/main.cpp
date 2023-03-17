/*
 * main.cpp
 * C++ main entry point for Verilator simulation.
 *
 * Copyright 2021 Edward Wang <edwardw@eecs.berkeley.edu>
 */
#include "VTop.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <iostream>
#include "uartsim.h"

uint64_t timestamp = 0;

double sc_time_stamp() {
  return timestamp;
}

int main(int argc, char* argv[]) {
  Verilated::commandArgs(argc, argv);
  bool trace_enabled = false;
  bool uart_file = false;
  int uart_file_argv = 0;

  // We accept 2 optional command line args, in arbitrary order
  // A) `--trace` to turn on VCD tracing
  // B) file_path.bin to a binary file which will be read into the uart rx pin of the InterPRET

  if (argc == 2) {
    if (!strcmp(argv[1], "--trace")) {
      std::cout << "Tracing enabled" << std::endl;
      trace_enabled = true;
      Verilated::traceEverOn(true);
    } else {
      uart_file = true;
      uart_file_argv = 1;
    }
  }
  
  if (argc == 3) {
    uart_file = true;
    if (!strcmp(argv[1], "--trace")) {
      std::cout << "Tracing enabled" << std::endl;
      trace_enabled = true;
      Verilated::traceEverOn(true);
      uart_file_argv = 2;
    } else if (argv[2], "--trace") {
      std::cout << "Tracing enabled" << std::endl;
      trace_enabled = true;
      Verilated::traceEverOn(true);
      uart_file_argv = 1;
    } else {
      std::cout << "Dont recognize command line arguments:" <<argv[1] <<"," <<argv[2] <<std::endl;
      std::cout << "Usage ip-verilator [uart_file] [--trace]" <<std::endl;
      exit(1);
    }
  }


  VTop* top = new VTop;
  VerilatedVcdC *trace;
  if (trace_enabled) {
    trace = new VerilatedVcdC;
    top->trace(trace, 99);
    trace->open("trace.vcd");
  }
  
  if (uart_file) {
    std::cout <<"Passing in uart_file: `" <<argv[uart_file_argv] <<"`" <<std::endl;
    uartsim_init(argv[uart_file_argv]);
  } else {
    uartsim_init("");
  }

  // Drive UART pin high by default
  top->io_uart_rx=1;

  // FIXME: Set this via command-line arguments.
  while (!Verilated::gotFinish()) {
    top->clock = 1;
    top->eval();
    timestamp+=1;
    
    if (trace_enabled) {
      trace->dump(10*timestamp);
    }

    if (timestamp > 10) {
      uartsim_print_rx( (int *) &top->io_uart_tx, 0);
      uartsim_write(&top->io_uart_rx);
    }
    top->clock = 0;
    top->eval();
    timestamp+=1;
    
    if (trace_enabled) {
      trace->dump(10*timestamp);
      trace->flush();
    }
  }
  
  if (trace_enabled) {
    trace->close();
    delete trace;
  }

  delete top;
  return 0;
}
