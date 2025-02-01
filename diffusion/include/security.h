/**
 * @file security.h
 *
 * Functions handling authorisation, security and user credentials.
 *
 * Copyright © 2014, 2015 Push Technology Ltd., All Rights Reserved.
 *
 * Use is subject to license terms.
 *
 * NOTICE: All information contained herein is, and remains the
 * property of Push Technology. The intellectual and technical
 * concepts contained herein are proprietary to Push Technology and
 * may be covered by U.S. and Foreign Patents, patents in process, and
 * are protected by trade secret or copyright law.
 */
#ifndef _diffusion_security_h_
#define	_diffusion_security_h_ 1

#include "types/common_types.h"
#include "types/security_types.h"

/**
 * Create an empty set of credentials.
 *
 * @retval "CREDENTIALS_T *"	Credentials with a type of "NONE".
 * @retval NULL			If an error occurs.
 */
CREDENTIALS_T *credentials_create_none(void);

/**
 * Create credentials for a given password.
 *
 * @param password	The password to encode into the credentials.
 * @retval "CREDENTIALS_T *"	Credentials with a type of "PLAIN_PASSWORD".
 * @retval NULL			If an error occurs.
 */
CREDENTIALS_T *credentials_create_password(const char *password);

/**
 * Create custom credentials, where the caller provides an opaque set
 * of bytes.
 *
 * @param data		An array of bytes representing the credentials.
 * @param len		The length of the array.
 * @retval "CREDENTIALS_T *"	Credentials with a type of "CUSTOM".
 * @retval NULL			If an error occurs.
 */
CREDENTIALS_T *credentials_create_custom(const char *data, const unsigned long len);

/**
 * Free a credentials structure. It is the responsibility of the caller to free
 * any memory associated with the credentials data (either a password or custom
 * data).
 *
 * @param creds		A set of credentials.
 */
void credentials_free(CREDENTIALS_T *creds);

/**
 * Deserialise credentials from a stream of wire format data.
 *
 * @param data		Pointer the the start of a serialised credentials
 *			structure in a char array.
 * @param credentials	A created credentials structure.
 * @return		A pointer to the position in the char array after the
 *			credentials structure.
 */
char *buf_read_credentials(const char *data, CREDENTIALS_T **credentials);

/**
 * Deserialise a credentials structure from a wire format BUF_T.
 *
 * @param buf		The buffer containing the serialised credentials.
 * @retval "CREDENTIALS_T *"	A credentials structure.
 * @retval NULL			If an error occurs.
 */
CREDENTIALS_T *credentials_unmarshal(const BUF_T *buf);

/**
 * Serialise a credentials structure to wire format.
 *
 * @param credentials	A credentials structure to be serialised.
 * @retval "BUF_T *"	A buffer with the credentials in wire format.
 * @retval NULL		If an error occurs.
 */
BUF_T *credentials_marshal(const CREDENTIALS_T *credentials);

#endif
