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

#ifndef SENSIRION_I2C_H
#define SENSIRION_I2C_H

#include "sensirion_config.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CRC_ERROR 1
#define I2C_BUS_ERROR 2
#define I2C_NACK_ERROR 3
#define BYTE_NUM_ERROR 4

#define CRC8_POLYNOMIAL 0x31
#define CRC8_INIT 0xFF
#define CRC8_LEN 1

#define SENSIRION_COMMAND_SIZE 2
#define SENSIRION_WORD_SIZE 2
#define SENSIRION_NUM_WORDS(x) (sizeof(x) / SENSIRION_WORD_SIZE)
#define SENSIRION_MAX_BUFFER_WORDS 32

/**
 * sensirion_i2c_general_call_reset() - Send a general call reset.
 *
 * @warning This will reset all attached I2C devices on the bus which support
 *          general call reset.
 *
 * @warning Isn't supported by all Sensirion devices.
 *
 * @return  NO_ERROR on success, an error code otherwise
 */
int16_t sensirion_i2c_general_call_reset(void);

/**
 * sensirion_i2c_generate_crc() - Generate the CRC used by Sensirion sensors.
 *
 * @param data        Pointer to data bytes for which the CRC should be
 *                    calculated.
 * @param data_length Number of bytes over which the CRC should be calculated.
 *
 * @return            CRC for the given data.
 *
 */
uint8_t sensirion_i2c_generate_crc(const uint8_t* data, uint16_t data_length);

/**
 * sensirion_i2c_check_crc() - Check if CRC is correct for given data.
 *
 * @param data        Pointer to data bytes for which the CRC was calculated.
 * @param data_length Number of bytes over which the CRC was calculated.
 * @param crc         Expected CRC result.
 *
 * @return            NO_ERROR when CRC is correct for given data, CRC_ERROR
 *                    otherwise.
 *
 */

int16_t sensirion_i2c_check_crc(const uint8_t* data, uint16_t data_length,
                                uint8_t crc);

/**
 * sensirion_i2c_write_cmd() - Writes a command to the sensor.
 *
 * @param address     Sensor I2C address.
 * @param command     Two byte command for the sensor.
 * @param data        Additional data send to the sensor.
 * @param data_length Number of additional bytes to send to the sensor
 *                    (without CRC bytes). Needs to be a multiple of
 *                    SENSIRION_WORD_SIZE, otherwise the function will
 *                    through BYTE_NUM_ERROR. Pad with zeros if necessary.
 *
 * @return            NO_ERROR on success, an error code otherwise
 */
int16_t sensirion_i2c_write_cmd(uint8_t address, uint16_t command,
                                const uint8_t* data, uint16_t data_length);

/**
 * sensirion_i2c_read_data() - Reads data from the sensor.
 *
 * @param address     Sensor I2C address
 * @param data        Allocated buffer to store the read data as bytes.
 * @param data_length Number of bytes to read (without CRC bytes). Needs to
 *                    be a multiple of SENSIRION_WORD_SIZE, otherwise the
 *                    function will through BYTE_NUM_ERROR.
 *
 * @return            NO_ERROR on success, an error code otherwise
 */
int16_t sensirion_i2c_read_data(uint8_t address, uint8_t* data,
                                uint16_t data_length);

/**
 * sensirion_i2c_write_delay_read_cmd() - Send a command, wait for the sensor to
 *                                        process and read data back.
 *
 * @param address           Sensor I2C address.
 * @param command           Two byte command for the sensor.
 * @param write_data        Additional data sent to the sensor.
 * @param write_data_length Number of additional bytes to send to the sensor
 *                          (without CRC bytes). Needs to be a multiple of
 *                          SENSIRION_WORD_SIZE, otherwise the function will
 *                          through BYTE_NUM_ERROR. Pad with zeros if necessary.
 * @param delay             Time in microseconds to delay sending the read
 *                          request.
 * @param read_data         Allocated buffer to store the read data as bytes.
 * @param read_data_length  Number of bytes to read (without CRC bytes). Needs
 *                          to be a multiple of SENSIRION_WORD_SIZE, otherwise
 *                          the function will through BYTE_NUM_ERROR.
 *
 * @return                  NO_ERROR on success, an error code otherwise.
 */
int16_t sensirion_i2c_write_delay_read_cmd(uint8_t address, uint16_t command,
                                           const uint8_t* write_data,
                                           uint16_t write_data_length,
                                           uint32_t delay_us,
                                           uint8_t* read_data,
                                           uint16_t read_data_length);

#ifdef __cplusplus
}
#endif

#endif /* SENSIRION_I2C_H */
