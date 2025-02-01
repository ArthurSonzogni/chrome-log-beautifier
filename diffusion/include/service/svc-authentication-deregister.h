#ifndef _svc_authentication_deregister_h_
#define _svc_authentication_deregister_h_ 1

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
 * @file svc-authentication-deregister.h
 *
 * Functions required for deregistering an authentication service in a control
 * client.
 */

#include "internal/service_macros.h"
#include "session.h"
#include "security.h"
#include "service/svc-authentication-register.h"

/**
 * @brief The request to deregister an authentication handler. The
 * same as the registration request.
 */
typedef SVC_AUTHENTICATION_REGISTER_REQUEST_T SVC_AUTHENTICATION_DEREGISTER_REQUEST_T;

/**
 * @brief Structure holding the deregistration response message.
 */
typedef struct svc_authentication_deregister_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_AUTHENTICATION_DEREGISTER_RESPONSE_T;

/**
 * @brief Callback for on_deregistration().
 *
 * @param session The currently active session.
 * @param context User-supplied context from the initial authentication_deregister() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_authentication_deregistration_cb)(SESSION_T *session, void *context);

/**
 * @brief Callbacks invoked in response to a deregistration request.
 */
#define AUTHENTICATION_DEREGISTRATION_HANDLERS                          \
        /** Callback when an authorisation handler deregistration response has been received. */ \
        on_authentication_deregistration_cb on_deregistration;          \
        /** Standard service error handler callback */                  \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard conversation discard callback */                   \
        DISCARD_HANDLER_T on_discard;

typedef struct authentication_deregistration_handlers_s {
        AUTHENTICATION_DEREGISTRATION_HANDLERS
} AUTHENTICATION_DEREGISTRATION_HANDLERS_T;

/**
 * @brief Structure supplied when issuing an authentication_deregister() request.
 */
typedef struct authentication_deregistration_params_s {
        AUTHENTICATION_DEREGISTRATION_HANDLERS
        /// The original request as returned by svc_authentication_register().
        const SVC_AUTHENTICATION_REGISTER_REQUEST_T *original_request;
        /// User-supplied context to return to callbacks.
        void *context;
} AUTHENTICATION_DEREGISTRATION_PARAMS_T;

/**
 * @ingroup PublicAPI_Authentication
 *
 * @brief Deregister an authentication handler
 *
 * @param session       The session handle. If NULL, this function returns immediately.
 * @param params        Parameters defining the request to deregister an
 *                      existing authentication handler.
 */
void authentication_deregister(SESSION_T *session,
                               const AUTHENTICATION_DEREGISTRATION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(authentication_deregister, SVC_AUTHENTICATION_DEREGISTER_REQUEST_T, SVC_AUTHENTICATION_DEREGISTER_RESPONSE_T)

#endif

/*  LocalWords:  svc deregistering PublicAPI params
 */
