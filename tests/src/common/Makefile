THIS_DIR := $(TEST_SRC)/common

all: test

test: FORCE
	$(Q)$(CC) $(C_TEST_FLAGS) -I $(IDIR) $(THIS_DIR)/test.c -o $(THIS_DIR)/$(TEST_EXEC_NAME)

FORCE:

run:
	$(Q)$(THIS_DIR)/$(TEST_EXEC_NAME)

clean:
	$(Q)rm -f $(THIS_DIR)/$(TEST_EXEC_NAME)
