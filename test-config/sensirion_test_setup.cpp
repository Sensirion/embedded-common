#include "CppUTest/CommandLineTestRunner.h"

#include "sensirion_i2c.h"

int16_t i2c_reset() {
    const uint8_t reset_data[] = {0x06};
    return sensirion_i2c_write(0x00, reset_data, (uint16_t)sizeof(reset_data));
}

int main(int argc, char** argv) {
    return CommandLineTestRunner::RunAllTests(argc, argv);
}
