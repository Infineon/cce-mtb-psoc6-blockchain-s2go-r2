/******************************************************************************
 * File Name:   main.c
 *
 * Description: This is the source code for the PSoC 6 MCU and Infineon's
 *               Blockchain Security 2Go Starterkit R2 for ModusToolbox.
 *
 *
 * Related Document: See README.md
 *
 *
 * The MIT License
 *
 * Copyright (c) 2023 Infineon Technologies AG
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
 * SOFTWARE
 *******************************************************************************/


#include "cy_pdl.h"
#include "cyhal.h"
#include "cybsp.h"
#include "cy_retarget_io.h"


#include "se_interface.h"

/*******************************************************************************
 * Macros
 *******************************************************************************/

#define KEY_INDEX                 (0x10) /* Key index used to execute the commands */

/*******************************************************************************
 * Function Name: main
 ********************************************************************************
 * Summary:
 * This is the main function for CM4 CPU. It accepts the command as an input
 * from the user and displays the results in UART terminal.
 *
 *
 * Parameters:
 *  none
 *
 * Return:
 *  int
 *
 *******************************************************************************/
int main(void)
{
	cy_rslt_t status;
	uint8_t read_data; /* Stores the received character from the user*/
	uint8_t *public_key = 0; /* Stores the Public key */
	uint8_t public_key_len = 0; /* length of the Public key  */

	/* SHA-256 digest used for Signing*/
	uint8_t data_to_sign[32]={0x8A,0x83,0x66,0x5F,0x37,0x98,0x72,0x7F,0x14,0xF9,0x2A,0xD0,
			0xE6,0xC9,0x9F,0xDA,0xB0,0x8E,0xE7,0x31,0xD6,0xCD,0x64,0x4C,
			0x13,0x12,0x23,0xFD,0x2F,0x4F,0xED,0x2A};

	uint8_t *signature=0;	/* Signature */
	size_t signature_len=0; /* length of the signature */
	uint8_t key_index; /* Stores the generated key index */
	char *version = 0;  /* Version */
	uint8_t id[BLOCK2GO_ID_LEN]; /* Stores the ID of the Secure Element*/
	block2go_curve curve; /* ECC curve type*/

	/* Initialize the device and board peripherals */
	status = cybsp_init();

	/* Board initialization failed. Stop program execution */
	if (status != CY_RSLT_SUCCESS)
	{
		CY_ASSERT(0);
	}

	/* Enable global interrupts */
	__enable_irq();

	/* Initialize retarget-io to use the debug UART port */
	cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);




	/* \x1b[2J\x1b[;H - ANSI ESC sequence for clear screen */
	printf("\x1b[2J\x1b[;H");

	printf("***********************************************************\r\n");
	printf("**********BLOCKCHAIN S2Go R2 KIT DEMO WITH PSOC 6**********\r\n");
	printf("***********************************************************\r\n\n");

	printf("1. SECURE ELEMENT INIT\r\n\n");
	printf("2. SELECT APPLICATION\r\n\n");
	printf("3. GET PUBLIC KEY\r\n\n");
	printf("4. GENERATE SIGNATURE\r\n\n");
	printf("5. VERIFY SIGNATURE\r\n\n");
	printf("6. GENERATE KEY\r\n\n");

	for (;;)
	{

		if (CY_RSLT_SUCCESS == cyhal_uart_getc(&cy_retarget_io_uart_obj, &read_data, 0))
		{
			switch (read_data)
			{
			case '1':
			{
				/* Initialize the Secure Element */
				status = se_interface_init();
				if (status!=CY_RSLT_SUCCESS)
				{
					break;
				}
				printf("\n\r SECURE ELEMENT INIT Successful \r\n\n");
				break;
			}
			case '2':
			{
				/* Execute the SELECT APP command */
				status = wrap_block2go_select(id, &version);
				if (status!=CY_RSLT_SUCCESS)
				{
					break;
				}
				printf("Version : %s", version);

				printf("\r\n\n");

				printf("ID      : ");

				for (int i = 1; i < BLOCK2GO_ID_LEN; i++)
				{
					printf("%02X", id[i]);
				}
				printf("\r\n\n");
				break;
			}
			case '3':
			{
				/* Execute the GET KEY INFO command */
				status = wrap_get_pub_key(KEY_INDEX, &public_key, &public_key_len,curve);
				if (status!=CY_RSLT_SUCCESS)
				{
					break;
				}
				printf("Public key at index %d : \n\r",KEY_INDEX);

				for (int i = 0; i < public_key_len; i++)
				{
					printf(" 0x%02X", public_key[i]);
				}
				printf("\r\n\n");
				break;
			}
			case '4':
			{
				/* Execute the GENERATE SIGNATURE command */
				printf("Signing the digest using the key at index %d.\n\r\n",KEY_INDEX);
				status=wrap_sign(KEY_INDEX,data_to_sign,&signature,&signature_len);
				if(status!=CY_RSLT_SUCCESS)
				{
					break;
				}
				printf("Signature :\n\r");
				for (int i = 0; i < signature_len; i++)
				{
					printf(" 0x%02X", signature[i]);
				}
				printf("\n\r\n");
				break;
			}
			case '5':
			{
				/* Execute the VERIFY SIGNATURE command */
				status=wrap_verify(public_key,sizeof(data_to_sign),signature,data_to_sign,curve);
				if(status!=CY_RSLT_SUCCESS)
				{
					break;
				}
				printf("VERIFY SIGNATURE is Successful\n\r\n");
				break;
			}
			case '6':
			{
				/* Execute the GENERATE KEY command */
				status=wrap_gen_key(&key_index);
				if(status!=CY_RSLT_SUCCESS)
				{
					break;
				}
				printf("\nKey Generated Successfully at index: %d\n\r",key_index);
				break;
			}
			default:
			{
				printf("Invalid Option\n\r");
				break;
			}
			}
		}
	}
}
