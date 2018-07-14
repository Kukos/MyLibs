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

VALGRIND := valgrind
VALGRIND_MEMCHECK_FLAGS := --leak-check=full
VALGRIND_MEMUSAGE_FLAGS := --tool=massif --stacks=yes
PASS_STRING := All heap blocks were freed -- no leaks are possible

all: arraylist avl base64 bitset bst ccache common compiler crc cstring darray fifo filebuffer hash heap klist list list2d partition rbt ringbuffer search selection sort stack tcp tls trie ufset

arraylist:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

avl:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

base64:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

bitset:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

bst:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

ccache:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

common:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

compiler:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

crc:
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

hash:
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

partition:
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

selection:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

sort:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

stack:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

tcp:
	$(call print_test_make,$@)
	$(Q)$(MAKE) -f $(TEST_SRC)/$@/Makefile --no-print-directory

tls:
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
	$(MAKE) -f $(TEST_SRC)/base64/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bitset/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ccache/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/crc/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/hash/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/partition/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/selection/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tcp/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tls/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile run --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile run --no-print-directory

clean:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/avl/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/base64/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bitset/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ccache/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/crc/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/hash/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/partition/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/selection/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tcp/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tls/Makefile clean --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile clean --no-print-directory

memcheck:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/avl/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/base64/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bitset/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ccache/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/crc/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/hash/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/partition/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/selection/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tcp/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tls/Makefile memcheck --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile memcheck --no-print-directory

memusage:
	$(Q)$(MAKE) -f $(TEST_SRC)/arraylist/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/avl/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/base64/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bitset/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/bst/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ccache/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/common/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/compiler/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/crc/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/cstring/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/darray/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/fifo/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/filebuffer/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/hash/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/heap/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/klist/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/list2d/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/partition/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/rbt/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ringbuffer/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/search/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/selection/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/sort/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/stack/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/trie/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tcp/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/tls/Makefile memusage --no-print-directory && \
	$(MAKE) -f $(TEST_SRC)/ufset/Makefile memusage --no-print-directory