/**
 * @file service_types.h
 *
 * This file lists the types of services supported by Diffusion (although
 * some of these may not yet be supported by the C API) and associated
 * structures.
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

#ifndef _diffusion_service_types_h_
#define _diffusion_service_types_h_ 1

#include "buf.h"
#include "types/conversation_types.h"
#include "types/common_types.h"

/**
 * These are the services (and commands) that can be sent and received between
 * Diffusion and a client.
 * They are equivalent to those listed in the Java API in the StandardServices
 * class.
 */
typedef enum {
        // Client services
        SVC_PING = 1,
        SVC_FETCH = 2,
        SVC_SUBSCRIBE = 3,
        SVC_UNSUBSCRIBE = 4,
        SVC_CHANGE_PRINCIPAL = 5,
        SVC_SEND_MSG = 6,

        // Control services
        SVC_SUBSCRIBE_CONTROL = 10,
        SVC_UNSUBSCRIBE_CONTROL = 11,

        SVC_QUEUE_EVENT_HANDLER = 13,
        SVC_CLOSE_CLIENT = 14,
        SVC_SET_CLIENT_CONFLATION = 15,
        SVC_THROTTLE_CLIENT = 16,
        SVC_CLIENT_DETAILS = 17,
        SVC_SERVER_CONTROL_REGISTRATION = 18,
        SVC_SERVER_CONTROL_DEREGISTRATION = 19,
        SVC_TOPIC_CONTROL_REGISTRATION = 20,
        SVC_TOPIC_CONTROL_DEREGISTRATION = 21,
        SVC_AUTHENTICATION_CONTROL_REGISTRATION = 22,
        SVC_AUTHENTICATION_CONTROL_DEREGISTRATION = 23,
        SVC_AUTHENTICATION = 24,
        SVC_SESSION_DETAILS_REGISTRATION = 25,
        SVC_SESSION_DETAILS_EVENT = 26,
        SVC_SESSION_CLOSE_VIEW_UPDATE = 27,
        SVC_SEND_RECEIVER = 28,
        SVC_SEND_MSG_TO_SESSION = 28, // Synonym for SVC_SEND_RECEIVER
        SVC_FILTER_SENDER = 29,
        SVC_SEND_MSG_TO_FILTER = 29, // Synonum for SVC_FILTER_SENDER

        // Topic source services (deprecated names))
        SVC_TOPIC_SOURCE_REGISTRATION = 30,
        SVC_TOPIC_SOURCE_DEREGISTRATION = 31,
        SVC_TOPIC_SOURCE_STATE = 32,
        SVC_TOPIC_SOURCE_UPDATE = 34,

        // As above, with non-deprecated names:
        SVC_UPDATE_SOURCE_REGISTRATION = 30,
        SVC_UPDATE_SOURCE_DEREGISTRATION = 31,
        SVC_UPDATE_SOURCE_STATE = 32,
        SVC_UPDATE_SOURCE_UPDATE = 35, // *not* 34

        SVC_NOTIFY_SUBSCRIPTION = 40,
        SVC_GET_TOPIC_DETAILS = 41,
        SVC_NOTIFY_UNSUBSCRIPTION = 42,

        // Other topic services
        SVC_ADD_TOPIC = 46,
        SVC_REMOVE_TOPICS = 47,

        SVC_MISSING_TOPIC = 50,
        SVC_ROUTING_SUBSCRIBE_CONTROL = 51,
        SVC_ROUTING_SUBSCRIBE_CALLBACK = 52,
        SVC_TOPIC_SCOPED_WILL_REGISTRATION = 53,
        SVC_TOPIC_SCOPED_WILL_DEREGISRATION = 54,

        SVC_PING_SYSTEM = 55,
        SVC_PING_USER = 56,

        SVC_GET_SYSTEM_AUTHENTICATION = 57,
        SVC_UPDATE_SYSTEM_AUTHENTICATION = 58,
        SVC_GET_SECURITY = 59,
        SVC_UPDATE_SECURITY = 60,

        SVC_SEND_RECEIVER_CLIENT = 62,
        SVC_MSG_RECEIVER_REGISTRATION = 63,
        SVC_MSG_RECEIVER_DEREGISTRATION = 64,

        SVC_GET_SESSION_PROPERTIES = 67,
        SVC_SESSION_PROPERTIES_REGISTRATION = 69,
        SVC_SESSION_PROPERTIES_EVENT = 70,

        SVC_SESSION_PROPERTIES_REGISTRATION_2 = 81,
        SVC_SESSION_PROPERTIES_EVENT_2 = 82,

        SVC_TOPIC_REMOVAL = 83
} SERVICE_TYPE_T;

/** The maximum value of the service identifier. */
#define SERVICE_TYPE_MAX_VALUE 0x7f

/**
 * Interactions with Diffusion take the form of a REQUEST or a RESPONSE.
 */
typedef enum {
        MODE_ERROR = 0,
        MODE_REQUEST = 1,
        MODE_RESPONSE = 2
} SERVICE_MODE_T;

/**
 * Service messages differ in their structure depending on the requirements
 * of the service, but they all begin with these common fields.
 */
#define SVC_MSG_COMMON_FIELDS                   \
        SERVICE_TYPE_T service_type;            \
        SERVICE_MODE_T service_mode;            \
        CONVERSATION_ID_T conversation_id;

/** Base type for service messages */
typedef struct svc_msg_common_s {
        SVC_MSG_COMMON_FIELDS
} SVC_MSG_COMMON_T;

/**
 * Every service that a client can provide has to implement this core set of
 * functions.
 */
typedef struct svc_defn_s {
        /// Service request handler.
        int (*request_handler)();
        /// Service response handler.
        int (*response_handler)();
        /// Service error handler.
        int (*error_handler)();
        /// Deserialise a request to the appropriate structure.
        void *(*request_unmarshal)();
        /// Serialise a request structure to bytes.
        BUF_T *(*request_marshal)();
        /// Deserialise a response to the appropriate structure.
        void *(*response_unmarshal)();
        /// Serialise a response structure to bytes.
        BUF_T *(*response_marshal)();
        /// Create a default request for this service.
        void *(*request_create)();
        /// Create a default response for this service.
        void *(*response_create)();
        /// Free memory associated with the request structure.
        void (*request_free)();
        /// Free memory associated with the response structure.
        void (*response_free)();
        /// Local data for service-specific use.
        void *local;
} SVC_DEFN_T;

#endif
