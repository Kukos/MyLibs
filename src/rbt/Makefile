all: librbt

NAME := rbt
THIS_DIR := $(SDIR)/$(NAME)

librbt: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c \
	$(SDIR)/log.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(O_HEADERS)/ && \
	mv $@.a $(O_LIBS)/
