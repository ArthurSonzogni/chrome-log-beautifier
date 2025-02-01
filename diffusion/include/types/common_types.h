/**
 * @file common_types.h
 *
 * Common types that don't fit anywhere else.
 *
 * Copyright © 2014, 2016 Push Technology Ltd., All Rights Reserved.
 *
 * Use is subject to license terms.
 *
 * NOTICE: All information contained herein is, and remains the
 * property of Push Technology. The intellectual and technical
 * concepts contained herein are proprietary to Push Technology and
 * may be covered by U.S. and Foreign Patents, patents in process, and
 * are protected by trade secret or copyright law.
 */

#ifndef _diffusion_common_types_h_
#define _diffusion_common_types_h_ 1

#include <stddef.h>
#include <apr.h>
#include "types/error_types.h"

/// Constant used in the API to indicate "false".
#define DIFFUSION_FALSE 0

/// Constant used in the API to indicate "true"
#define DIFFUSION_TRUE 1

/**
 * Constant used in the API to indicate that a message or service handler
 * has completed successfully.
 */
#define HANDLER_SUCCESS 0

/**
 * Constant used in the API to indicate that a message or service handler
 * has failed with an error.
 */
#define HANDLER_FAILURE -1

/**
 * Constant defining the V5 service/control topic name.
 */
#define COMMAND_TOPIC_NAME "@"

/**
 * Priority for messages being sent to Diffusion. The default is MEDIUM for
 * most messages.
 *
 * IMMEDIATE priority messages are for system use only, and should not
 * by used by the user.
 */
typedef enum {
        MESSAGE_PRIORITY_IMMEDIATE = 0,
        MESSAGE_PRIORITY_HIGH = 1,
        MESSAGE_PRIORITY_MEDIUM = 2,
        MESSAGE_PRIORITY_LOW = 3
} MESSAGE_PRIORITY_T;

/**
 * Priority of messages that Diffusion sends to control clients via
 * the send_client() call.
 */
typedef enum {
        CLIENT_SEND_PRIORITY_NORMAL = 0,
        CLIENT_SEND_PRIORITY_HIGH = 1,
        CLIENT_SEND_PRIORITY_LOW = 2
} CLIENT_SEND_PRIORITY_T;

/**
 * Reasons for session closure.
 */
typedef enum {
        SESSION_CLOSE_REASON_CONNECTION_LOST = 0,
        SESSION_CLOSE_REASON_IO_EXCEPTION = 1,
        SESSION_CLOSE_REASON_CLIENT_UNRESPONSIVE = 2,
        SESSION_CLOSE_REASON_MESSAGE_QUEUE_LIMIT_REACHED = 3,
        SESSION_CLOSE_REASON_CLOSED_BY_CLIENT = 4,
        SESSION_CLOSE_REASON_MESSAGE_TOO_LARGE = 5,
        SESSION_CLOSE_REASON_INTERNAL_ERROR = 6,
        SESSION_CLOSE_REASON_INVALID_INBOUND_MESSAGE = 7,
        SESSION_CLOSE_REASON_ABORTED = 8,
        SESSION_CLOSE_REASON_LOST_MESSAGES = 9,
        SESSION_CLOSE_REASON_SERVER_CLOSING = 10,
        SESSION_CLOSE_REASON_CLOSED_BY_CONTROLLER = 11,
        SESSION_CLOSE_REASON_FAILED_OVER = 12
} SESSION_CLOSE_REASON_T;

typedef enum {
        SUCCESS = 0,
        RETRIABLE = 1,
        FATAL = 2,
        FATAL_AUTHENTICATION = 3
} CONNECTION_RESPONSE_CODE_TYPE_T;

/**
 * Connection response codes.
 */
