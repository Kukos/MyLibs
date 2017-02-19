export

CC := gcc
CFLAGS := -std=gnu99 -Wall -pedantic -Werror -O3

PROJECT_DIR := $(shell pwd)

IDIR := $(PROJECT_DIR)/include
SDIR := $(PROJECT_DIR)/src
ODIR := $(PROJECT_DIR)/libs

all: filebuffer getch

filebuffer:
	mkdir -p $(ODIR)/filebuffer && $(MAKE) -f $(SDIR)/filebuffer/Makefile

getch:
	mkdir -p $(ODIR)/getch && $(MAKE) -f $(SDIR)/getch/Makefile

clean:
	$(MAKE) -f $(SDIR)/filebuffer/Makefile clean
	$(MAKE) -f $(SDIR)/getch/Makefile clean
