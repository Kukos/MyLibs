all: libfilebuffer

NAME := filebuffer
THIS_DIR := $(SDIR)/$(NAME)

libfilebuffer: $(THIS_DIR)/*.c $(IDIR)/$(NAME).h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/$(NAME).h $(ODIR)/$(NAME) && \
	mv $@.a $(ODIR)/$(NAME)

clean:
	rm -f $(ODIR)/$(NAME)/*
