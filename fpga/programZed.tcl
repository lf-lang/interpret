# Open existing project. Might not be necassary?
# open_project /home/erling/dev/interpret/vivado/zed/zed.xpr

open_hw_manager
connect_hw_server -allow_non_jtag
open_hw_target
set_property PROGRAM.FILE {zedboard.bit} [get_hw_devices xc7z020_1]
current_hw_device [get_hw_devices xc7z020_1]
refresh_hw_device -update_hw_probes false [lindex [get_hw_devices xc7z020_1] 0]

# Program Zedboard
program_hw_devices [get_hw_devices xc7z020_1]