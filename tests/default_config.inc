test_common_dir := ../tests-common-config

sensirion_i2c_dir = ../i2c
sensirion_shdlc_dir = ../shdlc
sensirion_common_dir = ../common

sensirion_i2c_sources = ${sensirion_i2c_dir}/sensirion_i2c.h \
                        ${sensirion_i2c_dir}/sensirion_i2c.c \
                        ${sensirion_i2c_dir}/sensirion_i2c_hal.h \
                        ${sensirion_i2c_dir}/hw/sensirion_hw_i2c_implementation.c

sensirion_shdlc_sources = ${sensirion_shdlc_dir}/sensirion_shdlc.h \
                          ${sensirion_shdlc_dir}/sensirion_shdlc.c \
                          ${sensirion_shdlc_dir}/sensirion_uart_hal.h \
                          ${sensirion_shdlc_dir}/sensirion_uart_implementation.c

sensirion_common_sources = ${sensirion_common_dir}/sensirion_config.h \
                           ${sensirion_common_dir}/sensirion_common.h \
                           ${sensirion_common_dir}/sensirion_common.c

sensirion_test_sources := ${test_common_dir}/sensirion_test_setup.h \
                          ${test_common_dir}/sensirion_test_setup.cpp

CFLAGS:= -Wall -Wextra -Wfloat-conversion -Wno-unused-parameter -Wstrict-aliasing=1 \
	-Wsign-conversion -I${sensirion_common_dir} -I${sensirion_i2c_dir} -I${sensirion_shdlc_dir}

CXXFLAGS ?= $(CFLAGS) -fsanitize=address -I${test_common_dir}
LDFLAGS ?= -lasan -lstdc++ -lCppUTest -lCppUTestExt

