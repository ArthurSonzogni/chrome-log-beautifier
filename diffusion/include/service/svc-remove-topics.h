#ifndef _svc_remove_topics_h_
#define _svc_remove_topics_h_ 1

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
 * @file svc-remove-topics.h
 *
 * Remove topics feature (topic control).
 */

#include "internal/service_int.h"
#include "types/content_types.h"
#include "hash.h"

/**
 * @brief Structure describing a "remove topic" request.
 */
typedef struct svc_remove_topics_request_s {
        /// The selector string describing the set of topics to be
        /// removed.
        char *topic_selector;
} SVC_REMOVE_TOPICS_REQUEST_T;

/**
 * @brief Structure describing a "remove topic" response.
 */
typedef struct svc_remove_topics_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_REMOVE_TOPICS_RESPONSE_T;

/**
 * @brief Callback for remove_topics() handlers.
 *
 * @param session The current active session.
 * @param response The response message from Diffusion.
 * @param context User-supplied context from the remove_topics() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_remove_topics_cb)(SESSION_T *session,
                                   const SVC_REMOVE_TOPICS_RESPONSE_T *response,
                                   void *context);

/**
 * @brief User-specified callbacks that may be invoked in response to
 * issuing a "remove topic" request to Diffusion.
 */
#define REMOVE_TOPICS_HANDLERS \
        /** Topic removed. */                                           \
        on_remove_topics_cb on_removed;                                 \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct remove_topics_handlers_s {
        REMOVE_TOPICS_HANDLERS
} REMOVE_TOPICS_HANDLERS_T;

/**
 * @brief Structure passed to remove_topics().
 */
typedef struct remove_topics_params_s {
        REMOVE_TOPICS_HANDLERS
        /// Selector describing topics to remove.
        const char *topic_selector;
        /// User-supplied context returned to callbacks.
        void *context;
} REMOVE_TOPICS_PARAMS_T;

/**
 * @ingroup PublicAPI_TopicControl
 *
 * @brief Remove topics from Diffusion.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure describing which topics to remove
 *                      and callbacks to handle success or failure
 *                      notifications.
 */
void remove_topics(SESSION_T *session, const REMOVE_TOPICS_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(remove_topics, SVC_REMOVE_TOPICS_REQUEST_T, SVC_REMOVE_TOPICS_RESPONSE_T)

#endif
