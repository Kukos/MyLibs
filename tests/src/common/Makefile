THIS_DIR := $(TEST_SRC)/common

all: test

test: FORCE
	$(CC) $(CFLAGS) -DSILENT_ERROR -I $(IDIR) $(THIS_DIR)/test.c -o $(THIS_DIR)/$(TEST_EXEC_NAME)

FORCE:

run:
	@$(THIS_DIR)/$(TEST_EXEC_NAME)

clean:
	rm -f $(THIS_DIR)/$(TEST_EXEC_NAME)
