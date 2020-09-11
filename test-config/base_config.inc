driver_dir := ../../
mux_dir := ../i2c-mux-testbed
test_common_dir := ../embedded-common/test-config

# Unlike colon-equal (:=) immediate assignments, the simple equal sign (=) lazy
# assignment is lazility evaluated, so resorting to ${sensirion_common_dir}
# which is not yet defined works.
i2c_mux_sources = ${mux_dir}/i2c_mux.h ${mux_dir}/i2c_mux.c
hw_i2c_impl_src = ${sensirion_common_dir}/hw_i2c/sample-implementations/linux_user_space/sensirion_hw_i2c_implementation.c
sw_i2c_impl_src = ${sensirion_common_dir}/sw_i2c/sample-implementations/linux_user_space/sensirion_sw_i2c_implementation.c

sensirion_test_sources := ${test_common_dir}/sensirion_test_setup.h \
                          ${test_common_dir}/sensirion_test_setup.cpp \
                          ${i2c_mux_sources}

CXXFLAGS ?= $(CFLAGS) -fsanitize=address -I${mux_dir} -I${test_common_dir}
LDFLAGS ?= -lasan -lstdc++ -lCppUTest -lCppUTestExt
