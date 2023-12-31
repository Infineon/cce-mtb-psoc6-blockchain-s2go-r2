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
 * \file crc.c
 * \brief Reusable CRC algorithms
 */
#include "bs2go/crc/crc.h"

/**
 * \brief Calculates 16 bit CRC according to CCITT x.25 specification.
 *
 * \param data Data to calculate CRC over
 * \param data_len Number of bytes in data
 * \return uint16_t CRC over data
 */
uint16_t
crc16_ccitt_x25 (uint8_t *data, size_t data_len)
{
	uint16_t crc = 0xffff;
	for (size_t i = 0; i < data_len; i++)
	{
		uint8_t b = data[i];
		crc ^= b;
		for (size_t j = 0; j < 8; j++)
		{
			if ((crc & 1) != 0)
			{
				crc = (crc >> 1) ^ 0x8408;
			}
			else
			{
				crc = crc >> 1;
			}
		}
	}
	return crc ^ 0xffff;
}

/**
 * \brief Calculates 16 bit CRC according to MCRF4xx specification
 *
 * \param data Data to calculate CRC over
 * \param data_len Number of bytes in data
 * \return uint16_t CRC over data
 */
uint16_t
crc16_mcrf4xx (uint8_t *data, size_t data_len)
{
	uint16_t crc = 0xffff;
	for (size_t i = 0; i < data_len; i++)
	{
		uint8_t b = data[i];
		crc ^= b;
		for (size_t j = 0; j < 8; j++)
		{
			if ((crc & 1) != 0)
			{
				crc = (crc >> 1) ^ 0x8408;
			}
			else
			{
				crc = crc >> 1;
			}
		}
	}
	return crc;
}

/**
 * \brief Calculates 16 bit CRC according to G+D T=1 protocol specification
 *
 * \todo Find out correct name of algorithm
 *
 * \param data Data to calculate CRC over
 * \param data_len Number of bytes in data
 * \return uint16_t CRC over data
 */
uint16_t
crc16_t1gd (uint8_t *data, size_t data_len)
{
	uint16_t crc = 0xffff;
	for (size_t i = 0; i < data_len; i++)
	{
		uint8_t b = data[i];
		for (size_t j = 0; j < 8; j++)
		{
			if ((crc ^ b) & 0x01)
			{
				crc ^= 0x10810;
			}
			crc >>= 1;
			b >>= 1;
		}
	}
	return crc;
}

/**
 * \brief Calculates 8 bit Longitudinal Redundancy Code (LRC)
 *
 * \param data Data to calculate LRC over
 * \param data_len Number of bytes in data
 * \return uint8_t LRC over data
 */
uint8_t
lrc8 (uint8_t *data, size_t data_len)
{
	uint8_t lrc = 0x00;
	for (size_t i = 0; i < data_len; i++)
	{
		lrc ^= data[i];
	}
	return lrc;
}
