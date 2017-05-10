export

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

all: arraylist common compiler darray fifo filebuffer sort stack

arraylist:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

common:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

compiler:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

darray:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

filebuffer:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

fifo:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

sort:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

stack:
	$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

run:
	@$(MAKE) -f $(TEST_SRC)/arraylist/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile run --no-print-directory

clean:
	$(MAKE) -f $(TEST_SRC)/arraylist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile clean --no-print-directory
