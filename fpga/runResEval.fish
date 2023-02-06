# Fish script for synthesizing 2x2, 3x3 and 4x4 InterPRET
# TODO: Use a loop

make -C ../ clean
make -C ../ fpga N_CORES=4 DSPM_KBYTES=16 ISPM_KBYTES=16 

vivado -mode tcl -source runResourceEvalBig.tcl -tclargs 2x2

make -C ../ clean
make -C ../ fpga N_CORES=16 DSPM_KBYTES=16 ISPM_KBYTES=16 

vivado -mode tcl -source runResourceEvalBig.tcl -tclargs 4x4

make -C ../ clean
make -C ../ fpga N_CORES=9 DSPM_KBYTES=16 ISPM_KBYTES=16 

vivado -mode tcl -source runResourceEvalBig.tcl -tclargs 3x3

