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
 * \file se_interface.h
 * \brief Wrapper for Blockchain Security 2Go Starter
 *        Kit v2 command set
 */
 
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "se_interface.h"
#include "bs2go/blocksec2go/blocksec2go.h"
#include "bs2go/error/error.h"
#include "protocol/protocol.h"
#include "bs2go/psoc6-i2c/ifx/psoc6-i2c.h"
#include "bs2go/psoc6-i2c/psoc6-i2c.h"
#include "bs2go/t1prime/ifx/t1prime.h"


static Protocol protocol;
static Protocol driver;

uint16_t
se_interface_init ()
{

	/* Initialize PSoC™ 6 I2C driver */
	int status = psoc6_i2c_initialize (&driver);
	if (status != PROTOCOLLAYER_INITIALIZE_SUCCESS)
	{
		printf ("rpi error: %i\n\r", status);
		return status;
	}

	/* Initialize T=1' protocol */
	status = t1prime_initialize (&protocol, &driver);
	if (status != PROTOCOLLAYER_INITIALIZE_SUCCESS)
	{
		printf ("t1prime error: %i\n\r", status);
		protocol_destroy (&driver);
		return status;
	}

	/* Set slave Address */
	i2c_set_slave_address (&driver, I2C_ADDRESS);

	/* Activate secure element */
	uint8_t *response = NULL;
	size_t response_len = 0;

	status = protocol_activate (&protocol, &response, &response_len);

	if (status != PROTOCOL_ACTIVATE_SUCCESS)
	{
		printf ("activate error: %i\n\r", status);
		protocol_destroy (&driver);
		return status;
	}
	free (response);

	return SUCCESS;
}

int
wrap_block2go_select (uint8_t id[BLOCK2GO_ID_LEN], char **version)
{
	return block2go_select (&protocol, id, version);
}

int
wrap_gen_key (uint8_t *key_index)
{
	int status = block2go_generate_key_permanent (
			&protocol, BLOCK2GO_CURVE_NIST_P256, key_index);
	if (status != BLOCK2GO_GENERATE_KEY_SUCCESS)
	{
		fprintf (stderr, "GENERATE KEY failed(0x%08x)\n", status);
		protocol_destroy (&driver);
	}
	return status;
}

int
wrap_get_pub_key (uint8_t key_index, uint8_t *public_key[65],
		uint8_t *public_key_len,block2go_curve curve)
{
	curve = BLOCK2GO_CURVE_NIST_P256;
	uint32_t global_counter = 0;
	uint32_t counter = 0;
	*public_key_len = BLOCK2GO_PUBLIC_KEY_LEN;

	int status = block2go_get_key_info_permanent (
			&protocol, key_index, &curve, &global_counter, &counter, public_key);

	if (status != BLOCK2GO_GET_KEY_INFO_SUCCESS)
	{
		fprintf (stderr, "GET KEY INFO failed (0x%08x)\n", status);
		protocol_destroy (&driver);
		free (*public_key);
	}
	return status;
}

int
wrap_sign (uint8_t key_index, uint8_t data_to_sign[32], uint8_t **signature,
		size_t *signature_len)
{
	uint32_t counter = 0;
	uint32_t global_counter = 0;

	int status = block2go_generate_signature_permanent (
			&protocol, key_index, data_to_sign, &global_counter, &counter, signature,
			signature_len);
	if (status != BLOCK2GO_GENERATE_SIGNATURE_SUCCESS)
	{
		fprintf (stderr, "\nGENERATE SIGNATURE failed (0x%08x)\n", status);
		protocol_destroy (&driver);
	}

	return status;
}

int
wrap_verify (uint8_t public_key[65], uint8_t message_len, uint8_t *signature,
		uint8_t *message,block2go_curve curve)
{

	int status = block2go_verify_signature (&protocol, curve, message,
			message_len, signature, public_key);
	if (status != BLOCK2GO_VERIFY_SIGNATURE_SUCCESS)
	{
		fprintf (stderr, "\n Verify SIGNATURE failed (0x%08x)\n", status);
		protocol_destroy (&driver);
	}

	return status;
}
