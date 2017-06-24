export

define print_test_make
	$(if $(Q), @echo "[MAKE TEST]    $(1)")
endef

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

all: arraylist common compiler darray fifo filebuffer klist list list2d sort stack

arraylist:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

common:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

compiler:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

darray:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

filebuffer:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

fifo:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

klist:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

list:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

list2d:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

sort:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

stack:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

run:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile run --no-print-directory

clean:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile clean --no-print-directory
