/**
 * @file client_details_types.h
 *
 * Types used for client (and by extension, session) details.
 *
 * Copyright © 2014, 2016 Push Technology Ltd., All Rights Reserved.
 *
 * Use is subject to license terms.
 *
 * NOTICE: All information contained herein is, and remains the
 * property of Push Technology. The intellectual and technical
 * concepts contained herein are proprietary to Push Technology and
 * may be covered by U.S. and Foreign Patents, patents in process, and
 * are protected by trade secret or copyright law.
 */
#ifndef _diffusion_client_details_types_h_
#define _diffusion_client_details_types_h_ 1

#include "set.h"

typedef struct session_details_summary_s {
        /// Principal of the connecting client.
        char *principal;

        /// Type of client making the connection request.
        CLIENT_TYPE_T client_type;

        /// Transport protocol of the connecting client.
        TRANSPORT_TYPE_T transport_type;
} SESSION_DETAILS_SUMMARY_T;

typedef struct session_details_summary_s CLIENT_DETAILS_SUMMARY_T;

/**
 * @brief This structure describes the network and physical location
 * of a client/session. If a field cannot be determined, it will be
 * NULL or set to the default value for the field type.
 */
typedef struct location_details_s {
        /// IP address of connecting client.
        char *address;

        /// Hostname of connecting client.
        char *hostname;

        /// The hostname of the connecting client, as returned by a
        /// WhoIs lookup.
        char *resolvedname;

        /// The type of address (external, or internal to the machine).
        ADDRESS_TYPE_T addresstype;

        /// ISO 3166 country code.
        char *country;

        /// ISO 369 language code.
        char *language;

        /// Client's latitude.
        float latitude;

        /// Client's longitude.
        float longitude;
} LOCATION_DETAILS_T;

typedef struct session_details_s {
        SET_T *available_details;
        CLIENT_DETAILS_SUMMARY_T *client_summary;
        LOCATION_DETAILS_T *location;
        char *connector_name;
        char *server_name;
} SESSION_DETAILS_T;

#endif
