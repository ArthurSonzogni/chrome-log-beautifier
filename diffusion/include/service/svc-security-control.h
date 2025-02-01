#ifndef _svc_security_control_h_
#define _svc_security_control_h_ 1

/*
 * Copyright (c) 2015, 2016 Push Technology Ltd., All Rights Reserved.
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
 * @file svc-security-control.h
 *
 * Services for querying and updating the security store.
 */

#include "buf.h"
#include "hash.h"
#include "set.h"
#include "internal/service_int.h"
#include "utils.h"

/**
 * Permissions that protect globally scoped, access-controlled
 * operations.
 */
typedef enum {
        GLOBAL_PERMISSION_AUTHENTICATE = 0,
        GLOBAL_PERMISSION_VIEW_SESSION = 1,
        GLOBAL_PERMISSION_MODIFY_SESSION = 2,
        GLOBAL_PERMISSION_REGISTER_HANDLER = 3,
        GLOBAL_PERMISSION_VIEW_SERVER = 4,
        GLOBAL_PERMISSION_CONTROL_SERVER = 5,
        GLOBAL_PERMISSION_VIEW_SECURITY = 6,
        GLOBAL_PERMISSION_MODIFY_SECURITY = 7
} GLOBAL_PERMISSIONS_T;

extern const int SECURITY_GLOBAL_PERMISSIONS_TABLE[];
extern const char *SECURITY_GLOBAL_PERMISSIONS_NAMES[];

/**
 * Permissions protecting access-controlled operations that are
 * evaluated for a specific topic path or message path.
 */
typedef enum {
        TOPIC_PERMISSION_READ_TOPIC = 0,
        TOPIC_PERMISSION_UPDATE_TOPIC = 1,
        TOPIC_PERMISSION_MODIFY_TOPIC = 2,
        TOPIC_PERMISSION_SEND_TO_MESSAGE_HANDLER = 3,
        TOPIC_PERMISSION_SEND_TO_SESSION = 4,
        TOPIC_PERMISSION_SELECT_TOPIC = 5
} TOPIC_PERMISSIONS_T;

extern const int SECURITY_TOPIC_PERMISSIONS_TABLE[];
extern const char *SECURITY_TOPIC_PERMISSIONS_NAMES[];

typedef struct security_store_role_s {
        /// Name of the role.
        char *name;
        /// A set of roles included within the role.
        SET_T *included_roles;
        /// A set of global permissions assigned to the role.
        SET_T *global_permissions;
        /// A set of default topic permissions assigned to the role.
        SET_T *default_topic_permissions;
        /// A hash of topic permissions assigned to the role.
        HASH_T *topic_permissions;
} SECURITY_STORE_ROLE_T;

typedef struct security_store_s {
        /// The roles in the security store.
        HASH_T *roles;

        /// Default roles for anonymous sessions.
        SET_T *anon_roles_default;

        /// Default roles for named sessions.
        SET_T *named_roles_default;
} SECURITY_STORE_T;

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Create a new, empty security store structure.
 *
 * The returned structure is unsynchronized, so appropriate mutexes should be
 * use if the structure is to be used concurrently.
 *
 * @return A newly allocated security store.
 */
SECURITY_STORE_T *security_store_create(void);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Free all memory associated with a security store structure.
 *
 * @param store The security store to be freed.
 */
void security_store_free(SECURITY_STORE_T *store);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Copy an existing security store structure.
 *
 * The returned structure is unsynchronized, so appropriate mutexes should be
 * use if the structure is to be used concurrently.
 *
 * @param store The security store to be copied.
 *
 * @return A deep copy of a security store.
 */
SECURITY_STORE_T *security_store_dup(const SECURITY_STORE_T *store);

/**
 * @brief Structure describing the "get security store" request.
 */
typedef struct svc_get_security_store_request_s {
        /// @cond HIDDEN
        void *_reserved;
        /// @endcond
} SVC_GET_SECURITY_STORE_REQUEST_T;

/**
 * @brief Structure describing a "get security store" response.
 */
typedef struct svc_get_security_store_response_s {
        /// The security store.
        SECURITY_STORE_T *store;
} SVC_GET_SECURITY_STORE_RESPONSE_T;

/**
 * @brief Callback for get_security_store().
 *
 * @param session The current active session.
 * @param response The response containing the security store contents.
 * This structure will be freed on exit so a copy should be taken if it
 * needs to be used outside of the scope of the callback.
 * @param context User-supplied context from the get_security_store() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_get_security_store_cb)(SESSION_T *session,
                                        const SECURITY_STORE_T store,
                                        void *context);

#define GET_SECURITY_STORE_HANDLERS                     \
        /** Received security store */                  \
        on_get_security_store_cb on_get;                \
        /** Standard service error handler. */          \
        ERROR_HANDLER_T on_error;                       \
        /** Standard service discard handler. */        \
        DISCARD_HANDLER_T on_discard;

