/*
 * @file topic_details_types.h
 *
 * Types used to define the structure of topics; used when adding new
 * topics, or in one of several server-initiated notifications (e.g.,
 * subscription).
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
#ifndef _topic_details_types_h_
#define _topic_details_types_h_ 1

#include <stdint.h>

#include "buf.h"
#include "hash.h"
#include "types/topic_details_types.h"

typedef enum {
        TOPIC_TYPE_NONE = 0,
        TOPIC_TYPE_STATELESS = 1,
        // TOPIC_TYPE_DELEGATED,  *Deprecated*
        TOPIC_TYPE_SINGLE_VALUE = 3,
        TOPIC_TYPE_RECORD,
        TOPIC_TYPE_PROTOCOL_BUFFER,
        TOPIC_TYPE_CUSTOM,
        TOPIC_TYPE_SLAVE = 7,
        TOPIC_TYPE_SERVICE,
        TOPIC_TYPE_PAGED_STRING,
        TOPIC_TYPE_PAGED_RECORD,
        TOPIC_TYPE_TOPIC_NOTIFY,
        TOPIC_TYPE_ROUTING,
        TOPIC_TYPE_CHILD_LIST
} TOPIC_TYPE_T;

typedef enum {
        TOPIC_DETAILS_LEVEL_BASIC = 1,
        TOPIC_DETAILS_LEVEL_SCHEMA,
        TOPIC_DETAILS_LEVEL_FULL
} TOPIC_DETAILS_LEVEL_T;

typedef enum {
        TOPIC_PROPERTIES_ALIASING = 1,
        TOPIC_PROPERTIES_ATTACHMENT_CLASS = 2,
        TOPIC_PROPERTIES_DATA_INITIALISER_CLASS = 3,
        TOPIC_PROPERTIES_DELTA_ACK_REQUIRED = 4,
        TOPIC_PROPERTIES_DELTA_ENCODING =5,
        TOPIC_PROPERTIES_DELTA_MESSAGE_CAPACITY = 8,
        TOPIC_PROPERTIES_FETCH_HANDLER_CLASS = 9,
        TOPIC_PROPERTIES_LOAD_ACK_REQUIRED = 10,
        TOPIC_PROPERTIES_LOAD_ENCODING = 11,
        TOPIC_PROPERTIES_LOAD_HEADERS = 14,
        TOPIC_PROPERTIES_LOAD_MESSAGE_CAPACITY = 15,
        TOPIC_PROPERTIES_LOCK_TIMEOUT = 16,
        TOPIC_PROPERTIES_LOCKABLE = 17,
        TOPIC_PROPERTIES_SUBSCRIPTION_HANDLER_CLASS = 18
} TOPIC_PROPERTIES_T;

/**
 * When specifying topic data, you can choose one of the following to
 * constrain the type of data the topic can contain.
 */
typedef enum {
        M_DATA_TYPE_CUSTOM_STRING = 0,
        M_DATA_TYPE_DECIMAL_STRING,
        M_DATA_TYPE_INTEGER_STRING,
        M_DATA_TYPE_STRING
} M_DATA_TYPE_T;

/**
 * Various attributes relating to topic details.
 */
typedef struct topic_details_attributes_s {
        /// Clients are automatically subscribed to this topic.
        int autoSubscribes;
        /// Internal reference.
        char *reference;
        /// This topic may be removed when the last client
        /// unsubscribes.
        int tidiesOnUnsubscribe;
        /// Topic-specfic properties (implementation dependent).
        HASH_T *properties;
} TOPIC_DETAILS_ATTRIBUTES_T;

/**
 * Paramaters for a M_DATA_TYPE_STRING
 */
typedef struct topic_details_string_s {
        /// The default value for the topic.
        char *default_value;
} TOPIC_DETAILS_STRING_T;

/**
 * Parameters for a M_DATA_TYPE_INTEGER_STRING.
 */
typedef struct topic_details_integer_s {
        /// The default value for the topic.
        int default_value;
} TOPIC_DETAILS_INTEGER_T;

/**
 * Parameters for a M_DATA_TYPE_DECIMAL_STRING.
 */
typedef struct topic_details_decimal_s {
        /// The default value for the topic.
        double default_value;
        /// The number of decimal places.
        int scale;
} TOPIC_DETAILS_DECIMAL_T;

/**
 * Structure containing information encapsulated within topic details,
 * either for creation or received through a notification by Diffusion.
 */
typedef struct topics_details_s {
        /// Describes the metadata that is associated with these topic
        /// details.
        TOPIC_DETAILS_LEVEL_T level;

        /// The type of topic.
        TOPIC_TYPE_T topic_type;

        /// An internal reference to cache topic data.
        uint32_t reference_id;

        /// Automatically generated, do not set manually.
        BUF_T *schema;

        /// When creating record topic data, this has to be filled
        /// with a serialised XML document describing the metadata
        /// structure.
        BUF_T *user_defined_schema;

        /// Various attributes that may be assigned to the topic.
        TOPIC_DETAILS_ATTRIBUTES_T *attributes;

        /// Some topics have implementation-specific attributes. We
        /// store them in a key/value map here, and deal with them as
        /// required.
        HASH_T *extra_attributes;

        /// The data type.
        M_DATA_TYPE_T data_type;

        /// The master topic
        const char *master_topic;

        /// Specify the parameters for the data type here.
        union {
                TOPIC_DETAILS_STRING_T string;
                TOPIC_DETAILS_INTEGER_T integer;
                TOPIC_DETAILS_DECIMAL_T decimal;
        } topic_details_params;
} TOPIC_DETAILS_T;

typedef struct topic_details_cache_s {
        int cached_on_server;
        TOPIC_DETAILS_T *topic_details;
} TOPIC_DETAILS_CACHE_T;

typedef struct topic_id_details_s {
        char *topic_id_str;
        TOPIC_DETAILS_T *topic_details;
} TOPIC_ID_DETAILS_T;

#endif
