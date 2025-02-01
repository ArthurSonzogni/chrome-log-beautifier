#ifndef _diffusion_svc_update_h_
#define _diffusion_svc_update_h_ 1

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
 * @file svc-update.h
 *
 * Functions for registering an update source and providing updates to
 * topics.
 *
 */

#include "internal/service_int.h"
#include "types/update_types.h"
#include "update.h"

/**
 * @brief Service states that an update source may be in.
 *
 * @remarks Updates should only be performed when the registered
 * source is an ACTIVE state.
 */
typedef enum {
        /// Initial source state, before state is received from the
        /// server.
        UPDATE_SOURCE_STATE_INIT = 0,
        /// Active state - the topic may be updated by this source.
        UPDATE_SOURCE_STATE_ACTIVE = 1,
        /// Terminal state; no further state changes may take place.
        UPDATE_SOURCE_STATE_CLOSED = 2,
        /// Inactive state that permits being set to Active or Closed.
        UPDATE_SOURCE_STATE_STANDBY = 3
} UPDATE_SOURCE_STATE_T;

/**
 * @brief Values indicating the result of an update request.
 */
typedef enum {
        /// The update request has been processed successfully.
        UPDATE_RESPONSE_OK = 0,
        /// An error occurred while processing the update request.
        UPDATE_RESPONSE_ERROR = 1
} UPDATE_RESPONSE_STATUS_T;

/**
 * @brief These values indicate the nature of an update failure.
 */
typedef enum {
        /// Error sending the response.
        UPDATE_RESPONSE_ERROR_SEND_FAILURE = 0,
        /// Not registered to update this topic.
        UPDATE_RESPONSE_INVALID_TOPIC = 1,
        /// Invalid content for this topic.
        UPDATE_RESPONSE_INVALID_CONTENT = 2,
        /// The updater is not valid for this topic.
        UPDATE_RESPONSE_INVALID_UPDATER = 3,
        /// The topic being updated does not exist.
        UPDATE_RESPONSE_TOPIC_DOES_NOT_EXIST = 4
} UPDATE_RESPONSE_ERROR_T;

/**
 * @brief Structure describing a request to register a topic update
 * source.
 */
typedef struct svc_update_registration_request_s {
        /// The conversation ID that this update source will use.
        CONVERSATION_ID_T *updater_id;
        /// The topic path below which this updater will issue updates.
        char *topic_path;
} SVC_UPDATE_REGISTRATION_REQUEST_T;

/**
 * @brief Structure describing the response to a topic update
 * registration request.
 */
typedef struct svc_update_registration_response_s {
        /// The state that the updater is in.
        UPDATE_SOURCE_STATE_T state;
} SVC_UPDATE_REGISTRATION_RESPONSE_T;

/**
 * @brief Structure describing a request to deregister a topic update
 * source.
 */
typedef struct svc_update_deregistration_request_s {
        /// The ID of the updater to be deregistered.
        CONVERSATION_ID_T *updater_id;
} SVC_UPDATE_DEREGISTRATION_REQUEST_T;

/**
 * @brief Structure describing the response to a topic update
 * deregistration request.
 */
typedef struct svc_update_deregistration_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_UPDATE_DEREGISTRATION_RESPONSE_T;

/**
 * @brief Structure describing a request to update a topic.
 */
typedef struct svc_update_request_s {
        /// The conversation id of the registered updater.
        CONVERSATION_ID_T *updater_id;
        /// The path to the topic which will be updated.
        char *topic_path;
        /// The update itself.
        UPDATE_T *update;
} SVC_UPDATE_REQUEST_T;

/**
 * @brief Structure describing the response to a topic update request.
 */
typedef struct svc_update_response_s {
        /// The result of the update.
        UPDATE_RESPONSE_STATUS_T status;
        /// On error, the reason that the error occurred.
        UPDATE_RESPONSE_ERROR_T error_code;
} SVC_UPDATE_RESPONSE_T;

