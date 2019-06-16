export

#Main Makefile
# Author: Michal Kukowski
# email: michalkukowski10@gmail.com

# Targets:
# help - print targets (visible for user) + parameters
# install[P = Path] - install libs to path P or default Path
# test - make static tests
# all[D=1] - make all libraries when D ==1 then target will be build in Debug mode
# memcheck - make memcheck using valgrind for all tests
# memusage - prepare mem measurements

# Makefile supports Verbose mode, put V=1 after target name to set verbose mode


MV := mv
RM := rm -rf
CP := cp 2>/dev/null
AR := ar rcs
WC := wc
BC := bc
GREP := grep -q

CC := gcc

CCWARNINGS := 	-Wall -Wextra -pedantic -Wcast-align \
				-Winit-self -Wlogical-op -Wmissing-include-dirs \
			 	-Wredundant-decls -Wshadow -Wstrict-overflow=5 -Wundef  \
				-Wwrite-strings -Wpointer-arith -Wmissing-declarations \
				-Wuninitialized -Wold-style-definition -Wstrict-prototypes \
				-Wmissing-prototypes -Wswitch-default -Wbad-function-cast \
				-Wnested-externs -Wconversion -Wunreachable-code

CFLAGS := -std=gnu99 $(CCWARNINGS) -Werror -O3 -DASSERT -rdynamic

CC_TEST_WARNINGS := -Wall -pedantic

C_TEST_FLAGS := -std=gnu99 $(CC_TEST_WARNINGS) -Werror -O3 -DASSERT -DSILENT_ERROR -rdynamic -D_GNU_SOURCE

PROJECT_DIR := $(shell pwd)

# Global directoriec
IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/obj

SCRIPT_DIR := $(PROJECT_DIR)/scripts
INSTALL_SCRIPT_NAME := install_libs.sh

# Object directories
O_LIBS := $(ODIR)/libs
O_HEADERS := $(ODIR)/include

# Test directories
TEST_DIR := $(PROJECT_DIR)/tests

# ALL headers
HEADERS=$(foreach d, $(IDIR), -I$d)

# Frameworks
F_SLIST := $(IDIR)/slist.h
F_ULIST := $(IDIR)/ulist.h
F_TREE := $(IDIR)/tree.h
F_ITERATORS := $(IDIR)/iterators.h
F_GENERIC := $(IDIR)/generic.h
F_COMMON := $(IDIR)/common.h
F_COMPILER := $(IDIR)/compiler.h
F_TEST := $(IDIR)/test.h

# Most useful frameworks for libs (maybe unsused by your lib but dependency is harmless)
F_LIB := $(F_COMMON) $(F_COMPILER) $(F_GENERIC) $(F_ITERATORS)

# Global (common srcs)
D_LOG := $(SDIR)
I_LOG := $(IDIR)/log.h $(F_COMMON) $(F_COMPILER)
S_LOG := $(D_LOG)/log.c

D_ASSERT := $(SDIR)
I_ASSER := $(IDIR)/assert.h $(F_COMMON) $(F_COMPILER) $(I_LOG)
S_ASSERT := $(D_ASSERT)/assert.c $(S_LOG)

