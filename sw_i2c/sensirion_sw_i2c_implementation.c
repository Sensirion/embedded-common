/*
 * Copyright (c) 2017, Sensirion AG
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

#include "sensirion_sw_i2c_gpio.h"
#include "sensirion_arch_config.h"

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 *
 * We use the following names for the two I2C signal lines:
 * SCL for the clock line
 * SDA for the data line
 */

/**
 * Initialize all hard- and software components that are needed to set the
 * SDA and SCL pins.
 */
void sensirion_init_pins()
{
    // IMPLEMENT
}

/**
 * Configure the SDA pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SDA_in()
{
    // IMPLEMENT
}

/**
 * Configure the SDA pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SDA_out()
{
    // IMPLEMENT
}

/**
 * Read the value of the SDA pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
u8 sensirion_SDA_read()
{
    // IMPLEMENT
    return 1;
}

/**
 * Configure the SCL pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SCL_in()
{
    // IMPLEMENT
}

/**
 * Configure the SCL pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SCL_out()
{
    // IMPLEMENT
}

/**
 * Read the value of the SCL pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
u8 sensirion_SCL_read()
{
    // IMPLEMENT
    return 1;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(u32 useconds) {
    // IMPLEMENT
}
