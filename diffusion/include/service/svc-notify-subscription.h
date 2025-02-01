#ifndef _diffusion_svc_notify_subscription_h_
#define _diffusion_svc_notify_subscription_h_ 1

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
 * @file svc-notify-subscription.h
 *
 * Functions for notifying a client that it has been subscribed to a
 * topic.
 */

#include "internal/service_int.h"
#include "types/topic_details_types.h"

/**
 * @brief Structure describing an incoming notification that the
 * client has been subscribed to a topic.
 */
typedef struct svc_notify_subscription_request_s {
        /// Topic identification.
        struct {
                /// Internal identifier for the topic.
                uint32_t        topic_id;
                /// Name of the topic which the client was subscribed
                /// to.
                char *          topic_path;
        } topic_info;
        /// Description of the structure of the topic.
        TOPIC_DETAILS_T *topic_details;
} SVC_NOTIFY_SUBSCRIPTION_REQUEST_T;

/**
 * @brief Response message to subscription notifications.
 */
typedef struct svc_notify_subscription_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_NOTIFY_SUBSCRIPTION_RESPONSE_T;

/**
 * @brief Callback for notify_subscription_register().
 *
 * @param session The current active session.
 * @param request The incoming notification request.
 * @param context User-supplied context from the initial registration call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_notify_subscription_cb)(SESSION_T *session,
                                         const SVC_NOTIFY_SUBSCRIPTION_REQUEST_T *request,
                                         void *context);

/**
 * @brief Callback handlers for subscription notification messages.
 */
#define NOTIFY_SUBSCRIPTION                                             \
        /** Callback fired when a subscription notification has been received. */ \
        on_notify_subscription_cb on_notify_subscription;               \
        /** Standard service error callback. */                         \
        ERROR_HANDLER_T on_error;

typedef struct notify_subscription_handlers_s {
        NOTIFY_SUBSCRIPTION
} NOTIFY_SUBSCRIPTION_HANDLERS_T;

/**
 * @brief Structure supplied when registering to receive topic
 * subscription notifications.
 */
typedef struct notify_subscription_params_s {
        NOTIFY_SUBSCRIPTION
        /// User-supplied context returned to callbacks.
        void *context;
} NOTIFY_SUBSCRIPTION_PARAMS_T;

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Register to receive subscription notifications.
 *
 * This function should be called by the application to register to
 * receive notifications that it has been subscribed to a topic.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Callbacks and other parameters. {@link NOTIFY_SUBSCRIPTION_PARAMS_T on_notify_subscription}
 *                      should be set to receive notifications.
 */
void notify_subscription_register(SESSION_T *session, const NOTIFY_SUBSCRIPTION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(notify_subscription, SVC_NOTIFY_SUBSCRIPTION_REQUEST_T, SVC_NOTIFY_SUBSCRIPTION_RESPONSE_T)

#endif
