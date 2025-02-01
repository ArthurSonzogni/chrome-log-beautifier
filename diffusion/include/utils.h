/**
 * @file utils.h
 *
 * Miscellaneous utility functions that don't fit anywhere else.
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
#ifndef _diffusion_utils_h_
#define _diffusion_utils_h_ 1

#include <inttypes.h>
#include <list.h>
#include <set.h>
#include <buf.h>

#ifdef WIN32
#include <WinSock2.h>
#include <Windows.h>
#define SLEEP_MS(x) Sleep(x)
#else
#include <time.h>
#define SLEEP_MS(x) nanosleep(&(struct timespec){.tv_sec = ((x) / 1000L), .tv_nsec = (((x) % 1000L) * 1000000L)}, NULL)
#endif

#ifdef WIN32
#define OS_SNPRINTF(a, b, c, d) _snprintf(a, b, c, d)
#else
#define OS_SNPRINTF(a, b, c, d) snprintf(a, b, c, d)
#endif



typedef BUF_T SCRIPT_T;
#ifndef script_create
#define script_create buf_create
#endif
#ifndef script_free
#define script_free buf_free
#endif

/**
 * Safely convert an unsigned integer to a NULL-terminated string.
 * The caller should free the return value after use.
 *
 * @param	val	The unsigned integer to convert.
 * @retval		A NULL-terminated string or NULL if the conversion failed.
 */
char *uint32_to_str(uint32_t val);

/**
 * Safely convert a signed integer to a NULL-terminated string.
 * The caller should free the return value after use.
 *
 * @param	val	The signed integer to convert.
 * @return		A NULL-terminated string or NULL if the conversion failed.
 */
char *int_to_str(int val);

/**
 * Safely convert a signed double to a NULL-terminated string. The "float"
 * format specifier is used.
 * The caller should free the return value after use.
 *
 * @param	val	The signed double to convert.
 * @return		A NULL-terminated string or NULL if the conversion failed.
 */
char *double_to_str(double val);

/**
 * Encode an ieee754 float as a uint32_t (4 bytes).
 *
 * @param	f	Float to encode
 * @return		A uint32_t
 */
uint32_t float_to_uint32(float f);

/**
 * Decocde an ieee754 float from a 4-byte unsigned int.
 *
 * @param	i	uint32_t containing IEEE754-encoded data.
 * @return		A float
 */
float uint32_to_float(uint32_t i);

/**
 * Returns the largest non-regexp part of a topic selector.
 *
 * @param selector	The topic selector to analyse.
 * @return		A pointer to a char array, which should be freed by the
 *			caller.
 */
char *selector_get_prefix(const char *selector);

// Used when creating scripts in the system authentication / security
// store DSLs.
SCRIPT_T *add_roles_to_script(SCRIPT_T *script, const LIST_T *roles);

// Used when creating scripts in the system authentication / security
// store DSLs.
SCRIPT_T *add_permissions_to_script(SCRIPT_T *script, const SET_T *permissions, const char **permissions_name_table);

// Writes a length-prepended list of strings to a buffer.
void buf_write_list_strings_enc(BUF_T *buf, const LIST_T *list);

// Reads a length-prepended list of strings, and returns a pointer to
// the position after the last string.
char *buf_read_list_strings_enc(const char *data, LIST_T *list);

// Writes a length-prepended collection of key/value pairs to a buffer.
void buf_write_hash_strings_enc(BUF_T *buf, const HASH_T *hash);

// Reads a length-prepended collection of key/value pairs and returns
// a pointer to the position of the last item in the source string.
char *buf_read_hash_strings_enc(const char *data, HASH_T *hash);

// Writes a key set (a length-prepended list of strings) to a buffer.
void buf_write_set_strings_enc(BUF_T *buf, const SET_T *set);

void **dup_pointer_array(const void **arr, void *(*dup_fn)(void *));

// Take a copy of an array of strings.
char **dup_string_array(const char **arr);

// Free an array of pointers
void free_pointer_array(void **arr, void (*free_fn)(void *));

// Free an array of strings.
void free_string_array(char **arr);

// Append a pointer to an array of pointers. Returns the address of
// the pointer array, which may be different to that which was passed
// in.
void **append_pointer_array(void **arr, void *val);

// Append a string to an array of strings. Returns the address of the
// pointer array, which may be different to that which was passed in.
char **append_string_array(char **arr, char *val);

#endif
