NAME := filebuffer
THIS_DIR := $(SDIR)/$(NAME)
TARGET := lib$(NAME)

_SRC := $(S_FILEBUFFER)
_INC := $(I_FILEBUFFER)
_OBJ := $(_SRC:%.c=%.o)

%.o: %.c $(_INC)
	$(call print_cc,$<)
	$(Q)$(CC) $(CFLAGS) $(HEADERS) -c $< -o $@

all: $(TARGET)

$(TARGET): $(_OBJ)
	$(call print_ar,$@.a)
	$(Q)$(AR) $@.a $^ && \
	$(CP) $(_INC) $(O_HEADERS)/ && \
	$(MV) $@.a $(O_LIBS)/

clean:
	$(Q)$(RM) $(_OBJ)