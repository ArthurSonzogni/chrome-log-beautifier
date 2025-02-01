#ifndef _diffusion_svc_send_receiver_client_h_
#define _diffusion_svc_send_receiver_client_h_ 1

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

/**
 * @file svc-send-receiver-client.h
 *
 * Functions for sending messages to handlers in control clients via
 * topic endpoints.
 */

#include "internal/service_int.h"
#include "types/content_types.h"
#include "types/session_types.h"
#include "types/conversation_types.h"
#include "types/common_types.h"
#include "types/messaging_types.h"

/**
 * @brief Structure containing the information received and passed to
 * a topic endpoint handler, including the message content.
 */
typedef struct svc_send_receiver_client_request_s {
        /// Endpoint from which this message was received.
        char *topic_path;
        /// Content of message.
        CONTENT_T *content;
        /// Session ID of sending client.
        SESSION_ID_T session_id;
        /// Additional options relating to message delivery.
        SEND_OPTIONS_T send_options;
        /// Properties associated with the message handler.
        HASH_T *session_properties;
        /// Conversation ID of sender, if response required.
        CONVERSATION_ID_T conversation_id;
} SVC_SEND_RECEIVER_CLIENT_REQUEST_T;

/**
 * @brief Response structure.
 */
typedef struct svc_send_receiver_client_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SEND_RECEIVER_CLIENT_RESPONSE_T;

/**
 * @brief Callback when a message is received by a registered handler
 * on a topic endpoint.
 *
 * @param session The currently active session.
 * @param request Details about the incoming message, and the message
 *        itself.
 * @param context User-supplied context from the initial
 *        register_msg_handler() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_send_receiver_client_msg_cb)(SESSION_T *session,
                                              const SVC_SEND_RECEIVER_CLIENT_REQUEST_T *request,
                                              void *context);

/**
 * @brief Handlers for notification of a message being received.
 */
#define SEND_RECEIVER_CLIENT_HANDLERS \
        on_send_receiver_client_msg_cb on_message; \
        ERROR_HANDLER_T on_error;

/**
 * @brief Internal structure used when issuing a
 * register_msg_handler() call.
 */
typedef struct send_receiver_client_params_s {
        SEND_RECEIVER_CLIENT_HANDLERS
        /// Topic endpoint used for registration.
        const char *topic_path;
        /// User-supplied context.
        void *context;
} SEND_RECEIVER_CLIENT_PARAMS_T;

DECLARE_SERVICE_PROTOTYPES(send_receiver_client, SVC_SEND_RECEIVER_CLIENT_REQUEST_T, SVC_SEND_RECEIVER_CLIENT_RESPONSE_T)

#endif
