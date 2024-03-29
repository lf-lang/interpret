
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

.PHONY: integration-tests
integration-tests: $(TEST_RESULTS) bootloader-test
	@echo FINISHED

# For all TEST_SRCS, go into the directory. Build the test program 
# run the emulator on it (so env.bash must be called). And parse results 
$(TEST_DIR)/%/test_res.txt: $(TEST_DIR)/%
	@echo Executing $^
	@cd $^; make rebuild
	@cd $^; if ! (ip-verilator > test_res.txt 2>&1); then continue; fi
	@test_result_parse.sh $@

bootloader-test:
	@cd programs/bootloader; riscv-clean.sh; make; compile_app.sh 1 hello hello.c
	@cd programs/bootloader; if ! (ip-verilator hello.app > test_res.txt 2>&1); then continue; fi
	@cd programs/bootloader; test_result_parse.sh test_res.txt

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
