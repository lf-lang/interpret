
# TCL script automating synthesis and resource utilzation print-out
# on a big FPGA (Virtex-UltraScale VCU110 Evaluation Platform)
puts "Running synthesis, implementation and programming script for VCU110"
set outputDir ./vivado
set projectName interpret_vcu110

# Check if the number of command line arguments is exactly one
if { $argc != 1 } {
  puts "Error: exactly one argument is required"
  exit 1
}


set config [lindex $argv 1]

file mkdir $outputDir/$projectName
create_project -force -part xc7z020clg484-1 $outputDir/$projectName

puts "Adding design sources"
# Add default ISPM contents with bootloader
add_files -norecurse ispm.mem

# Add verilog sources
add_files -norecurse {DualPortBram.v zedboardTop.v interpret.v}

# Add constraints file
add_files -fileset constrs_1 -norecurse zedboard.xdc
update_compile_order -fileset sources_1

puts "Creating clocking wizard IP"
# Create clocking wizard IP to control the frequency of the CPU
create_ip -name clk_wiz -vendor xilinx.com -library ip -version 6.0 -module_name clk_wiz_0
set_property -dict [list CONFIG.CLKOUT1_REQUESTED_OUT_FREQ {50} CONFIG.MMCM_CLKOUT0_DIVIDE_F {20.000} CONFIG.CLKOUT1_JITTER {151.636}] [get_ips clk_wiz_0]
update_compile_order -fileset sources_1

puts "Running synthesis"
# Synthesis
reset_run synth_1
launch_runs synth_1 -jobs 15
wait_on_runs synth_1

open_run synth_1

# Write resource utilization to a file
report_utilization -file "util_$config.txt"

# close project
puts "Finished"
close_project