/**
 * @brief Structure passed when requesting the security store.
 */
typedef struct get_security_store_params_s {
        GET_SECURITY_STORE_HANDLERS
        /// User-supplied context returned to callbacks.
        void *context;
} GET_SECURITY_STORE_PARAMS_T;

/**
 * @brief Structure describing an "update security store" request.
 */
typedef struct svc_update_security_store_request_s {
        /// The script to be sent to the server.
        char *script_text;
} SVC_UPDATE_SECURITY_STORE_REQUEST_T;

/**
 * @brief Structure describing the response to an "update security
 *        store" request.
 */
typedef struct svc_update_security_store_response_s {
        /// A list of error reports, if applicable, or NULL if no
        /// errors.
        LIST_T *error_reports;
} SVC_UPDATE_SECURITY_STORE_RESPONSE_T;

/**
 * @brief Callback for update_security_store().
 *
 * @param session The current active session.
 * @param error_reports Any errors that have occurred while processing the updated.
 * @param context User-supplied context from the update_security_store() call.
 *
 * @return HANDLER_SUCCESS or HANDLER_FAILURE.
 */
typedef int (*on_update_security_store_cb)(SESSION_T *session,
                                          const LIST_T *error_reports,
                                          void *context);

#define UPDATE_SECURITY_STORE_HANDLERS                  \
        /** Update received by Diffusion */             \
        on_update_security_store_cb on_update;          \
        /** Standard service error handler. */          \
        ERROR_HANDLER_T on_error;                       \
        /** Standard service discard handler. */        \
        DISCARD_HANDLER_T on_discard;

/**
 * @brief Structure passed when updating the security store.
 */
typedef struct update_security_store_params_s {
        UPDATE_SECURITY_STORE_HANDLERS
        /// Update script to apply to the security store.
        SCRIPT_T *update_script;
        /// User-supplied context returned to callbacks.
        void *context;
} UPDATE_SECURITY_STORE_PARAMS_T;

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get the server's security store.
 *
 * @param session       The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure containing callbacks to receive the
 *                      contents of the security store.
 */
void get_security_store(SESSION_T *session,
                        const GET_SECURITY_STORE_PARAMS_T params);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get the default roles for anonymous connections.
 *
 * @param store         A security store.
 * @return A NULL-terminated array of strings.
 */
char **get_security_default_anonymous_roles(const SECURITY_STORE_T store);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get the default roles for connections made with a principal.
 *
 * @param store         A security store.
 * @return A NULL-terminated array of strings.
 */
char **get_security_default_named_roles(const SECURITY_STORE_T store);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get the names of all roles defined in the security store.
 *
 * @param store         A security store.
 * @return A NULL-terminated array of strings.
 */
char **get_security_role_names(const SECURITY_STORE_T store);


/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get the names of other roles that this role includes.
 *
 * @param store         A security store.
 * @param role_name     The name of the role in the store.
 * @return A NULL-terminated array of strings.
 */
char **get_security_included_roles(const SECURITY_STORE_T store,
                                   const char *role_name);


/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get an array of global permissions assigned to a role.
 *
 * @param store         A security store.
 * @param role_name     The name of the role in the store.
 *
 * @return A NULL-terminated array of \ref GLOBAL_PERMISSIONS_T. The caller
 *         is responsible for freeing the returned pointer, but the
 *         array elements are shared and should not be freed.
 */
GLOBAL_PERMISSIONS_T **get_security_global_permissions(const SECURITY_STORE_T store,
                                                       const char *role_name);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get an array of default topic permissions assigned to a role.
 *
 * @param store         A security store.
 * @param role_name     The name of the role in the store.
 *
 * @return A NULL-terminated array of \ref TOPIC_PERMISSIONS_T. The caller
 *         is responsible for freeing the returned pointer, but the
 *         array elements are shared and should not be freed.
 */
TOPIC_PERMISSIONS_T **get_security_default_topic_permissions(const SECURITY_STORE_T store,
                                                             const char *role_name);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Get an array of topic permissions assigned to a role, for a specified topic.
 *
 * @param store         A security store.
 * @param role_name     The name of the role in the store.
 * @param topic_name    The name of the topic to look up.
 *
 * @return A NULL-terminated array of \ref TOPIC_PERMISSIONS_T or NULL if
 *         the lookup fails (e.g. role_name not in the store, or there
 *         are no specific permissions for the role_name/topic_name
 *         pair). The caller is responsible for freeing the returned
 *         pointer, but the array elements are shared and should not
 *         be freed.
 */
