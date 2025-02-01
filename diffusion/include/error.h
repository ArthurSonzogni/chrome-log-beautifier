/**
 * @file error.h
 *
 * Functions relating to error reporting within the API.
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
#ifndef _diffusion_error_h_
#define _diffusion_error_h_ 1

#include <apr_errno.h>

#include "types/error_types.h"
#include "list.h"

/**
 * Given a Diffusion API error code, return the related error text.
 *
 * @param code			The Diffusion API error code
 * @retval "const char *"	A pointer to the error message.
 * @retval NULL			If there is no error message for this code.
 */
const char *diffusion_error_str(const ERROR_CODE_T code);

/**
 * @deprecated This function is deprecated from version 5.9 onwards, and may
 * be removed.
 *
 * @brief Given a Diffusion API error code, say whether this indicated that a
 * connection to the server couldn't be made but an immediate retry is
 * may be successful.
 *
 * @param error_code            The Diffusion API error code
 * @retval int                  1 for recoverable, else 0.
 */
int is_recoverable_connection_error(ERROR_CODE_T error_code);

/**
 * @brief Structure describing an error report which may arise when
 *        calling various API functions.
 */
typedef struct error_report_s {
        /// Error message.
        char *message;
        /// Line in the update script at which the error occurred.
        unsigned int line;
        /// Column in the line of the update script at which the
        /// error occurred.
        unsigned int column;
} ERROR_REPORT_T;

/**
 * @brief Create a new ERROR_REPORT_T structure.
 */
ERROR_REPORT_T *error_report_create(void);

/**
 * @brief Free an existing ERROR_REPORT_T structure and associated
 *        memory.
 */
void error_report_free(ERROR_REPORT_T *error_report);

/**
 * @brief Create a duplicate (deep copy) of an ERROR_REPORT_T
 *        structure.
 */
ERROR_REPORT_T *error_report_dup(const ERROR_REPORT_T *error_report);

char *buf_read_error_report(const char *data, ERROR_REPORT_T **error_report);
char *buf_read_error_reports(const char *data, LIST_T **error_reports);

#endif
