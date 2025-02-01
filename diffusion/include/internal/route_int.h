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

#ifndef _diffusion_route_int_h_
#define _diffusion_route_int_h_ 1

#include "session.h"
#include "transport-dpt_int.h"

void route_message(SESSION_T *session, MESSAGE_T *message);

#endif
