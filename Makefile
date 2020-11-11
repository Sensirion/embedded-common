CFLAGS:= -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -Wstrict-aliasing=1 \
	-Wsign-conversion -Icommon -Ii2c -Ii2c/sw -Ishdlc

ifdef CI
	CFLAGS += -Werror
endif

OBJECTS = \
	common/sensirion_common.o \
	i2c/sensirion_i2c.o \
	i2c/sw/sensirion_sw_i2c.o \
	i2c/sw/sensirion_sw_i2c_implementation.o \
	i2c/sw/sample-implementations/linux_user_space/sensirion_sw_i2c_implementation.o \
	i2c/hw/sensirion_hw_i2c_implementation.o \
	i2c/hw/sample-implementations/linux_user_space/sensirion_hw_i2c_implementation.o \
	shdlc/sensirion_shdlc.o \
	shdlc/sensirion_uart_implementation.o \
	shdlc/sample-implementations/linux/sensirion_uart_implementation.o

.PHONY: clean test

test: $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

style-check:
	clang-format-6.0 -i $$(find . -name "*.[ch]")
	git diff --exit-code
