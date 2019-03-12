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

#include "sensirion_i2c.h"
#include "sensirion_common.h"

#include "cy_pdl.h"
#include "cycfg.h"



/*******************************************************************************
 * global variables
 *******************************************************************************/
static cy_stc_scb_i2c_context_t mI2C_context;

/* Combine master error statuses in single mask  */
#define MASTER_ERROR_MASK  (CY_SCB_I2C_MASTER_DATA_NAK | CY_SCB_I2C_MASTER_ADDR_NAK    | \
                            CY_SCB_I2C_MASTER_ARB_LOST | CY_SCB_I2C_MASTER_ABORT_START | \
                            CY_SCB_I2C_MASTER_BUS_ERR)

/*******************************************************************************
* Forward declaration
*******************************************************************************/
void mI2C_Interrupt(void);


void mI2C_Interrupt(void)
{
    Cy_SCB_I2C_Interrupt(mI2C_HW, &mI2C_context);
}

/* INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init()
{
	cy_en_scb_i2c_status_t initStatus;
	cy_en_sysint_status_t sysStatus;
	cy_stc_sysint_t mI2C_SCB_IRQ_cfg =
	{
			/*.intrSrc =*/ scb_3_interrupt_IRQn,
			/*.intrPriority =*/ 2UL
	};
	/* Initialize and enable the I2C in master mode. */
	initStatus = Cy_SCB_I2C_Init(mI2C_HW, &mI2C_config, &mI2C_context);
	if(initStatus != CY_SCB_I2C_SUCCESS)
	{
		// return STATUS_FAIL;
	}

	/* Hook interrupt service routine. */
	sysStatus = Cy_SysInt_Init(&mI2C_SCB_IRQ_cfg, &mI2C_Interrupt);
	if(sysStatus != CY_SYSINT_SUCCESS)
	{
		// return STATUS_FAIL;
	}

	/* Enable interrupt in NVIC. */
	NVIC_EnableIRQ((IRQn_Type) mI2C_SCB_IRQ_cfg.intrSrc);

	/* Enable I2C master hardware. */
	Cy_SCB_I2C_Enable(mI2C_HW);
	// return STATUS_OK;
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
	uint8_t status = STATUS_FAIL;
	cy_en_scb_i2c_status_t errorStatus;
	uint32_t masterStatus;
	/* Timeout 1 sec (one unit is us) */
	uint32_t timeout = 1000000UL;

	/* Setup transfer specific parameters */
	cy_stc_scb_i2c_master_xfer_config_t transferConfig =
	{
		.slaveAddress = address,
		.buffer       = NULL,
		.bufferSize   = 0U,
		.xferPending  = false
	};
	transferConfig.buffer     = data;
	transferConfig.bufferSize = count;

	/* Initiate read transaction */
	errorStatus = Cy_SCB_I2C_MasterRead(mI2C_HW, &transferConfig, &mI2C_context);
	if(errorStatus == CY_SCB_I2C_SUCCESS)
	{
		/* Wait until master complete read transfer or time out has occured */
		do
		{
			masterStatus  = Cy_SCB_I2C_MasterGetStatus(mI2C_HW, &mI2C_context);
			Cy_SysLib_DelayUs(CY_SCB_WAIT_1_UNIT);
			timeout--;

		} while ((0UL != (masterStatus & CY_SCB_I2C_MASTER_BUSY)) && (timeout > 0));

	   if (timeout <= 0)
		{
			/* Timeout recovery */
			Cy_SCB_I2C_Disable(mI2C_HW, &mI2C_context);
			Cy_SCB_I2C_Enable (mI2C_HW);
		}
		else
		{
			/* Check transfer status */
			if (0u == (MASTER_ERROR_MASK & masterStatus))
			{
				status = STATUS_OK;
			}
		}
	}
	return (status);
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
	uint8_t status = STATUS_FAIL;
	cy_en_scb_i2c_status_t  errorStatus;
	uint32_t masterStatus;
	/* Timeout 1 sec (one unit is us) */
	uint32_t timeout = 1000000UL;

	/* Setup transfer specific parameters. */
	cy_stc_scb_i2c_master_xfer_config_t transferConfig =
	{
	    .slaveAddress = address,
	    .buffer       = NULL,
	    .bufferSize   = 0U,
	    .xferPending  = false
	};
	transferConfig.buffer     = data;
	transferConfig.bufferSize = count;

	/* Initiate write transaction. */
	errorStatus = Cy_SCB_I2C_MasterWrite(mI2C_HW, &transferConfig, &mI2C_context);
	if(errorStatus == CY_SCB_I2C_SUCCESS)
	{
		/* Wait until master complete read transfer or time out has occurred */
		do
		{
			masterStatus  = Cy_SCB_I2C_MasterGetStatus(mI2C_HW, &mI2C_context);
			Cy_SysLib_DelayUs(CY_SCB_WAIT_1_UNIT);
			timeout--;
		} while ((0UL != (masterStatus & CY_SCB_I2C_MASTER_BUSY)) && (timeout > 0));

		if (timeout <= 0)
		{
			/* Timeout recovery */
			Cy_SCB_I2C_Disable(mI2C_HW, &mI2C_context);
			Cy_SCB_I2C_Enable (mI2C_HW);
		}
		else
		{
			if ((0u == (MASTER_ERROR_MASK & masterStatus)) &&
				(count == Cy_SCB_I2C_MasterGetTransferCount(mI2C_HW, &mI2C_context)))
			{
				status = STATUS_OK;
			}
		}
	}

	return (status);
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {
	Cy_SysLib_DelayUs(useconds);
}
