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

#ifndef _diffusion_session_int_h_
#define _diffusion_session_int_h_ 1

#include "types/common_types.h"
#include "types/session_types.h"
#include "types/service_types.h"
#include "types/topic_types.h"
#include "types/conversation_types.h"
#include "service/svc-notify-unsubscription.h"
#include "d_semaphore.h"

/*
 * Creates a new handler set with no handlers defined.
 */
HANDLER_SET_T *handler_set_create();

/*
 * Free memory associated with a hnalder set.
 */
void handler_set_free(HANDLER_SET_T *handlers);

/*
 * Add a handler for a (v5) service message.
 */
void add_service_handler(SESSION_T *session, SERVICE_TYPE_T type, SVC_DEFN_T *svc);


char *buf_read_session_id(const char *data, SESSION_ID_T *session_id);
void buf_write_session_id(BUF_T *buf, const SESSION_ID_T *session_id);

/*
 * Add a handler for any type of message. The handler is passed a pointer to the current session pointer to
 * the message to be processed.
 */
void add_message_handler(SESSION_T *session, const MESSAGE_TYPE_T type, void(*handler)(SESSION_T *session, MESSAGE_T *message));

/*
 * Adds a handler for a message with a specific conversation ID.
 */
void add_conversation_handler(SESSION_T *session, const CONVERSATION_ID_T conversation_id, const HANDLER_SET_T *handlers);

/*
 * Remove a conversation handler for a specific ID, and return that handler or NULL
 * if not found.
 */
HANDLER_SET_T *remove_conversation_handler(SESSION_T *session, const CONVERSATION_ID_T conversation_id);

/*
 * Internal only. Unsynchronized version of remove_conversation_handler().
 */
HANDLER_SET_T *remove_conversation_handler_unsync(SESSION_T *session, const CONVERSATION_ID_T conversation_id);

/*
 * It's usually necessary for the client to be able to respond to ping messages from the server ("PING_CLIENT messages"),
 * and this handler back the appropriate response. It is installed by default, but can be overridden using the
 * add_message_handler() function.
 */
void session_default_ping_client_handler(SESSION_T *session, MESSAGE_T *message);

/*
 * When sending data to the server, we should (almost) always go via a queue. This gives us the opportunity to
 * do batching, conflation, failover, etc.
 */
int session_queue_data(SESSION_T *session, const void *data, const MESSAGE_PRIORITY_T priority, DIFFUSION_ERROR_T *error);

/*
 * Register a fetch handler (remote control) with a specific path.
 */
void register_service_fetch(SESSION_T *session, const char *path, int (*handler)(SESSION_T *, void *, void *));

SESSION_ID_T *session_id_dup(const SESSION_ID_T *session_id);

int send_command(SESSION_T *session, const BUF_T *buf, const MESSAGE_PRIORITY_T priority, DIFFUSION_ERROR_T *error);
int send_message(SESSION_T *session, const MESSAGE_T *message, const MESSAGE_PRIORITY_T priority, DIFFUSION_ERROR_T *error);

extern int session_state_change(SESSION_T *session,
                                SESSION_STATE_T new_state, SESSION_STATE_T *old_state,
                                DIFFUSION_ERROR_T *error);

/*
 * Returns the most earliest message sequence number that can be sent
 * to the client.
 */
int session_message_first_available_client_sequence(SESSION_T *session);

/*
 * Returns the sequence number of the latest message received from the
 * server.
 */
int session_message_last_server_received_sequence(SESSION_T *session);

/*
 * Returns the sequence number of the latest message sent to the
 * client.
 */
int session_message_last_client_sent_sequence(SESSION_T *session);

/*
 * Removes all topic subscription information from the session caches and
 * invokes the unsubscription callback for each topic.
 */
void clear_and_notify_unsubscriptions(SESSION_T *session, NOTIFY_UNSUBSCRIPTION_REASON_T reason);

#endif
