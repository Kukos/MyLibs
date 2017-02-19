all: libfilebuffer

THIS_DIR := $(SDIR)/filebuffer

libfilebuffer: $(THIS_DIR)/*.c $(IDIR)/filebuffer.h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/filebuffer.h $(ODIR)/filebuffer && \
	mv $@.a $(ODIR)/filebuffer

clean:
	rm -f $(ODIR)/filebuffer/*