# For each target put here
# D_X -> directory for libX
# I_X -> headers of libX
# S_X -> srcs of libX
D_PARTITION := $(SDIR)/partition
I_PARTITION := $(IDIR)/partition.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_PARTITION := $(wildcard $(D_PARTITION)/*.c) $(S_LOG) $(S_ASSERT)

D_SORT := $(SDIR)/sort
I_SORT := $(IDIR)/sort.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_PARTITION)
S_SORT := $(wildcard $(D_SORT)/*.c) $(S_LOG) $(S_ASSERT) $(S_PARTITION)

D_SEARCH := $(SDIR)/search
I_SEARCH := $(IDIR)/search.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_SEARCH := $(wildcard $(D_SEARCH)/*.c) $(S_LOG) $(S_ASSERT)

D_SELECTION := $(SDIR)/selection
I_SELECTION := $(IDIR)/selection.h $(F_LIB) $(I_LOG) $(I_PARTITION) $(I_SORT) $(I_SEARCH)
S_SELECTION := $(wildcard $(D_SELECTION)/*.c) $(S_LOG) $(S_PARTITION) $(S_SORT) $(S_SEARCH)

D_ARRAY := $(SDIR)/array
I_ARRAY := $(IDIR)/array.h $(F_LIB) $(I_LOG) $(I_SORT) $(I_SEARCH) $(I_SELECTION)
S_ARRAY := $(wildcard $(D_ARRAY)/*.c) $(S_LOG) $(S_SORT) $(S_SEARCH) $(S_SELECTION)

D_DARRAY := $(SDIR)/darray
I_DARRAY := $(IDIR)/darray.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_ARRAY)
S_DARRAY := $(wildcard $(D_DARRAY)/*.c) $(S_LOG) $(S_ASSERT) $(S_ARRAY)

D_ARRAYLIST := $(SDIR)/arraylist
I_ARRAYLIST := $(IDIR)/arraylist.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(F_ULIST)
S_ARRAYLIST := $(wildcard $(D_ARRAYLIST)/*.c) $(S_LOG) $(S_ASSERT)

D_KLIST := $(SDIR)/klist
I_KLIST := $(IDIR)/klist.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_KLIST := $(wildcard $(D_KLIST)/*.c) $(S_LOG) $(S_ASSERT)

D_LIST := $(SDIR)/list
I_LIST := $(IDIR)/list.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(F_SLIST)
S_LIST := $(wildcard $(D_LIST)/*.c) $(S_LOG) $(S_ASSERT)

D_LIST2D := $(SDIR)/list2d
I_LIST2D := $(IDIR)/list2d.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(F_SLIST)
S_LIST2D := $(wildcard $(D_LIST2D)/*.c) $(S_LOG) $(S_ASSERT)

D_BST := $(SDIR)/bst
I_BST := $(IDIR)/bst.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(F_TREE)
S_BST := $(wildcard $(D_BST)/*.c) $(S_LOG) $(S_ASSERT)

D_RBT := $(SDIR)/rbt
I_RBT := $(IDIR)/rbt.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(F_TREE)
S_RBT := $(wildcard $(D_RBT)/*.c) $(S_LOG) $(S_ASSERT)

D_AVL := $(SDIR)/avl
I_AVL := $(IDIR)/avl.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(F_TREE)
S_AVL := $(wildcard $(D_AVL)/*.c) $(S_LOG) $(S_ASSERT)

D_TRIE := $(SDIR)/trie
I_TRIE := $(IDIR)/trie.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_DARRAY)
S_TRIE := $(wildcard $(D_TRIE)/*.c) $(S_LOG) $(S_ASSERT) $(S_DARRAY)

D_CSTRING := $(SDIR)/cstring
I_CSTRING := $(IDIR)/cstring.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_DARRAY)
S_CSTRING := $(wildcard $(D_CSTRING)/*.c) $(S_LOG) $(S_ASSERT) $(S_DARRAY)

D_FIFO := $(SDIR)/fifo
I_FIFO := $(IDIR)/fifo.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_FIFO := $(wildcard $(D_FIFO)/*.c) $(S_LOG) $(S_ASSERT)

D_FILEBUFFER := $(SDIR)/filebuffer
I_FILEBUFFER := $(IDIR)/filebuffer.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_FILEBUFFER := $(wildcard $(D_FILEBUFFER)/*.c) $(S_LOG) $(S_ASSERT)

D_GETCH := $(SDIR)/getch
I_GETCH := $(IDIR)/getch.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_GETCH := $(wildcard $(D_GETCH)/*.c) $(S_LOG) $(S_ASSERT)

D_HASH := $(SDIR)/hash
I_HASH := $(IDIR)/hash.h $(F_LIB) $(I_LOG)
S_HASH := $(wildcard $(D_HASH)/*.c) $(S_LOG)

D_HEAP := $(SDIR)/heap
I_HEAP := $(IDIR)/heap.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_DARRAY)
S_HEAP := $(wildcard $(D_HEAP)/*.c) $(S_LOG) $(S_ASSERT) $(S_DARRAY)

D_RINGBUFFER := $(SDIR)/ringbuffer
I_RINGBUFFER := $(IDIR)/ringbuffer.h $(F_LIB) $(I_LOG) $(I_ASSERT)
S_RINGBUFFER := $(wildcard $(D_RINGBUFFER)/*.c) $(S_LOG) $(S_ASSERT)

D_STACK := $(SDIR)/stack
I_STACK := $(IDIR)/stack.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_DARRAY)
S_STACK := $(wildcard $(D_STACK)/*.c) $(S_LOG) $(S_ASSERT) $(S_DARRAY)

D_UFSET := $(SDIR)/ufset
I_UFSET := $(IDIR)/ufset.h $(F_LIB) $(I_LOG) $(I_ASSERT) $(I_DARRAY)
S_UFSET := $(wildcard $(D_UFSET)/*.c) $(S_LOG) $(S_ASSERT) $(S_DARRAY)

D_CRC := $(SDIR)/crc
I_CRC := $(IDIR)/crc.h $(F_LIB) $(I_LOG)
S_CRC := $(wildcard $(D_CRC)/*.c) $(S_LOG)

D_BITSET := $(SDIR)/bitset
I_BITSET := $(IDIR)/bitset.h $(F_LIB) $(I_LOG)
S_BITSET := $(wildcard $(D_BITSET)/*.c) $(S_LOG)

D_HISTOGRAM := $(SDIR)/histogram
I_HISTOGRAM := $(IDIR)/histogram.h $(F_LIB) $(I_LOG) $(I_ASSER)
S_HISTOGRAM := $(wildcard $(D_HISTOGRAM)/*.c) $(S_LOG) $(S_ASSERT)

D_BASE64 := $(SDIR)/base64
I_BASE64 := $(IDIR)/base64.h $(F_LIB) $(I_LOG)
S_BASE64 := $(wildcard $(D_BASE64)/*.c) $(S_LOG)

D_CCACHE := $(SDIR)/ccache
I_CCACHE := $(IDIR)/ccache.h $(F_LIB) $(I_LOG) $(I_CSTRING)
S_CCACHE := $(wildcard $(D_CCACHE)/*.c) $(S_LOG) $(S_CSTRING)

D_TCP := $(SDIR)/tcp
I_TCP := $(IDIR)/tcp.h $(F_LIB) $(I_LOG)
S_TCP := $(wildcard $(D_TCP)/*.c) $(S_LOG)

D_TLS := $(SDIR)/tls
I_TLS := $(IDIR)/tls.h $(F_LIB) $(I_LOG)
S_TLS := $(wildcard $(D_TLS)/*.c) $(S_LOG)

D_TUPLE := $(SDIR)/tuple
I_TUPLE := $(IDIR)/tuple.h $(F_LIB) $(I_LOG)
S_TUPLE := $(wildcard $(D_TUPLE)/*.c) $(S_LOG)

# Needed to testting
TEST_COMMON_INC := $(F_TEST) $(I_COMPILER) $(I_COMMON) $(I_SORT) $(I_SEARCH)
TEST_COMMON_SRC := $(S_COMPILER) $(S_COMMON) $(S_SORT) $(S_SEARCH)

# Verbose mode
ifeq ("$(origin V)", "command line")
  VERBOSE = $(V)
endif

ifndef VERBOSE
  VERBOSE = 0
endif

ifeq ($(VERBOSE),1)
  Q =
else
  Q = @
endif

INSTALL_PATH =

# Path for install libs
ifeq ("$(origin P)", "command line")
  INSTALL_PATH = $(P)
endif

# DEBUG MODE
ifeq ("$(origin D)", "command line")
  CFLAGS += -g3 -D DEBUG_MODE -D TRACE_MODE
endif

# Print functions
define print_info
	$(if $(Q), @echo "$(1)")
endef

define print_make
	$(if $(Q), @echo "[MAKE]        $(1)")
endef

define print_cc
	$(if $(Q), @echo "[CC]          $$(1)")
endef

define print_ar
	$(if $(Q), @echo "[AR]          $$(1)")
endef

define print_bin
	$(if $(Q), @echo "[BIN]         $$(1)")
endef

all: prepare array arraylist avl base64 bitset bst ccache crc cstring darray fifo filebuffer getch hash heap histogram klist list list2d partition rbt ringbuffer search selection sort stack tcp tls trie tuple ufset final

prepare:
	$(call print_info,Preparing dirs)
	$(Q)mkdir -p $(ODIR) && mkdir -p $(O_LIBS) && mkdir -p $(O_HEADERS)

array: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

arraylist: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

avl: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

base64: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

bitset: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

bst: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

ccache: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

crc: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

cstring: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

darray: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

fifo: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

filebuffer: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

getch: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

hash: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

heap: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

histogram: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

klist: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

list: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

list2d: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

partition: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

rbt: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

ringbuffer: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

search: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

selection: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

sort: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

stack: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

tcp: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

tls: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

trie: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

tuple: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

ufset: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory


final: prepare array arraylist avl base64 bitset bst ccache crc cstring darray fifo filebuffer getch hash heap histogram klist list list2d partition rbt ringbuffer search sort stack tcp tls trie tuple ufset
	$(call print_info,Finalizing)
	$(Q)$(CP) $(IDIR)/common.h $(O_HEADERS) && \
	$(CP) $(IDIR)/compiler.h $(O_HEADERS) && \
	$(CP) $(IDIR)/bits.h $(O_HEADERS) && \
	$(CP) $(IDIR)/terminal_colors.h $(O_HEADERS) && \
	$(CP) $(IDIR)/template_array.h $(O_HEADERS) && \
	$(CP) $(IDIR)/assert.h $(O_HEADERS) && \
	$(CP) $(IDIR)/log.h $(O_HEADERS) && \
	$(CP) $(IDIR)/iterators.h $(O_HEADERS) && \
	$(CP) $(IDIR)/test.h $(O_HEADERS) && \
	$(CP) $(IDIR)/generic.h $(O_HEADERS) && \
	$(CP) $(IDIR)/tree.h $(O_HEADERS) && \
	$(CP) $(IDIR)/ulist.h $(O_HEADERS) && \
	$(CP) $(IDIR)/slist.h $(O_HEADERS) && \
	$(CP) $(IDIR)/nargs.h $(O_HEADERS) && \
	$(CP) $(SDIR)/log.c $(O_LIBS) && \
	$(CP) $(SDIR)/assert.c $(O_LIBS)

test:
	$(Q)$(MAKE) -f $(TEST_DIR)/Makefile --no-print-directory && \
	$(MAKE) -f $(TEST_DIR)/Makefile run --no-print-directory

clean:
	$(call print_info,Cleaning)
	$(Q)$(MAKE) -f $(D_ARRAY)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_ARRAYLIST)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_AVL)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_BASE64)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_BITSET)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_BST)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_CCACHE)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_CRC)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_CSTRING)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_DARRAY)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_FIFO)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_FILEBUFFER)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_GETCH)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_HASH)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_HEAP)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_HISTOGRAM)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_KLIST)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_LIST)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_LIST2D)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_PARTITION)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_RBT)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_RINGBUFFER)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_SEARCH)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_SELECTION)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_SORT)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_STACK)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_TCP)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_TLS)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_TRIE)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_TUPLE)/Makefile clean --no-print-directory && \
	$(MAKE) -f $(D_UFSET)/Makefile clean --no-print-directory
	$(Q)$(MAKE) -f $(TEST_DIR)/Makefile clean --no-print-directory
	$(Q)$(RM) $(ODIR)

install:
	$(Q)cd $(SCRIPT_DIR) && \
	./$(INSTALL_SCRIPT_NAME) $(INSTALL_PATH) && \
	cd -

memcheck:
	$(Q)$(MAKE) -f $(TEST_DIR)/Makefile --no-print-directory && \
	$(MAKE) -f $(TEST_DIR)/Makefile memcheck --no-print-directory

memusage:
	$(Q)$(MAKE) -f $(TEST_DIR)/Makefile --no-print-directory && \
	$(MAKE) -f $(TEST_DIR)/Makefile memusage --no-print-directory

regression: clean all test memcheck

help:
	@echo "Main Makefile"
	@echo -e
	@echo "Targets:"
	@echo "    all[D=1]          - build libraries, D=1 --> debug mode"
	@echo "    test              - make static tests"
	@echo "    install[P = Path] - install libs to path P or default Path"
	@echo "    memcheck          - make mem check using valgrind for tests"
	@echo "    memusage          - prepare mem measurements"
	@echo "    regression        - regression tests use it before commit to master"
	@echo -e
	@echo "Makefile supports Verbose mode when V=1"
