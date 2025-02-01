/**
 * @file conversation.h
 *
 * Functions for handling conversations and conversation IDs.
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
#ifndef _diffusion_conversation_h_
#define _diffusion_conversation_h_ 1

#include "types/common_types.h"
#include "types/session_types.h"
#include "types/conversation_types.h"
#include "internal/conversation_int.h"

/**
 * Create and allocate a new conversation id for this session.
 *
 * @param session	The session which this conversation is associated with.
 * @retval "CONVERSATION_ID_T *"	A conversation ID.
 * @retval NULL		If the conversation ID cannot be created.
 */
CONVERSATION_ID_T *conversation_id_create(struct session_s *session);

/**
 * Free memory allocated for a conversation id.
 *
 * @param conversation_id	The conversation id to be freed.
 */
void conversation_id_free(CONVERSATION_ID_T *conversation_id);

/**
 * Create a deep-copy duplicate of a conversation id.
 *
 * @param src		The conversation id to be copied.
 * @return		A newly allocated conversation id, or NULL if an error
 *			occurred.
 */
CONVERSATION_ID_T *conversation_id_dup(const CONVERSATION_ID_T *src);

/**
 * Take a conversation id and convert it to string representation.
 *
 * @param conversation_id	The conversation id to be converted.
 * @retval "char *"	The conversation id as a string.
 * @retval NULL		If an error occurs.
 */
char *conversation_id_to_string(const CONVERSATION_ID_T conversation_id);

/**
 * Takes a string representation of a conversation id and creates a
 * CONVERSATION_ID_T structure from it.
 *
 * @param str		The conversation id in string form.
 * @retval "OCNVERSATION_ID_T *"	A conversation id.
 * @retval NULL		If an error occurs.
 */
CONVERSATION_ID_T *conversation_id_create_from_string(const char *str);

#endif
