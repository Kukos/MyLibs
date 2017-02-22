all: libstack

NAME := stack
THIS_DIR := $(SDIR)/$(NAME)

libstack: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c $(SDIR)/darray/*.c -c \
	&& ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(ODIR)/$(NAME) && \
	mv $@.a $(ODIR)/$(NAME)

clean:
	rm -f $(ODIR)/$(NAME)/*
