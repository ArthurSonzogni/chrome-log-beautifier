#ifndef _diffusion_svc_send_msg_to_session_h_
#define _diffusion_svc_send_msg_to_session_h_ 1

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
 * @file svc-send-msg-to-session.h
 *
 * Functions for sending messages to client sessions via topic
 * endpoints.
 */

#include "internal/service_int.h"
#include "types/content_types.h"
#include "types/session_types.h"
#include "types/messaging_types.h"

typedef struct svc_send_to_session_request_s {
        char *topic_path;
        CONTENT_T *content;
        SEND_OPTIONS_T *options;
        SESSION_ID_T *session_id;
} SVC_SEND_MSG_TO_SESSION_REQUEST_T;

typedef struct svc_send_to_session_response_s {
        // @cond HIDDEN
        void *_reserved;
        // @endcond
} SVC_SEND_MSG_TO_SESSION_RESPONSE_T;

typedef int (*on_send_msg_to_session_cb)(SESSION_T *session,
                                         void *context);

#define SEND_MSG_TO_SESSION_HANDLERS       \
        on_send_msg_to_session_cb on_send; \
        ERROR_HANDLER_T on_error;          \
        DISCARD_HANDLER_T on_discard;

typedef struct send_msg_to_session_params_s {
        SEND_MSG_TO_SESSION_HANDLERS
        const char *topic_path;
        const CONTENT_T content;
        const SEND_OPTIONS_T options;
        const SESSION_ID_T session_id;
        void *context;
} SEND_MSG_TO_SESSION_PARAMS_T;

void send_msg_to_session(SESSION_T *session, SEND_MSG_TO_SESSION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(send_msg_to_session, SVC_SEND_MSG_TO_SESSION_REQUEST_T, SVC_SEND_MSG_TO_SESSION_RESPONSE_T)

#endif
