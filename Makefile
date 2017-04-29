export

CC := gcc
CFLAGS := -std=gnu99 -Wall -pedantic -Werror -O3 -DASSERT

PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/obj
O_LIBS := $(ODIR)/libs
O_HEADERS := $(ODIR)/include
TEST_DIR := $(PROJECT_DIR)/tests

all: prepare arraylist avl bst darray fifo filebuffer getch heap list list2d rbt sort stack trie ufset final

prepare:
	mkdir -p $(ODIR) && mkdir -p $(O_LIBS) && mkdir -p $(O_HEADERS)

arraylist:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

avl:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

bst:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

darray:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

fifo:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

filebuffer:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

getch:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

heap:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

list:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

list2d:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

rbt:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

sort:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

stack:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

trie:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

ufset:
	$(MAKE) -f $(SDIR)/$@/Makefile --no-print-directory

final:
	cp $(IDIR)/common.h $(O_HEADERS) && \
	cp $(IDIR)/compiler.h $(O_HEADERS) && \
	cp $(IDIR)/assert.h $(O_HEADERS) && \
	cp $(IDIR)/log.h $(O_HEADERS) && \
	cp $(IDIR)/iterators.h $(O_HEADERS) && \
	cp $(IDIR)/test.h $(O_HEADERS) && \
	cp $(IDIR)/generic.h $(O_HEADERS) && \
	cp $(SDIR)/log.c $(O_LIBS)

test:
	$(MAKE) -f $(TEST_DIR)/Makefile --no-print-directory && \
	$(MAKE) -f $(TEST_DIR)/Makefile run --no-print-directory

clean:
	rm -rf $(ODIR)
	$(MAKE) -f $(TEST_DIR)/Makefile clean --no-print-directory
