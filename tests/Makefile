export

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

all: compiler common

compiler:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

common:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

run:
	@$(MAKE) -f $(TEST_SRC)/compiler/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile run --no-print-directory

clean:
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile clean --no-print-directory
