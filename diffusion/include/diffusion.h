#ifndef _diffusion_h_
#define _diffusion_h_ 1

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
 * @file diffusion.h
 *
 * Main include file.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "doxygen-defs.h"

#ifndef APR_DECLARE_STATIC
#define APR_DECLARE_STATIC 1
#endif
#ifndef APU_DECLARE_STATIC
#define APU_DECLARE_STATIC 1
#endif

/** The version of Diffusion from which the C API library was built. */
#if defined(__GNUC__)
static const char *DIFFUSION_VERSION __attribute__ ((unused))= "5.9.24 24_01#70279";
#else
static const char *DIFFUSION_VERSION = "5.9.24 24_01#70279";
#endif

#include <stdlib.h>
#include <apr.h>

#include "hash.h"
#include "error.h"

#include "conversation.h"
#include "session.h"
#include "topic.h"
#include "security.h"
#include "content.h"
#include "services.h"
#include "internal/service_int.h"
#include "internal/transport_int.h"
#include "internal/route_int.h"

#ifdef __cplusplus
}
#endif

#endif
