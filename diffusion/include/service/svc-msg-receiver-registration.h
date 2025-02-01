#ifndef _diffusion_svc_msg_receiver_registration_h_
#define _diffusion_svc_msg_receiver_registration_h_ 1

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
 * @file svc-msg-receiver-registration.h
 *
 * Functions for registering message handlers.
 */

#include "service/svc-send-receiver-client.h"
#include "internal/service_int.h"
#include "internal/transport_int.h"

/// Default control group if not specified.
#define DEFAULT_MSG_RECEIVER_GROUP_NAME "default"

/**
 * @brief Structure containing information passed to Diffusion in
 * order to register a message handler at a topic endpoint.
 */
typedef struct svc_msg_receiver_registration_request_s {
        /// Control group: if NULL a default is used.
        char *control_group;
        /// Topic path under which to register.
        char *topic_path;
        /// A list of properties to be sent with any message received
        /// by the handler.
        LIST_T *session_properties;
        /// ID representing this handler's conversation session.
        CONVERSATION_ID_T *conversation_id;
} SVC_MSG_RECEIVER_REGISTRATION_REQUEST_T;

/**
 * @brief Response structure when a handler is registered.
 */
typedef struct svc_msg_receiver_registration_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_MSG_RECEIVER_REGISTRATION_RESPONSE_T;

/**
 * @brief Callback when a handler is registered with the server.
 *
 * @param session The currently active session.
 * @param context User-supplied context from the initial
 *        register_msg_handler() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_msg_receiver_registration_cb)(SESSION_T *session,
                                               void *context);

/**
 * @brief Handlers for notification of registration of handler
 *        and message receipt.
 */
#define MSG_RECEIVER_REGISTRATION_HANDLERS             \
        on_msg_receiver_registration_cb on_registered; \
        on_send_receiver_client_msg_cb on_message;     \
        ERROR_HANDLER_T on_error;                      \
        DISCARD_HANDLER_T on_discard;

/**
 * @brief Structure supplied when issuing a register_msg_handler() call.
 */
typedef struct msg_receiver_registration_params_s {
        MSG_RECEIVER_REGISTRATION_HANDLERS
        /// Topic endpoint used for registration.
        const char *topic_path;
        /// Control group. Leave as NULL to use a default value.
        const char *control_group;
        /// Properties associated with this message handler.
        LIST_T *session_properties;
        /// User-supplied context.
        void *context;
} MSG_RECEIVER_REGISTRATION_PARAMS_T;

/**
 * @ingroup PublicAPI_MessagingControl
 *
 * @brief Register a message handler.
 *
 * Message handlers receive messages which are sent from clients via
 * topic endpoints. Only one handler for a particular endpoint may be
 * active at any one time, as determined by the server.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters defining the request to register a
 *                      message handler.
 */
void register_msg_handler(SESSION_T *session, const MSG_RECEIVER_REGISTRATION_PARAMS_T params);

/**
 * @ingroup PublicAPI_MessagingControl
 *
 * @brief Deregister a message handler.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        The same parameters passed when registering the
 *                      message handler.
 */
void deregister_msg_handler(SESSION_T *session, const MSG_RECEIVER_REGISTRATION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(msg_receiver_registration, SVC_MSG_RECEIVER_REGISTRATION_REQUEST_T, SVC_MSG_RECEIVER_REGISTRATION_RESPONSE_T)

#endif
