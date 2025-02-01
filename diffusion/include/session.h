#ifndef _diffusion_session_h_
#define _diffusion_session_h_ 1

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
 * @file session.h
 *
 * Functions relating to a session and session management.
 */

#include <apr_pools.h>
#include <apr_atomic.h>

#include "hash.h"
#include "reconnect.h"
#include "internal/queue_int.h"

#include "types/error_types.h"
#include "types/security_types.h"
#include "types/topic_types.h"
#include "types/session_types.h"

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Create a new session and synchronously open a connection to the
 * Diffusion server at the endpoint specified by the URL.
 *
 * @deprecated The DPT (dpt:// and dpts://) transport is deprecated and will
 * be removed in a future release. Use the websockets (ws:// and wss://)
 * protocol instead.
 *
 * @note This call retains a reference to the memory allocated for the {@link
 * CREDENTIALS_T credentials} and it is the callers' responsibility to ensure
 * that this memory is not freed or overwritten for the lifetime of the
 * session structure.
 *
 * @note The error structure contains the result of the session creation
 * attempt and will not be overwritten if the session is subsequently
 * reconnected or fails over. Once session_create() returns, it is safe to
 * release any memory owned by the error structure.
 *
 * @param url_str		A URL describing the endpoint to connect to.
 * @param principal		Typically, the username of the connecting user or application.
 *				NULL indicates that the principal will not be passed on connection.
 * @param credentials		The credentials associated with the principal, or NULL.
 *                              \em MUST be available for the duration of the session.
 * @param listener		A pointer to a {@link SESSION_LISTENER_T} structure containing callbacks
 *				to be called on session-specific events.
 * @param reconnection_strategy How to handle reconnection situations when the server is initially
 *				unavailable. If NULL, a default strategy is provided that
 *				attempts to connect every 5 seconds for up to 1 minute.
 * @param error			A pointer to an error structure, initialized to zero, which is populated in 
 *				case of error, or NULL if not required.
 * @retval "SESSION_T *"	A pointer to a session handle.
 * @retval NULL			If the session could not be created.
 */
SESSION_T *session_create(const char *url_str,
                          const char *principal,
                          CREDENTIALS_T *credentials,
                          SESSION_LISTENER_T *listener,
                          RECONNECTION_STRATEGY_T *reconnection_strategy,
                          DIFFUSION_ERROR_T *error);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Create a new session, but do not wait for the connection handshake
 * to be performed.
 *
 * @deprecated The DPT (dpt:// and dpts://) transport is deprecated and will
 * be removed in a future release. Use the websockets (ws:// and wss://)
 * protocol instead.
 *
 * @deprecated The error parameter is deprecated and will be removed
 * in a future release.
 *
 * @note This call retains a reference to the memory allocated for the {@link
 * CREDENTIALS_T credentials}, and it is the callers' responsibility to ensure
 * that this memory is not freed or overwritten for the lifetime of the
 * session structure.
 *
 * @note The error structure contains the result of the session creation
 * attempt and will not be overwritten if the session is subsequently
 * reconnected or fails over. Since session_create_async() returns
 * immediately, you must ensure the memory that this parameter points to is
 * available until the session has been created, or the creation has
 * failed. It is highly recommended that the error callback (see the {@link
 * SESSION_CREATE_CALLBACK_T callbacks} parameter) is used instead, and NULL
 * passed here.
 *
 * @param url_str		A URL describing the endpoint to connect to.
 * @param principal		Typically, the username of the connecting user or application.
 *				NULL indicates that the principal will not be passed on connection.
 * @param credentials		The credentials associated with the principal, or NULL.
 *                              \em MUST be available for the duration of the session.
 * @param listener		A pointer to a \ref SESSION_LISTENER_T structure containing callbacks
 *				to be called on session-specific events.
 * @param reconnection_strategy How to handle reconnection situations when the server is initially
 *				unavailable. If NULL, a default strategy is provided that
 *				attempts to connect every 5 seconds for up to 1 minute.
 * @param callbacks		A pointer to a structure containing callbacks for successful
 *				connection or error reporting.
 * @param error			A pointer to an error structure to be asynchronously populated if
 *                              an error occurs while the session is established, or NULL if not required.
 *                              \em MUST be available for the duration of the session.
 * @retval "SESSION_T *" A pointer to a session handle that should be closed
 *                              and freed on exit.  The session is not open
 *                              when returned and you should use the {@link
 *                              SESSION_CREATE_CALLBACK_T on_connected}
 *                              callback to obtain a valid session. This
 *                              handle is merely provided for proper cleanup.
 */
SESSION_T *
session_create_async(const char *url_str,
                     const char *principal,
                     CREDENTIALS_T *credentials,
                     SESSION_LISTENER_T *listener,
                     RECONNECTION_STRATEGY_T *reconnection_strategy,
                     SESSION_CREATE_CALLBACK_T *callbacks,
                     DIFFUSION_ERROR_T *error);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Free memory associated with a session.
 *
 * The session must be closed with session_close() prior to calling
 * this function. If session is NULL, the function returns
 * immediately.
 *
 * @param session	The session to be freed.
 */
void session_free(SESSION_T *session);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Start a session, ie begin accepting messages from a Diffusion server.
 *
 * @deprecated This function is deprecated; a session is automatically
 *             started when it is successfully created. Always returns
 *             a value indicating success, regardless of the
 *             parameters passed.
 *
 * @param session	The session handle.
 * @param error		A structure for storing error messages, or NULL if
 *			detailed error reporting is not required.
 * @retval int		0 on error
 * @retval int		1 on success
 */
int session_start(SESSION_T *session, DIFFUSION_ERROR_T *error);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Stop accepting messages from a Diffusion server and close the connection.
 *
 * @param session	The session handle. If NULL, an error is returned.
 * @param error		A structure for storing error messages, or NULL if
 *			detailed error reporting is not required.
 * @retval int		0 on error
 * @retval int		1 on success
 */
int session_close(SESSION_T *session, DIFFUSION_ERROR_T *error);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Returns the current session state.
 *
 * @param session The session handle. If NULL, \ref SESSION_STATE_UNKNOWN
 *                is returned.
 *
 * @retval SESSION_STATE_T The session state, or \ref SESSION_STATE_UNKNOWN
 *                         if session is NULL.
 */
SESSION_STATE_T session_state_get(SESSION_T *session);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Returns the last connection response code.
 *
 * @param session       The session handle. May not be NULL.
 * @retval CONNECTION_RESPONSE_CODE_T The last connection response code.
 */
CONNECTION_RESPONSE_CODE_T session_connection_response_code(SESSION_T *session);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Returns a human-readable representation of the session state.
 *
 * @param state		A session state value.
 * @return		A textual representation of the session state.
 */
const char *session_state_as_string(const SESSION_STATE_T state);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Determines if a session is connected to a server.
 *
 * @param session A session handle. A NULL value returns
 *                \ref DIFFUSION_FALSE.
 * @retval DIFFUSION_TRUE	If the session is connected.
 * @retval DIFFUSION_FALSE	If the session is not connected.
 */
int session_is_connected(const SESSION_T *session);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Determines if a session is in a recovering state, where it is trying
 * to reconnect to a server.
 *
 * @param session		A session handle. A NULL value returns \ref DIFFUSION_FALSE.
 * @retval DIFFUSION_TRUE	If the session is recovering.
 * @retval DIFFUSION_FALSE	If the session is not recovering.
 */
int session_is_recovering(const SESSION_T *session);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Determines if a session is closed.
 *
 * @param session		A session handle. A NULL value returns \ref DIFFUSION_TRUE.
 * @retval DIFFUSION_TRUE	If the session is closed.
 * @retval DIFFUSION_FALSE	If the session is not closed.
 */
int session_is_closed(const SESSION_T *session);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Takes a session ID and produces a human-readable string. It is the
 * responsibility of the caller to free the memory allocated for the returned
 * string.
 *
 * @param session_id	A \ref SESSION_ID_T. If the session ID is NULL, this function
 *                      returns NULL.
 * @retval "char *"	A string containing a textual representation of the
 *			session id.
 * @retval NULL		If an error occurs.
 */
char *session_id_to_string(const SESSION_ID_T *session_id);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Given a session id in string form, this function returns it to the
 * internal structure form.
 *
 * @param str			The session id as a string. If NULL, this function
 *                              returns NULL.
 * @retval "SESSION_ID_T *"	A populated session id structure.
 * @retval NULL			If an error occurs.
 */
SESSION_ID_T *session_id_create_from_string(const char *str);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Free memory associated with a session ID structure.
 *
 * @param session_id            The session id structure. If NULL, this function
 *                              has no effect.
 */
void session_id_free(SESSION_ID_T *session_id);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Compare two session IDs.
 *
 * @param s1 The first session ID to compare.
 * @param s2 The second session ID to compare.
 * @retval 0 if the session IDs are equal.
 * @retval 1 if only their server instances are equal.
 * @retval -1 otherwise.
 */
int session_id_cmp(const SESSION_ID_T s1, const SESSION_ID_T s2);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Sets the handler for topics received but not subscribed to.
 *
 * If a topic message is received that does not have a registered
 * handler, it is passed to the global topic handler. This function
 * allows you to override the default handler (which does nothing).
 *
 * @param session       The session handle. <em>MUST NOT</em> be NULL.
 * @param handler       Pointer to a function which will receive the
 *                      unhandled messages, or NULL to reset to the default
 *                      handler.
 * @retval              The previous topic handler.
 */
TOPIC_HANDLER_T set_global_topic_handler(SESSION_T *session, const TOPIC_HANDLER_T handler);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Sets the maximum size of the outbound queue.
 *
 * The session maintains a queue of messages which have been submitted
 * but not yet sent on the wire to the server. This function allows
 * the user to override the maximum size of this queue. If the
 * supplied queue size is smaller than the number of messages
 * currently on the queue, any further additions to the queue will
 * block until space becomes available.
 *
 * @param session       The session handle. <em>MUST NOT</em> be NULL.
 * @param size          The new queue capacity (in messages), or -1 for
 *                      unbounded.
 *
 * @retval The previous capacity.
 */
int session_set_maximum_outbound_queue_size(SESSION_T *session, int size);

#endif
