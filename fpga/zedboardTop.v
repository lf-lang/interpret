`timescale 1ns / 1ps

module zedboardTop(
  input CLK_100MHZ_FPGA,
  output [7:0] LEDS,
  input [7:0] SWS,
  input [3:0] BTNS,
  output UART_TX,
  input UART_RX,
  input [3:0] JBI,
  output [3:0] JBO
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
      .clk_in1(CLK_100MHZ_FPGA)
  );
 
  Top top(
    .clock(sysclk),
    .io_gpio_out_0(LEDS),
    .io_uart_rx(UART_RX),
    .io_uart_tx(UART_TX),
    .io_gpio_in_0(SWS),
    .io_gpio_out_1(JBO),
    .io_gpio_in_1(JBI),
    .io_gpio_in_2(BTNS)
  );
  
endmodule