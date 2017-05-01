export

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

all: common compiler filebuffer sort

common:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

compiler:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

filebuffer:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

sort:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

run:
	@$(MAKE) -f $(TEST_SRC)/common/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile run --no-print-directory


clean:
	$(MAKE) -f $(TEST_SRC)/common/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile clean --no-print-directory
