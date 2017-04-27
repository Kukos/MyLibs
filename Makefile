export

CC := gcc
CFLAGS := -std=gnu99 -Wall -pedantic -Werror -O3 -DASSERT

PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/obj
O_LIBS := $(ODIR)/libs
O_HEADERS := $(ODIR)/include

all: prepare arraylist avl bst darray fifo filebuffer getch heap list list2d rbt sort stack trie ufset final

prepare:
	mkdir -p $(ODIR) && mkdir -p $(O_LIBS) && mkdir -p $(O_HEADERS)

arraylist:
	$(MAKE) -f $(SDIR)/$@/Makefile

avl:
	$(MAKE) -f $(SDIR)/$@/Makefile

bst:
	$(MAKE) -f $(SDIR)/$@/Makefile

darray:
	$(MAKE) -f $(SDIR)/$@/Makefile

fifo:
	$(MAKE) -f $(SDIR)/$@/Makefile

filebuffer:
	$(MAKE) -f $(SDIR)/$@/Makefile

getch:
	$(MAKE) -f $(SDIR)/$@/Makefile

heap:
	$(MAKE) -f $(SDIR)/$@/Makefile

list:
	$(MAKE) -f $(SDIR)/$@/Makefile

list2d:
	$(MAKE) -f $(SDIR)/$@/Makefile

rbt:
	$(MAKE) -f $(SDIR)/$@/Makefile

sort:
	$(MAKE) -f $(SDIR)/$@/Makefile

stack:
	$(MAKE) -f $(SDIR)/$@/Makefile

trie:
	$(MAKE) -f $(SDIR)/$@/Makefile

ufset:
	$(MAKE) -f $(SDIR)/$@/Makefile

final:
	cp $(IDIR)/common.h $(O_HEADERS) && \
	cp $(IDIR)/compiler.h $(O_HEADERS) && \
	cp $(IDIR)/assert.h $(O_HEADERS) && \
	cp $(IDIR)/log.h $(O_HEADERS) && \
	cp $(IDIR)/iterators.h $(O_HEADERS) && \
	cp $(IDIR)/test.h $(O_HEADERS) && \
	cp $(SDIR)/log.c $(O_LIBS)

clean:
	rm -rf $(ODIR)
