
TEST_DIR:=programs
TEST_RES_DIR:=$(TEST_DIR)/results
TEST_SCRIPTS=$(TEST_DIR)/scripts
TEST_SRCS= \
	$(TEST_DIR)/DelayUntil \
	$(TEST_DIR)/InterruptExpire \
	$(TEST_DIR)/noc/HelloWorld \
	$(TEST_DIR)/noc/PingPong

TEST_RESULTS = $(patsubst $(TEST_DIR)/%,$(TEST_DIR)/%/test_res.txt,$(TEST_SRCS))

.PHONY: integration-tests
integration-tests: $(TEST_RESULTS)
	@echo FINISHED

$(TEST_DIR)/%/test_res.txt: $(TEST_DIR)/%
	@echo Executing $^
	@mkdir -p $(TEST_RES_DIR)
	@cd $^; make rebuild
	@cd $^; if ! (fp-emu > test_res.txt 2>&1); then continue; fi


	@bash programs/scripts/test_result_parse.sh $@

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
