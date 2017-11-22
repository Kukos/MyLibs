export

MAX_MEMCHECK_LINE_LENGTH = 16

define print_test_make
	$(if $(Q), @echo "[MAKE TEST]   $(1)")
endef

# Mem check info
define print_memcheck
	$(if $(Q), @echo -n "[MEMTEST]    $$(1)"; printf "%*s" $$(shell echo "$(MAX_MEMCHECK_LINE_LENGTH) - $$(shell echo -n $$(1) | $(WC) -m)" | $(BC)) "")
endef

define passed_memcheck
    echo "\033[0;32m[PASSED]\033[0m"
endef

define failed_memcheck
    echo "\033[1;31m[FAILED]\033[0m"
endef

# Mem usage
define print_memusage
	$(if $(Q), @echo "[MEMUSAGE]   $$(1)")
endef

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

all: bptree

arraylist:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

avl:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

bptree:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

bst:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

common:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

compiler:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

cstring:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

darray:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

fifo:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

filebuffer:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

heap:
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

rbt:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

ringbuffer:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

search:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

sort:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

stack:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

trie:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

ufset:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

run:
	$(Q)$(MAKE) -f $(TEST_SRC)/bptree/Makefile run --no-print-directory

clean:
	$(Q)$(MAKE) -f $(TEST_SRC)/bptree/Makefile clean --no-print-directory
