#ifndef _diffusion_svc_subscription_control_h_
#define _diffusion_svc_subscription_control_h_ 1

/*
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

/**
 * @file svc-subscription-control.h
 *
 * Functions for subscribing and unsubscribing connected clients from
 * topics.
 */

#include "internal/service_int.h"
#include "types/session_types.h"

/**
 * @brief Structure describing a subscription control request.
 */
typedef struct svc_subscription_control_request_s {
        /// The client session.
        SESSION_ID_T *session_id;
        /// The selector string describing which topics the client
        /// should be subscribed to or unsubscribed from.
        char *topic_selector;
} SVC_SUBSCRIPTION_CONTROL_REQUEST_T;

/**
 * @brief Structure describing a subscription control response.
 */
typedef struct svc_subscription_control_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SUBSCRIPTION_CONTROL_RESPONSE_T;

/**
 * @brief Callback for subscribe_client() and unsubscribe_client().
 *
 * @param session       The current active session.
 * @param context       User-supplied context from the originating call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_subscription_control_cb)(SESSION_T *session, void *context);

/**
 * @brief Callback handlers that may be registered so the caller may
 * receive notifcation that a subscription or unsubscription request
 * has completed.
 */
#define SUBSCRIPTION_CONTROL_HANDLERS                                   \
        /** A callback to indicate that the subscription or  unsubscription request has completed. */ \
        on_subscription_control_cb on_complete;                         \
        /** Standard service error callback */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard callback */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct subscription_control_params_s {
        /// Callback handlers.
        SUBSCRIPTION_CONTROL_HANDLERS
        /// ID of the session to be subscribed to or unsubscribed from
        /// the topic selector.
        SESSION_ID_T session_id;
        /// Topic selector.
        char *topic_selector;
        /// User-supplied context returned to callbacks.
        void *context;
} SUBSCRIPTION_CONTROL_PARAMS_T;

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Subscribe a session to topics.
 *
 * This function requests that a client is subscribed to the topics
 * specified by a topic selector.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters describing the subscription control
 *                      request and callback handlers which may be invoked
 *                      in response.
 */
void subscribe_client(SESSION_T *session, const SUBSCRIPTION_CONTROL_PARAMS_T params);

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Unsubscribe a session from topics.
 *
 * This function requests that a client is unsubscribed from the
 * topics specified by a topic selector.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters describing the subscription control
 *                      request and callback handlers which may be invoked
 *                      in response.
 */
void unsubscribe_client(SESSION_T *session, const SUBSCRIPTION_CONTROL_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(subscription_control, SVC_SUBSCRIPTION_CONTROL_REQUEST_T, SVC_SUBSCRIPTION_CONTROL_RESPONSE_T)

#endif
