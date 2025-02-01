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

#ifndef _diffusion_topic_int_h_
#define _diffusion_topic_int_h_ 1

#include "hash.h"
#include "buf.h"
#include "types/topic_types.h"

/**
 * Maps a DPT topic message into a general-purpose application topic message.
 * In reality it's not much different, but it does provide the opportunity for
 * simple accessors for things such as the topic name without having to parse
 * the headers.
 *
 * Note that the new structure only contains pointers to data that already exists,
 * and you shouldn't free() this memory unless you are sure about what you're doing.
 */
TOPIC_MESSAGE_T *topic_parse_dpt_topic_message(const MESSAGE_T *dpt_message, HASH_T *topic_aliases);

/**
 * Only frees the data specific to a TOPIC_MESSAGE_T and not the underlying data
 * provided by the MESSAGE_T used to create it.
 */
void topic_message_free(TOPIC_MESSAGE_T *topic_message);

TOPIC_MESSAGE_T *topic_message_dup(const TOPIC_MESSAGE_T *topic_message);

V5_MESSAGE_T *v5_message_parse(const BUF_T *buf);

void v5_message_free(V5_MESSAGE_T *msg);

#endif