/**
 * @brief Structure describing a request to change an updater's state.
 */
typedef struct svc_update_source_state_request_s {
        CONVERSATION_ID_T *updater_id;
        UPDATE_SOURCE_STATE_T new_state;
        UPDATE_SOURCE_STATE_T old_state;
} SVC_UPDATE_SOURCE_STATE_REQUEST_T;

/**
 * @brief Structure for responding to a request to change and
 * updater's state.
 */
typedef struct svc_update_source_state_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_UPDATE_SOURCE_STATE_RESPONSE_T;


/* Callback functions */

/**
 * @brief Callback for register_update_source().
 *
 * @param session The current active session.
 * @param updater_id The updater_id identifying this updater.
 * @param response The registration response message from Diffusion.
 * @param context User-supplied context from the initial registration call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_update_source_registration_cb)(SESSION_T *session,
                                                const CONVERSATION_ID_T *updater_id,
                                                const SVC_UPDATE_REGISTRATION_RESPONSE_T *response,
                                                void *context);

/**
 * @brief User-supplied handlers that may be invoked in response to a
 * request to register a topic source.
 */
#define UPDATE_SOURCE_REGISTRATION_HANDLERS                             \
        /** Topic has no state yet. */                                  \
        on_update_source_registration_cb on_init;                       \
        /** Update handler registered. */                               \
        on_update_source_registration_cb on_registered;                 \
        /** Update handler is active. */                                \
        on_update_source_registration_cb on_active;                     \
        /** Update handler is in standby mode. */                       \
        on_update_source_registration_cb on_standby;                    \
        /** Update handler is closed. */                                \
        on_update_source_registration_cb on_close;                      \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct update_source_registration_handlers_s {
        UPDATE_SOURCE_REGISTRATION_HANDLERS
} UPDATE_SOURCE_REGISTRATION_HANDLERS_T;

/**
 * @brief Structure passed when registering an update source.
 */
typedef struct update_source_registration_params_s {
        /// Callback handlers.
        UPDATE_SOURCE_REGISTRATION_HANDLERS
        /// Path under which to register as an update source.
        const char *topic_path;
        /// User-supplied context returned to callbacks.
        void *context;
} UPDATE_SOURCE_REGISTRATION_PARAMS_T;

/**
 * @brief Callback for deregiser_update_source().
 *
 * @param session The current active session.
 * @param response The deregistration response message from Diffusion.
 * @param context User-supplied context from the deregistration call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_update_source_deregistration_cb)(SESSION_T *session,
                                                  const CONVERSATION_ID_T *updater_id,
                                                  void *context);

#define UPDATE_SOURCE_DEREGISTRATION_HANDLERS                           \
        on_update_source_deregistration_cb on_deregistered;             \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct update_source_deregistration_handlers_s {
        UPDATE_SOURCE_DEREGISTRATION_HANDLERS
} UPDATE_SOURCE_DEREGISTRATION_HANDLERS_T;

/**
 * @brief Structure passed when deregistering an update source.
 */
typedef struct update_source_deregistration_params_s {
        /// Callback handlers.
        UPDATE_SOURCE_DEREGISTRATION_HANDLERS
        /// Updater ID to deregister.
        const CONVERSATION_ID_T *updater_id;
        /// User-supplied context returned to callbacks.
        void *context;
} UPDATE_SOURCE_DEREGISTRATION_PARAMS_T;

/**
 * @brief User-supplied handlers that are invoked in response to
 * sending an update request to a Diffusion topic.
 */
#define UPDATE_SOURCE_HANDLERS                                          \
        /** Callback indicating that the update succeeded. */           \
        int (*on_success)(SESSION_T *session, const CONVERSATION_ID_T *updater_id, const SVC_UPDATE_RESPONSE_T *response, void *context); \
        /** Callback indicating that the update failed. */              \
        int (*on_failure)(SESSION_T *session, const CONVERSATION_ID_T *updater_id, const SVC_UPDATE_RESPONSE_T *response, void *context); \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct update_source_handlers_s {
        UPDATE_SOURCE_HANDLERS
} UPDATE_SOURCE_HANDLERS_T;

