#ifndef _diffusion_svc_get_session_properties_h_
#define _diffusion_svc_get_session_properties_h_ 1

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
 * @file svc-get-session-properties.h
 *
 * Functions for directly requesting session properties of a connected
 * client.
 */

#include "internal/service_int.h"
#include "types/session_types.h"
#include "set.h"
#include "hash.h"

typedef struct svc_get_session_properties_request_s {
        /// The session ID of the connected client whose properties we
        /// are requesting.
        SESSION_ID_T *session_id;
        /// A set containing the names of the properties we are
        /// requesting.
        SET_T *required_properties;
} SVC_GET_SESSION_PROPERTIES_REQUEST_T;

typedef struct svc_get_session_properties_response_s {
        /// Returned properties for the session.
        HASH_T *properties;
} SVC_GET_SESSION_PROPERTIES_RESPONSE_T;

typedef int (*get_session_properties_cb)(SESSION_T *session,
                                         const SVC_GET_SESSION_PROPERTIES_RESPONSE_T *response,
                                         void *context);

#define GET_SESSION_PROPERTIES_HANDLERS                             \
        /** Callback fired when session properties are received. */ \
        get_session_properties_cb on_session_properties;            \
        /** Error callback. */                                      \
        ERROR_HANDLER_T on_error;                                   \
        /** Discard callback. */                                    \
        DISCARD_HANDLER_T on_discard;

typedef struct get_session_properties_params_s {
        GET_SESSION_PROPERTIES_HANDLERS
        /// The session ID of the connected client whose properties we
        /// are requesting.
        SESSION_ID_T *session_id;
        /// A set containing properties of clients to be reported back
        /// to the listener. The properties are NULL-terminated
        /// strings, or the constants PROPERTIES_SELECTOR_ALL_FIXED_PROPERTIES or
        /// PROPERTIES_SELECTOR_ALL_USER_PROPERTIES.
        SET_T *required_properties;
        /// User-supplied context to return to callbacks.
        void *context;
} GET_SESSION_PROPERTIES_PARAMS_T;

/**
 * @ingroup PublicAPI_ClientControl
 *
 * @brief Request properties of a connected client.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure describing which client and
 *                      properties we're interested in, and the callbacks
 *                      to be invoked.
 */
void get_session_properties(SESSION_T *session, const GET_SESSION_PROPERTIES_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(get_session_properties, SVC_GET_SESSION_PROPERTIES_REQUEST_T, SVC_GET_SESSION_PROPERTIES_RESPONSE_T)

#endif
