#ifndef _svc_topic_removal_h_
#define _svc_topic_removal_h_ 1

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
 * @file svc-topic-removal.h
 *
 * Topic removal feature (topic control).
 */

#include "internal/service_int.h"
#include "types/content_types.h"
#include "hash.h"

/**
 * @brief Structure describing a "topic removal" request.
 */
typedef struct svc_topic_removal_request_s {
        /// The selector string describing the set of topics to be
        /// removed.
        char *topic_selector;
} SVC_TOPIC_REMOVAL_REQUEST_T;

/**
 * @brief Structure describing a "topic removal" response.
 */
typedef struct svc_topic_removal_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_TOPIC_REMOVAL_RESPONSE_T;

/**
 * @brief Callback for topic_removal() handlers.
 *
 * @param session The current active session.
 * @param response The response message from Diffusion.
 * @param context User-supplied context from the topic_removal() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_topic_removal_cb)(SESSION_T *session,
                                   const SVC_TOPIC_REMOVAL_RESPONSE_T *response,
                                   void *context);

/**
 * @brief User-specified callbacks that may be invoked in response to
 * issuing a "topic removal" request to Diffusion.
 */
#define TOPIC_REMOVAL_HANDLERS \
        /** Topic removed. */                                           \
        on_topic_removal_cb on_removed;                                 \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct topic_removal_handlers_s {
        TOPIC_REMOVAL_HANDLERS
} TOPIC_REMOVAL_HANDLERS_T;

/**
 * @brief Structure passed to topic_removal().
 */
typedef struct topic_removal_params_s {
        TOPIC_REMOVAL_HANDLERS
        /// Selector describing topics to remove.
        const char *topic_selector;
        /// User-supplied context returned to callbacks.
        void *context;
} TOPIC_REMOVAL_PARAMS_T;

/**
 * @ingroup PublicAPI_TopicControl
 *
 * @brief Remove topics from Diffusion.
 *
 * Send a request to remove one or more topics at the server.  The topics to
 * remove will depend upon the nature of the topic selector specified. If the
 * selector does not have descendant pattern qualifiers (i.e. / or //), only
 * those topics that exist at paths indicated by the selector will be removed
 * and not their descendants. If a single / qualifier is specified, all
 * descendants of the matching topic paths will be removed. If // is
 * specified, all branches of the topic tree that match the selector (i.e
 * topics at the selected paths and all descendants of the selected paths)
 * will be removed.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure describing which topics to remove
 *                      and callbacks to handle success or failure
 *                      notifications.
 */
void topic_removal(SESSION_T *session, const TOPIC_REMOVAL_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(topic_removal, SVC_TOPIC_REMOVAL_REQUEST_T, SVC_TOPIC_REMOVAL_RESPONSE_T)

#endif
