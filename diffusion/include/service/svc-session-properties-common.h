#ifndef _diffusion_svc_session_properties_common_h_
#define _diffusion_svc_session_properties_common_h_ 1

typedef enum {
        /// Indicates a session has been opened (e.g. new client
        /// connection).
        SESSION_PROPERTIES_EVENT_TYPE_OPEN = 0,
        /// Indicates that a session has updated properties.
        SESSION_PROPERTIES_EVENT_TYPE_UPDATE = 1,
        /// Indicates that a session has been closed.
        SESSION_PROPERTIES_EVENT_TYPE_CLOSE = 2,
        /// Indicates that a session has been disconnected
        SESSION_PROPERTIES_EVENT_TYPE_DISCONNECTED = 3
} SESSION_PROPERTIES_EVENT_TYPE_T;

typedef enum {
        /// The session properties have been updated.
        SESSION_PROPERTIES_UPDATE_TYPE_UPDATED = 0,
        /// The session properties have been updated due to
        /// reconnection with the server.
        SESSION_PROPERTIES_UPDATE_TYPE_RECONNECTED = 1,
        /// The session properties have been updated due to the
        /// session failing over to a different host.
        SESSION_PROPERTIES_UPDATE_TYPE_FAILED_OVER = 2,
        /// Session has disconnected
        SESSION_PROPERTIES_UPDATE_TYPE_DISCONNECTED = 3
} SESSION_PROPERTIES_UPDATE_TYPE_T;

typedef struct session_properties_event_s {
        /// The reason why a property event was triggered.
        SESSION_PROPERTIES_EVENT_TYPE_T type;
        /// If the session was closed, this indicates the reason for
        /// closure.
        SESSION_CLOSE_REASON_T close_reason;
        /// If the session properties were updated, this indicates the
        /// reason for the change.
        SESSION_PROPERTIES_UPDATE_TYPE_T update_type;
        /// The session ID of the client session.
        SESSION_ID_T session_id;
        /// A key/value map of properties of the client, as requested
        /// when registering the session properties listener.
        HASH_T *properties;
} SESSION_PROPERTIES_EVENT_T;

/// Callbacks for session open/close and property updates have this
/// function signature.
typedef int (*on_session_properties_event_cb)(SESSION_T *session,
                                              const SESSION_PROPERTIES_EVENT_T *event,
                                              void *context);

#define SESSION_PROPERTIES_EVENT_HANDLERS                               \
        /** Callback fired when a new client session is opened. */      \
        on_session_properties_event_cb on_session_open;                 \
        /** Callback fired when an existing client session closes. */   \
        on_session_properties_event_cb on_session_close;                \
        /** Callback fired when a client's session detail are updated. */ \
        on_session_properties_event_cb on_session_update;               \
        /** Callback fired when a client was disconnected. */           \
        on_session_properties_event_cb on_session_disconnect;           \
        /** Error callback if an error occurs with a session update.  */ \
        ERROR_HANDLER_T on_session_error;

#endif
