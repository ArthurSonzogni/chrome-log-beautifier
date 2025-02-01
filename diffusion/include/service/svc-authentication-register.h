#ifndef _svc_authentication_register_h_
#define _svc_authentication_register_h_ 1

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
 * @file svc-authentication-register.h
 *
 * Functions required for registering an authentication service in a control
 * client.
 */

#include "internal/service_macros.h"
#include "svc-authentication.h"
#include "session.h"
#include "security.h"

/**
 * If not specified, the group name "default" is used when registering
 * authentication handlers.
 */
#define DEFAULT_AUTH_GROUP_NAME "default"

/**
 * @brief Degree of session detail required.
 *
 * When registering an authentication handler, you can request various levels
 * of information about connecting clients to enable you to make a decision
 * about the validity of the client. This enumeration lists the available
 * details that can be requested.
 */
typedef enum {
        /// Request summary information only.
        SESSION_DETAIL_SUMMARY = 0,
        /// Also request GeoIP information.
        SESSION_DETAIL_LOCATION = 1,
        /// Request information about the connector which the client
        /// has used while connecting to Diffusion.
        SESSION_DETAIL_CONNECTOR_NAME = 2
} SESSION_DETAIL_TYPE_T;

/**
 * @brief Structure containing the information required when sending a
 * request to register as an authentication service.
 */
typedef struct svc_authentication_register_request_s {
        /// Service ID, at the moment this is always SVC_AUTHENTICATION
        uint32_t service_id;

        /// The control group name to register with; "default" if not
        /// specified.
        char *control_group;

        /// A name which this handler is known by, and which matches a
        /// name defined in Server.xml.
        char *handler_name;

        /// A hashmap of the types of information requested when the
        /// authorisation handler is called.
        HASH_T *session_detail_type_set;

        /// A conversation ID to correlate requests and responses
        /// associated to this request.
        CONVERSATION_ID_T *conversation_id;
} SVC_AUTHENTICATION_REGISTER_REQUEST_T;

/**
 * @brief A response to an authentication registration request is considered successful
 * by virtue of being received; there is no service-specific data returned.
 */
typedef struct svc_authentication_register_response_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_AUTHENTICATION_REGISTER_RESPONSE_T;

/**
 * @brief Callback for on_registration().
 *
 * @param session The currently active session.
 * @param context User-supplied context from the initial authentication_register() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_authentication_registration_cb)(SESSION_T *session,
                                                 void *context);

/**
 * @brief Handlers for notification of registration of an
 * authentication handler.
 */
#define AUTHENTICATION_REGISTRATION_HANDLERS                            \
        /** Callback when Diffusion acknowledges the request for the service to be registered. */ \
        on_authentication_registration_cb on_registration;              \
        /**  Standard service error handler callback. */                \
        ERROR_HANDLER_T on_error;                                       \
        /** Standard conversation discard callback */                   \
        DISCARD_HANDLER_T on_discard;

typedef struct authentication_registration_handlers_s {
        AUTHENTICATION_REGISTRATION_HANDLERS
} AUTHENTICATION_REGISTRATION_HANDLERS_T;

/**
 * @brief Structure supplied when issuing an authentication_register() request.
 */
typedef struct authentication_registration_params_s {
        AUTHENTICATION_REGISTRATION_HANDLERS
        struct {
                AUTHENTICATION_HANDLERS
        } authentication_handlers;
        /// Name of the authentication handler, as specified in
        /// Diffusion's Server.xml configuration file.
        const char *name;
        /// A set of detail types to be request on a registration
        /// request. The hash should contain keys which map to
        /// SESSION_DETAILS_TYPE_T values, converted to
        /// NULL-terminated strings.
        const HASH_T *detail_set;
        /// User-supplied context to return to callbacks.
        void *context;
} AUTHENTICATION_REGISTRATION_PARAMS_T;

/**
 * @ingroup PublicAPI_Authentication
 *
 * @brief Register an authentication handler.
 *
 * @param session       The session handle. If NULL, this function returns immediately
 *                      with the value NULL.
 * @param params        Parameters defining the request to register an
 *                      authentication handler.
 *
 * @return              The request structure which is required for later
 *                      deregistering the authorisation handler, or NULL if the session
 *                      handle was NULL.
 */
SVC_AUTHENTICATION_REGISTER_REQUEST_T *authentication_register(SESSION_T *session,
                                                               const AUTHENTICATION_REGISTRATION_PARAMS_T params);

DECLARE_SERVICE_PROTOTYPES(authentication_register, SVC_AUTHENTICATION_REGISTER_REQUEST_T, SVC_AUTHENTICATION_REGISTER_RESPONSE_T)

#endif
