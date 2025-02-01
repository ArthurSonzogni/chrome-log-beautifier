#ifndef _diffusion_svc_notify_unsubscription_h_
#define _diffusion_svc_notify_unsubscription_h_ 1

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
 * @file svc-notify-unsubscription.h
 *
 * Functions for notifying a client that it has been unsubscribed from
 * a topic.
 */

#include "internal/service_int.h"
#include "types/topic_details_types.h"

/**
 * @brief Reason for unsubscription.
 */
typedef enum {
        /// Unsubscribed by the subscribing client.
        UNSUBSCRIPTION_REASON_REQUESTED = 0,
        /// The unsubscription was requested either by another client
        /// or by the server.
        UNSUBSCRIPTION_REASON_CONTROL = 1,
        /// The unsubscription occurred because the topic was removed.
        UNSUBSCRIPTION_REASON_REMOVAL = 2,
        /// The unsubscription occurred because the session is no longer
        /// authorized to access the topic.
        UNSUBSCRIPTION_REASON_AUTHORIZATION = 3,
        /**
         * The server has re-subscribed this session to the topic. Existing
         * streams are unsubscribed because the topic type and other
         * attributes may have changed.
         *
         * <p>
         * This can happen if a set of servers is configured to use session
         * replication, and a session connected to one server reconnects
         * ("fails over") to a different server.
         *
         * <p>
         * A stream that receives an unsubscription notification with this
         * reason will also receive a subscription notification with the new
         * {@link TOPIC_DETAILS_T topic details}.
         */
        UNSUBSCRIPTION_REASON_SUBSCRIPTION_REFRESH = 4,
        /**
         * A fallback stream has been unsubscribed due to the addition of a
         * stream that selects the topic. (Not currently used in the C API).
         */
        UNSUBSCRIPTION_REASON_STREAM_CHANGE = 5
} NOTIFY_UNSUBSCRIPTION_REASON_T;

/**
 * @brief Structure describing an incoming unsubscription
 * notification.
 */
typedef struct svc_notify_unsubscription_request_s {
        /// The internal identifier of the topic.
        uint32_t topic_id;
        /// The topic path, or NULL if not available.
        char *topic_path;
        /// The reason why the unsubscription happened.
        NOTIFY_UNSUBSCRIPTION_REASON_T reason;
} SVC_NOTIFY_UNSUBSCRIPTION_REQUEST_T;

/**
 * @brief Response message to unsubscription notifications.
 */
typedef struct svc_notify_unsubscription_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_NOTIFY_UNSUBSCRIPTION_RESPONSE_T;

/**
 * @brief Callback for notify_unsubscription_register.
 *
 * @param session The current active session.
 * @param request The incoming notification message.
 * @param context User-supplied context from the initial registration call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_notify_unsubscription_cb)(SESSION_T *session,
                                           const SVC_NOTIFY_UNSUBSCRIPTION_REQUEST_T *request,
                                           void *context);

/**
 * @brief Callback handlers for unsubscription notification callbacks.
 */
#define NOTIFY_UNSUBSCRIPTION                                           \
        /** Callback fired an an unsubscription notification has been received. */ \
        on_notify_unsubscription_cb on_notify_unsubscription;             \
        /** Standard service error callback. */                         \
        ERROR_HANDLER_T on_error;

typedef struct notify_unsubscription_handlers_s {
        NOTIFY_UNSUBSCRIPTION
} NOTIFY_UNSUBSCRIPTION_HANDLERS_T;

/**
 * @brief Structure supplied when registering to receive topic
 * unsubscription notifications.
 */
typedef struct notify_unsubscription_params_s {
        NOTIFY_UNSUBSCRIPTION
        /// User-supplied context returned to callbacks.
        void *context;
} NOTIFY_UNSUBSCRIPTION_PARAMS_T;

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Register to receive unsubscription notifications.
 *
 * This function should be called by the application to receive
 * notifications that it has been unsubscribed to a topic.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Callbacks and other parameters. {@link NOTIFY_UNSUBSCRIPTION_PARAMS_T on_notify_unsubscription}
 *                      should be set to receive notifications.
 */
void notify_unsubscription_register(SESSION_T *session, const NOTIFY_UNSUBSCRIPTION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(notify_unsubscription, SVC_NOTIFY_UNSUBSCRIPTION_REQUEST_T, SVC_NOTIFY_UNSUBSCRIPTION_RESPONSE_T)

#endif
