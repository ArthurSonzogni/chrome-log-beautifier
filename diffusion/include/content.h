/**
 * @file content.h
 *
 * Defines functions that operate on the CONTENT_T data structure,
 * which is used to wrap data used in updates to topics.
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

#ifndef _diffusion_content_type_h_
#define _diffusion_content_type_h_ 1

#include "types/content_types.h"

/**
 * Create a new CONTENT_T structure.
 *
 * @param encoding	The encoding type (only NONE is supported at the moment).
 * @param data		A pointer to a BUF_T containing data to be wrapped in
 *			the CONTENT_T. The BUF_T is copied, so the source can be
 *			freed by the caller.
 * @return		A new CONTENT_T structure, or NULL on error.
 */
CONTENT_T *content_create(const CONTENT_ENCODING_T encoding, const BUF_T *data);

/**
 * Free all memory associated with a CONTENT_T.
 *
 * @param content	The CONTENT_T to be freed.
 */
void content_free(CONTENT_T *content);

/**
 * Serialise a CONTENT_T to bytes for transmission to Diffusion.
 *
 * @param content	The CONTENT_T to be marshalled.
 * @return		A pointer to a newly allocated BUF_T containing the
 *			serialised data, or NULL if an error occurs.
 */
BUF_T *content_marshal(const CONTENT_T *content);

/**
 * Read a CONTENT_T structure from a char array. If content is NULL the
 * data will be serialised but omitted.
 *
 * @param data		The char array containing the serialised CONTENT_T
 * @param content	The location to store a newly allocated CONTENT_T
 * @return		A pointer to the first char following the serialised
 *			CONTENT_T in the char array, or NULL on error.
 */
char *buf_read_content(const char *data, CONTENT_T **const content);

/**
 * Read a CONTENT_T structure from a char array. The destination CONTENT_T
 * structure must exist (ie, cannot be NULL) and will be replaced with the
 * deserialised data.
 *
 * @param data          The char darray containing the serialised CONTENT_T
 * @param content       The location of an existing CONTENT_T structure.
 * @return              A pointer to the first char following the serialised
 *                      CONTENT_T in the char array, or NULL on error.
 */
char *buf_read_into_content(const char *data, CONTENT_T *content);

/**
 * Deserialise bytes from Diffusion to a CONTENT_T structure.
 *
 * @param str		The bytes to be unmarshalled.
 * @return		A pointer to a newly allocated CONTENT_T, or NULL if an
 *			error occurs.
 */
CONTENT_T *content_unmarshal(const char *str);

/**
 * Create a deep-copy duplicate of a CONTENT_T structure.
 *
 * @param src		The CONTENT_T to be copied.
 * @return		A newly allocated CONTENT_T, or NULL if in error occurs.
 */
CONTENT_T *content_dup(const CONTENT_T *src);

/**
 * Utility function for taking a copy of the data in a CONTENT_T, and
 * returning it as a NULL-terminated string.
 *
 * @param content       The source CONTENT_T
 * @return              A newly allocated pointer to a NULL-terminated
 *                      string of characters.
 */
char *content_to_string(const CONTENT_T *content);

#endif
