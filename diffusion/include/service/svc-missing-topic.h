#ifndef _diffusion_svc_missing_topic_h_
#define _diffusion_svc_missing_topic_h_ 1

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
 * @file svc-missing-topic.h
 *
 * Allow a client to register to receive notification of subscription/fetch
 * requests for topics that do not exist.
 */

#include "internal/service_macros.h"
#include "session.h"

/**
 * @brief Specifies whether a missing topic notification should cause Diffusion
 * to retry the original request (proceed) or not (cancel).
 *
 * If the requested topic is missing, the handler can elect to create the topic and
 * call missing_topic_proceed() to instruct Diffusion to retry the original operation.
 */
typedef enum {
        /// Cancel the original operation.
        MISSING_TOPIC_CANCEL = 0,
        /// Retry the original operations.
        MISSING_TOPIC_PROCEED = 1
} MISSING_TOPIC_ACTION_T;

/**
 * @brief Structure of a request to register for missing topic notifications.
 */
typedef struct svc_missing_topic_request_s {
        /// The session id of the client making the request for the topic.
        SESSION_ID_T *session_id;
        /// Register for notifications of missing topics that match
        /// this topic selector.
        char *topic_selector;
        /// The conversation ID to use in the response message.
        CONVERSATION_ID_T *conversation_id;
        /// Internal - action to take on this request.
        int action;
} SVC_MISSING_TOPIC_REQUEST_T;

/**
 * @brief Structure of a response to register for missing topic notifications.
 */
typedef struct svc_missing_topic_response_s {
        /// Instruct Diffusion to retry processing the request for the
        /// topic, or not.
        MISSING_TOPIC_ACTION_T action;
} SVC_MISSING_TOPIC_RESPONSE_T;

/**
 * @brief Callback for missing_topic_register_handler().
 *
 * @param session The current active session.
 * @param request The incoming notification message.
 * @param context User-supplied context from the initial registration call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_missing_topic_cb)(SESSION_T *session,
                                   const SVC_MISSING_TOPIC_REQUEST_T *request,
                                   void *context);

/**
 * @brief Callback handlers for missing topic notification messages.
 */
#define MISSING_TOPIC_HANDLERS                                          \
        /** Callback when a client requests a topic that doesn't exist. */ \
        on_missing_topic_cb on_missing_topic;                           \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct missing_topic_handlers_s {
        MISSING_TOPIC_HANDLERS
} MISSING_TOPIC_HANDLERS_T;

/**
 * @brief Structure supplied when registering to receive missing topic
 * notifications.
 */
typedef struct missing_topic_params_s {
        MISSING_TOPIC_HANDLERS
        /// Receive notifications for missing topics under this topic.
        const char *topic_path;
        /// User-supplied context return to callbacks.
        void *context;
} MISSING_TOPIC_PARAMS_T;

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Register to receive notifications about missing topics that
 * clients attempt to fetch/subscribe.
 *
 * Register a handler for receiving missing topic notifications when a client
 * attempts to fetch or subscribe to a topic underneath a given root topic, but
 * that requested topic doesn't exist.
 *
 * @param session       The current session. If NULL, this function returns immediately
 *                      with the value NULL.
 * @param params        Parameters defining the registration request.
 * @return              A conversation id that can be used to deregister the
 *                      handler at a later time, or NULL if the supplied session is NULL.
 */
CONVERSATION_ID_T *missing_topic_register_handler(SESSION_T *session, const MISSING_TOPIC_PARAMS_T params);

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Cease receiving missing topic notifications.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param conversation_id       The conversation id returned from a previous
 *                              call to missing_topic_register_handler().
 */
void missing_topic_deregister_handler(SESSION_T *session,
                                      const CONVERSATION_ID_T *conversation_id);

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Proceed (retry) after a processing a missing topic notification.
 *
 * To be called in a missing topic notification handler, this notifies
 * Diffusion that it may complete processing of the original request.
 *
 * @param session       The current session.
 * @param request       The request which gave rise to the missing topic
 *                      notification.
 */
void missing_topic_proceed(SESSION_T *session,
                           SVC_MISSING_TOPIC_REQUEST_T *request);

/**
 * @ingroup PublicAPI_SubscriptionControl
 *
 * @brief Cancel the operation leading to this missing topic notification.
 *
 * To be called in a missing topic notification handler, this notifies
 * Diffusion that it should not continue processing the original request.
 *
 * @param session       The current session.
 * @param request       The request which gave rise to the missing topic
 *                      notification.
 */
void missing_topic_cancel(SESSION_T *session,
                          SVC_MISSING_TOPIC_REQUEST_T *request);

DECLARE_SERVICE_PROTOTYPES(missing_topic, SVC_MISSING_TOPIC_REQUEST_T, SVC_MISSING_TOPIC_RESPONSE_T)

#endif
