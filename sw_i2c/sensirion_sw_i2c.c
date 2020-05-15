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

#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_sw_i2c_gpio.h"

#define DELAY_USEC (SENSIRION_I2C_CLOCK_PERIOD_USEC / 2)

static int8_t sensirion_wait_while_clock_stretching(void) {
    /* Maximal timeout of 150ms (SCD30) in sleep polling cycles */
    uint32_t timeout_cycles = 150000 / SENSIRION_I2C_CLOCK_PERIOD_USEC;

    while (--timeout_cycles) {
        if (sensirion_SCL_read())
            return NO_ERROR;
        sensirion_sleep_usec(SENSIRION_I2C_CLOCK_PERIOD_USEC);
    }

    return STATUS_FAIL;
}

static int8_t sensirion_i2c_write_byte(uint8_t data) {
    int8_t nack, i;
    for (i = 7; i >= 0; i--) {
        sensirion_SCL_out();
        if ((data >> i) & 0x01)
            sensirion_SDA_in();
        else
            sensirion_SDA_out();
        sensirion_sleep_usec(DELAY_USEC);
        sensirion_SCL_in();
        sensirion_sleep_usec(DELAY_USEC);
        if (sensirion_wait_while_clock_stretching())
            return STATUS_FAIL;
    }
    sensirion_SCL_out();
    sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    if (sensirion_wait_while_clock_stretching())
        return STATUS_FAIL;
    nack = (sensirion_SDA_read() != 0);
    sensirion_SCL_out();

    return nack;
}

static uint8_t sensirion_i2c_read_byte(uint8_t ack) {
    int8_t i;
    uint8_t data = 0;
    sensirion_SDA_in();
    for (i = 7; i >= 0; i--) {
        sensirion_sleep_usec(DELAY_USEC);
        sensirion_SCL_in();
        if (sensirion_wait_while_clock_stretching())
            return 0xFF;  // return 0xFF on error
        data |= (sensirion_SDA_read() != 0) << i;
        sensirion_SCL_out();
    }
    if (ack)
        sensirion_SDA_out();
    else
        sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    sensirion_sleep_usec(DELAY_USEC);
    if (sensirion_wait_while_clock_stretching())
        return 0xFF;  // return 0xFF on error
    sensirion_SCL_out();
    sensirion_SDA_in();

    return data;
}

static int8_t sensirion_i2c_start(void) {
    sensirion_SCL_in();
    if (sensirion_wait_while_clock_stretching())
        return STATUS_FAIL;

    sensirion_SDA_out();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_out();
    sensirion_sleep_usec(DELAY_USEC);
    return NO_ERROR;
}

static void sensirion_i2c_stop(void) {
    sensirion_SDA_out();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
}

int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data,
                           uint16_t count) {
    int8_t ret;
    uint16_t i;

    ret = sensirion_i2c_start();
    if (ret != NO_ERROR)
        return ret;

    ret = sensirion_i2c_write_byte(address << 1);
    if (ret != NO_ERROR) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++) {
        ret = sensirion_i2c_write_byte(data[i]);
        if (ret != NO_ERROR) {
            sensirion_i2c_stop();
            break;
        }
    }
    sensirion_i2c_stop();
    return ret;
}

int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count) {
    int8_t ret;
    uint8_t send_ack;
    uint16_t i;

    ret = sensirion_i2c_start();
    if (ret != NO_ERROR)
        return ret;

    ret = sensirion_i2c_write_byte((address << 1) | 1);
    if (ret != NO_ERROR) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++) {
        send_ack = i < (count - 1); /* last byte must be NACK'ed */
        data[i] = sensirion_i2c_read_byte(send_ack);
    }

    sensirion_i2c_stop();
    return NO_ERROR;
}

void sensirion_i2c_init(void) {
    sensirion_init_pins();
    sensirion_SCL_in();
    sensirion_SDA_in();
}

void sensirion_i2c_release(void) {
    sensirion_SCL_in();
    sensirion_SDA_in();
    sensirion_release_pins();
}
