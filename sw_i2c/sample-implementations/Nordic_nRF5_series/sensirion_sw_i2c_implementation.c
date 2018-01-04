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

#include "sensirion_arch_config.h"
#include "sensirion_sw_i2c_gpio.h"

#include <nrf_delay.h>

/* Nordic specific include for Nordic specific configuration below */
#include <nrf_gpio.h>

/**
 * Nordic specific configuration. Change the pin numbers if you use other pins
 * than defined below.
 */
#define SENSIRION_SDA_PIN 0
#define SENSIRION_SCL_PIN 2

u32 sda;
u32 scl;
volatile u32 *sclPinRegAddr;
volatile u32 *sdaPinRegAddr;


/* configuration register value for I2C output pin */
static const u32 REGVALOUT = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
  | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
  | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
  | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_INPUT_Pos)
  | (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos);
/* configuration register value for I2C input pin */
static const u32 REGVALIN = (GPIO_PIN_CNF_SENSE_Disabled << GPIO_PIN_CNF_SENSE_Pos)
  | (GPIO_PIN_CNF_DRIVE_S0S1 << GPIO_PIN_CNF_DRIVE_Pos)
  | (GPIO_PIN_CNF_PULL_Disabled << GPIO_PIN_CNF_PULL_Pos)
  | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_INPUT_Pos)
  | (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos);

/**
 * Initialize all hard- and software components that are needed to set the
 * SDA and SCL pins.
 */
void sensirion_init_pins() {
    scl = SENSIRION_SCL_PIN;
    sda = SENSIRION_SDA_PIN;
    sclPinRegAddr = &NRF_GPIO->PIN_CNF[(u32)scl];
    sdaPinRegAddr = &NRF_GPIO->PIN_CNF[(u32)sda];
    *sclPinRegAddr = REGVALIN;
    *sdaPinRegAddr = REGVALIN;
}

/**
 * Configure the SDA pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
void sensirion_SDA_in() {
    *sdaPinRegAddr = REGVALIN;
}

/**
 * Configure the SDA pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */

void sensirion_SDA_out() {
    *sdaPinRegAddr = REGVALOUT;
}

/**
 * Read the value of the SDA pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
u8 sensirion_SDA_read() {
    return (u8)nrf_gpio_pin_read(sda);
}

/**
 * Configure the SCL pin as an input. The pin should be either left floating
 * or pulled up to the supply voltage.
 */
 void sensirion_SCL_in() {
    *sclPinRegAddr = REGVALIN;
}

/**
 * Configure the SCL pin as an output and drive it low. The pin must be pulled
 * to ground or set to logical false.
 */
void sensirion_SCL_out() {
    *sclPinRegAddr = REGVALOUT;
}

/**
 * Read the value of the SCL pin.
 * @returns 0 if the pin is low and 1 otherwise.
 */
u8 sensirion_SCL_read() {
    return (u8)nrf_gpio_pin_read(scl);
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(u32 useconds) {
    nrf_delay_us(useconds);
}
