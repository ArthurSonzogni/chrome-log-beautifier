#ifndef _diffusion_int_h_
#define _diffusion_int_h_ 1

#include <stdio.h>
#include <apr_portable.h>
#define GET_TID ((unsigned int)((unsigned long int)apr_os_thread_current() % 1000))

#ifdef TRACE_FUNCTION_CALLS
#define TRACE_FN_ENTRY() printf("--> [%03d] %s():%d\n", GET_TID, __FUNCTION__, __LINE__)
#define TRACE_FN_EXIT() printf("<-- [%03d] %s():%d\n", GET_TID, __FUNCTION__, __LINE__)
#else
#define TRACE_FN_ENTRY()
#define TRACE_FN_EXIT()
#endif

#define DIFF_LOG(format, ...) fprintf(stderr, "    [%03d] %s():%d " format "\n", GET_TID, __FUNCTION__, __LINE__, ##__VA_ARGS__)


#ifdef DIFFUSION_DEBUG
#define DIFF_DEBUG(format, ...) DIFF_LOG(format,  ##__VA_ARGS__)
#else
#define DIFF_DEBUG(...)
#endif

#include <apr_thread_proc.h>
extern apr_pool_t *diffusion_global_pool;
extern apr_thread_mutex_t *diffusion_global_mutex;

/*
 * APPLY_ALL, maps each of the arguments to argument `t` as a single argument.
 */

#define _APPLY0(t, dummy)
#define _APPLY1(t, a) t(a)
#define _APPLY2(t, a, b) t(a) t(b)
#define _APPLY3(t, a, b, c) t(a) t(b) t(c)
#define _APPLY4(t, a, b, c, d) t(a) t(b) t(c) t(d)
#define _APPLY5(t, a, b, c, d, e) t(a) t(b) t(c) t(d) t(e)
#define _APPLY6(t, a, b, c, d, e, f) t(a) t(b) t(c) t(d) t(e) t(f)

#define _NUM_ARGS_H1(dummy, x6, x5, x4, x3, x2, x1, x0, ...) x0
#define _NUM_ARGS(...) _NUM_ARGS_H1(dummy, ##__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define _APPLY_ALL_H3(t, n, ...) _APPLY##n(t, __VA_ARGS__)
#define _APPLY_ALL_H2(t, n, ...) _APPLY_ALL_H3(t, n, __VA_ARGS__)
#define APPLY_ALL(t, ...) _APPLY_ALL_H2(t, _NUM_ARGS(__VA_ARGS__), __VA_ARGS__)

#define _DIFF_CHECK_NULL_ARG(ARG) if (!(ARG)) {DIFF_LOG("Argument " #ARG " cannot be NULL"); abort();}

/**
 * DIFF_CHECK_NULL_ARGS(arg0, arg1, argn, ...) unrolls up to 6 arguments as a discrete call to _DIFF_CHECK_NULL_ARG for each.
 */
#define DIFF_CHECK_NULL_ARGS(...) APPLY_ALL(_DIFF_CHECK_NULL_ARG, __VA_ARGS__)

#endif