typedef enum {
        CONNECTION_RESPONSE_CODE_OK = 100,
        CONNECTION_RESPONSE_CODE_DOWNGRADE = 102,
        CONNECTION_RESPONSE_CODE_RECONNECTED = 105,
        CONNECTION_RESPONSE_CODE_RECONNECTED_WITH_MESSAGE_LOSS = 106,
        CONNECTION_RESPONSE_CODE_REJECTED = 111,
        CONNECTION_RESPONSE_CODE_CONNECTION_UNSUPPORTED = 112,
        CONNECTION_RESPONSE_CODE_LICENSE_EXCEEDED = 113,
        CONNECTION_RESPONSE_CODE_RECONNECTION_UNSUPPORTED = 114,
        CONNECTION_RESPONSE_CODE_CONNECTION_PROTOCOL_ERROR = 115,
        CONNECTION_RESPONSE_CODE_AUTHENTICATION_FAILED = 116,
        CONNECTION_RESPONSE_CODE_UNKNOWN_SESSION = 117,
        CONNECTION_RESPONSE_CODE_RECONNECTION_FAILED_MESSAGE_LOSS = 118,
        CONNECTION_RESPONSE_CODE_ERROR = 127,
} CONNECTION_RESPONSE_CODE_T;

typedef struct {
        const CONNECTION_RESPONSE_CODE_T code;
        const char *msg;
        const CONNECTION_RESPONSE_CODE_TYPE_T type;
        const ERROR_CODE_T diffusion_error_code;
} CONNECTION_RESPONSE_T;

static const CONNECTION_RESPONSE_T connectionResponseList[] = {
        {
                CONNECTION_RESPONSE_CODE_OK,
                "Connected successfully",
                SUCCESS,
                DIFF_ERR_SUCCESS
        },
        {
                CONNECTION_RESPONSE_CODE_DOWNGRADE,
                "Server does not support the requested protocol level",
                FATAL,
                DIFF_ERR_DOWNGRADE
        },
        {
                CONNECTION_RESPONSE_CODE_RECONNECTED,
                "Reconnected successfully",
                SUCCESS,
                DIFF_ERR_SUCCESS
        },
        {
                CONNECTION_RESPONSE_CODE_RECONNECTED_WITH_MESSAGE_LOSS,
                "Reconnected with message loss",
                SUCCESS,
                DIFF_ERR_SUCCESS,
        },
        {
                CONNECTION_RESPONSE_CODE_REJECTED,
                "Connection rejected",
                FATAL_AUTHENTICATION,
                DIFF_ERR_CONNECTION_REJECTED
        },
        {
                CONNECTION_RESPONSE_CODE_CONNECTION_UNSUPPORTED,
                "Connection type not supported by connector",
                FATAL,
                DIFF_ERR_CONNECTION_UNSUPPORTED
        },
        {
                CONNECTION_RESPONSE_CODE_LICENSE_EXCEEDED,
                "Connection rejected due to license limit",
                FATAL,
                DIFF_ERR_LICENCE_EXCEEDED
        },
        {
                CONNECTION_RESPONSE_CODE_RECONNECTION_UNSUPPORTED,
                "Reconnection not supported by connector",
                FATAL,
                DIFF_ERR_RECONNECTION_UNSUPPORTED
        },
        {
                CONNECTION_RESPONSE_CODE_CONNECTION_PROTOCOL_ERROR,
                "Connection failed - protocol error",
                FATAL,
                DIFF_ERR_CONNECTION_PROTOCOL_ERROR
        },
        {
                CONNECTION_RESPONSE_CODE_AUTHENTICATION_FAILED,
                "Authentication failed",
                FATAL_AUTHENTICATION,
                DIFF_ERR_AUTHENTICATION_FAILED
        },
        {
                CONNECTION_RESPONSE_CODE_UNKNOWN_SESSION,
                "Reconnection failed - the session is unknown",
                FATAL,
                DIFF_ERR_UNKNOWN_SESSION
        },
        {
                CONNECTION_RESPONSE_CODE_RECONNECTION_FAILED_MESSAGE_LOSS,
                "Reconnection failed due to message loss",
                FATAL,
                DIFF_ERR_MESSAGE_LOSS
        },
        {
                CONNECTION_RESPONSE_CODE_ERROR,
                "Connection failed due to server error",
                RETRIABLE,
                DIFF_ERR_CONNECTION_ERROR_UNDEFINED
        },
        // Terminator - NULL description - other values ignored.
        { CONNECTION_RESPONSE_CODE_OK, NULL, SUCCESS, DIFF_ERR_SUCCESS }
};

#endif
