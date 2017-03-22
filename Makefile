export

CC := gcc
CFLAGS := -std=gnu99 -Wall -pedantic -Werror -O3 -DASSERT

PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/libs

all: arraylist darray fifo filebuffer getch heap list list2d sort stack ufset

arraylist:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

darray:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

fifo:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

filebuffer:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

getch:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

heap:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

list:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

list2d:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

sort:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

stack:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

ufset:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

clean:
	$(MAKE) -f $(SDIR)/arraylist/Makefile clean
	$(MAKE) -f $(SDIR)/darray/Makefile clean
	$(MAKE) -f $(SDIR)/fifo/Makefile clean
	$(MAKE) -f $(SDIR)/filebuffer/Makefile clean
	$(MAKE) -f $(SDIR)/getch/Makefile clean
	$(MAKE) -f $(SDIR)/heap/Makefile clean
	$(MAKE) -f $(SDIR)/list/Makefile clean
	$(MAKE) -f $(SDIR)/list2d/Makefile clean
	$(MAKE) -f $(SDIR)/sort/Makefile clean
	$(MAKE) -f $(SDIR)/stack/Makefile clean
	$(MAKE) -f $(SDIR)/ufset/Makefile clean
