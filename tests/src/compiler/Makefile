THIS_DIR := $(TEST_SRC)/compiler

all: test

test: $(THIS_DIR)/test.c $(IDIR)/compiler.h $(IDIR)/common.h
	$(CC) $(CFLAGS) -I $(IDIR) $(THIS_DIR)/test.c -o $(THIS_DIR)/$(TEST_EXEC_NAME)

run:
	@$(THIS_DIR)/$(TEST_EXEC_NAME)

clean:
	rm -f $(THIS_DIR)/$(TEST_EXEC_NAME)
