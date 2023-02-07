VERILOG_FPGA=fpga/interpret.v

$(VERILOG_FPGA): $(VERILOG_RAW)
	cp $(VERILOG_RAW) $(VERILOG_FPGA)
	cp $(RESOURCE_DIR)/DualPortBram.v $(FPGA_DIR)/DualPortBram.v
