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

#ifndef _diffusion_transport_int_h_
#define _diffusion_transport_int_h_ 1

#include <apr_network_io.h>
#include <apr_thread_proc.h>
#include <apr_uri.h>

#include "hash.h"
#include "types/common_types.h"
#include "types/conversation_types.h"
#include "types/error_types.h"
#include "types/session_types.h"
#include "types/topic_types.h"
#include "types/transport_types.h"

#include "internal/transport-ws_int.h"
#include "internal/transport-dpt_int.h"
#include "internal/transport-dummy_int.h"

int transport_create(SESSION_T *session, apr_pool_t *pool, DIFFUSION_ERROR_T *error);
void transport_free(TRANSPORT_T *transport);

int transport_connect(SESSION_T *session, DIFFUSION_ERROR_T *error);

int transport_parse_url(apr_pool_t *pool, const char *url_str, CONNECTION_URL_T *connection_url);

/**
 * Helper function to write an entire buffer to a socket. Handles plaintext and
 * SSL/TLS encrypted data.
 *
 * @param transport The transport definition for this connection.
 * @param buf       The BUF_T containing data to be written to the socket.
 * @param error     A DIFFUSION_ERROR_T holding error information if something failed.
 *
 * @return 0 on failure, 1 on success.
 */
int sync_send_buf(TRANSPORT_T *transport, BUF_T *buf, DIFFUSION_ERROR_T *error);

/**
 * Helper function to read data from a socket. Handles plaintext and SSL/TLS
 * encrypted sockets.
 *
 * @param transport  The transport definition for this connection.
 * @param rcv_buf    A pre-allocated buffer into which this function will
 *                   store data read from the socket.
 * @param rcv_buflen On entry, the size available in rcv_buf and on successful
 *                   exit, the number of bytes read.
 * @param error      An error structure to be filled in the event of an error.
 *
 * @return 0 indicating an error occurred, else a non-zero value for success.
 */
int sync_socket_read(TRANSPORT_T *transport, char *rcv_buf, size_t *rcv_buflen, DIFFUSION_ERROR_T *error);

/**
 * Helper function to read data from a socket. Handles plaintext and SSL/TLS
 * encrypted sockets.
 *
 * @param transport  The transport definition for this connection.
 * @param rcv_buf    A pre-allocated buffer into which this function will
 *                   store data read from the socket.
 * @param rcv_buflen The number of bytes to read.
 * @param error      An error structure to be filled in the event of an error.
 *
 * @return 0 indicating an error occurred, else a non-zero value for success.
 */
int sync_socket_read_size(TRANSPORT_T *transport, char *rcv_buf, size_t rcv_buflen, DIFFUSION_ERROR_T *error);

/**
 * Helper function to read data from a socket, blocking until a timeout expires.
 * Handles plaintext and SSL/TLS encrypted sockets.
 *
 * @param transport  The transport definition for this connection.
 * @param rcv_buf    A pre-allocated buffer into which this function will
 *                   store data read from the socket.
 * @param rcv_buflen The number of bytes to read.
 * @param timeout    The timeout in milliseconds (< 0 = block forever).
 * @param error      An error structure to be filled in the event of an error.
 *
 * @return 0 indicating an error occurred, else a non-zero value for success.
 */
int sync_socket_read_size_timeout(TRANSPORT_T *transport, char *rcv_buf, size_t rcv_buflen, long timeout, DIFFUSION_ERROR_T *error);

/**
 * Wraps len data bytes in a V5 style envelope and returns a BUF_T containing the enveloped
 * message.
 */
BUF_T *transport_envelope_data(const SERVICE_TYPE_T type,
                               const SERVICE_MODE_T mode,
                               const CONVERSATION_ID_T conversation_id,
                               const long len, const void *data);


BUF_T *transport_envelope_buf(const SERVICE_TYPE_T type,
                              const SERVICE_MODE_T mode,
                              const CONVERSATION_ID_T conversation_id,
                              const BUF_T *buf);


void broadcast_transport_closed(TRANSPORT_T *transport);

void transport_SSL_locks_init();
void transport_SSL_locks_destroy();
void transport_SSL_init();
void transport_SSL_destroy();

#endif
