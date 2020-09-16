#include "CppUTest/CommandLineTestRunner.h"

#include "sensirion_common.h"

int16_t i2c_reset() {
    return sensirion_i2c_general_call_reset();
}

int main(int argc, char** argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
