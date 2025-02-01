/**
 * @file topic_types.h
 *
 * Types relating to topic messages and service messages.
 *
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
#ifndef _diffusion_topic_types_h_
#define _diffusion_topic_types_h_ 1

#include "list.h"
#include "content.h"
#include "types/common_types.h"
#include "types/conversation_types.h"
#include "types/service_types.h"

/** DPT protocol message delimiter */
#define DPT_MESSAGE_DELIM 0x00
/** DPT protocol record delimiter */
#define DPT_RECORD_DELIM 0x01
/** DPT protocol field delimiter */
#define DPT_FIELD_DELIM 0x02

/** DPT protocol indicator in connection handshake */
#define DPT_PROTOCOL_BYTE 0x23
/** DPT protocol version indicator */
#define DPT_PROTOCOL_VERSION 0x05
/** DPT protocol client type = "C" */
#define DPT_CLIENT_TYPE 0x1d

/** Diffusion protocol version to use with websocket connections */
#define WS_PROTOCOL_VERSION 0x08
/** WS protocol client type = "WC" */
#define WS_CLIENT_TYPE "WC"

/**
 * Bitmask used to negotiate the client capabilities. The C API
 * only supports "Feature Clients" at this time.
 *
 * - 0000 0000 None
 * - 0000 0001 Encrypt
 * - 0000 0010 Compress
 * - 0000 0100 Base64
 * - 0000 1000 Feature
 */
#define DPT_CAPABILITIES 0x08

/** The connection is raw with no encoding. */
#define DPT_ENCODING_NONE 0x00
/** The connection can handle encrypted data. */
#define DPT_ENCODING_ENCRYPTION_REQUESTED 0x01
/** The connection can handle compressed data. */
#define DPT_ENCODING_COMPRESSION_REQUESTED 0x02
/** The connection can handle base64 encoded data. */
#define DPT_ENCODING_BASE64_REQUESTED 0x03
/** The connection is encrypted. */
#define DPT_ENCODING_ENCRYPTED 0x11
/** The connection is compress. */
#define DPT_ENCODING_COMPRESSED 0x12
/** The connection uses base64 encoding. */
#define DPT_ENCODING_BASE64 0x13

/** DPT recvonnection request indicator */
#define DPT_RECONNECTION_REQUEST 0x02

/**
 * An enumeration of messages types, corresponding to messages in the
 * DPT v4 protocol.
 */
typedef enum {
        MESSAGE_TYPE_UNDEFINED = 0x00,
        /** Full topic contents */
        MESSAGE_TYPE_TOPIC_LOAD = 0x14,
        /** Topic updated, potentially only contains differences with prior contents */
        MESSAGE_TYPE_DELTA = 0x15,
        MESSAGE_TYPE_SUBSCRIBE = 0x16,
        MESSAGE_TYPE_UNSUBSCRIBE = 0x17,
        MESSAGE_TYPE_PING_SERVER = 0x18,
        MESSAGE_TYPE_PING_CLIENT = 0x19,
        MESSAGE_TYPE_CREDENTIALS = 0x1a,
        MESSAGE_TYPE_CREDENTIALS_REJECTED = 0x1b,
        MESSAGE_TYPE_ABORT_NOTIFICATION = 0x1c,
        MESSAGE_TYPE_CLOSE_REQUEST = 0x1d,
        MESSAGE_TYPE_TOPIC_LOAD_ACK_REQD = 0x1e,
        MESSAGE_TYPE_DELTA_ACK_REQD = 0x1f,
        MESSAGE_TYPE_ACK = 0x20,
        MESSAGE_TYPE_FETCH = 0x21,
        MESSAGE_TYPE_FETCH_REPLY = 0x22,
        MESSAGE_TYPE_TOPIC_STATUS_NOTIFICATION = 0x23,
        MESSAGE_TYPE_COMMAND_MESSAGE = 0x24,
        MESSAGE_TYPE_COMMAND_TOPIC_LOAD = 0x28,
        MESSAGE_TYPE_COMMAND_TOPIC_NOTIFICATION = 0x29,
} MESSAGE_TYPE_T;

/** The maximum allowed message type value. */
#define MESSAGE_TYPE_MAX_VALUE 0x7f

/**
 * Generic message; all DPT messages should fit into this structure.
 */
typedef struct dpt_message_s {
        /// Type of message (Diffusion v4 protocol).
        MESSAGE_TYPE_T type;
        /// Headers attached to the message.
        LIST_T *headers;
        /// Payload of the message (minus headers).
        BUF_T *payload;
} MESSAGE_T;

/**
 * Structure for slightly simplified access to topic messages.
 * Identical to MESSAGE_T except for the name field.
 */
typedef struct topic_message_s {
        /// Type of message (Diffusion v4 protocol).
        MESSAGE_TYPE_T type;
        /// Headers attached to the message.
        LIST_T *headers;
        /// Payload of the message (minus headers).
        BUF_T *payload;
        /// The topic name.
        const char *name;
} TOPIC_MESSAGE_T;

/**
 * Service messages over the V5 protocol are parsed into this common
 * structure, with the unparsed service-specific content in the payload
 * field.
 */
typedef struct v5_message_s {
        /// The SERVICE_TYPE_T of the message.
        SERVICE_TYPE_T service_type;

        /// The SERVICE_MODE_T of the message (error, request,
        /// response).
        SERVICE_MODE_T service_mode;

        /// The conversation ID associated with the message.
        CONVERSATION_ID_T conversation_id;

        /// Optional message payload.
        BUF_T *payload;
} V5_MESSAGE_T;

/**
 * Additional options associated with a STREAM_MESSAGE_T.
 */
typedef struct stream_message_options_s {
        /// Priority with which the message is relayed to clients.
        CLIENT_SEND_PRIORITY_T priority;
        /// Headers attached to the message.
        LIST_T *headers;
} STREAM_MESSAGE_OPTIONS_T;

/**
 * Messages directed to this session are received through a globally
 * registered handler, and are parsed into this structure.
 */
typedef struct stream_message_s {
        /// Topic endpoint through which the message is sent.
        char *topic_path;
        /// Content of the message.
        CONTENT_T content;
        /// Additional options associated with the message.
        STREAM_MESSAGE_OPTIONS_T options;
} STREAM_MESSAGE_T;

#endif
