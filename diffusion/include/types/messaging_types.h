/**
 * @file messaging_types.h
 *
 * Types used across messaging and messaging control.
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

#ifndef _diffusion_messaging_types_h_
#define _diffusion_messaging_types_h_ 1

/**
 * @brief Options sub-structure related to requests.
 */
typedef struct send_options_s {
        /// Priority of message from Diffusion to handling client.
        CLIENT_SEND_PRIORITY_T priority;
        /// List of headers associated with this message (strings).
        LIST_T *headers;
} SEND_OPTIONS_T;


#endif
