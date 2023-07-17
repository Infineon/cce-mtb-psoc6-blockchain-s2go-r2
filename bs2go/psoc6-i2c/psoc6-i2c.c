/*
 * MIT License
 *
 * Copyright (c) 2020 Infineon Technologies AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
/**
* \file psoc6-i2c.c
 * \brief  PSoC™ 6 I2C driver implementation
 */
#include <stdio.h>
#include <stdlib.h>
#include "cyhal.h"
#include "cybsp.h"



#include "bs2go/i2c/i2c.h"
#include "bs2go/protocol/protocol.h"
#include "bs2go/psoc6-i2c/ifx/psoc6-i2c.h"
#include "bs2go/psoc6-i2c/psoc6-i2c.h"
#include "bs2go/blocksec2go/blocksec2go.h"


/*******************************************************************************
 * Macros
 *******************************************************************************/

#define mI2C_SCL                    (CYBSP_I2C_SCL) /* I2C SCL pin mapping */

#define mI2C_SDA                    (CYBSP_I2C_SDA) /* I2C SDA pin mapping */

/*******************************************************************************
 * Global Variables
 *******************************************************************************/
static cyhal_i2c_t mI2C; /* I2C object*/

cy_rslt_t result;

static uint16_t I2C_SLAVE_ADDR; /* I2C slave address to communicate with */

static uint32_t I2C_FREQ; /* I2C bus frequency in Hz */


/**
 * \brief Initializes the I2C driver for PSoC™ 6
 *
 * \return int CY_RSLT_SUCCESS if successful, any other value
 * in case of error
 */

uint32_t
i2c_init ()
{

	cyhal_i2c_cfg_t mI2C_cfg; /* I2C configuration object */

	/* I2C Master configuration settings */
	mI2C_cfg.is_slave = false;
	mI2C_cfg.address = 0;
	mI2C_cfg.frequencyhal_hz = I2C_FREQ;

	/* Initialize I2C master */
	result = cyhal_i2c_init (&mI2C, mI2C_SDA, mI2C_SCL, NULL);

	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

	/* Configure I2C Master */
	result = cyhal_i2c_configure (&mI2C, &mI2C_cfg);
	if (result != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}
	return result;
}
/**
 * \brief Returns current protocol state for of PSoC™ 6 I2C driver layer
 *
 * \param self I2C driver layer to get protocol state for
 * \param protocol_state_buffer Buffer to store protocol state in
 * \return int   PROTOCOL_GETPROTPERTY_SUCCESS if successful, any other value
 * in case of error
 */

int
i2c_get_protocol_state (Protocol *self, ProtocolState **protocol_state_buffer)
{
	/* Verify that correct protocol layer called this function */
	if (self->_layer_id != PSOC6_I2C_PROTOCOLLAYER_ID)
	{
		if (self->_base == NULL)
		{
			return IFX_ERROR (LIBPSOC6I2C, PROTOCOL_GETPROPERTY,
					INVALID_PROTOCOLSTACK);
		}
		return i2c_get_protocol_state (self->_base, protocol_state_buffer);
	}

	/* Check if protocol state has been initialized */
	if (self->_properties == NULL)
	{
		/* Lazy initialize properties */
		self->_properties = malloc (sizeof (ProtocolState));
		if (self->_properties == NULL)
		{
			return IFX_ERROR (LIBPSOC6I2C, PROTOCOL_GETPROPERTY, OUT_OF_MEMORY);
		}
		ProtocolState *properties = (ProtocolState *)self->_properties;
		properties->slave_address = (uint16_t)I2C_DEFAULT_SLAVE_ADDRESS;
		properties->clock_frequency = (uint32_t)I2C_DEFAULT_CLOCK_FREQUENCY;
	}

	*protocol_state_buffer = (ProtocolState *)self->_properties;
	return PROTOCOL_GETPROPERTY_SUCCESS;
}

/**
 * \brief Initializes \ref Protocol object for PSoC™ 6 driver layer
 *
 * \param self \ref Protocol object to be initialized.
 * \return int   PROTOCOLLAYER_INITIALIZE_SUCCESS if successful, any other
 * value in case of error.
 */

int
psoc6_i2c_initialize (Protocol *self)
{
	/* Validate parameters */
	if (self == NULL)
	{
		return IFX_ERROR (LIBPSOC6I2C, PROTOCOLLAYER_INITIALIZE, ILLEGAL_ARGUMENT);
	}

	/* Populate object */
	int status = protocollayer_initialize (self);
	if (status != PROTOCOLLAYER_INITIALIZE_SUCCESS)
	{
		return status;
	}

	self->_layer_id = PSOC6_I2C_PROTOCOLLAYER_ID;
	self->_activate = NULL;
	self->_transmit = psoc6_i2c_transmit;
	self->_receive = psoc6_i2c_receive;
	self->_destructor = psoc6_i2c_destroy;

	/* Set I2C clock frequency in [Hz] */
	i2c_set_clock_frequency (self, I2C_FREQUENCY);

	/*Initialization of the I2C */
	result = i2c_init ();

	if (result!=CY_RSLT_SUCCESS)
	{
		return -1;
	}
	return PROTOCOLLAYER_INITIALIZE_SUCCESS;
}

/**
 * \brief \ref protocol_destroyfunction_t for PSoC™ 6 driver layer
 *
 * \see protocol_destroyfunction_t
 */
