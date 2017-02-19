all: libgetch

THIS_DIR := $(SDIR)/getch

libgetch: $(THIS_DIR)/*.c $(IDIR)/getch.h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/*.c -c && ar rcs $@.a *.o && \
	rm -f *.o && cp $(IDIR)/getch.h $(ODIR)/getch && \
	mv $@.a $(ODIR)/getch

clean:
	rm -f $(ODIR)/getch/*
