/**
 * @file update.h
 * 
 * Functions which are used for updating topics.
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
#ifndef _diffusion_update_h_
#define _diffusion_update_h_ 1

#include "types/update_types.h"

/**
 * Create a new UPDATE_T structure for updating topic content.
 *
 * @param action	The operation that will be performed on the target topic data
 *			(refresh, update, match).
 * @param type		The type of update being performed (typically, Content change).
 * @param content	The data to be sent to Diffusion.
 * @return		A new UPDATE_T structure, or NULL on error.
 */
UPDATE_T *update_create(const UPDATE_ACTION_T action, const UPDATE_TYPE_T type, const CONTENT_T *content);

/**
 * Free all memory associated with an UPDATE_T.
 *
 * @param update	The UPDATE_T to be freed.
 */
void update_free(UPDATE_T *update);

/**
 * Serialise an UPDATE_T to bytes for transmission to Diffusion.
 *
 * @param update	The UPDATE_T to be mashalled.
 * @return		A pointer to a newly allocated BUF_T containing the
 *			serialised data, or NULL if an error occurs.
 */
BUF_T *update_marshal(const UPDATE_T *update);

/**
 * Read an UPDATE_T structure from a char array.
 *
 * @param data		The char array containing the serialised UPDATE_T
 * @param update	The location to store a newly allocated UPDATE_T
 * @return		A pointer to the first char following the serialised
 *			UPDATE_T in the char array, or NULL on error.
 */
char *buf_read_update(const char *data, UPDATE_T **update);

/**
 * Deserialise bytes from Diffusion to an UPDATE_T structure.
 *
 * @param str		The bytes to be unmarshalled.
 * @return		A pointer to a newly allcoated UPDATE_T, or NULL if an
 *			error occurs.
 */
UPDATE_T *update_unmarshal(const char *str);

/**
 * Create a deep copy of an UPDATE_T structure.
 *
 * @param src		The UPDATE_T to copy.
 * @return		A newly allocated UPDATE_T, or NULL on error.
 */
UPDATE_T *update_dup(const UPDATE_T *src);

#endif
