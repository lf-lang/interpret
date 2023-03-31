
TEST_DIR:=programs
TEST_RES_DIR:=$(TEST_DIR)/results
TEST_SCRIPTS=$(TEST_DIR)/scripts
TEST_SRCS= \
	$(TEST_DIR)/delayUntil \
	$(TEST_DIR)/interruptExpire \
	$(TEST_DIR)/noc/HelloWorld \
	$(TEST_DIR)/noc/PingPong \
	$(TEST_DIR)/threaded \
	$(TEST_DIR)/print

TEST_RESULTS = $(patsubst $(TEST_DIR)/%,$(TEST_DIR)/%/test_res.txt,$(TEST_SRCS))

RES_PARSER := $(INTERPRET_ROOT_DIR)/programs/scripts/test_result_parse.sh
IP_EMU := $(INTERPRET_ROOT_DIR)/emulator/ip-emu

.PHONY: integration-tests
integration-tests: $(TEST_RESULTS) bootloader-test
	@echo FINISHED

# For all TEST_SRCS, go into the directory. Build the test program 
# run the emulator on it (so env.bash must be called). And parse results 
$(TEST_DIR)/%/test_res.txt: $(TEST_DIR)/%
	@echo Executing $^
	@cd $^; make recompile
	@cd $^; make run | tee test_res.txt
	@$(RES_PARSER) $@

bootloader-test:
	@cd programs/helloWorld; make clean; make app;
	@cd programs/bootloader; make recompile; 
	@cd programs/bootloader; $(IP_EMU) bootloader.mem ../helloWorld/helloworld.app | tee test_res.txt
	@cd programs/bootloader; $(RES_PARSER) test_res.txt

.PHONY: integration-clean
# Loop through all the test dirs and do `make clean` which should remove all
# generated files. It is done in bash oneliner to enable cd'ing into the directory
# and doing some commands inside it.
integration-clean: $(TEST_SRCS)
	@for test in $^ ; do \
	 	cur=$$(pwd);\
		cd $$test; \
		make clean; \
		cd $$cur; \
	done
