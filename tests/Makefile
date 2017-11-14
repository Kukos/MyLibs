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

TEST_SRC := $(TEST_DIR)/src
TEST_EXEC_NAME := test.out

VALGRIND := valgrind
VALGRIND_FLAGS := --leak-check=full
PASS_STRING := All heap blocks were freed -- no leaks are possible

all: arraylist avl bst common compiler cstring darray fifo filebuffer heap klist list list2d rbt ringbuffer search sort stack trie ufset

arraylist:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

avl:
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
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/avl/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile run --no-print-directory

clean:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/avl/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile clean --no-print-directory

memcheck:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/avl/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile memcheck --no-print-directory