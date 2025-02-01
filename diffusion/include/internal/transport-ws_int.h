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

#ifndef _diffusion_transport_ws_int_h_
#define _diffusion_transport_ws_int_h_ 1

#ifdef WIN32
#pragma warning( disable: 4005 )
#endif

#include "libwebsockets.h"
#include "transport_int.h"
#include "d_semaphore.h"

#define WS_MSG_LENGTH_SIZE 4

typedef struct ws_semaphore_data_s {
        volatile uint8_t connected;
        volatile uint8_t processed_handshake;
        volatile uint8_t connection_error;
} WS_SEMAPHORE_DATA_T;

typedef struct ws_transport_impl_s {
        struct lws_context *context;
        struct lws_context_creation_info creation_info;
        struct lws_client_connect_info connection_info;
        struct lws *wsi;
        SEMAPHORE_T *connection_semaphore;
        SESSION_T *session;
        volatile uint8_t close_requested;
        volatile uint8_t terminate_connection;
        uint8_t allow_self_signed_cert;
        apr_pool_t *_connection_pool;
} WS_TRANSPORT_IMPL_T;

int ws_transport_create(TRANSPORT_T *transport);

#endif
