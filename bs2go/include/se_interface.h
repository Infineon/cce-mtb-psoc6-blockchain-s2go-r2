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
 * \file ifx/se_interface.h
 * \brief Wrapper for Blockchain Security 2Go Starter
 *              Kit v2 command set
 */

#ifndef _IFX_SE_INTERFACE_H_
#define _IFX_SE_INTERFACE_H_


#ifdef __cplusplus
extern "C"
{
#endif
#include "bs2go/blocksec2go/blocksec2go.h"
#include <stdint.h>
/**
 * \brief Initializes Secure Element
 *
 * \param  none
 *
 * \return uint16_t   SUCCESS if successful, any other value in case of error.
 */
  uint16_t se_interface_init ();
/**
 * \brief SELECT the Blockchain Security 2Go application.
 *
 * \param[out] id         buffer to copy SE id to
 * \param[out] version    pointer to write address of allocated zero terminated
 *
 * \retval SUCCESS in case of success
 */
  int wrap_block2go_select (uint8_t id[BLOCK2GO_ID_LEN], char **version);
/**
 * \brief Creates new ECC public/private keypair.
 *
 * \param[out] key_index   received keyslot index
 *
 * \retval SUCCESS in case of success
 */  
  int wrap_gen_key (uint8_t *key_index);
/**
 * \brief Returns the public key.
 *
 * \param[in] key_index   		key index for which info should be given
 * \param[out] public_key  		Sec1 encoded umcompressed public key (65 bytes)
 * \param[out] public_key_len   Public key length
 * \param[out] curve      ECC curve type
 *
 * \retval SUCCESS in case of success
 */
  int wrap_get_pub_key (uint8_t key_index, uint8_t *public_key[65],
                        uint8_t *public_key_len,block2go_curve curve);
/**
 * \brief Signs a given block of prehashed data using the stored private key
 * that is associated with the key.
 *
 * \param[in] data_to_sign    hashed data that should be signed
 * \param[in] key_index       key index to be used for signing
 * \param[out] signature      buffer for storing ANS.1 DER encoded signature 
 * \param[out] signature_len  buffer to copy length of the signature in bytes into
 *
 * \retval SUCCESS in case of success
 */
  int wrap_sign (uint8_t key_index, uint8_t data_to_sign[32],
                 uint8_t **signature, size_t *signature_len);
/**
 * \brief Checks whether a given ECDSA signature is valid.
 *
 * \param[in] message     hashed message
 * \param[in] message_len length of message in bytes
 * \param[in] signature   ANS.1 DER encoded signature which is to be verifed
 * \param[in] public_key  Sec1 encoded umcompressed public key (65 bytes)
 * \param[in] curve       ECC curve type
 *
 * \retval SUCCESS in case of success
 */				 
  int wrap_verify (uint8_t public_key[65], uint8_t message_len,
                   uint8_t *signature, uint8_t *message,block2go_curve curve);

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _IFX_SE_INTERFACE_H_ */
