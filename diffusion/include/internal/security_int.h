/*
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

#ifndef _diffusion_security_int_h_
#define _diffusion_security_int_h_ 1

/**
 * Convert credentials into a form that may be used to transmit to Diffusion.
 * Only PLAIN_PASSWORD is supported at this time.
 * 
 * @param credentials	The credentials to be serialised.
 * @retval "BUF_T *"	A buffer containing the serialised credentials.
 * @retval NULL		If the credentials could not be serialised.
 */
BUF_T *credentials_serialise(const CREDENTIALS_T *credentials);

#endif
