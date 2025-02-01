#ifndef _diffusion_svc_topic_control_registration_h_
#define _diffusion_svc_topic_control_registration_h_ 1

/*
 * Copyright (c) 2015 Push Technology Ltd., All Rights Reserved.
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
 * @file svc-topic-control-registration.h
 *
 * Register in interest in TOPIC_CONTROL_REGISTRATION service messages.
 */

#include "internal/service_macros.h"
#include "session.h"

/**
 * @brief Structure for requesting control over providing topic state.
 */
typedef struct svc_topic_control_registration_request_s {
        /// Service ID (internal use only).
        uint32_t service_id;

        /// Control group.
        char *control_group;

        /// Topic path of registration request.
        char *topic_path;

        /// Conversation ID (internal use only).
        CONVERSATION_ID_T cid;
} SVC_TOPIC_CONTROL_REGISTRATION_REQUEST_T;

typedef struct svc_topic_control_registration_response_s {
        // Empty.
        void *_reserved;
} SVC_TOPIC_CONTROL_REGISTRATION_RESPONSE_T;

DECLARE_SERVICE_PROTOTYPES(topic_control_registration, SVC_TOPIC_CONTROL_REGISTRATION_REQUEST_T, SVC_TOPIC_CONTROL_REGISTRATION_RESPONSE_T)

#endif
