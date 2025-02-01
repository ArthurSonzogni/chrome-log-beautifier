/*
 * @file update_types.h
 *
 * Types required for topic updates.
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
#ifndef _diffusion_update_types_h_
#define _diffusion_update_types_h_ 1

#include "content.h"

typedef enum {
	UPDATE_ACTION_UPDATE = 0,
	UPDATE_ACTION_REFRESH = 1,
} UPDATE_ACTION_T;

typedef enum {
	UPDATE_TYPE_CONTENT = 0,
	UPDATE_TYPE_PAGED_RECORD_ORDERED = 1,
	UPDATE_TYPE_PAGED_STRING_ORDERED = 2,
	UPDATE_TYPE_PAGED_RECORD_UNORDERED = 3,
	UPDATE_TYPE_PAGED_STRING_UNORDERED = 4
} UPDATE_TYPE_T;

/**
 * This represents an update to apply to a topic.
 */
typedef struct update_s {
        /// The type of update.
	UPDATE_TYPE_T type;
        /// How the update should be applied.
	UPDATE_ACTION_T action;
        /// The byte content of the update.
	CONTENT_T *content;
} UPDATE_T;

#endif
