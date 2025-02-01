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

#ifndef _diffusion_win_string_h_
#define _diffusion_win_string_h_ 1

#ifdef WIN32
#include <stddef.h>

extern char* strndup(const char *string, size_t length);

#endif

#endif
