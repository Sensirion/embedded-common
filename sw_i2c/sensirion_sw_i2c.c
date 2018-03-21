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
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "sensirion_arch_config.h"
#include "sensirion_common.h"
#include "sensirion_i2c.h"
#include "sensirion_sw_i2c_gpio.h"

#define DELAY_USEC (I2C_CLOCK_PERIOD_USEC / 2)

static s8 sensirion_i2c_write_byte(u8 data)
{
    s8 nack, i;
    for (i = 7; i >= 0; i--) {
        sensirion_SCL_out();
        if ((data >> i) & 0x01)
            sensirion_SDA_in();
        else
            sensirion_SDA_out();
        sensirion_sleep_usec(DELAY_USEC);
        sensirion_SCL_in();
        sensirion_sleep_usec(DELAY_USEC);
    }
    sensirion_SCL_out();
    sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    nack = (sensirion_SDA_read() != 0);
    sensirion_SCL_out();

    return nack;
}

static u8 sensirion_i2c_read_byte(u8 ack)
{
    s8 i;
    u8 data = 0;
    sensirion_SDA_in();
    for (i = 7; i >= 0; i--) {
        sensirion_sleep_usec(DELAY_USEC);
        sensirion_SCL_in();
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
    sensirion_SCL_out();
    sensirion_SDA_in();

    return data;
}

static void sensirion_i2c_start(void)
{
    sensirion_SDA_out();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_out();
    sensirion_sleep_usec(DELAY_USEC);
}

static void sensirion_i2c_stop(void)
{
    sensirion_SDA_out();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SCL_in();
    sensirion_sleep_usec(DELAY_USEC);
    sensirion_SDA_in();
    sensirion_sleep_usec(DELAY_USEC);
}

s8 sensirion_i2c_write(u8 address, const u8* data, u16 count)
{
    s8 ret;
    u16 i;
    sensirion_i2c_start();
    ret = sensirion_i2c_write_byte(address << 1);
    if (ret != STATUS_OK) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++) {
        ret = sensirion_i2c_write_byte(data[i]);
        if (ret != STATUS_OK) {
            sensirion_i2c_stop();
            break;
        }
    }
    sensirion_i2c_stop();
    return ret;
}

s8 sensirion_i2c_read(u8 address, u8* data, u16 count)
{
    s8 ret;
    u16 i;
    sensirion_i2c_start();
    ret = sensirion_i2c_write_byte((address << 1) | 1);
    if (ret != STATUS_OK) {
        sensirion_i2c_stop();
        return ret;
    }
    for (i = 0; i < count; i++)
        data[i] = sensirion_i2c_read_byte(1);

    sensirion_i2c_stop();
    return ret;
}

void sensirion_i2c_init()
{
    sensirion_init_pins();
}
