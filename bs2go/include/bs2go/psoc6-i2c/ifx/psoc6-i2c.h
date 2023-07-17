
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
 * \brief PSoC™ 6 I2C driver implementation
 */
#ifndef _IFX_PSOC6_I2C_H_
#define _IFX_PSOC6_I2C_H_

#include "bs2go/error/error.h"
#include "bs2go/i2c/i2c.h"
#include "bs2go/protocol/protocol.h"

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief IFX error code module identifer
 */
#define LIBPSOC6I2C 0x35

/**
 * \brief Initializes \ref Protocol object for PSoC™ 6 driver layer
 *
 * \param self \ref Protocol object to be initialized.
 * \return int   PROTOCOLLAYER_INITIALIZE_SUCCESS if successful, any other
 * value in case of error.
 */
int psoc6_i2c_initialize (Protocol *self);

#ifdef __cplusplus
}
#endif

#endif /* _IFX_PSOC6_I2C_H_*/
