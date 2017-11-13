NAME := bst
THIS_DIR := $(TEST_SRC)/$(NAME)

_SRC := $(TEST_COMMON_SRC) $(S_BST) $(THIS_DIR)/test.c
_INC := $(TEST_COMMON_INC) $(I_BST)
_OBJ := $(_SRC:%.c=%_test.o)

%_test.o: %.c $(_INC)
	$(call print_cc,$<)
	$(Q)$(CC) $(C_TEST_FLAGS) $(HEADERS) -c $< -o $@

all: test

test: $(_OBJ)
	$(call print_bin,$(THIS_DIR)/$(TEST_EXEC_NAME))
	$(Q)$(CC) $(C_TEST_FLAGS) $(HEADERS) $^ -o $(THIS_DIR)/$(TEST_EXEC_NAME)

run:
	$(Q)$(THIS_DIR)/$(TEST_EXEC_NAME)

memcheck:
	$(call print_memcheck,$(NAME))
	$(Q)($(VALGRIND) $(VALGRIND_FLAGS) $(THIS_DIR)/$(TEST_EXEC_NAME) 2>&1 | $(GREP) "$(PASS_STRING)" && $(call passed_memcheck)) || $(call failed_memcheck)

clean:
	$(Q)$(RM) $(_OBJ)