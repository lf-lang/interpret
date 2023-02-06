// Toplevel module for the hypothetical "big" fpga. Just used to get synthesis results.
`timescale 1ns / 1ps

module bigTop(
  input CLK_100MHZ_FPGA,
  output [7:0] LEDS,
  input [7:0] SWS,
  input [3:0] BTNS,
  output UART_TX,
  input UART_RX,
  input [3:0] JBI,
  output [3:0] JBO
);

 
  Top top(
    .clock(CLK_100MHZ_FPGA),
    .io_gpio_out_0(LEDS),
    .io_uart_rx(UART_RX),
    .io_uart_tx(UART_TX),
    .io_gpio_in_0(SWS),
    .io_gpio_out_1(JBO),
    .io_gpio_in_1(JBI),
    .io_gpio_in_2(BTNS)
  );
  
endmodule
