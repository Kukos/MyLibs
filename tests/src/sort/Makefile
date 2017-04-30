NAME := sort

THIS_DIR := $(TEST_SRC)/$(NAME)
SRCS = $(wildcard $(SDIR)/$(NAME)/*.c)
all: test

test: $(SRCS) $(THIS_DIR)/test.c $(IDIR)/compiler.h $(IDIR)/common.h
	$(CC) $(CFLAGS) -I $(IDIR) $(SDIR)/$(NAME)/*.c $(SDIR)/log.c \
	$(SDIR)/assert.c $(THIS_DIR)/test.c -o $(THIS_DIR)/$(TEST_EXEC_NAME)

run:
	@$(THIS_DIR)/$(TEST_EXEC_NAME)

clean:
	rm -f $(THIS_DIR)/$(TEST_EXEC_NAME)
