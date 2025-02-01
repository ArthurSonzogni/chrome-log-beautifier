#ifndef _diffusion_svc_session_details_event_h_
#define _diffusion_svc_session_details_event_h_ 1

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
 * @file svc-session-details-event.h
 *
 * Functions for notifying a control client that other clients
 * have opened, closed or been updated.
 */

#include "internal/service_int.h"
#include "types/client_details_types.h"
#include "svc-session-details-common.h"

typedef struct svc_session_details_event_request_s {
        SESSION_DETAILS_EVENT_TYPE_T type;
        SESSION_CLOSE_REASON_T close_reason;
        SESSION_ID_T session_id;
        SESSION_DETAILS_T *session_details;
        CONVERSATION_ID_T conversation_id;
} SVC_SESSION_DETAILS_EVENT_REQUEST_T;

typedef struct svc_session_details_event_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SESSION_DETAILS_EVENT_RESPONSE_T;

typedef struct session_details_event_params_s {
        SESSION_DETAILS_EVENT_HANDLERS
        /// User-supplied context to return to callbacks.
        void *context;
} SESSION_DETAILS_EVENT_PARAMS_T;

DECLARE_SERVICE_PROTOTYPES(session_details_event, SVC_SESSION_DETAILS_EVENT_REQUEST_T, SVC_SESSION_DETAILS_EVENT_RESPONSE_T)

#endif