void
psoc6_i2c_destroy (Protocol *self)
{
	if (self != NULL)
	{
		/* Free properties */
		if (self->_properties != NULL)
		{
			free (self->_properties);
			self->_properties = NULL;
		}
		/* Deinitialize the i2c object */
		cyhal_i2c_free (&mI2C);
	}
}

/**
 * \brief \ref protocol_transmitfunction_t for PSoC™ 6 driver layer
 *
 * \see protocol_transmitfunction_t
 */
int
psoc6_i2c_transmit (Protocol *self, uint8_t *data, size_t data_len)
{
	/* TODO: Implement */
	/* Validate parameters */

	if ((self == NULL) || (data == NULL) || (data_len == 0)
			|| (data_len > 0xffffffff))
	{

		return IFX_ERROR (LIBPSOC6I2C, PROTOCOL_TRANSMIT, ILLEGAL_ARGUMENT);
	}

	/* Send packet with command to the slave */
	result = cyhal_i2c_master_write (&mI2C, I2C_SLAVE_ADDR, data, data_len, 0,
			true);

	if (result!= CY_RSLT_SUCCESS)
	{

		return result;
	}

	return PROTOCOL_TRANSMIT_SUCCESS;
}

/**
 * \brief \ref protocol_receivefunction_t for PSoC™ 6 driver layer
 *
 * \see protocol_receivefunction_t
 */
int
psoc6_i2c_receive (Protocol *self, size_t expected_len, uint8_t **response,
		size_t *response_len)
{

	/* TODO: Implement */
	/* Validate parameters */
	if ((self == NULL) || (expected_len == 0) || (expected_len > 0xffffffff)
			|| (response == NULL) || (response_len == NULL))
	{

		return IFX_ERROR (LIBPSOC6I2C, PROTOCOL_RECEIVE, ILLEGAL_ARGUMENT);
	}

	/* Allocate buffer for I2C receive */
	*response = (uint8_t *)malloc (expected_len);
	if ((*response) == NULL)
	{

		return IFX_ERROR (LIBPSOC6I2C, PROTOCOL_RECEIVE, OUT_OF_MEMORY);
	}

	/* Read response packet from the slave */
	result = cyhal_i2c_master_read (&mI2C, I2C_SLAVE_ADDR, (*response),
			expected_len, 0, true);

	if (result!=CY_RSLT_SUCCESS)
	{
		free (*response);
		*response = NULL;
		*response_len = 0;
		

		return result;
	}

	*response_len = expected_len;

	return PROTOCOL_RECEIVE_SUCCESS;
}

/**
 * \brief Getter for I2C clock frequency in [Hz]
 *
 * \param self Protocol object to get clock frequency for
 * \param frequency_buffer Buffer to store clock frequency in
 * \return int   PROTOCOL_GETPROPERTY_SUCCESS if successful, any other value
 * in case of error
 */
int
i2c_get_clock_frequency (Protocol *self, uint32_t *frequency_buffer)
{
	ProtocolState *protocol_state;
	int status = i2c_get_protocol_state (self, &protocol_state);
	if (status != PROTOCOL_GETPROPERTY_SUCCESS)
	{
		return status;
	}
	*frequency_buffer = protocol_state->clock_frequency;
	return PROTOCOL_GETPROPERTY_SUCCESS;
}

/**
 * \brief Sets I2C clock frequency in [Hz]
 *
 * \param self Protocol object to set clock frequency for
 * \param frequency Desired clock frequency in [Hz]
 * \return int   PROTOCOL_SETPROPERTY_SUCCESS if successful, any other value
 * in case of error
 */
int
i2c_set_clock_frequency (Protocol *self, uint32_t frequency)
{
	ProtocolState *protocol_state;
	int status = i2c_get_protocol_state (self, &protocol_state);
	if (status != PROTOCOL_GETPROPERTY_SUCCESS)
	{
		return status;
	}
	protocol_state->clock_frequency = frequency;
	I2C_FREQ = frequency;

	return PROTOCOL_SETPROPERTY_SUCCESS;
}

/**
 * \brief Getter for I2C slave address
 *
 * \param self Protocol object to get I2C slave address for
 * \param address_buffer Buffer to store I2C address in
 * \return int   PROTOCOL_GETPROPERTY_SUCCESS if successful, any other value
 * in case of error
 */
int
i2c_get_slave_address (Protocol *self, uint16_t *address_buffer)
{
	/* TODO: Implement */
	ProtocolState *protocol_state;
	int status = i2c_get_protocol_state (self, &protocol_state);
	if (status != PROTOCOL_GETPROPERTY_SUCCESS)
	{
		return status;
	}
	*address_buffer = protocol_state->slave_address;
	return PROTOCOL_GETPROPERTY_SUCCESS;
}

/**
 * \brief Sets I2C slave address
 *
 * \param self Protocol object to set I2C slave address for
 * \param address Desired I2C slave address
 * \return int   PROTOCOL_SETPROPERTY_SUCCESS if successful, any other value
 * in case of error
 */
int
i2c_set_slave_address (Protocol *self, uint16_t address)
{
	/* TODO: Implement */
	/* Validate parameters */
	if (self == NULL)
	{
		return IFX_ERROR (LIBPSOC6I2C, PROTOCOLLAYER_INITIALIZE, ILLEGAL_ARGUMENT);
	}

	ProtocolState *protocol_state;
	int status = i2c_get_protocol_state (self, &protocol_state);
	if (status != PROTOCOL_GETPROPERTY_SUCCESS)
	{
		return status;
	}
	protocol_state->slave_address = address;
	I2C_SLAVE_ADDR = address;
	return PROTOCOL_SETPROPERTY_SUCCESS;
}
