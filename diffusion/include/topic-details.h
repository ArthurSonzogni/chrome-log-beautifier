/**
 * @file topic-details.h
 *
 * Functions used for working with topic details (TOPIC_DETAILS_T).
 * Topic details describe the structure of a topic.
 *
 * If you intend to create many topics which have the same type of
 * topic details, e.g. single value string topics, then it is
 * recommended that you only create a single instance of topic details
 * to describe them. This will allow the server to cache the details,
 * significantly improving performance and reducing memory load.
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
#ifndef _diffusion_topic_details_h_
#define _diffusion_topic_details_h_ 1

#include "types/topic_details_types.h"

/**
 * Create a TOPIC_DETAILS_T which describes a topic with no topic details.
 */
TOPIC_DETAILS_T *create_topic_details_none(void);

/**
 * Create a TOPIC_DETAILS_T which describes a stateless topic, ie one
 * that delegates its content to a publisher or control client.
 */
TOPIC_DETAILS_T *create_topic_details_stateless(void);

/**
 * Create a TOPIC_DETAILS_T which describes a topic with a simple value.
 */
TOPIC_DETAILS_T *create_topic_details_single_value(const M_DATA_TYPE_T type);

/**
 * Create a TOPIC_DETAILS_T which for a topic with records and
 * fields. The schema describing the structure of these records and
 * fields must be provided separately.
 */
TOPIC_DETAILS_T *create_topic_details_record(void);

/**
 * Create a TOPIC_DETAILS_T for a slave topic. This function will return NULL
 * if master_topic is NULL.
 *
 * @param master_topic    The full path of the topic to which the slave topic
 *                        shall be an alias.
 */
TOPIC_DETAILS_T *create_topic_details_slave(const char *const master_topic);

/**
 * Creates an XML document that represents the topic details that has
 * been passed as an argument. The XML is serialised to a BUF_T for
 * transmission to Diffusion when creating new topics.
 */
BUF_T *topic_details_schema_build(const TOPIC_DETAILS_T *details);

/**
 * Take existing topic details (with an associated schema) and
 * serialise into a form suitable for transmitting over the wire to
 * Diffusion.
 */
BUF_T *topic_details_marshal(TOPIC_DETAILS_T *details);

/**
 * Create a duplicate (copy) of an existing TOPIC_DETAILS_T.
 */
TOPIC_DETAILS_T *topic_details_dup(const TOPIC_DETAILS_T *src);

/**
 * Free memory associated with a TOPIC_DETAILS_T.
 */
void topic_details_free(TOPIC_DETAILS_T *details);

/**
 * @brief Set the empty field value (record topic data only).
 *
 * For a client to differentiate between unchanged fields and empty
 * fields in delta messages, it's necessary to use a special value to
 * represent an empty field.
 *
 * @param details       Topic details for which to set the value.
 * @param value         New value for empty fields, or NULL to reset
 *                      to the default (an empty string).
 * @return The previous value.
 */
char *set_empty_field_value(const TOPIC_DETAILS_T *details, const char *value);


/**
 * @brief Get the current empty field value (record topic data only).
 *
 * This returns the value used to distinguish between a field which is
 * unchanged from one which is merely empty. A value of NULL indicates
 * that this value has never been set, and is treated internally as an
 * empty string.
 *
 * @param details       Record topic details.
 * @return The value used to represent empty fields, or NULL.
 */
char *get_empty_field_value(const TOPIC_DETAILS_T *details);

/* Internal */
void topic_id_details_free(TOPIC_ID_DETAILS_T *val);

#endif
