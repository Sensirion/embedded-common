/*
 * Copyright (c) 2021, Sensirion AG
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


#include "delay.h"
#include "asf.h"


/*
*   Fixed I2C on SAMR architectures, this code its done to work with ASF from Microchip.
*   15/11/2021
*   On the i2c init, you should provide the CONF_MASTER_SDA_PINMUX and CONF_MASTER_SCK_PINMUX pin configuration.
*   Then, we should configure the SERCOM module to use, with the CONF_I2C_MASTER_MODULE parameter, an example configuration for the ATSAMDR34J18 could be like:
*   
*   #define CONF_I2C_MASTER_MODULE    SERCOM3
*   #define CONF_MASTER_SDA_PINMUX    PINMUX_PA16D_SERCOM3_PAD0
*   #define CONF_MASTER_SCK_PINMUX    PINMUX_PA17D_SERCOM3_PAD1
*   #define TIMEOUT 1000
*
*  
*/

/**
 * Select the current i2c bus by index.
 * All following i2c operations will be directed at that bus.
 *
 * THE IMPLEMENTATION IS OPTIONAL ON SINGLE-BUS SETUPS (all sensors on the same
 * bus)
 *
 * @param bus_idx   Bus index to select
 * @returns         0 on success, an error code otherwise
 */
int16_t sensirion_i2c_select_bus(uint8_t bus_idx) {
    // IMPLEMENT or leave empty if all sensors are located on one single bus
    return SENSIRION_STATUS_FAIL;
}

struct i2c_master_module i2c_master_instance;

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init(void) {
    /* Initialize config structure and software module. */
    struct i2c_master_config config_i2c_master;
    i2c_master_get_config_defaults(&config_i2c_master);
    /* Change buffer timeout to something longer if needed. */
    config_i2c_master.buffer_timeout = TIMEOUT;
    config_i2c_master.pinmux_pad0    = CONF_MASTER_SDA_PINMUX;
    config_i2c_master.pinmux_pad1    = CONF_MASTER_SCK_PINMUX;

    /* Initialize and enable device with config. */
    i2c_master_init(&i2c_master_instance, CONF_I2C_MASTER_MODULE, &config_i2c_master);
    i2c_master_enable(&i2c_master_instance);
}

/**
 * Release all resources initialized by sensirion_i2c_init().
 */
void sensirion_i2c_disable(void) {
    i2c_master_disable(&i2c_master_instance);
	
}

void sensirion_i2c_enable(void){
	i2c_master_enable(&i2c_master_instance);
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
int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count) {
     uint16_t timeout = 0;
	 
	 
	    struct i2c_master_packet packet = {
		    .address = address,
		    .data_length = count,
		    .data = (uint8_t*)data,
		    .ten_bit_address = false,
		    .high_speed = false,
			.hs_master_code  = 0x0,
	    };
		
	    
		    while (i2c_master_read_packet_wait(&i2c_master_instance, &packet) !=
		    STATUS_OK) {
			    /* Increment timeout counter and check if timed out if needed. */
			    if (timeout++ == TIMEOUT) {
					return -1;
				    break;
			    }
		    }
			
		return 0;


		

}

/**
 * Execute one write transaction on the I2C bus, sending a given number of
 * bytes. The bytes in the supplied buffer must be sent to the given address. If
 * the slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data,
                           uint16_t count) {
	uint16_t timeout = 0;
    struct i2c_master_packet packet = {
	    .address = address,
	    .data_length = count,
	    .data = (uint8_t*)data,
	    .ten_bit_address = false,
	    .high_speed = false,
    };
	    while (i2c_master_write_packet_wait(&i2c_master_instance, &packet) !=
	    STATUS_OK) {
		    /* Increment timeout counter and check if timed out. */
		    if (timeout++ == TIMEOUT) {
			    break;
				return -1;
		    }
	    }


    return 0;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * Despite the unit, a <10 millisecond precision is sufficient.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {
	delay_cycles_us(useconds);
}
