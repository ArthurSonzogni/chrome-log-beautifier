#ifndef _diffusion_svc_send_msg_to_filter_h_
#define _diffusion_svc_send_msg_to_filter_h_ 1

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
 * @file svc-send-msg-to-filter.h
 *
 * Functions for sending messages to client sessions via topic
 * endpoints, using a filter to select the clients.
 */

#include "internal/service_int.h"
#include "types/content_types.h"
#include "types/session_types.h"
#include "types/messaging_types.h"

typedef struct svc_send_to_filter_request_s {
        char *topic_path;
        CONTENT_T *content;
        SEND_OPTIONS_T *options;
        char *filter;
} SVC_SEND_MSG_TO_FILTER_REQUEST_T;

typedef struct svc_send_to_filter_response_s {
        uint32_t sent_count;
        LIST_T *error_reports;
} SVC_SEND_MSG_TO_FILTER_RESPONSE_T;

typedef int (*on_send_msg_to_filter_cb)(SESSION_T *session,
                                        const SVC_SEND_MSG_TO_FILTER_RESPONSE_T *response,
                                        void *context);

#define SEND_MSG_TO_FILTER_HANDLERS       \
        on_send_msg_to_filter_cb on_send; \
        ERROR_HANDLER_T on_error;         \
        DISCARD_HANDLER_T on_discard;

typedef struct send_msg_to_filter_params_s {
        SEND_MSG_TO_FILTER_HANDLERS
        const char *topic_path;
        const CONTENT_T content;
        const SEND_OPTIONS_T options;
        const char *filter;
        void *context;
} SEND_MSG_TO_FILTER_PARAMS_T;

void send_msg_to_filter(SESSION_T *session, SEND_MSG_TO_FILTER_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(send_msg_to_filter, SVC_SEND_MSG_TO_FILTER_REQUEST_T, SVC_SEND_MSG_TO_FILTER_RESPONSE_T)

#endif
