/**
 * @file d_semaphore.h
 *
 * Wrappers to provide simple semaphores based on APR mutexes and condition variables.
 *
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


#ifndef _diffusion_semaphore_h_
#define _diffusion_semaphore_h_ 1

#include <stdlib.h>

#include <apr.h>
#include <apr_thread_mutex.h>
#include <apr_thread_cond.h>

struct _semaphore_internal {
        volatile int pool_alloc;
};

typedef struct semaphore_s {
        apr_pool_t *pool;
        apr_thread_mutex_t *mutex;
        apr_thread_cond_t *cond;
        apr_uint32_t count;
        void *_internal;
        void *user_data;
} SEMAPHORE_T;

SEMAPHORE_T *semaphore_create(apr_pool_t *pool);

SEMAPHORE_T *semaphore_create_reentrant(apr_pool_t *pool);

void semaphore_free(SEMAPHORE_T *sem);

int semaphore_wait(SEMAPHORE_T *sem, apr_interval_time_t timeout);

void semaphore_lock(SEMAPHORE_T *sem);

void semaphore_unlock(SEMAPHORE_T *sem);

void semaphore_broadcast(SEMAPHORE_T *sem);

void semaphore_set_data(SEMAPHORE_T *sem, void *data);

void *semaphore_get_data(SEMAPHORE_T *sem);

apr_uint32_t semaphore_get_count(SEMAPHORE_T *sem);

apr_uint32_t semaphore_set_count(SEMAPHORE_T *sem, apr_uint32_t count);

#endif
