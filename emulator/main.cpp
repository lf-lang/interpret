/*
 * main.cpp
 * C++ main entry point for Verilator simulation.
 *
 * Copyright 2021 Edward Wang <edwardw@eecs.berkeley.edu>
 */
#include "VTop.h"
#include "verilated.h"
#include "uartsim.h"

uint64_t timestamp = 0;

double sc_time_stamp() {
  return timestamp;
}

int main(int argc, char* argv[]) {
  Verilated::commandArgs(argc, argv);
  Verilated::traceEverOn(true);


  VTop* top = new VTop;
  
  if (argc == 2) {
    uartsim_init(argv[1]);
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
    if (timestamp > 10) {
      uartsim_print_rx(&top->io_uart_tx);
      uartsim_write(&top->io_uart_rx);
    }
    top->clock = 0;
    top->eval();
    timestamp+=1;
  }

  delete top;
  return 0;
}
