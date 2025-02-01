#ifndef _diffusion_svc_session_properties_event_batch_h_
#define _diffusion_svc_session_properties_event_batch_h_ 1

/*
 * Copyright © 2017 Push Technology Ltd., All Rights Reserved.
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
 * @file svc-session-properties-event-batch.h
 *
 * Functions for notifying a control client that other clients have opened,
 * closed or been updated. This is the service that receives events in
 * batches, rather than individually.
 */

#include "internal/service_int.h"
#include "types/client_details_types.h"
#include "svc-session-properties-common.h"
#include "svc-session-properties-event.h"

typedef struct svc_session_properties_event_batch_request_s {
        /// Conversation ID of messages associated with the registered listener.
        CONVERSATION_ID_T conversation_id;
        /// The number of session property events in the batch
        uint32_t batch_size;
        /// A list of SVC_SESSION_PROPERTIES_EVENT_REQUEST_T structures.
        LIST_T *events;
} SVC_SESSION_PROPERTIES_EVENT_BATCH_REQUEST_T;

typedef struct svc_session_properties_event_batch_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_SESSION_PROPERTIES_EVENT_BATCH_RESPONSE_T;

typedef SESSION_PROPERTIES_EVENT_PARAMS_T SESSION_PROPERTIES_EVENT_BATCH_PARAMS_T;

DECLARE_SERVICE_PROTOTYPES(session_properties_event_batch, SVC_SESSION_PROPERTIES_EVENT_BATCH_REQUEST_T, SVC_SESSION_PROPERTIES_EVENT_BATCH_RESPONSE_T)

#endif
