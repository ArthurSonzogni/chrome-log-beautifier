#ifndef _diffusion_svc_session_properties_event_h_
#define _diffusion_svc_session_properties_event_h_ 1

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
 * @file svc-session-properties-event.h
 *
 * Functions for notifying a control client that other clients
 * have opened, closed or been updated.
 */

#include "internal/service_int.h"
#include "types/client_details_types.h"
#include "svc-session-properties-common.h"

typedef struct svc_session_properties_event_request_s {
        /// Conversaton ID of messages associated with the registered listener.
        CONVERSATION_ID_T conversation_id;
        /// Session ID of the client session.
        SESSION_ID_T session_id;
        /// The type of event (e.g. open, close, update)
        SESSION_PROPERTIES_EVENT_TYPE_T type;
        /// If the event is for a session close, this gives the reason
        /// for the closure.
        SESSION_CLOSE_REASON_T close_reason;
        /// If the event is for session properties updates, this gives
        /// the reason for the update.
        SESSION_PROPERTIES_UPDATE_TYPE_T update_type;
        /// A map of the values of the properties that have changed
        HASH_T *old_properties;
        /// A map of the requested session properties.
        HASH_T *properties;
} SVC_SESSION_PROPERTIES_EVENT_REQUEST_T;

typedef struct svc_session_properties_event_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SESSION_PROPERTIES_EVENT_RESPONSE_T;

typedef struct session_properties_event_params_s {
        SESSION_PROPERTIES_EVENT_HANDLERS
        /// User-supplied context to return to callbacks.
        void *context;
} SESSION_PROPERTIES_EVENT_PARAMS_T;

DECLARE_SERVICE_PROTOTYPES(session_properties_event, SVC_SESSION_PROPERTIES_EVENT_REQUEST_T, SVC_SESSION_PROPERTIES_EVENT_RESPONSE_T)

#endif
