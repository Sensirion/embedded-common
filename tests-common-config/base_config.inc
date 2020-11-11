mux_dir := ../i2c-mux-testbed
test_common_dir := ../embedded-common/tests-common-config

# Unlike colon-equal (:=) immediate assignments, the simple equal sign (=) lazy
# assignment is lazility evaluated, so resorting to ${sensirion_common_dir}
# which is not yet defined works.
i2c_mux_sources = ${mux_dir}/i2c_mux.h ${mux_dir}/i2c_mux.c
hw_i2c_impl_src = ${sensirion_i2c_dir}/hw/sample-implementations/linux_user_space/sensirion_hw_i2c_implementation.c
sw_i2c_impl_src = ${sensirion_i2c_dir}/sw/sample-implementations/linux_user_space/sensirion_sw_i2c_implementation.c
uart_sources = ${sensirion_shdlc_dir}/sample-implementations/linux/sensirion_uart_implementation.c

sensirion_test_sources := ${test_common_dir}/sensirion_test_setup.h \
                          ${test_common_dir}/sensirion_test_setup.cpp

CXXFLAGS ?= $(CFLAGS) -fsanitize=address -I${test_common_dir}
LDFLAGS ?= -lasan -lstdc++ -lCppUTest -lCppUTestExt
