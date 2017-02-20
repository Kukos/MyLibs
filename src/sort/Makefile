all: libsort

THIS_DIR := $(SDIR)/sort

libsort: $(THIS_DIR)/*.c $(IDIR)/sort.h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c $(SDIR)/log.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/sort.h $(ODIR)/sort && \
	mv $@.a $(ODIR)/sort

clean:
	rm -f $(ODIR)/sort/*
