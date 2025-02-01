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

#ifndef _diffusion_error_int_h_
#define _diffusion_error_int_h_ 1

/**
 * Populate an error structure with details of the error, including the error
 * message text. If an APR error has occurred (apr_status = APR_SUCCESS), then
 * the text is copied from the APR API, else the Diffusion error message is
 * used.
 *
 * @param error		An error structure to populate. If NULL, return without
 *			doing anything.
 * @param code		The Diffusion error code, if appropriate.
 * @param apr_status	The APR error code, or APR_SUCCESS.
 */
void diffusion_error_set(DIFFUSION_ERROR_T *error, ERROR_CODE_T code, apr_status_t apr_status);

#endif
