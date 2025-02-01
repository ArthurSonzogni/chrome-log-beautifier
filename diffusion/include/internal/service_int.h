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

#ifndef _diffusion_service_int_h_
#define _diffusion_service_int_h_ 1

#include "internal/service_macros.h"
#include "types/session_types.h"
#include "types/topic_types.h"
#include "types/conversation_types.h"
#include "types/service_types.h"

void svc_handler_call(SESSION_T *session, const SVC_DEFN_T *svc_defn, const V5_MESSAGE_T *msg);
void service_route(SESSION_T *session, const V5_MESSAGE_T *msg);
void svc_parse_common_fields(SVC_MSG_COMMON_T *dst_msg, const V5_MESSAGE_T *src_msg);
void free_svc_common_fields(SVC_MSG_COMMON_T *msg);

#endif
