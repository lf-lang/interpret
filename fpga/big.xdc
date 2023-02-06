# Constraint file for "big" FPGA. This is just used for synthesizing big designs
# and getting resource utilization
create_clock -period 10.000 -name CLK_100MHZ_FPGA -waveform {0.000 5.000} -add [get_ports CLK_100MHZ_FPGA]

