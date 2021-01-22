#include "sensirion_i2c.h"
#include "sensirion_shdlc.h"
#include "sensirion_test_setup.h"

TEST_GROUP (EmbeddedCommon_SHDLC_Tests) {
    void setup() {
    }

    void teardown() {
    }
};

TEST_GROUP (EmbeddedCommon_I2C_Tests) {
    void setup() {
    }

    void teardown() {
    }
};

TEST (EmbeddedCommon_SHDLC_Tests, Frame_Test_1) {
    uint8_t outdata[] = {0x7E, 0x00, 0x80, 0x05, 0x00, 0x00,
                         0x00, 0x00, 0x00, 0x7A, 0x7E};
    uint8_t buffer[11];
    uint32_t auto_clean_interval = 0;
    uint8_t command = 0x80;
    uint8_t address = 0;
    uint8_t sub_command = 0x00;
    uint8_t data_length = 5; /* sub_command(1) + auto_clean_interval(4) */
    struct sensirion_shdlc_buffer frame;
    sensirion_shdlc_begin_frame(&frame, &buffer[0], command, address,
                                data_length);
    sensirion_shdlc_add_uint8_t_to_frame(&frame, sub_command);
    sensirion_shdlc_add_uint32_t_to_frame(&frame, auto_clean_interval);
    sensirion_shdlc_finish_frame(&frame);
    MEMCMP_EQUAL(outdata, buffer, 11);
}

TEST (EmbeddedCommon_SHDLC_Tests, Frame_Test_2) {
    uint8_t outdata[] = {0x7E, 0x00, 0x80, 0x05, 0x00, 0x01,
                         0x02, 0x03, 0x04, 0x70, 0x7E};
    uint8_t buffer[11];
    uint32_t auto_clean_interval = 0x01020304;
    uint8_t command = 0x80;
    uint8_t address = 0;
    uint8_t sub_command = 0x00;
    uint8_t data_length = 5; /* sub_command(1) + auto_clean_interval(4) */
    struct sensirion_shdlc_buffer frame;
    sensirion_shdlc_begin_frame(&frame, &buffer[0], command, address,
                                data_length);
    sensirion_shdlc_add_uint8_t_to_frame(&frame, sub_command);
    sensirion_shdlc_add_uint32_t_to_frame(&frame, auto_clean_interval);
    sensirion_shdlc_finish_frame(&frame);
    MEMCMP_EQUAL(outdata, buffer, 11);
}

TEST (EmbeddedCommon_SHDLC_Tests, Frame_Test_3) {
    uint8_t outdata[] = {0x7E, 0x00, 0x80, 0x05, 0x00, 0x7D, 0x5E, 0x7D,
                         0x5D, 0x7D, 0x31, 0x7D, 0x33, 0x5B, 0x7E};
    uint8_t buffer[15];
    uint32_t auto_clean_interval = 0x7E7D1113;
    uint8_t command = 0x80;
    uint8_t address = 0;
    uint8_t sub_command = 0x00;
    uint8_t data_length = 5; /* sub_command(1) + auto_clean_interval(4) */
    struct sensirion_shdlc_buffer frame;
    sensirion_shdlc_begin_frame(&frame, &buffer[0], command, address,
                                data_length);
    sensirion_shdlc_add_uint8_t_to_frame(&frame, sub_command);
    sensirion_shdlc_add_uint32_t_to_frame(&frame, auto_clean_interval);
    sensirion_shdlc_finish_frame(&frame);
    MEMCMP_EQUAL(outdata, buffer, 15);
}

TEST (EmbeddedCommon_I2C_Tests, Frame_Test_1) {
    uint8_t outdata[] = {0x28, 0x0E, 0xBE, 0xEF, 0x92};
    uint8_t buffer[5];
    uint16_t command = 0x280E;
    uint16_t data = 0xBEEF;
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, command);
    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, data);
    MEMCMP_EQUAL(outdata, buffer, 5);
}

TEST (EmbeddedCommon_I2C_Tests, Frame_Test_2) {
    uint8_t outdata[] = {0x26, 0x0F, 0x80, 0x00, 0xA2, 0x66, 0x66, 0x93};
    uint8_t buffer[8];
    uint16_t command = 0x260F;
    uint16_t data1 = 0x8000;
    uint16_t data2 = 0x6666;
    uint16_t offset = 0;
    offset = sensirion_i2c_add_command_to_buffer(&buffer[0], offset, command);
    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, data1);
    offset = sensirion_i2c_add_uint16_t_to_buffer(&buffer[0], offset, data2);
    MEMCMP_EQUAL(outdata, buffer, 8);
}
