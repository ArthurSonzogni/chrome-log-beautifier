#ifndef _diffusion_svc_session_properties_registration_h_
#define _diffusion_svc_session_properties_registration_h_ 1

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
 * @file svc-session-properties-registration.h
 *
 * Functions for registering a registration for a control client, so that
 * it can receive notifications that other clients have opened, closed
 * or been updated.
 */

#include "internal/service_int.h"
#include "types/client_details_types.h"
#include "svc-session-properties-common.h"

#define PROPERTIES_SELECTOR_ALL_FIXED_PROPERTIES "*F"
#define PROPERTIES_SELECTOR_ALL_USER_PROPERTIES "*U"

typedef struct svc_session_properties_registration_request_s {
        /// The properties requested from a session when it is opened,
        /// closed or updated.
        SET_T *required_properties;
        /// Conversation ID of the registered listener.
        CONVERSATION_ID_T conversation_id;
} SVC_SESSION_PROPERTIES_REGISTRATION_REQUEST_T;

typedef struct svc_session_properties_registration_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SESSION_PROPERTIES_REGISTRATION_RESPONSE_T;

typedef int (*on_session_properties_registered_cb)(SESSION_T *session, void *context);

#define SESSION_PROPERTIES_REGISTRATION_HANDLERS                        \
        /** Callback fired when the listener has been registered. */    \
        on_session_properties_registered_cb on_registered;              \
        /** Error callback if an error occurs during listener registration.  */ \
        ERROR_HANDLER_T on_registration_error;                          \
        /** Standard discard handler. */                                \
        DISCARD_HANDLER_T on_discard;

typedef struct session_properties_registration_handlers_s {
        SESSION_PROPERTIES_REGISTRATION_HANDLERS
        SESSION_PROPERTIES_EVENT_HANDLERS
} SESSION_PROPERTIES_REGISTRATION_HANDLERS_T;

typedef struct session_properties_registration_params_s {
        SESSION_PROPERTIES_REGISTRATION_HANDLERS
        SESSION_PROPERTIES_EVENT_HANDLERS
        /// A set containing properties of clients to be reported back
        /// to the listener. The properties are NULL-terminated
        /// strings, or the constants PROPERTIES_SELECTOR_ALL_FIXED_PROPERTIES or
        /// PROPERTIES_SELECTOR_ALL_USER_PROPERTIES.
        SET_T *required_properties;
        /// User-supplied context to return to callbacks.
        void *context;
} SESSION_PROPERTIES_REGISTRATION_PARAMS_T;

/**
 * @ingroup PublicAPI_ClientControl
 *
 * @brief Register a listener for changes to other clients' properties.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure describing which properties should
 *                      be reported, and the callbacks to be invoked.
 */
void session_properties_listener_register(SESSION_T *session,
                                          const SESSION_PROPERTIES_REGISTRATION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(session_properties_registration, SVC_SESSION_PROPERTIES_REGISTRATION_REQUEST_T, SVC_SESSION_PROPERTIES_REGISTRATION_RESPONSE_T)

#endif
