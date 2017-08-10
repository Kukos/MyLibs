export

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

C_TEST_FLAGS := -std=gnu99 $(CC_TEST_WARNINGS) -Werror -O3 -DASSERT -DSILENT_ERROR -rdynamic


PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/obj
O_LIBS := $(ODIR)/libs
O_HEADERS := $(ODIR)/include
TEST_DIR := $(PROJECT_DIR)/tests

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

define print_info
	$(if $(Q), @echo "$(1)")
endef

define print_make
	$(if $(Q), @echo "[MAKE]    $(1)")
endef

all: prepare arraylist avl bst cstring darray fifo filebuffer getch heap klist list list2d rbt ringbuffer search sort stack trie ufset final

prepare:
	$(call print_info,Preparing dirs)
	$(Q)mkdir -p $(ODIR) && mkdir -p $(O_LIBS) && mkdir -p $(O_HEADERS)

arraylist: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

avl: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

bst: prepare
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

heap: prepare
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

rbt: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

ringbuffer: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

search: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

sort: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

stack: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

trie: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

ufset: prepare
	$(call print_make,$@)
	$(Q)$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory


final: prepare
	$(call print_info,Finalizing)
	$(Q)cp $(IDIR)/common.h $(O_HEADERS) && \
	cp $(IDIR)/compiler.h $(O_HEADERS) && \
	cp $(IDIR)/assert.h $(O_HEADERS) && \
	cp $(IDIR)/log.h $(O_HEADERS) && \
	cp $(IDIR)/iterators.h $(O_HEADERS) && \
	cp $(IDIR)/test.h $(O_HEADERS) && \
	cp $(IDIR)/generic.h $(O_HEADERS) && \
	cp $(IDIR)/tree.h $(O_HEADERS) && \
	cp $(IDIR)/ulist.h $(O_HEADERS) && \
	cp $(IDIR)/slist.h $(O_HEADERS) && \
	cp $(SDIR)/log.c $(O_LIBS) && \
	cp $(SDIR)/assert.c $(O_LIBS)

test:
	$(Q)$(MAKE) -f $(TEST_DIR)/Makefile --no-print-directory && \
	$(MAKE) -f $(TEST_DIR)/Makefile run --no-print-directory

clean:
	$(call print_info,Cleaning)
	$(Q)rm -rf $(ODIR)
	$(Q)$(MAKE) -f $(TEST_DIR)/Makefile clean --no-print-directory