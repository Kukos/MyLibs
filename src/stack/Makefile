all: libstack

NAME := stack
THIS_DIR := $(SDIR)/$(NAME)

libstack: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(Q)$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/sort/*.c \
	$(SDIR)/log.c $(SDIR)/assert.c $(SDIR)/darray/*.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(O_HEADERS)/ && \
	mv $@.a $(O_LIBS)/
