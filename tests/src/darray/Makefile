NAME := darray

THIS_DIR := $(TEST_SRC)/$(NAME)
SRCS = $(wildcard $(SDIR)/$(NAME)/*.c)
all: test

test: FORCE
	$(CC) $(CFLAGS) -I $(IDIR) $(SDIR)/$(NAME)/*.c $(SDIR)/sort/*.c $(SDIR)/log.c \
	$(SDIR)/assert.c $(THIS_DIR)/test.c -o $(THIS_DIR)/$(TEST_EXEC_NAME)

FORCE:

run:
	@$(THIS_DIR)/$(TEST_EXEC_NAME)

clean:
	rm -f $(THIS_DIR)/$(TEST_EXEC_NAME)
