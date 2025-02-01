#ifndef _diffusion_reconnect_h_
#define _diffusion_reconnect_h_ 1

/*
 * Copyright © 2016 Push Technology Ltd., All Rights Reserved.
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
 * @file reconnect.h
 *
 * Functions relating to session reconnection.
 */

#include "types/session_types.h"
#include "utils.h"

// For backwards compatibility
typedef RECONNECTION_STRATEGY_T SESSION_FAILOVER_STRATEGY_T;

RECONNECTION_ATTEMPT_ACTION_T reconnection_repeating(SESSION_T *session, void *cb_args);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Creates a reconnection strategy that will retry a connection
 *        for a given number of times, with a delay between retry
 *        attempts.
 *
 * A retry_count of 0 means that no reconnection attempts will be
 * made.
 *
 * A retry_delay greater than 0 and a retry_delay of 0 will retry for a set
 * amount of time (\ref DIFFUSION_DEFAULT_RECONNECT_TIMEOUT) before giving up.
 *
 * Otherwise, retries will continue for (retry_count * retry_delay) seconds unless
 * explicitly overridden with reconnection_strategy_set_timeout().
 *
 * @param retry_count   The number of attempts to make.
 * @param retry_delay   The time between connection attempts, in milliseconds,
 *                      or 0 to request the system default timeout.
 *
 * @retval "RECONNECTION_STRATEGY_T *"  A pointer to a new reconnection
 *                                      strategy that can be passed to a
 *                                      session creation function.
 */
RECONNECTION_STRATEGY_T *make_reconnection_strategy_repeating_attempt(long retry_count, long retry_delay);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief Create a new reconnection strategy whose behavior is determined by
 *        a user-supplied function.
 *
 * @param reconnect_cb  A function to be called that will return
 *                      \ref RECONNECTION_ATTEMPT_ACTION_START when a reconnection
 *                      should be tried, else
 *                      \ref RECONNECTION_ATTEMPT_ACTION_ABORT.
 * @param args          Pointer to user-defined data that can be used by the
 *                      reconnection strategy callbacks. This data is not
 *                      freed by free_reconnection_strategy(), nor is it copied
 *                      during session_create() or session_create_async().
 * @param success_cb    If not NULL, a function to be invoked on successful
 *                      reconnection.
 * @param failure_cb    If not NULL, a function to be invoked if the
 *                      reconnection fails.
 * @param cascade_cb    If not NULL, a function to be invoked if the
 *                      reconnection attempt leads to a cascade to a different
 *                      URL.
 *
 * @retval "RECONNECTION_STRATEGY_T *"  A pointer to a new reconnection
 *                                      strategy that can then be passed to a
 *                                      session creation function.
 */
RECONNECTION_STRATEGY_T *make_reconnection_strategy_user_function(PERFORM_RECONNECTION_CB reconnect_cb,
                                                                  void *args,
                                                                  PERFORM_RECONNECTION_AFTER_ACTION_CB success_cb,
                                                                  PERFORM_RECONNECTION_AFTER_ACTION_CB failure_cb,
                                                                  PERFORM_RECONNECTION_AFTER_ACTION_CB cascade_cb);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief       Frees memory associated with a reconnection strategy.
 *
 * If a user-defined reconnection strategy is being used, then it is the
 * user's responsibility to free memory associated with the args parameter
 * passed to make_reconnection_strategy_user_function().
 *
 * @param strategy The reconnection strategy to be freed. <em>MUST
 *                 NOT</em> be NULL.
 */
void free_reconnection_strategy(RECONNECTION_STRATEGY_T *strategy);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief	Explicitly requests the length of time that a session is
 *              available on the server in the event that this client is
 *              disconnected.
 *
 * A reconnection timeout of 0 means that no reconnection attempt will be made
 * to the same server, but it may cascade to another server specified by a
 * call to reconnection_strategy_set_cascade_urls().
 *
 * This is only effective when the session is created. If the timeout is
 * changed after session_create() is called, there is no effect.
 *
 * @param strategy	The reconnection strategy for this client session.
 *                      <em>MUST NOT</em> be NULL.
 * @param timeout       After this timeout (in milliseconds) has elapsed, no
 *                      further reconnection attempts will be made for this
 *                      URL.
 */
void reconnection_strategy_set_timeout(RECONNECTION_STRATEGY_T *strategy, long timeout);

/**
 * @ingroup PublicAPI_Session
 *
 * @brief       Specifies a list of URLs that the client will cascade through
 *              in an attempt to make a connection to the server. The initial
 *              URL set during session_create() is effectively prepended to
 *              these URLs and does not need to be specified.
 *
 * @param strategy      The reconnection strategy for this client session.
 *                      <em>MUST NOT</em> be NULL.
 * @param urls          A NULL-terminated array of URL strings, or NULL to
 *                      specify that there are no URLs available for cascading to.
 */
void reconnection_strategy_set_cascade_urls(RECONNECTION_STRATEGY_T *strategy, const char **urls);

#endif
