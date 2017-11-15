NAME := sort
THIS_DIR := $(TEST_SRC)/$(NAME)

_SRC := $(TEST_COMMON_SRC) $(S_SORT) $(THIS_DIR)/test.c
_INC := $(TEST_COMMON_INC) $(I_SORT)
_OBJ := $(_SRC:%.c=%_test.o)

MEMUSAGE_FILE := $(NAME)_memusage.txt

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
	$(Q)($(VALGRIND) $(VALGRIND_MEMCHECK_FLAGS) $(THIS_DIR)/$(TEST_EXEC_NAME) 2>&1 | $(GREP) "$(PASS_STRING)" && $(call passed_memcheck)) || $(call failed_memcheck)

memusage:
	$(call print_memusage,$(NAME))
	$(Q)$(VALGRIND) $(VALGRIND_MEMUSAGE_FLAGS) --massif-out-file=$(MEMUSAGE_FILE) $(THIS_DIR)/$(TEST_EXEC_NAME) >/dev/null 2>&1 

clean:
	$(Q)$(RM) $(_OBJ)
	$(Q)$(RM) $(MEMUSAGE_FILE)
