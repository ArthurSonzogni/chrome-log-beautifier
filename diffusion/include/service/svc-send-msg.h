#ifndef _diffusion_svc_send_msg_h_
#define _diffusion_svc_send_msg_h_ 1

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
 * @file svc-send-msg.h
 *
 * Functions for sending messages to handlers in control clients via
 * topic endpoints.
 */

#include "internal/service_int.h"

/*
 * Additional options when sending a message.
 */
typedef STREAM_MESSAGE_OPTIONS_T SEND_MSG_OPTIONS_T;

/*
 * send-msg request structure.
 */
typedef STREAM_MESSAGE_T SVC_SEND_MSG_REQUEST_T;

/*
 * send-msg response structure.
 */
typedef struct svc_send_msg_response_s {
        // @cond HIDDEN
        void *_reserved;
        // @endcond
} SVC_SEND_MSG_RESPONSE_T;

/**
 * @brief Callback for send_msg()
 *
 * @param session The current active session.
 * @param context User-supplied context from the initial registration call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_send_msg_cb)(SESSION_T *session, void *context);

/**
 * @brief User-supplied function which receives messages directed to the session
 * via a topic endpoint.
 *
 * @param session The current active session.
 * @param request A structure containing the incoming message.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_msg_cb)(SESSION_T *session, SVC_SEND_MSG_REQUEST_T *request);

/**
 * @brief User-supplied handlers that may be invoked in response to a
 * send_msg() call.
 */
#define SEND_MSG_HANDLERS                       \
        on_send_msg_cb on_send;                 \
        ERROR_HANDLER_T on_error;               \
        DISCARD_HANDLER_T on_discard;

/**
 * @brief Structure supplied to a send_msg() call.
 */
typedef struct send_msg_params_s {
        SEND_MSG_HANDLERS
        /// The name of the topic used as an endpoint when sending
        /// messages to Diffusion.
        const char *topic_path;
        /// The message to be sent.
        const BUF_T payload;
        /// Any headers to be sent.
        const LIST_T *headers;
        /// The priority of the message when sent to clients.
        const CLIENT_SEND_PRIORITY_T priority;
        /// User-supplied context returned to callbacks.
        void *context;
} SEND_MSG_PARAMS_T;

/**
 * @brief Structure supplied when registering a message listener.
 */
typedef struct msg_listener_registration_params_s {
        /// Topic endpoint
        const char *topic_path;
        /// Function to be invoked when a message is received on the endpoint.
        const STREAM_MESSAGE_LISTENER_T listener;
        /// User-supplied context returned to callbacks.
        void *context;
} MSG_LISTENER_REGISTRATION_PARAMS_T;

/**
 * @brief Structure supplied when deregistering a message listener.
 */
typedef struct msg_listener_deregistration_params_s {
        /// Topic endpoint
        const char *topic_path;
} MSG_LISTENER_DEREGISTRATION_PARAMS_T;

/**
 * @ingroup PublicAPI_Messaging
 *
 * @brief Synchronously send a message to Diffusion via a topic endpoint.
 *
 * Messages are sent for a particular topic path. The topic path acts
 * as a context for the recipient which will see the message as having
 * come 'from' the topic as opposed to the actual originator. A topic
 * with the path need not exist on the server; if one does exist, it
 * is unaffected by messaging.
 *
 * The calling session must have the SEND_TO_CLIENT permission for the
 * topic path.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters defining the message to be sent.
 */
void send_msg(SESSION_T *session, SEND_MSG_PARAMS_T params);

/**
 * @ingroup PublicAPI_Messaging
 *
 * @brief Register to accept messages (not topic updates) that are
 * sent to this session via a topic endpoint.
 *
 * Control clients may send messages directly to a client (or a group
 * of clients) via a topic endpoint. Registering a listener for that
 * endpoint allows a client to receive these messages and have them be
 * handled in a user-supplied function.
 *
 * Specifying a NULL topic path is a special case that catches all
 * messages that do not have an explicit listener registered for it.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters defining the listener registration.
 */
void register_msg_listener(SESSION_T *session, MSG_LISTENER_REGISTRATION_PARAMS_T params);

/**
 * @ingroup PublicAPI_Messaging
 *
 * @brief Deregister a listener for a topic endpoint.
 *
 * If the topic specified in the params has not been registered as a
 * listener endpoint, then this function has no effect.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters describing the listener to be
 *                      deregistered.
 */
void deregister_msg_listener(SESSION_T *session, MSG_LISTENER_DEREGISTRATION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(send_msg, SVC_SEND_MSG_REQUEST_T, SVC_SEND_MSG_RESPONSE_T)

#endif
