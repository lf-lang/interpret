
#  
# TCL script automating synthesis and resource utilzation print-out
# on a big FPGA (Virtex-UltraScale VCU110 Evaluation Platform)
puts "Running synthesis, implementation and programming script for BIG fpga"
set outputDir ./vivado
set projectName interpret_big

# Check if the number of command line arguments is exactly one
if { $argc != 1 } {
  puts "Error: exactly one argument is required"
  exit 1
}

set config [lindex $argv 0]

puts "Running resource utilization evaluation with config=$config"

file mkdir $outputDir/$projectName
create_project -force -part  xcku035-fbva900-1-c $outputDir/$projectName

puts "Adding design sources"
# Add default ISPM contents with bootloader
add_files -norecurse ispm.mem

# Add verilog sources
add_files -norecurse {DualPortBram.v bigTop.v interpret.v}

# Add constraints file
add_files -fileset constrs_1 -norecurse big.xdc
update_compile_order -fileset sources_1

puts "Creating clocking wizard IP"
# Create clocking wizard IP to control the frequency of the CPU

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

exit

