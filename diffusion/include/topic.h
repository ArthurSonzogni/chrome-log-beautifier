/**
 * @file topic.h
 * 
 * Functions which are used for accessing topics and their contents.
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
#ifndef _diffusion_feature_topics_h_
#define _diffusion_feature_topics_h_ 1

#include "list.h"
#include "hash.h"

#include "types/common_types.h"
#include "types/topic_types.h"

/**
 * Access a topic message header by indexed position.
 * 
 * @param topic_message		A topic message.
 * @param index			The index of the header to be returned.
 * @retval "const char *"	A pointer to the header in the message (not a
 *				copy).
 * @retval NULL			If the header could not be found (e.g. index out
 *				of range).
 */
const char *topic_get_header_indexed(const TOPIC_MESSAGE_T *topic_message, const int index);

#endif
