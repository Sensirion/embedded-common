CFLAGS:= -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -Wstrict-aliasing=1 \
	-Wsign-conversion -Icommon -Ii2c -Ii2c/sample-implementations/GPIO_bit_banging -Ishdlc

ifdef CI
	CFLAGS += -Werror
endif

OBJECTS = \
	common/sensirion_common.o \
	i2c/sensirion_i2c.o \
	i2c/sample-implementations/GPIO_bit_banging/sensirion_i2c_gpio.o \
	i2c/sample-implementations/GPIO_bit_banging/sensirion_i2c_hal.o \
	i2c/sample-implementations/GPIO_bit_banging/sample-implementations/linux_user_space/sensirion_i2c_gpio.o \
	i2c/sensirion_i2c_hal.o \
	i2c/sample-implementations/linux_user_space/sensirion_i2c_hal.o \
	shdlc/sensirion_shdlc.o \
	shdlc/sensirion_uart_hal.o \
	shdlc/sample-implementations/linux_user_space/sensirion_uart_hal.o

.PHONY: clean test

test: $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

style-check:
	git ls-files | grep -e '\.\(c\|h\|cpp\)$$' | xargs clang-format -i -style=file;
	git diff --exit-code