/**
 * @brief Structure passed when updating a topic.
 */
typedef struct update_source_params_s {
        UPDATE_SOURCE_HANDLERS
        /// Updater ID returned from a successful registration.
        const CONVERSATION_ID_T *updater_id;
        /// Name of the topic to update.
        const char *topic_path;
        /// The update to apply to the topic.
        const UPDATE_T *update;
        /// User-supplied context returned to callbacks.
        void *context;
} UPDATE_SOURCE_PARAMS_T;

#define UPDATE_SOURCE_STATE_HANDLERS                                    \
        /** Topic has no state yet. */                                  \
        on_update_source_registration_cb on_init;                       \
        /** Update handler is active. */                                \
        on_update_source_registration_cb on_active;                     \
        /** Update handler is in standby mode. */                       \
        on_update_source_registration_cb on_standby;                    \
        /** Update handler is closed. */                                \
        on_update_source_registration_cb on_close;                      \
        /** Standard service error handler. */                          \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler. */                        \
        DISCARD_HANDLER_T on_discard;

typedef struct update_source_state_handlers_s {
        UPDATE_SOURCE_STATE_HANDLERS
} UPDATE_SOURCE_STATE_HANDLERS_T;

typedef struct update_source_state_params_s {
        UPDATE_SOURCE_STATE_HANDLERS
        void *context;
} UPDATE_SOURCE_STATE_PARAMS_T;

/**
 * @ingroup PublicAPI_TopicUpdateControl
 *
 * @brief Register a topic update source.
 *
 * Registers an update source with Diffusion, requesting
 * responsibility for managing topic state for all topics from a given
 * point and below.
 *
 * @param session       The session handle. If NULL, this function returns
 *                      immediately with a NULL value.
 * @param params        Parameter structure describing the registration
 *                      request.
 * @return              A conversation ID that can be used as a handle to
 *                      future topic updates by this update source, or NULL
 *                      if the supplied session is NULL.
 */
CONVERSATION_ID_T *register_update_source(SESSION_T *session, const UPDATE_SOURCE_REGISTRATION_PARAMS_T params);

/**
 * @ingroup PublicAPI_TopicUpdateControl
 *
 * @brief Deregister a topic update source.
 *
 * Deregisters an existing update source from Diffusion.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameter structure describing the deregistration
 *                      request.
 */
void deregister_update_source(SESSION_T *session, const UPDATE_SOURCE_DEREGISTRATION_PARAMS_T params);

/**
 * @ingroup PublicAPI_TopicUpdateControl
 *
 * @brief Update topic content.
 *
 * Sends new topic content to Diffusion, for transmission to subscribed clients.
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameter structure describing which topic to update,
 *                      the new content, how the update is to be applied, and
 *                      callbacks to handle success or failure notifications.
 */
void update(SESSION_T *session, const UPDATE_SOURCE_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(update_registration, SVC_UPDATE_REGISTRATION_REQUEST_T, SVC_UPDATE_REGISTRATION_RESPONSE_T)
DECLARE_SERVICE_PROTOTYPES(update_deregistration, SVC_UPDATE_DEREGISTRATION_REQUEST_T, SVC_UPDATE_DEREGISTRATION_RESPONSE_T)
DECLARE_SERVICE_PROTOTYPES(update, SVC_UPDATE_REQUEST_T, SVC_UPDATE_RESPONSE_T)
DECLARE_SERVICE_PROTOTYPES(update_source_state, SVC_UPDATE_SOURCE_STATE_REQUEST_T, SVC_UPDATE_SOURCE_STATE_RESPONSE_T)

#endif
