#ifndef _diffusion_svc_authentication_h_
#define _diffusion_svc_authentication_h_ 1

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
 * @file svc-authentication.h
 *
 * Functions required for handling authentication requests in a control client.
 */

#include "internal/service_macros.h"
#include "session.h"
#include "types/transport_types.h"
#include "types/client_details_types.h"


/**
 * @brief This structure is populated in the authentication request
 * when the detail type SESSION_DETAIL_CONNECTOR_NAME has been
 * requested.
 */
typedef struct connector_details_s {
        /// The name of the connector to which the client has connected.
        char *name;
} CONNECTOR_DETAILS_T;

/**
 * @brief The action to take for the authentication request.
 *
 * On receipt of an authentication request, the service may either
 * deny the connecting client, allow it or abstain from answering
 * which passes the request to the next handler in the chain.
 */
typedef enum {
        /// Authentication failed.
        AUTHENTICATION_DENY = 0,
        /// Authentication has neither passed nor failed.
        AUTHENTICATION_ABSTAIN = 1,
        /// Authentication passed, no result supplied.
        AUTHENTICATION_ALLOW = 2,
        /// Authentication passed, result supplied.
        AUTHENTICATION_ALLOW_WITH_RESULT = 3
} SVC_AUTHENTICATION_RESPONSE_VALUE_T;

/**
 * @brief The structure passed to an authentication handler.
 */
typedef struct svc_authentication_request_s {
        /// The principal of the connecting client.
        char *principal;

        /// Credentials of the connecting client.
        CREDENTIALS_T *credentials;

        /// If requested by the authentication handler, these are the
        /// session details of the connecting client.
        SESSION_DETAILS_SUMMARY_T *session_details;

        /// If requested by the authentication handler, these are the
        /// GeoIP details of the connecting client.
        LOCATION_DETAILS_T *location_details;

        /// If requested by the authentication handler, these are the
        /// details of the Diffusion connector through which the
        /// client is connecting.
        CONNECTOR_DETAILS_T *connector_details;

        /// The conversation ID to be used in the response so
        /// Diffusion can correlate it to the originating request.
        CONVERSATION_ID_T conversation_id;
} SVC_AUTHENTICATION_REQUEST_T;

/**
 * @brief Structure holding the response message from the
 * authentication handler.
 */
typedef struct svc_authentication_response_s {
        /// Indicates whether to deny, abstain or allow the request.
        SVC_AUTHENTICATION_RESPONSE_VALUE_T value;
        /// List of additional roles which the principal has (when
        /// value is AUTHENTICATION_ALLOW_WITH_RESULT).
        LIST_T *roles;
        /// Hashmap of additional properties (when value is
        /// AUTHENTICATION_ALLOW_WITH_RESULT).
        HASH_T *properties;
} SVC_AUTHENTICATION_RESPONSE_T;

/**
 * @brief Callback for on_authentication().
 *
 * @param session The current active session.
 * @param request The incoming authentication request.
 * @param response The response to be returned from the handler.
 * @param context User-supplied context from initial authentication handler registration.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_authentication_cb)(SESSION_T *session,
                                    const SVC_AUTHENTICATION_REQUEST_T *request,
                                    SVC_AUTHENTICATION_RESPONSE_T *response,
                                    void *context);

/**
 * @brief Callbacks that can be supplied by an authentication service.
 */
#define AUTHENTICATION_HANDLERS                         \
        /** Authentication request callback. */         \
        on_authentication_cb on_authentication;         \
        /** Standard service error handler callback. */ \
        ERROR_HANDLER_T on_error;

typedef struct authentication_handlers_s {
        AUTHENTICATION_HANDLERS
} AUTHENTICATION_HANDLERS_T;

/**
 * @brief Parameters supplied when registering this authentication handler which are required
 * for the authentication process but not related to the registration itself.
 */
typedef struct authentication_params_s {
        AUTHENTICATION_HANDLERS
        /// User-supplied context to return to callbacks.
        void *context;
} AUTHENTICATION_PARAMS_T;

DECLARE_SERVICE_PROTOTYPES(authentication, SVC_AUTHENTICATION_REQUEST_T, SVC_AUTHENTICATION_RESPONSE_T)

#endif
