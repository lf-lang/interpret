`timescale 1ns / 1ps

module z1top(
  input CLK_125MHZ_FPGA,
  output [3:0] LEDS,
  output UART_TX,
  input UART_RX
);

  // Outputs
  wire sysclk;
  
  clk_wiz_0 inst(
      // Clock out ports  
      .clk_out1(sysclk),
      // Status and control signals               
      .reset(reset), 
      .locked(locked),
     // Clock in ports
      .clk_in1(CLK_125MHZ_FPGA)
  );
 
  Top top(
    .clock(sysclk),
    .io_gpio_out_0(LEDS),
    .io_uart_rx(UART_RX),
    .io_uart_tx(UART_TX)
  );
  
endmodule