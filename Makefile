export

CC := gcc
CFLAGS := -std=gnu99 -Wall -pedantic -Werror -O3

PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/libs

all: filebuffer getch sort darray stack fifo

filebuffer:
	mkdir -p $(ODIR)/filebuffer && $(MAKE) -f $(SDIR)/filebuffer/Makefile

getch:
	mkdir -p $(ODIR)/getch && $(MAKE) -f $(SDIR)/getch/Makefile

sort:
	mkdir -p $(ODIR)/sort && $(MAKE) -f $(SDIR)/sort/Makefile

darray:
	mkdir -p $(ODIR)/darray && $(MAKE) -f $(SDIR)/darray/Makefile

stack:
	mkdir -p $(ODIR)/stack && $(MAKE) -f $(SDIR)/stack/Makefile

fifo:
	mkdir -p $(ODIR)/fifo && $(MAKE) -f $(SDIR)/fifo/Makefile

clean:
	$(MAKE) -f $(SDIR)/filebuffer/Makefile clean
	$(MAKE) -f $(SDIR)/getch/Makefile clean
	$(MAKE) -f $(SDIR)/sort/Makefile clean
	$(MAKE) -f $(SDIR)/darray/Makefile clean
	$(MAKE) -f $(SDIR)/stack/Makefile clean
	$(MAKE) -f $(SDIR)/fifo/Makefile clean
