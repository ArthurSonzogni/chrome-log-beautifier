#ifndef _diffusion_svc_subscribe_
#define _diffusion_svc_subscribe_ 1

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
 * @file svc-subscribe.h
 *
 * Functions and structures required for a client to be able to subscribe to
 * topics on a Diffusion server.
 */

#include "internal/service_int.h"
#include "session.h"
#include "service/svc-notify-subscription.h"

/**
 * @brief Structure describing a subscription request.
 */
typedef struct svc_subscribe_request_s {
        /// The client session.
        SESSION_ID_T *session_id;
        /// The selector string describing the set of topics to be
        /// subscribed to.
        char *topic_selector;
} SVC_SUBSCRIBE_REQUEST_T;

/**
 * @brief Structure describing a subscription response.
 */
typedef struct svc_subscribe_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SUBSCRIBE_RESPONSE_T;

/**
 * @brief Callback for on_subscribe().
 *
 * @param session The current active session.
 * @param context User-supplied context from the initial subscribe() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_subscribe_cb)(SESSION_T *session, void *context);

/**
 * @brief Callback handlers that may be registered to deal with
 * messages sent by Diffusion in response to the subscription request.
 */
#define SUBSCRIPTION_HANDLERS                                           \
        /** A callback for when a topic message has been received. */   \
        TOPIC_HANDLER_T on_topic_message;                               \
        /** A callback to indicate that the subscription request has been received. */ \
        on_subscribe_cb on_subscribe;                                   \
        /** Standard service error callback. */                         \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard callback. */                       \
        DISCARD_HANDLER_T on_discard;

typedef struct subscription_handlers_s {
        SUBSCRIPTION_HANDLERS
} SUBSCRIPTION_HANDLERS_T;

/**
 * @brief Structure supplied when subscribing to a topic.
 */
typedef struct subscription_params_s {
        /// Callback handlers.
        SUBSCRIPTION_HANDLERS
        /// Topic selected describing which topics to subscribe to.
        const char *topic_selector;
        /// User-supplied context return to callbacks.
        void *context;
} SUBSCRIPTION_PARAMS_T;

/**
 * @ingroup PublicAPI_Topics
 *
 * @brief Subscribe to topics.
 *
 * This function has the roles of subscribing to the topics specified by the
 * selector and installing handlers for received \ref TOPIC_MESSAGE_T messages
 * which match that selector and also any other service messages related to
 * subscription. These handlers are called before (and in addition to) any
 * handlers installed using add_message_handler() for \ref
 * MESSAGE_TYPE_TOPIC_LOAD or \ref MESSAGE_TYPE_DELTA messages.
 *
 * If a topic handler was already registered for the supplied selector, it is
 * replaced and the old handlers are returned.
 *
 * @param session       The session handle. If NULL, the function returns immediately.
 * @param params        Parameters describing the subscription request and
 *                      callbacks handlers which may be invoked in response.
 * @return              Previous topic handler if replacing, else NULL.
 */
TOPIC_HANDLER_T subscribe(SESSION_T *session, const SUBSCRIPTION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(subscribe, SVC_SUBSCRIBE_REQUEST_T, SVC_SUBSCRIBE_RESPONSE_T)

#endif
