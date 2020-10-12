/*
 * Copyright (c) 2018, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * \file
 *
 * This module provides functionality that is common to all Sensirion drivers
 */

#include "sensirion_common.h"
#include "sensirion_i2c.h"

uint16_t sensirion_bytes_to_uint16_t(const uint8_t* bytes) {
    return (uint16_t)bytes[0] << 8 | (uint16_t)bytes[1];
}

uint32_t sensirion_bytes_to_uint32_t(const uint8_t* bytes) {
    return (uint32_t)bytes[0] << 24 | (uint32_t)bytes[1] << 16 |
           (uint32_t)bytes[2] << 8 | (uint32_t)bytes[3];
}

float sensirion_bytes_to_float(const uint8_t* bytes) {
    union {
        uint32_t u32_value;
        float float32;
    } tmp;

    tmp.u32_value = sensirion_bytes_to_uint32_t(bytes);
    return tmp.float32;
}

uint8_t sensirion_common_generate_crc(const uint8_t* data, uint16_t count) {
    uint16_t current_byte;
    uint8_t crc = CRC8_INIT;
    uint8_t crc_bit;

    /* calculates 8-Bit checksum with given polynomial */
    for (current_byte = 0; current_byte < count; ++current_byte) {
        crc ^= (data[current_byte]);
        for (crc_bit = 8; crc_bit > 0; --crc_bit) {
            if (crc & 0x80)
                crc = (crc << 1) ^ CRC8_POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

int8_t sensirion_common_check_crc(const uint8_t* data, uint16_t count,
                                  uint8_t checksum) {
    if (sensirion_common_generate_crc(data, count) != checksum)
        return STATUS_FAIL;
    return NO_ERROR;
}

int16_t sensirion_i2c_general_call_reset(void) {
    const uint8_t data = 0x06;
    return sensirion_i2c_write(0, &data, (uint16_t)sizeof(data));
}

int16_t sensirion_i2c_read_words_as_bytes(uint8_t address, uint8_t* data,
                                          uint16_t num_words) {
    int16_t error;
    uint16_t i, j;
    uint16_t size = num_words * (SENSIRION_WORD_SIZE + CRC8_LEN);
    uint16_t word_buf[SENSIRION_MAX_BUFFER_WORDS];
    uint8_t* const buf8 = (uint8_t*)word_buf;

    error = sensirion_i2c_read(address, buf8, size);
    if (error != NO_ERROR) {
        return error;
    }

    /* check the CRC for each word */
    for (i = 0, j = 0; i < size; i += SENSIRION_WORD_SIZE + CRC8_LEN) {

        error = sensirion_common_check_crc(&buf8[i], SENSIRION_WORD_SIZE,
                                           buf8[i + SENSIRION_WORD_SIZE]);
        if (error != NO_ERROR)
            return error;

        data[j++] = buf8[i];
        data[j++] = buf8[i + 1];
    }

    return NO_ERROR;
}

int16_t sensirion_i2c_write_cmd(uint8_t address, uint16_t command) {
    return sensirion_i2c_write_cmd_with_args(address, command, NULL, 0);
}

int16_t sensirion_i2c_write_cmd_with_args(uint8_t address, uint16_t command,
                                          const uint16_t* data_words,
                                          uint16_t num_words) {
    uint8_t buf[SENSIRION_MAX_BUFFER_WORDS];
    uint8_t i;
    uint8_t crc;
    uint16_t num_bytes = 0;

    // generate CRC
    buf[num_bytes++] = (uint8_t)((command & 0xFF00) >> 8);
    buf[num_bytes++] = (uint8_t)((command & 0x00FF) >> 0);

    for (i = 0; i < num_words; ++i) {
        buf[num_bytes++] = (uint8_t)((data_words[i] & 0xFF00) >> 8);
        buf[num_bytes++] = (uint8_t)((data_words[i] & 0x00FF) >> 0);

        crc = sensirion_common_generate_crc((uint8_t*)&buf[num_bytes - 2],
                                            SENSIRION_WORD_SIZE);
        buf[num_bytes++] = crc;
    }
    return sensirion_i2c_write(address, buf, num_bytes);
}

int16_t sensirion_i2c_delayed_read_cmd(uint8_t address, uint16_t command,
                                       uint32_t delay_us, uint8_t* data,
                                       uint16_t num_words) {
    int16_t error;
    error = sensirion_i2c_write_cmd_with_args(address, command, NULL, 0);
    if (error != NO_ERROR) {
        return error;
    }

    if (delay_us) {
        sensirion_sleep_usec(delay_us);
    }

    return sensirion_i2c_read_words_as_bytes(address, data, num_words);
}
