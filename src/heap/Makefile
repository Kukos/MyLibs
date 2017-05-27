all: libheap

NAME := heap
THIS_DIR := $(SDIR)/$(NAME)

libheap: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(Q)$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c $(SDIR)/assert.c \
	$(SDIR)/darray/*.c $(SDIR)/sort/*.c -c \
	&& ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(O_HEADERS)/ && \
	mv $@.a $(O_LIBS)/
