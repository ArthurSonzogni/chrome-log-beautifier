/**
 * @file error_types.h
 *
 * Types and structures relating to reported errors.
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

#ifndef _diffusion_error_types_h_
#define _diffusion_error_types_h_ 1

/**
 * Errors which can be raised by API functions.
 */
typedef enum {
        DIFF_ERR_SUCCESS = 0,
        DIFF_ERR_UNKNOWN,
        DIFF_ERR_SERVICE,
        DIFF_ERR_SESSION_CREATE_FAILED,
        DIFF_ERR_TRANSPORT_CREATE_FAILED,
        DIFF_ERR_NO_SESSION,
        DIFF_ERR_NO_TRANSPORT,
        DIFF_ERR_NO_START_FN,
        DIFF_ERR_NO_CLOSE_FN,
        DIFF_ERR_NO_SERVERS_DEFINED,
        DIFF_ERR_ADDR_LOOKUP_FAIL,
        DIFF_ERR_SOCKET_CREATE_FAIL,
        DIFF_ERR_SOCKET_CONNECT_FAIL,
        DIFF_ERR_HANDSHAKE_SEND_FAIL,
        DIFF_ERR_HANDSHAKE_RECV_FAIL,
        DIFF_ERR_INVALID_CONNECTION_PROTOCOL,
        DIFF_ERR_INVALID_TOPIC_SPECIFICATION,
        DIFF_ERR_TOPIC_ALREADY_EXISTS,
        DIFF_ERR_CONNECTION_REJECTED,
        DIFF_ERR_CONNECTION_ERROR_UNDEFINED,
        DIFF_ERR_MESSAGE_QUEUE_FAIL,
        DIFF_ERR_MESSAGE_SEND_FAIL,
        DIFF_ERR_PARSE_URL,
        DIFF_ERR_UNKNOWN_TRANSPORT,
        DIFF_ERR_SOCKET_READ_FAIL,
        DIFF_ERR_SOCKET_WRITE_FAIL,
        DIFF_ERR_DOWNGRADE,
        DIFF_ERR_CONNECTION_UNSUPPORTED,
        DIFF_ERR_LICENCE_EXCEEDED,
        DIFF_ERR_RECONNECTION_UNSUPPORTED,
        DIFF_ERR_CONNECTION_PROTOCOL_ERROR,
        DIFF_ERR_AUTHENTICATION_FAILED,
        DIFF_ERR_PROTOCOL_VERSION,
        DIFF_ERR_UNKNOWN_SESSION,
        DIFF_ERR_MESSAGE_LOSS
} ERROR_CODE_T;

/**
 * A structure which is filled by the API when an error occurs.
 */
typedef struct diffusion_error_s {
        /// The error code.
        ERROR_CODE_T code;
        /// Textual description of the error.
        char *message;
        /// User-supplied context.
        void *context;
} DIFFUSION_ERROR_T;

#endif
