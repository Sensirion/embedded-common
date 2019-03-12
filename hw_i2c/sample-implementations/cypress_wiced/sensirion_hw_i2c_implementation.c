/*
 * Copyright (c) 2018-2019, Sensirion AG
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
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "wiced.h"

#include "sensirion_i2c.h"

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

static wiced_i2c_device_t gWicedi2cDeviceTmpl = {
    .port = WICED_I2C_1, /* default */
    .address = 0x0,
    .address_width = I2C_ADDRESS_WIDTH_7BIT,
    .speed_mode = I2C_STANDARD_SPEED_MODE,
};

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init()
{
    wiced_i2c_init(&gWicedi2cDeviceTmpl);
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count)
{
    wiced_result_t wres;
    wiced_i2c_message_t msg;

    /* create a copy to avoid a race condition */
    static wiced_i2c_device_t tmpDevice;
    memcpy(&tmpDevice, &gWicedi2cDeviceTmpl, sizeof(wiced_i2c_device_t));
    tmpDevice.address = address;

    if((wres = wiced_i2c_init_rx_message(&msg, data, count, 1, WICED_TRUE)) != WICED_SUCCESS){
        return wres;
    }

    return wiced_i2c_transfer(&tmpDevice, &msg, 1);
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of bytes.
 * The bytes in the supplied buffer must be sent to the given address. If the
 * slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count)
{
    wiced_result_t wres;
    wiced_i2c_message_t msg;

    /* create a copy to avoid a race condition */
    static wiced_i2c_device_t tmpDevice;
    memcpy(&tmpDevice, &gWicedi2cDeviceTmpl, sizeof(wiced_i2c_device_t));
    tmpDevice.address = address;

    if((wres = wiced_i2c_init_tx_message(&msg, data, count, 1, WICED_TRUE)) != WICED_SUCCESS){
        return wres;
    }

    return wiced_i2c_transfer(&tmpDevice, &msg, 1);
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {
    wiced_rtos_delay_milliseconds((useconds / 1000) + 1);
}


/**
 * Wiced specific: select a different I2C port
 *
 *
 * @param port the WICED i2c port to use
 * @param flags flags for the I2C device
 */
wiced_result_t sensirion_wiced_setup_i2c_port(wiced_i2c_t port, uint8_t flags)
{
    gWicedi2cDeviceTmpl.port = port;
    gWicedi2cDeviceTmpl.flags = flags;

    return wiced_i2c_init(&gWicedi2cDeviceTmpl);
}

