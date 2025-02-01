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

#ifndef _diffusion_conversation_int_h_
#define _diffusion_conversation_int_h_ 1

/**
 * Read a conversation id from a char array.
 *
 * @param data		A pointer to the char array containing the conversation
 *			id.
 * @param conversation_id A pre-allocated conversation id structure.
 * @return		The address in the char array immediately following
 *			the conversation id.
 */
char *buf_read_conversation_id(const char *data, CONVERSATION_ID_T *conversation_id);

/**
 * Append a conversation id to a buffer.
 *
 * @param buf		The buffer to be written to.
 * @param conversation_id The conversation ID to be written.
 */
void buf_write_conversation_id(BUF_T *buf, const CONVERSATION_ID_T conversation_id);

#endif
