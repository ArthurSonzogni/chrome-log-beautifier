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

#ifndef _diffusion_transport_dpt_int_h_
#define _diffusion_transport_dpt_int_h_ 1

#include "transport_int.h"

typedef enum {
        DPT_MONITOR_RUNNING = 0,
        DPT_MONITOR_CLOSE_REQUESTED = 1, // Close requested
        DPT_MONITOR_CLOSING = 2,
        DPT_MONITOR_CLOSED = 3
} DPT_MONITOR_STATE;

typedef struct dpt_transport_impl_s {
        int protocol_byte;
        int protocol_version;
        int response_code;
        unsigned int msg_length_size;

        struct {
                volatile DPT_MONITOR_STATE state;
                apr_pool_t *pool;
                apr_thread_t *monitor_thread;
                apr_thread_t *reader_thread;
                apr_thread_t *writer_thread;
        } _monitor;

        apr_pool_t *_connection_pool;
        apr_pool_t *_monitor_pool;
} DPT_TRANSPORT_IMPL_T;

int dpt_transport_create(TRANSPORT_T *transport);

#endif
