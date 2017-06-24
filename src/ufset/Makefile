all: libufset

NAME := ufset
THIS_DIR := $(SDIR)/$(NAME)

libufset: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(Q)$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c -c \
	&& ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(O_HEADERS)/ && \
	mv $@.a $(O_LIBS)/
