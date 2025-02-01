#ifndef _diffusion_svc_fetch_
#define _diffusion_svc_fetch_ 1

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
 * @file svc-fetch.h
 *
 * Client functions for fetching data from topics.
 */

#include "internal/service_int.h"
#include "session.h"

/**
 * @brief Structure used to model a fetch request.
 */
typedef struct svc_fetch_request_s {
        /// A topic selector describing the set of topics to fetch
        /// data for.
        char *topic_selector;

        /// A conversation ID used to correlate fetch status response
        /// messages.
        CONVERSATION_ID_T *conversation_id;
} SVC_FETCH_REQUEST_T;

/**
 * @brief Structure used to model a fetch response.
 */
typedef struct svc_fetch_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_FETCH_RESPONSE_T;

/**
 * @brief Structure passed to fetch response callback.
 */
typedef struct svc_fetch_status_response_s {
        /// Name of topic fetched.
        char *topic_path;
        /// Status flag; DIFFUSION_TRUE if fetch completed, or
        /// DIFFUSION_FALSE if fetch failed.
        int status_flag;
        /// Payload bytes.
        BUF_T *payload;
} SVC_FETCH_STATUS_RESPONSE_T;

/**
 * @brief Callback for on_fetch() command having been received by Diffusion.
 *
 * @param session The currently active session.
 * @param context User-supplied context from the fetch() call.
 *
 * @return HANDLERS_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_fetch_cb)(SESSION_T *session, void *context);

/**
 * @brief Callback for on_status_message() indicating the end of the
 * fetch conversation.
 *
 * @param session The currently active session.
 * @param status The fetch status response.
 * @param context User-supplied context from the fetch() call.
 * @return HANDLERS_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_fetch_status_message_cb)(SESSION_T *session,
                                          const SVC_FETCH_STATUS_RESPONSE_T *status,
                                          void *context);


/**
 * @brief Callbacks which can be registered for handling the responses
 * to the fetch request.
 */
#define FETCH_HANDLERS                                                  \
        /** Called when a message containing topic data has been received. */ \
        TOPIC_HANDLER_T on_topic_message;                               \
        /** Called when the fetch request has been processed by Diffusion. */ \
        on_fetch_cb on_fetch;                                           \
        /**  When the fetch request has completed successfully or has errored. */ \
        on_fetch_status_message_cb on_status_message;                   \
        /** Standard service error handler callback. */                 \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler callback. */               \
        DISCARD_HANDLER_T on_discard;

typedef struct fetch_handlers_s {
        FETCH_HANDLERS
} FETCH_HANDLERS_T;

/**
 * @brief Structure supplied to a fetch() request.
 */
typedef struct fetch_params_s {
        FETCH_HANDLERS
        /// The topics for which state is being requested.
        const char *selector;
        /// User-supplied context to return to callbacks.
        void *context;
} FETCH_PARAMS_T;

/**
 * @ingroup PublicAPI_Topics
 *
 * @brief Fetch state for a topic (or many topics).
 *
 * @param session    The session handle. If NULL, the function returns immediately.
 * @param params     Parameters and callback handlers to be invoked to process
 *                   the results of the fetch() call.
 */
void fetch(SESSION_T *session, const FETCH_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(fetch, SVC_FETCH_REQUEST_T, SVC_FETCH_RESPONSE_T)

#endif
