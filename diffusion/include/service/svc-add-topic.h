#ifndef _svc_add_topic_h_
#define _svc_add_topic_h_ 1

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
 * @file svc-add-topic.h
 *
 * Add topic feature (topic control).
 */

#include "internal/service_int.h"
#include "types/content_types.h"
#include "types/topic_details_types.h"
#include "topic-details.h"
#include "hash.h"

/**
 * @brief Structure describing an "add topic" request.
 */
typedef struct svc_add_topic_request_s {
        /// The topic path being added.
        char *topic_path;
        /// The topic details that describe the structure of this
        /// topic.
        TOPIC_DETAILS_T *topic_details;
        /// Initial content for the topic.
        CONTENT_T *content;
} SVC_ADD_TOPIC_REQUEST_T;

/**
 * @brief Possible values indicating the success or failure reasons in
 * response to a topic add request.
 */
typedef enum {
        /// Topic added OK - details not cached.
        ADD_TOPIC_RESPONSE_OK = 0,
        /// Topic added OK and details cached.
        ADD_TOPIC_RESPONSE_OK_CACHED,
        /// Topic add failed for supplied reason.
        ADD_TOPIC_RESPONSE_FAIL,
        /// Requested details not found in server side cache - send
        /// again.
        ADD_TOPIC_RESPONSE_CACHE_FAILURE
} SVC_ADD_TOPIC_RESPONSE_CODE_T;

typedef enum {
        /// No error while adding topic.
        ADD_TOPIC_FAILURE_REASON_SUCCESS = 0,
        /// Topic already existed with exactly the same details.
        ADD_TOPIC_FAILURE_REASON_EXISTS = 1,
        /// Topic already exists with the same name but different
        /// details.
        ADD_TOPIC_FAILURE_REASON_EXISTS_MISMATCH =  2,
        /// The name of the topic was found to be invalid.
        ADD_TOPIC_FAILURE_REASON_INVALID_NAME =  3,
        /// Some aspect of the details failed validation.
        ADD_TOPIC_FAILURE_REASON_INVALID_DETAILS =  4,
        /// A user supplied class could either not be found or could
        /// not be instantiated at the server.
        ADD_TOPIC_FAILURE_REASON_USER_CODE_ERROR =  5,
        /// A referenced toic was not found or was not of the correct
        /// type.
        ADD_TOPIC_FAILURE_REASON_TOPIC_NOT_FOUND =  6,
        /// The client was denied permission to add a topic of the
        /// specified type a the given point in the topic hierarchy.
        ADD_TOPIC_FAILURE_REASON_PERMISSIONS_FAILURE =  7,
        /// Initialisation of topic with the supplied content failed,
        /// possibly because it was of the wrong format.
        ADD_TOPIC_FAILURE_REASON_INITIALISE_ERROR =  8,
        /// An unexpected error occurred whilst attempting to create
        /// the topic.
        ADD_TOPIC_FAILURE_REASON_UNEXPECTED_ERROR =  9
} SVC_ADD_TOPIC_FAILURE_REASON_T;

/**
 * Structure describing an "add topic" response.
 */
typedef struct svc_add_topic_response_s {
        /// The result of the topic add request.
        SVC_ADD_TOPIC_RESPONSE_CODE_T response_code;
        /// Reason for failure, if ADD_TOPIC_RESPONSE_FAIL.
        SVC_ADD_TOPIC_FAILURE_REASON_T reason;
} SVC_ADD_TOPIC_RESPONSE_T;

/**
 * @brief Callback for add_topic().
 *
 * @param session The current active session.
 * @param response The response message from Diffusion.
 * @param context User-supplied context from the initial add_topic() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_add_topic_cb)(SESSION_T *session,
                               const SVC_ADD_TOPIC_RESPONSE_T *response,
                               void *context);
/**
 * @brief User-specified callbacks that may be invoked in response to
 * issuing an "add topic" request to Diffusion.
 */
#define ADD_TOPIC_HANDLERS                                              \
        /** Topic added successfully. */                                \
        on_add_topic_cb on_topic_added;                                 \
        /** Topic failed to be added. */                                \
        on_add_topic_cb on_topic_add_failed;                            \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard conversation discard callback */                   \
        DISCARD_HANDLER_T on_discard;

typedef struct add_topic_handlers_s {
        ADD_TOPIC_HANDLERS
} ADD_TOPIC_HANDLERS_T;

/**
 * @brief Structure passed when adding a topic.
 */
typedef struct add_topic_params_s {
        ADD_TOPIC_HANDLERS
        /// Path of the topic to be added.
        const char *topic_path;
        /// Details describing the topic structure.
        const TOPIC_DETAILS_T *details;
        /// Initial content for the topic.
        const CONTENT_T *content;
        /// User-supplied context returned to callbacks.
        void *context;
} ADD_TOPIC_PARAMS_T;

/**
 * @ingroup PublicAPI_TopicControl
 *
 * @brief Add a topic to Diffusion.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure describing the topic to be added,
 *                      its definition, initial content and callbacks to
 *                      handle success or failure notifications.
 */
void add_topic(SESSION_T *session, const ADD_TOPIC_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(add_topic, SVC_ADD_TOPIC_REQUEST_T, SVC_ADD_TOPIC_RESPONSE_T)

#endif
