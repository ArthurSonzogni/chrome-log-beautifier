#ifndef _diffusion_svc_unsubscribe_
#define _diffusion_svc_unsubscribe_ 1

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
 * @file svc-unsubscribe.h
 *
 * Functions and structures required for a client to be able to unsubscribe from
 * topics on a Diffusion server.
 */

#include "internal/service_int.h"
#include "session.h"

/**
 * @brief Structure describing a unsubscription request.
 */
typedef struct svc_unsubscribe_request_s {
        /// The set of topics to request unsubscription from.
        char *topic_selector;
} SVC_UNSUBSCRIBE_REQUEST_T;

/**
 * @brief Structure describing a unsubscription response.
 */
typedef struct svc_unsubscribe_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_UNSUBSCRIBE_RESPONSE_T;

/**
 * @brief Callback for on_unsubscribe().
 *
 * @param session The current active session.
 * @param context User-supplied context from the initial unsubscribe() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_unsubscribe_cb)(SESSION_T *session, void *context);

/**
 * @brief Callback handlers that may be registered to deal with
 * messages sent by Diffusion in response to the unsubscription
 * request.
 */
#define UNSUBSCRIPTION_HANDLERS                                         \
        /** A callback to indicate that the unsubscription request has been received. */ \
        on_unsubscribe_cb on_unsubscribe;                               \
        /** Standard error handler callback. */                         \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard discard handler callback. */                       \
        DISCARD_HANDLER_T on_discard;

typedef struct unsubscription_handlers_s {
        UNSUBSCRIPTION_HANDLERS
} UNSUBSCRIPTION_HANDLERS_T;

/**
 * @brief Structure supplied when unsubscribing from a topic.
 */
typedef struct unsubscription_params_s {
        UNSUBSCRIPTION_HANDLERS
        /// Topic selector describing which topics to unsubscribe from.
        const char *topic_selector;
        /// User-supplied context returned to callbacks.
        void *context;
} UNSUBSCRIPTION_PARAMS_T;

/**
 * @ingroup PublicAPI_Topics
 *
 * @brief Unsubscribe from one or more topics.
 *
 * This function has the roles of subscribing to the topics specified by the
 * selector and installing handlers for received \ref TOPIC_MESSAGE_T messages
 * which match that selector and also any other service messages related to
 * unsubscription.
 *
 * @param session     The session handle. If NULL, this function returns immediately.
 * @param params      Parameters describing the unsubscription request and
 *                    callback handlers which may be invoked in response.
 */
void unsubscribe(SESSION_T *session, const UNSUBSCRIPTION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(unsubscribe, SVC_UNSUBSCRIBE_REQUEST_T, SVC_UNSUBSCRIBE_RESPONSE_T)

#endif
