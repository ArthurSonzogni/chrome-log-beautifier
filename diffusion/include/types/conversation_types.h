/**
 * @file conversation_types.h
 *
 * Definitions for types representing conversations and conversation IDs.
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

#ifndef _diffusion_conversation_types_h_
#define _diffusion_conversation_types_h_ 1

#include <inttypes.h>

/**
 * Structure representing a conversation ID.
 */
typedef struct conversation_id_s {
        /// Internal conversation identifier.
        uint64_t id;
} CONVERSATION_ID_T;

#endif
