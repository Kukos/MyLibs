all: libdarray

NAME := darray
THIS_DIR := $(SDIR)/$(NAME)

libdarray: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/sort/*.c \
	$(SDIR)/log.c $(SDIR)/assert.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(O_HEADERS)/ && \
	mv $@.a $(O_LIBS)/
