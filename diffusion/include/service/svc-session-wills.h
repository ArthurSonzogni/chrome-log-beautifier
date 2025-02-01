#ifndef _diffusion_svc_session_wills_h_
#define _diffusion_svc_session_wills_h_ 1

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
 * @file svc-session-wills.h
 *
 * Functions relating to topic lifecycle; what should Diffusion do
 * with topics created in by this control client session when that
 * session terminates?
 */

#include "internal/service_int.h"

/**
 * @brief Action to perform on session disconnection.
 */
typedef enum {
        /// Remove the branch of the topic tree.
        SESSION_WILL_REMOVE_TOPICS = 0
} SESSION_WILL_T;

/**
 * @brief Result of the request to register a topic will.
 */
typedef enum {
        /// Registration succeeded.
        SESSION_WILL_SUCCESS = 0,
        /// Registration failed due to an existing will registered
        /// elsewhere in the topic tree.
        SESSION_WILL_GROUP_CONFLICT_WITH_EXISTING_REGISTRATION = 1,
        /// Registration failed due to an existing will registered
        /// elsewhere in the topic tree.
        SESSION_WILL_TOPIC_TREE_CONFLICT_WITH_EXISTING_REGISTRATION = 2
} SVC_SESSION_WILLS_RESULT_T;

/**
 * @brief Structure defining a request to register a session will for a topic.
 */
typedef struct svc_session_wills_request_s {
        /// The name of the topic to apply the will to.
        char *topic_path;
        /// The will to perform on session disconnection.
        SESSION_WILL_T will;
} SVC_SESSION_WILLS_REQUEST_T;

/**
 * Structure representing a session will registration response.
 */
typedef struct svc_session_wills_response_s {
        /// The result of the session will registration.
        SVC_SESSION_WILLS_RESULT_T result;
} SVC_SESSION_WILLS_RESPONSE_T;

/**
 * @brief callback for session_wills_remove_topics().
 *
 * @param session The current active session.
 * @param topic_path The path on which the will has been registered.
 * @param context User-supplied context from the initial session_wills_remove_topics() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_session_will_registration_cb)(SESSION_T *session,
                                               const char *topic_path,
                                               void *context);

/**
 * @brief User callbacks that may be defined when registering a session will
 * pertaining to topic removal.
 */
#define SESSION_WILLS_REMOVE_TOPIC_HANDLERS                             \
        /** Callback invoked on successful registration of the session will. */ \
        on_session_will_registration_cb on_registered;                  \
        /** Callback invoked if the session is closed during registration. */ \
        on_session_will_registration_cb on_close;                       \
        /** Standard service error callback. */                         \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard callback. */                       \
        DISCARD_HANDLER_T on_discard;

typedef struct session_wills_remove_topic_handlers_s {
        SESSION_WILLS_REMOVE_TOPIC_HANDLERS
} SESSION_WILLS_REMOVE_TOPIC_HANDLERS_T;

/**
 * @brief Structure supplied when registering a topic will to remove topics.
 */
typedef struct session_wills_remove_topic_params_s {
        SESSION_WILLS_REMOVE_TOPIC_HANDLERS
        /// Path to remove when this session will is invoked.
        const char *topic_path;
        /// User-supplied context returned to callbacks.
        void *context;
} SESSION_WILLS_REMOVE_TOPIC_PARAMS_T;

/**
 * @ingroup PublicAPI_TopicControl
 *
 * @brief Register a topic removal session will.
 *
 * An application should call this function to register a session
 * will, whereby the specified topics are removed on session
 * disconnection.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameters describing the session will registration request
 *                      and the callback handlers which may be invoked in response.
 */
void session_wills_remove_topics(SESSION_T *session, const SESSION_WILLS_REMOVE_TOPIC_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(session_wills, SVC_SESSION_WILLS_REQUEST_T, SVC_SESSION_WILLS_RESPONSE_T)

#endif
