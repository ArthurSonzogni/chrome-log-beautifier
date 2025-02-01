/*
 * @file service_macros.h
 *
 * Macros to be used when defining a service/feature for Diffusion v5.
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
#ifndef _diffusion_service_macros_h_
#define _diffusion_service_macros_h_ 1

#ifdef WIN32
#pragma warning(disable: 4100) // disable unreferenced formal parameter warning
#pragma warning(disable: 4054) // disable type cast from function pointer to data pointer warning
#pragma warning(disable: 4152) // disable nonstandard extension, function/data pointer conversion in expression warning

// NOTE: The data pointer to function pointer castings are undefined behaviour by the standard, however most compilers seem to have
// expected behaviour in there as some form of extension. If however we ever want to be standard conform we need to cast to function
// pointers instead to data pointers. Something to consider for future updates.
#endif

#define DECLARE_SERVICE_PROTOTYPES(SERVICE_NAME, REQUEST_TYPE, RESPONSE_TYPE) \
int svc_ ## SERVICE_NAME ## _request_handler(SESSION_T *session, const REQUEST_TYPE *request, RESPONSE_TYPE *response, const void *user_params, void *system_context); \
int svc_ ## SERVICE_NAME ## _response_handler(SESSION_T *session, const RESPONSE_TYPE *response, const void *user_params, void *system_context); \
int svc_ ## SERVICE_NAME ## _error_handler(SESSION_T *session, DIFFUSION_ERROR_T *error, const void *user_params, void *system_context); \
REQUEST_TYPE *svc_ ## SERVICE_NAME ## _request_unmarshal(SESSION_T *session, const BUF_T *buf); \
RESPONSE_TYPE *svc_ ## SERVICE_NAME ## _response_unmarshal(SESSION_T *session, const BUF_T *buf); \
BUF_T *svc_ ## SERVICE_NAME ## _request_marshal(SESSION_T *session, const REQUEST_TYPE *request); \
BUF_T *svc_ ## SERVICE_NAME ## _response_marshal(SESSION_T *session, const RESPONSE_TYPE *response); \
REQUEST_TYPE *svc_ ## SERVICE_NAME ## _request_create(SESSION_T *session); \
RESPONSE_TYPE *svc_ ## SERVICE_NAME ## _response_create(SESSION_T *session); \
void svc_ ## SERVICE_NAME ## _request_free(SESSION_T *session, REQUEST_TYPE *request); \
void svc_ ## SERVICE_NAME ## _response_free(SESSION_T *session, RESPONSE_TYPE *response); \
SVC_DEFN_T *svc_ ## SERVICE_NAME ## _create_defn(void);

#define DEFINE_SERVICE_DEFAULTS(SERVICE_NAME) \
SVC_DEFN_T * \
svc_ ## SERVICE_NAME ## _create_defn() {\
        SVC_DEFN_T *defn = calloc(1, sizeof(SVC_DEFN_T));\
        defn->request_handler = svc_ ## SERVICE_NAME ## _request_handler;\
        defn->response_handler = svc_ ## SERVICE_NAME ## _response_handler;\
        defn->error_handler = svc_ ## SERVICE_NAME ## _error_handler;\
        defn->request_unmarshal = (void *)svc_ ## SERVICE_NAME ## _request_unmarshal;\
        defn->request_marshal = (void *)svc_ ## SERVICE_NAME ## _request_marshal;\
        defn->response_unmarshal = (void *)svc_ ## SERVICE_NAME ## _response_unmarshal;\
        defn->response_marshal = (void *)svc_ ## SERVICE_NAME ## _response_marshal;\
        defn->request_create = (void *)svc_ ## SERVICE_NAME ## _request_create;\
        defn->response_create = (void *)svc_ ## SERVICE_NAME ## _response_create;\
        defn->request_free = svc_ ## SERVICE_NAME ## _request_free;\
        defn->response_free = svc_ ## SERVICE_NAME ## _response_free;\
        return defn;\
}

#define DEFAULT_SERVICE_ERROR_HANDLER(SERVICE_NAME, PARAMS_TYPE)        \
int                                                                     \
svc_ ## SERVICE_NAME ## _error_handler(SESSION_T *session,              \
                                       DIFFUSION_ERROR_T *error,        \
                                       const void *user_params,         \
                                       void *system_context)            \
{                                                                       \
        PARAMS_TYPE *params = (PARAMS_TYPE *)user_params;               \
        if(params != NULL && params->on_error != NULL) {                \
                error->context = params->context;                       \
                return params->on_error(session, error);                \
        }                                                               \
        else {                                                          \
                return HANDLER_SUCCESS;                                 \
        }                                                               \
}

#define DEFAULT_SERVICE_DISCARD_HANDLER(SERVICE_NAME, PARAMS_TYPE)      \
int                                                                     \
svc_ ## SERVICE_NAME ## _discard_handler(SESSION_T *session,            \
                                         void *user_params)             \
{                                                                       \
        PARAMS_TYPE *params = (PARAMS_TYPE *)user_params;               \
        if(params != NULL && params->on_discard != NULL) {              \
                return params->on_discard(session, params->context);    \
        }                                                               \
        else {                                                          \
                return HANDLER_SUCCESS;                                 \
        }                                                               \
}

// Macro to dynamically allocate memory and copy an existing structure.
// Does not perform a deep copy.
#define PARAMS_STRUCT_COPY(target, source, type) type *target = calloc(1, sizeof(type));\
        memmove(target, source, sizeof(type))

#endif
