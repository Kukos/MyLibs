all: libfilebuffer

NAME := filebuffer
THIS_DIR := $(SDIR)/$(NAME)

libfilebuffer: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(Q)$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c $(SDIR)/assert.c -c \
	&& ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(O_HEADERS)/ && \
	mv $@.a $(O_LIBS)/
