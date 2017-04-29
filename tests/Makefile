export

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

all: compiler_and_common

compiler_and_common:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

run:
	@$(MAKE) -f $(TEST_SRC)/compiler_and_common/Makefile run --no-print-directory

clean:
	$(MAKE) -f $(TEST_SRC)/compiler_and_common/Makefile clean --no-print-directory
