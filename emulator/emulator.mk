# Helper fragment to help run flexpret with verilator.
# Authors:
# Edward Wang <edwardw@eecs.berkeley.edu>
# Shaokai Lin <shaokai@berkeley.edu>

EMULATOR_BIN = $(EMULATOR_DIR)/ip-verilator

$(EMULATOR_BIN): $(VERILOG_RAW) $(EMULATOR_DIR)/main.cpp $(EMULATOR_DIR)/uartsim.cpp
	cp $(RESOURCE_DIR)/DualPortBram.v $(EMULATOR_DIR)/DualPortBram.v
	cp $(VERILOG_RAW) $(EMULATOR_DIR)/$(MODULE).sim.v
	(cd $(EMULATOR_DIR) && verilator --cc $(MODULE).sim.v --timescale 1ns/1ns --exe --trace --build main.cpp uartsim.cpp)
	cp $(EMULATOR_DIR)/obj_dir/V$(MODULE) $(EMULATOR_BIN)
