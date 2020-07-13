CFLAGS:= -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -Wstrict-aliasing=1 -Wsign-conversion -I. -Isw_i2c

ifdef CI
	CFLAGS += -Werror
endif

OBJECTS = \
	sensirion_common.o \
	sw_i2c/sensirion_sw_i2c.o \
	sw_i2c/sensirion_sw_i2c_implementation.o \
	sw_i2c/sample-implementations/linux_user_space/sensirion_sw_i2c_implementation.o \
	hw_i2c/sensirion_hw_i2c_implementation.o \
	hw_i2c/sample-implementations/linux_user_space/sensirion_hw_i2c_implementation.o

.PHONY: clean test

test: $(OBJECTS)

clean:
	$(RM) $(OBJECTS)

style-check:
	clang-format-6.0 -i $$(find . -name "*.[ch]")
	git diff --exit-code
