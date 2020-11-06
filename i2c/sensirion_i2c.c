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

#include "sensirion_i2c.h"
#include "sensirion_common.h"
#include "sensirion_config.h"
#include "sensirion_i2c_hal.h"

int16_t sensirion_i2c_general_call_reset(void) {
    const uint8_t data = 0x06;
    return sensirion_i2c_hal_write(0, &data, (uint16_t)sizeof(data));
}

uint8_t sensirion_i2c_generate_crc(const uint8_t* data, uint16_t count) {
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

int8_t sensirion_i2c_check_crc(const uint8_t* data, uint16_t count,
                               uint8_t checksum) {
    if (sensirion_i2c_generate_crc(data, count) != checksum)
        return CRC_ERROR;
    return NO_ERROR;
}

int16_t sensirion_i2c_write_cmd(uint8_t address, uint16_t command,
                                const uint8_t* data, uint16_t data_length) {
    uint8_t i;
    uint8_t crc;
    uint8_t buf[SENSIRION_WORD_SIZE * SENSIRION_MAX_BUFFER_WORDS];
    uint16_t num_bytes = 0;

    if (data_length % SENSIRION_WORD_SIZE != 0) {
        return BYTE_NUM_ERROR;
    }

    // generate CRC
    buf[num_bytes++] = (uint8_t)((command & 0xFF00) >> 8);
    buf[num_bytes++] = (uint8_t)((command & 0x00FF) >> 0);

    for (i = 0; i < data_length; i += 2) {
        buf[num_bytes++] = data[i];
        buf[num_bytes++] = data[i + 1];

        crc = sensirion_i2c_generate_crc((uint8_t*)&buf[num_bytes - 2],
                                         SENSIRION_WORD_SIZE);
        buf[num_bytes++] = crc;
    }
    return sensirion_i2c_hal_write(address, buf, num_bytes);
}

int16_t sensirion_i2c_read_data(uint8_t address, uint8_t* data,
                                uint16_t data_length) {
    int16_t error;
    uint16_t i, j;
    uint16_t size =
        (data_length / SENSIRION_WORD_SIZE) * (SENSIRION_WORD_SIZE + CRC8_LEN);
    uint8_t buf[SENSIRION_WORD_SIZE * SENSIRION_MAX_BUFFER_WORDS];

    if (data_length % SENSIRION_WORD_SIZE != 0) {
        return BYTE_NUM_ERROR;
    }

    error = sensirion_i2c_hal_read(address, buf, size);
    if (error)
        return error;

    /* check the CRC for each word */
    for (i = 0, j = 0; i < size; i += SENSIRION_WORD_SIZE + CRC8_LEN) {

        error = sensirion_i2c_check_crc(&buf[i], SENSIRION_WORD_SIZE,
                                        buf[i + SENSIRION_WORD_SIZE]);
        if (error) {
            return error;
        }
        data[j++] = buf[i];
        data[j++] = buf[i + 1];
    }

    return NO_ERROR;
}

int16_t sensirion_i2c_write_delay_read_cmd(uint8_t address, uint16_t command,
                                           const uint8_t* write_data,
                                           uint16_t write_data_length,
                                           uint32_t delay_us,
                                           uint8_t* read_data,
                                           uint16_t read_data_length) {
    int16_t error;

    error = sensirion_i2c_write_cmd(address, command, write_data,
                                    write_data_length);
    if (error) {
        return error;
    }

    if (delay_us) {
        sensirion_i2c_hal_sleep_usec(delay_us);
    }

    return sensirion_i2c_read_data(address, read_data, read_data_length);
}
