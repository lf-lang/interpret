SRC_DIR = src/main/scala
MODULE = Top
FPGA_DIR = fpga
EMULATOR_DIR = emulator
SCRIPTS_DIR = flexpret/scripts
BUILD_DIR = build
RESOURCE_DIR = flexpret/src/main/resources



# Compiler options.
CXX = g++
CXXFLAGS = -g -O2

# Test directory
TEST_DIR = programs/tests

# -----------------------------------------------------------------------------
# Core and target configuration
# -----------------------------------------------------------------------------
THREADS ?= 8
FLEXPRET ?= false
ISPM_KBYTES ?= 16
DSPM_KBYTES ?= 16
MUL ?= false
SUFFIX ?= all
N_CORES ?= 4

# Construct core configuration string (used for directory naming).
# Note: '?=' not used so string is only constructed once.
CORE_CONFIG := $(THREADS)t$(if $(findstring true, $(FLEXPRET)),f)-$(ISPM_KBYTES)i-$(DSPM_KBYTES)d$(if $(findstring true, $(MUL)),-mul)-$(SUFFIX)
SOC_CONFIG := $(N_CORES)

# Default will build target and selected programs.
all: $(TARGET)

# -----------------------------------------------------------------------------
# FPGA
# -----------------------------------------------------------------------------

# Raw Verilog generated from FIRRTL.
VERILOG_RAW = build/$(MODULE).v
verilog_raw: $(VERILOG_RAW)

# --no-dedup flag is to make it possible to load programs directly into the ISpms
$(VERILOG_RAW): 
	sbt 'run $(CORE_CONFIG) $(SOC_CONFIG) --no-dedup --target-dir $(BUILD_DIR)'

# FPGA Verilog generation
# FPGA_SRC_DIR = $(FPGA_DIR)/generated-$(CORE_CONFIG)
VERILOG_FPGA = $(FPGA_DIR)/$(MODULE).v
# $(FPGA_SRC_DIR)/$(MODULE).v

# Must provide rules for generating verilog file $(VERILOG)
include $(FPGA_DIR)/fpga.mk

fpga: $(VERILOG_FPGA)

# Provide rules for simulator
include $(EMULATOR_DIR)/emulator.mk

# Alias
emulator: $(EMULATOR_BIN)

# Integration tests
include programs/integration-tests.mk

# -----------------------------------------------------------------------------
#  Tests
# -----------------------------------------------------------------------------
unit-tests:
	sbt 'test'

test: unit-tests integration-tests
# -----------------------------------------------------------------------------
#  Cleanup
# -----------------------------------------------------------------------------

# Remake emulator
remulator: clean emulator

# Clean the emulator and the generated source.
clean: integration-clean
	rm -rf $(FPGA_DIR)/generated-src
	rm -rf $(FPGA_DIR)/build
	rm -rf $(FPGA_DIR)/interpret.v
	rm -f $(EMULATOR_BIN)
	rm -rf ./build
	rm -rf emulator/obj_dir
	rm -f emulator/*.v
	rm -rf out

# Clean for all configurations, targets, and test outputs.
cleanall: integration-clean
	rm -rf $(FPGA_DIR)/generated-src
	rm -rf $(FPGA_DIR)/build
	rm -f $(EMULATOR_BIN)
	rm -rf ./build
	rm -rf emulator/obj_dir
	rm -f emulator/$(MODULE).sim.v
	rm -rf out
	rm -f fpga/firrtl.jar
	rm -f mill
	rm -rf out
	rm -rf test_run_dir
	cd $(TEST_DIR) && $(MAKE) clean

.PHONY: run fpga emulator remulator firrtl_raw verilog_raw clean cleanall test unit-tests
