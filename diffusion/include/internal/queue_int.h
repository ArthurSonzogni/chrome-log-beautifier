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
 * Basic Diffusion message queue, with mutex guard to prevent corruption that
 * may occur with multiple threads accessing it, and priority queueing levels.
 */

#ifndef _diffusion_queue_int_h_
#define _diffusion_queue_int_h_ 1

#include <apr_pools.h>
#include <apr_queue.h>
#include <apr_time.h>
#include <apr_thread_mutex.h>
#include <apr_thread_cond.h>

#include "buf.h"

typedef enum {
        MESSAGE_QUEUE_EVENT_TYPE_EMPTY = 1,
        MESSAGE_QUEUE_EVENT_TYPE_FULL = 2
} MESSAGE_QUEUE_EVENT_TYPE_T;

typedef enum {
        MESSAGE_QUEUE_ENTRY_TYPE_MESSAGE = 1,
        MESSAGE_QUEUE_ENTRY_TYPE_COMMAND = 2,
        MESSAGE_QUEUE_ENTRY_TYPE_RAW = 3
} MESSAGE_QUEUE_ENTRY_TYPE_T;

typedef struct message_queue_entry_s {
        MESSAGE_QUEUE_ENTRY_TYPE_T type;
        int requeue;
        void *data;
} MESSAGE_QUEUE_ENTRY_T;

typedef struct message_queue_s {
        apr_pool_t *pool;

        // Queue access lock
        apr_thread_mutex_t *mutex_lock;
        apr_thread_cond_t *cond_lock;

        // Multiple queues wrapped in this message queue, one for each
        // possible message priority.
        apr_queue_t *priority_queue[MESSAGE_PRIORITY_LOW + 1];

        // Maximum number of entries in this queue (-1 = unbounded)
        int capacity;

        // The number of entries currently in the queue.
        apr_uint32_t size;

        // Flag specifying if the queue recently became full.
        apr_uint32_t is_full;

        // Microseconds to block waiting for space in a full queue.
        // Default is 5 seconds.
        apr_interval_time_t queue_full_block_timeout;

        // If add_closed is set (not zero), then queue_add() will fail
        // without adding the entry to the queue.
        int add_closed;
} MESSAGE_QUEUE_T;

MESSAGE_QUEUE_T *queue_create(const int capacity);
int queue_add(MESSAGE_QUEUE_T *queue, const MESSAGE_QUEUE_ENTRY_T *message, const MESSAGE_PRIORITY_T priority);
MESSAGE_QUEUE_ENTRY_T *queue_get(MESSAGE_QUEUE_T *queue, apr_status_t *status);
MESSAGE_QUEUE_ENTRY_T *queue_get_with_timeout(MESSAGE_QUEUE_T *queue, apr_interval_time_t timeout, apr_status_t *status);
void queue_free(MESSAGE_QUEUE_T *queue);
void queue_entry_free(MESSAGE_QUEUE_ENTRY_T *entry);
void queue_add_close(MESSAGE_QUEUE_T *queue);
void queue_add_open(MESSAGE_QUEUE_T *queue);
int queue_size(MESSAGE_QUEUE_T *queue);
int queue_set_capacity(MESSAGE_QUEUE_T *queue, int capacity);

#endif