TOPIC_PERMISSIONS_T **get_security_topic_permissions(const SECURITY_STORE_T store,
                                                     const char *role_name,
                                                     const char *topic_name);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Send updates to the security store.
 *
 * @param session	The current session. If NULL, this function returns immediately.
 * @param params        Parameter structure describing the updates
 *                      to perform and callbacks to handle success
 *                      or failure notifications.
 */
void update_security_store(SESSION_T *session,
                           const UPDATE_SECURITY_STORE_PARAMS_T params);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to assign global permissions to a security
 *        role.
 *
 * @param script        The script to update.
 * @param role_name     The role which will receive the new permissions.
 * @param permissions   A set of \ref GLOBAL_PERMISSIONS_T specifying the
 *                      permissions to assign.
 *
 * @return The updated script or NULL on error. (It is also modified in-place).
 */
SCRIPT_T *update_security_store_global_role_permissions(SCRIPT_T *script,
                                                        const char *role_name,
                                                        const SET_T *permissions);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to assign default topic permissions to a
 *        security role.
 *
 * The collection of permissions to be assigned should be passed in a
 * set created with set_new_int().
 *
 * @param script        The script to update.
 * @param role_name     The role which will receive the new permissions.
 * @param permissions   A set of \ref TOPIC_PERMISSIONS_T specifying the
 *                      permissions to assign.
 * @return The updated script or NULL on error (It is also modified in-place.)
 */
SCRIPT_T *update_security_store_default_topic_permissions(SCRIPT_T *script,
                                                          const char *role_name,
                                                          const SET_T *permissions);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to assign permissions for named role and
 *        topic.
 *
 * @param script        The script to update.
 * @param role_name     The role which will receive the new permissions.
 * @param topic_path    The path to the topic for which the permissions
 *                      will apply.
 * @param permissions   A set of \ref TOPIC_PERMISSIONS_T specifying the
 *                      permissions to assign.
 * @return The updated script or NULL on error. (It is also modified in-place.)
 */
SCRIPT_T *update_security_store_topic_permissions(SCRIPT_T *script,
                                                  const char *role_name,
                                                  const char *topic_path,
                                                  const SET_T *permissions);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to remove permissions for a named role
 *        and topic.
 *
 * @param script        The script to update.
 * @param role_name     The role for which permissions will be removed.
 * @param topic_path    The path for which permissions will be removed.
 * @return The updated script or NULL on error. (It is also modified in-place.)
 */
SCRIPT_T *update_security_store_remove_topic_permissions(SCRIPT_T *script,
                                                         const char *role_name,
                                                         const char *topic_path);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to include other roles within a role.
 *
 * @param script        The script to update.
 * @param role_name     The role name which will include other roles.
 * @param included_roles A list of strings representing the roles to
 *                       be included in the named role.
 * @return The updated script or NULL on error. (It is also modified in-place.)
 */
SCRIPT_T *update_security_store_include_roles(SCRIPT_T *script,
                                              const char *role_name,
                                              const LIST_T *included_roles);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to set the roles assigned to named
 *        sessions.
 *
 * @param script        The script to update.
 * @param roles         A list of strings representing the roles which
 *                      are assigned to authenticated (named) sessions.
 * @return The updated script or NULL on error. (It is also modified in-place.)
 */
SCRIPT_T *update_security_store_named_session_roles(SCRIPT_T *script,
                                                    const LIST_T *roles);

/**
 * @ingroup PublicAPI_SecurityControl
 *
 * @brief Updates a script to set the roles assigned to anonymous
 *        sessions.
 *
 * @param script        The script to update.
 * @param roles         A list of strings representing the roles which
 *                      are assigned to anonymous sessions.
 * @return The updated script or NULL on error. (It is also modified in-place.)
 */
SCRIPT_T *update_security_store_anonymous_session_roles(SCRIPT_T *script,
                                                        const LIST_T *roles);

DECLARE_SERVICE_PROTOTYPES(get_security_store,
                           SVC_GET_SECURITY_STORE_REQUEST_T,
                           SVC_GET_SECURITY_STORE_RESPONSE_T);

DECLARE_SERVICE_PROTOTYPES(update_security_store,
                           SVC_UPDATE_SECURITY_STORE_REQUEST_T,
                           SVC_UPDATE_SECURITY_STORE_RESPONSE_T);

#endif
