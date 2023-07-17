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
 * \file ifx/i2c.h
 * \author Martin Kloesch (martin.kloesch-ee@infineon.com)
 * \brief Generic API for I2C drivers
 */
#ifndef _IFX_I2C_H_
#define _IFX_I2C_H_

#include <stdint.h>
#include "bs2go/protocol/protocol.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief IFX error code module identifer
 */
#define LIBI2C 0x23

  /**
   * \brief Getter for I2C clock frequency in [Hz]
   *
   * \param self Protocol object to get clock frequency for
   * \param frequency_buffer Buffer to store clock frequency in
   * \return int   PROTOCOL_GETPROPERTY_SUCCESS if successful, any other value
   * in case of error
   */
  int i2c_get_clock_frequency (Protocol *self, uint32_t *frequency_buffer);

  /**
   * \brief Sets I2C clock frequency in [Hz]
   *
   * \param self Protocol object to set clock frequency for
   * \param frequency Desired clock frequency in [Hz]
   * \return int   PROTOCOL_SETPROPERTY_SUCCESS if successful, any other value
   * in case of error
   */
  int i2c_set_clock_frequency (Protocol *self, uint32_t frequency);

  /**
   * \brief Getter for I2C slave address
   *
   * \param self Protocol object to get I2C slave address for
   * \param address_buffer Buffer to store I2C address in
   * \return int   PROTOCOL_GETPROPERTY_SUCCESS if successful, any other value
   * in case of error
   */
  int i2c_get_slave_address (Protocol *self, uint16_t *address_buffer);

  /**
   * \brief Sets I2C slave address
   *
   * \param self Protocol object to set I2C slave address for
   * \param address Desired I2C slave address
   * \return int   PROTOCOL_SETPROPERTY_SUCCESS if successful, any other value
   * in case of error
   */
  int i2c_set_slave_address (Protocol *self, uint16_t address);

#ifdef __cplusplus
}
#endif

#endif /* _IFX_I2C_H_*/
