export

CC := gcc
CFLAGS := -std=gnu99 -Wall -pedantic -Werror -O3

PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/libs

all: filebuffer getch sort darray stack fifo list list2d

filebuffer:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

getch:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

sort:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

darray:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

stack:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

fifo:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

arraylist:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile

list:
	mkdir -p $(ODIR)/$@ && $(MAKE) -f $(SDIR)/$@/Makefile


clean:
	$(MAKE) -f $(SDIR)/filebuffer/Makefile clean
	$(MAKE) -f $(SDIR)/getch/Makefile clean
	$(MAKE) -f $(SDIR)/sort/Makefile clean
	$(MAKE) -f $(SDIR)/darray/Makefile clean
	$(MAKE) -f $(SDIR)/stack/Makefile clean
	$(MAKE) -f $(SDIR)/fifo/Makefile clean
	$(MAKE) -f $(SDIR)/arraylist/Makefile clean
	$(MAKE) -f $(SDIR)/list/Makefile clean
