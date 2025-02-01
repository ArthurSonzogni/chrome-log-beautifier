/**
 * @file security_types.h
 *
 * Constants and types relating to security and authorisation functions.
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
#ifndef _diffusion_security_types_h_
#define _diffusion_security_types_h_ 1

#include "buf.h"
#include "types/common_types.h"

/**
 * Credentials associated with a principal can come in multiple forms.
 * This enumeration lists the ones that may be received.
 */
typedef enum {
	NONE,
	PLAIN_PASSWORD,
	CUSTOM
} CREDENTIALS_TYPE_T;

/**
 * This structure is used for passing credentials around in the API.
 */
typedef struct credentials_s {
        /// The type of credentials being used.
	CREDENTIALS_TYPE_T type;
        /// Bytes containing credential data.
	BUF_T *data;
} CREDENTIALS_T;

#endif
