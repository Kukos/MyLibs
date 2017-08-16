NAME := darray
THIS_DIR := $(TEST_SRC)/$(NAME)

_SRC := $(TEST_COMMON_SRC) $(S_DARRAY) $(THIS_DIR)/test.c
_INC := $(TEST_COMMON_INC) $(I_DARRAY)
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

clean:
	$(Q)$(RM) $(_OBJ)