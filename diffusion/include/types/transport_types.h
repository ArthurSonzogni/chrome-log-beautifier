#ifndef _diffusion_transport_types_h_
#define _diffusion_transport_types_h_ 1

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
 * @file transport_types.h
 *
 * Structures and definitions specific to transport related functions.
 */

#include <openssl/ssl.h>

#include <apr_uri.h>
#include <apr_thread_proc.h>

#include "hash.h"
#include "d_semaphore.h"
#include "types/common_types.h"
#include "types/error_types.h"
#include "types/session_types.h"
#include "types/topic_types.h"

/* Contains the parsed URL used for connecting to Diffusion. */
typedef apr_uri_t CONNECTION_URL_T;

/**
 * Indicates the transport mechanism by which the client connected.
 */
typedef enum {
        /// Websocket protocol.
        TRANSPORT_TYPE_WEBSOCKET = 0,
        /// HTTP long polling transport.
        TRANSPORT_TYPE_HTTP_LONG_POLL,
        /// HTTP long poll, re-issued with HTML `<script>` tags.
        TRANSPORT_TYPE_IFRAME_LONG_POLL,
        /// HTTP 1.1 chunked transfer delivered to a browser "forever
        /// frame", wrapped in HTML `<script>` tags.
        TRANSPORT_TYPE_IFRAME_STREAMING,
        /// Diffusion protocol based on TCP sockets.
        TRANSPORT_TYPE_DPT,
        /// Diffusion protocol based on HTTP 1.1. chunked transfer.
        TRANSPORT_TYPE_HTTPC,
        /// Diffusion protocol based on HTTP 1.1. chunked transfer.
        TRANSPORT_TYPE_HTTPC_DUPLEX,
        /// Transport type is not known to the local session, possibly
        /// because the remote client is using a different version of
        /// Diffusion.
        TRANSPORT_TYPE_OTHER = 7
} TRANSPORT_TYPE_T;

/**
 * Indicates the type of client which is interacting with the
 * application (if known).
 */
typedef enum {
        /// JavaScript, running within a web browser.
        CLIENT_TYPE_JAVASCRIPT_BROWSER = 0,
        /// JavaScript, running within a web browser, using a
        /// Flash plugin to host a transport bridge.
        CLIENT_TYPE_JAVASCRIPT_FLASH,
        /// JavaScript, running within a web browser, using a
        /// Sliverlight plugin to host a transport bridge.
        CLIENT_TYPE_JAVASCRIPT_SILVERLIGHT,
        /// Android.
        CLIENT_TYPE_ANDROID,
        /// iOS.
        CLIENT_TYPE_IOS,
        /// J2ME. Includes Blackberry clients.
        CLIENT_TYPE_J2ME,
        /// Flash / Flex.
        CLIENT_TYPE_FLASH,
        /// Silverlight.
        CLIENT_TYPE_SILVERLIGHT,
        /// Java.
        CLIENT_TYPE_JAVA,
        /// .Net.
        CLIENT_TYPE_DOTNET,
        /// C.
        CLIENT_TYPE_C,
        /// Diffusion internal client. For example, the Introspector.
        CLIENT_TYPE_INTERNAL,
        /// Client type is not known to the local session, possibly
        /// because the remote client is using a different version of
        /// Diffusion.
        CLIENT_TYPE_OTHER = 12
} CLIENT_TYPE_T;

/**
 * Describes the origin of an address; whether it is external to the
 * host machine, local, loopback or unknown.
 */
typedef enum {
        /// The address is a standard global internet address.
        ADDRESS_TYPE_GLOBAL = 1,
        /// The address is a site local address. The address is part
        /// of the IP subset that is reserved for private networks.
        ADDRESS_TYPE_LOCAL = 2,
        /// The address is assigned to the server loopback interface.
        ADDRESS_TYPE_LOOPBACK = 3,
        /// The address type is unknown.
        ADDRESS_TYPE_UNKNOWN = 4
} ADDRESS_TYPE_T;

/*
 * Structure containing parameters and state relating the transport
 * of data between the client and Diffusion.
 */
typedef struct transport_s {
        // URL to connect to Diffusion server.
        CONNECTION_URL_T connection_url;
        // 0 if insecure, 1 if secure (SSL/TLS) and 2 if secure but we
        // accept self-signed certificates.
        int secure;

        // (Internal) Temporary read buffer.
        BUF_T _read_buf;

        // (Internal) sockaddr structure
        apr_sockaddr_t _addr;
        // (Internal) socket handle.
        apr_socket_t *_socket;
        // (Internal) coordinate read/write threads.
        volatile apr_uint32_t _running;
        // (Internal) Thread used to read from server.
        // (May be used for writing with websocket transport).
        apr_thread_t *_read_thread;
        // (Internal) Thread used to write to server.
        // (Not used by websocket transport).
        apr_thread_t *_write_thread;

        // (Internal) Broadcast semaphore when session is disconnected
        // and controlling threads are exiting.
        SEMAPHORE_T *sem_disconnect;

        // For internal use when secure transports have been selected.
        struct {
                SSL *handle;
                SSL_CTX *context;
        } _ssl;

        /*
         * Functions to be overridden by the transport implementation.
         */
        // Make a connection to Diffusion.
        int (*connect)(SESSION_T *session, DIFFUSION_ERROR_T *error);
        // Start the connection (begin receiving messages).
        int (*start)(SESSION_T *session, DIFFUSION_ERROR_T *error);
        // Gracefully close the connection.
        void (*close)(SESSION_T *session);
        // Clean up connection (DPT: close apr socket).
        void (*cleanup)(SESSION_T *session);
        // Callback when there is data ready to be sent.
        void (*notify_data_to_send)(SESSION_T *session);
        // Implementation specific data.
        void *impl;
        // Function to free memory associated with
        // implementation-specific data.
        void (*impl_free)(struct transport_s *transport);
        // Function called to get min/max protocol versions for this
        // transport.
        int (*protocol_version)(void);
} TRANSPORT_T;

#endif
