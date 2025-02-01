#ifndef _diffusion_change_principal_h_
#define _diffusion_change_principal_h_ 1

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
 * @file svc-change-principal.h
 *
 * Functions that allow a client to change its principal (usually, username)
 * which is using to identify itself to Diffusion.
 */

/**
 * @brief Structure used internally to represent the request that will
 * be sent to Diffusion.
 */
typedef struct svc_change_principal_request_s {
        /// Principal for the change request.
        char *principal;
        /// Associated credentials for the change request.
        CREDENTIALS_T *credentials;
} SVC_CHANGE_PRINCIPAL_REQUEST_T;

/**
 * @brief Structure used internally to notify the client that the
 * request has been processed.
 */
typedef struct svc_change_principal_response_s {
        /// 1 for success, 0 for failure.
        int result;
} SVC_CHANGE_PRINCIPAL_RESPONSE_T;

/**
 * @brief Callback indicating that a response has been received to a
 * change principal request.
 *
 * @param session The current active session.
 * @param context User-supplied context from the original change_principal() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_change_principal_cb)(SESSION_T *session, void *context);

/**
 * @brief Structure holding callbacks to be invoked after a request
 * to change the principal has been sent to and processed by
 * Diffusion.
 */
#define CHANGE_PRINCIPAL_HANDLERS                                       \
        /** Called when the change principal request is successful. */  \
        on_change_principal_cb on_change_principal;                     \
        /** Called when the change principal request results in failure. */ \
        on_change_principal_cb on_change_principal_failure;             \
        /** Standard service error handler callback. */                 \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard service discard handler callback. */               \
        DISCARD_HANDLER_T on_discard;

typedef struct change_principal_handlers_s {
        CHANGE_PRINCIPAL_HANDLERS
} CHANGE_PRINCIPAL_HANDLERS_T;

/**
 * @brief Structure supplied when issuing a change_principal() request.
 */
typedef struct change_principal_params_s {
        CHANGE_PRINCIPAL_HANDLERS
        /// The new principal (username).
        const char *principal;
        /// The credentials associated with the principal.
        const CREDENTIALS_T *credentials;
        /// User-supplied context to return to callbacks.
        void *context;
} CHANGE_PRINCIPAL_PARAMS_T;

/**
 * @ingroup PublicAPI_Security
 *
 * @brief Issue a request to change the principal for the current (active)
 * session.
 *
 * @param session       The session handle. If NULL, this function has no effect.
 * @param params        Parameters defining the change_prinicpal() request
 *                      and notification callbacks.
 */
void change_principal(SESSION_T *session, const CHANGE_PRINCIPAL_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(change_principal, SVC_CHANGE_PRINCIPAL_REQUEST_T, SVC_CHANGE_PRINCIPAL_RESPONSE_T)

#endif
