/**
 * @file args.h
 *
 * Utilities for parsing command line arguments, as used in the examples.
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

#ifndef _diffusion_args_h_
#define _diffusion_args_h_ 1

#ifdef __cplusplus
extern "C" {
#endif

#include "hash.h"

/**
 * This structure represents a possible argument to an application.
 */
typedef struct arg_opts_s {
        /// The short form of the argument without the leading dash
        /// e.g. -x, -h
        char short_arg;

        /// The long form of the argument without the leading
        /// double-dash e.g. --help
        const char *long_arg;

        /// A description of the argument used when generating
        /// application help text.
        const char *description;

        /// If non-zero (ARG_REQUIRED), this argument is mandatory.
        int required;

        /// If non-zero (ARG_HAS_VALUE), this argument is followed by
        /// a value to be captured and associated with it.
        int has_value;

        /// If an argument is optional but not provided, and this is
        /// non-NULL, then it is used as that argument's value.
        const char *default_value;
} ARG_OPTS_T;

/** The argument is mandatory. */
#define ARG_REQUIRED 1
/** The argument is optional. */
#define ARG_OPTIONAL 0

/** The argument is followed by a value. */
#define ARG_HAS_VALUE 1
/** The argument is not followed by a value. */
#define ARG_NO_VALUE 0

/** Default argument declaration for "help". */
#define ARG_OPTS_HELP { 'h', "help", "Show usage", ARG_OPTIONAL, ARG_NO_VALUE, NULL }
/** Default "end of arguments" declaration. */
#define END_OF_ARG_OPTS { '\0', NULL, NULL, ARG_OPTIONAL, ARG_NO_VALUE, NULL }

/**
 * Parse argc/argv according to an array of ARG_OPTS_T (which itself
 * is terminated by a entry in the form of END_OF_ARG_OPTS).
 */
HASH_T *parse_cmdline(int argc, char **argv, ARG_OPTS_T *arg_opts);

/**
 * Display usage information to stdout.
 */
void show_usage(int argc, char **argv, ARG_OPTS_T *arg_opts);

#ifdef __cplusplus
}
#endif

#endif
