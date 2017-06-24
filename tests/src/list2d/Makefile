NAME := list2d

THIS_DIR := $(TEST_SRC)/$(NAME)
SRCS = $(wildcard $(SDIR)/$(NAME)/*.c)
all: test

test: FORCE
	$(Q)$(CC) $(C_TEST_FLAGS) -I $(IDIR) $(SDIR)/$(NAME)/*.c $(SDIR)/log.c \
	$(SDIR)/assert.c $(THIS_DIR)/test.c -o $(THIS_DIR)/$(TEST_EXEC_NAME)

FORCE:

run:
	$(Q)$(THIS_DIR)/$(TEST_EXEC_NAME)

clean:
	$(Q)rm -f $(THIS_DIR)/$(TEST_EXEC_NAME)
