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
 * \file psoc6-i2c.h
 *
 * \brief Internal definitions for PSoC™ 6 I2C driver layer
 */
#ifndef _PSOC6_I2C_H_
#define _PSOC6_I2C_H_

#include <stdint.h>

#include "bs2go/protocol/protocol.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief State of I2C driver layer
 */
typedef struct ProtocolState
{
	uint16_t slave_address;   /**< I2C address currently in use */
	uint32_t clock_frequency; /**< I2C clock frequency in [Hz] */
} ProtocolState;

/**
 * \brief Protocol Layer ID for PSoC™ 6 driver layer
 *
 * \details Used to verify that correct protocol layer has called member
 * functionality
 */
#define PSOC6_I2C_PROTOCOLLAYER_ID 0x34

/**
 * \brief Default value for I2C address used
 */
#define I2C_DEFAULT_SLAVE_ADDRESS ((uint8_t)0x50)

/**
 * \brief Default value for I2C clock frequency in [Hz]
 */
#define I2C_DEFAULT_CLOCK_FREQUENCY ((uint32_t)400000)

/**
 * \brief \ref protocol_transmitfunction_t for PSoC™ 6 driver layer
 *
 * \see protocol_transmitfunction_t
 */
int psoc6_i2c_transmit (Protocol *self, uint8_t *data, size_t data_len);

/**
 * \brief \ref protocol_receivefunction_t for PSoC™ 6 driver layer
 *
 * \see protocol_receivefunction_t
 */
int psoc6_i2c_receive (Protocol *self, size_t expected_len, uint8_t **response,
		size_t *response_len);

/**
 * \brief \ref protocol_destroyfunction_t for PSoC™ 6 driver layer
 *
 * \see protocol_destroyfunction_t
 */
void psoc6_i2c_destroy (Protocol *self);

#ifdef __cplusplus
}
#endif

#endif /* _PSOC6_I2C_H_*/
