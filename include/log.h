#ifndef LOG_H
#define LOG_H

/*
	Wrapper to simple log and errors

	Author: Michal Kukowski
	email: michalkukowski10@gmail.com

	LICENCE: GPL 3.0
*/

#include <stdio.h>
#include <compiler.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*
	Modes to debug:
	DEBUG_MODE -> if defined, log works

	TRACE_MODE -> if defined trace_call works

    SILENT_ERROR -> if defined error logged errors

    Usage:
    1. First init logs: log_init
        IF you want to store logs in file pass LOG_TO_FILE to function
    2. Normal usage: TRACE, LOG, ERROR in your functions
    3. Clean log system: log_deinit

*/

#include <stdio.h> /* FILE* */
#include <string.h>

#define PATH_TO_FILE (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#define LOG_LEVEL_ALWAYS  0
#define LOG_LEVEL_CRIT    1
#define LOG_LEVEL_WARNING 2
#define LOG_LEVEL_INFO    3
#define LOG_LEVEL_NOTICE  4
#define LOG_LEVEL_DEBUG   5

#ifndef DEBUG_MODE
    #define DEBUG_LVL 0
#else
    #define DEBUG_LVL DEBUG_MODE
#endif

#define FATAL_EXIT_CODE 1

/* PRIVATE MACROS TO GET [H | T ] from __VA_ARGS__ */
#define LOG_ARGS_HEAD(...) _LOG_ARGS_HEAD(__VA_ARGS__, "")
#define _LOG_ARGS_HEAD(H, ...) H

#define LOG_ARGS_TAIL(...) _LOG_ARGS_TAIL(__VA_ARGS__, "")
#define _LOG_ARGS_TAIL(H, ...) __VA_ARGS__

/* PRIVATE LOG MACROS */
#define LOG_LEVEL(LVL, ...) _LOG_LEVEL(LVL, LOG_ARGS_HEAD(__VA_ARGS__), LOG_ARGS_TAIL(__VA_ARGS__))
#define _LOG_LEVEL(LVL, msg, ...) __LOG("[LOG %d]\t%s:%s.%d\t" msg "%s", (int)LVL, PATH_TO_FILE, __func__,  __LINE__, __VA_ARGS__)
#define __LOG(msg, ...) __log__(msg, __VA_ARGS__)

/* PRIVATE TRACE MACROS */
#define _TRACE() __trace_call__("[TRACE]\t%s:%s\n", PATH_TO_FILE, __func__)

/* PRIVATE ERROR MACROS */
#define ERROR_LOG(...) _ERROR_LOG(LVL, LOG_ARGS_HEAD(__VA_ARGS__), LOG_ARGS_TAIL(__VA_ARGS__))
#define _ERROR_LOG(LVL, msg, ...) __ERROR_LOG("[ERROR]\t%s:%s.%d\t" msg "%s%s", PATH_TO_FILE, __func__, __LINE__, __VA_ARGS__)
#define __ERROR_LOG(msg, ...) __error__(msg, __VA_ARGS__)

#define _ERROR(msg, ...) __ERROR(msg, __VA_ARGS__, "")
#define __ERROR(msg, errno, ...) \
    do { \
        ERROR_LOG(msg, __VA_ARGS__); \
        return errno; \
    } while (0)

/* PRIVATE FATAL MACROS */
#define FATAL_LOG(...) _FATAL_LOG(LVL, LOG_ARGS_HEAD(__VA_ARGS__), LOG_ARGS_TAIL(__VA_ARGS__))
#define _FATAL_LOG(LVL, msg, ...) __FATAL_LOG("[FATAL]\t%s:%s.%d\t" msg "%s%s", PATH_TO_FILE, __func__, __LINE__, __VA_ARGS__)
#define __FATAL_LOG(msg, ...) __fatal__(msg, __VA_ARGS__)

#define _FATAL(...) __FATAL(__VA_ARGS__, "")
#define __FATAL(msg, ...) \
    do { \
        FATAL_LOG(msg, __VA_ARGS__); \
        stack_trace(); \
        exit(FATAL_EXIT_CODE); \
    } while (0)

static ___inline___ void __attribute__(( format(printf,1, 2) )) __trash__(const char *msg, ...)
{
    unused_param(msg);
}

/* **************************************************** */
/* USE MACROS BELOW TO LOG YOUR CODE */
#define TRACE() _TRACE()
#define LOG(...) LOG_LEVEL(LOG_LEVEL_ALWAYS, __VA_ARGS__)
#define ERROR(msg, ...) _ERROR(msg, __VA_ARGS__)
#define FATAL(...) _FATAL(__VA_ARGS__)

#if DEBUG_LVL >= LOG_LEVEL_CRIT
    #define LOG_CRIT(...) LOG_LEVEL(LOG_LEVEL_CRIT, __VA_ARGS__)
#else
    #define LOG_CRIT(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_WARNING
    #define LOG_WARN(...) LOG_LEVEL(LOG_LEVEL_WARNING, __VA_ARGS__)
#else
    #define LOG_WARN(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_INFO
    #define LOG_INFO(...) LOG_LEVEL(LOG_LEVEL_INFO, __VA_ARGS__)
#else
    #define LOG_INFO(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_NOTICE
    #define LOG_NTCE(...) LOG_LEVEL(LOG_LEVEL_NOTICE, __VA_ARGS__)
#else
    #define LOG_NTCE(...) __trash__(__VA_ARGS__)
#endif

#if DEBUG_LVL >= LOG_LEVEL_DEBUG
    #define LOG_DEBG(...) LOG_LEVEL(LOG_LEVEL_DEBUG, __VA_ARGS__)
#else
    #define LOG_DEBG(...) __trash__(__VA_ARGS__)
#endif

/* **************************************************** */

#define NO_LOG_TO_FILE  0
#define LOG_TO_FILE     1

/*
	Simple log function
	FUNCTION WORKS IFF DEBUG_MODE is defined

	PARAMS
	@IN msg - log message

	RETURN:
	This is a void function
*/
void __attribute__(( format(printf,1, 2) )) __log__(const char *msg, ...);

/*
	Simple trace calling funcion

	FUNCTION WORKS IFF TRACE_MODE is defined

	PARAMS
	@IN msg - trace message

	RETURN:
	This is a void function
*/
void __attribute__(( format(printf,1, 2) ))  __trace_call__(const char *msg, ...);

/*
	Simple log error function
	FUNCTION WORKS IFF SILENT_ERROR is not defined

	PARAMS
	@IN msg - log message

	RETURN:
	This is a void function
*/
void __attribute__(( format(printf,1, 2) ))  __error__(const char *msg, ...);

/*
	Simple log fatal function
	FUNCTION works always

	PARAMS
	@IN msg - log message

	RETURN:
	This is a void function
*/
void __attribute__(( format(printf,1, 2) ))  __fatal__(const char *msg, ...);


/*
    initialize log system

    PARAMS
    @IN fd - FILE*, if NULL use default stderr
    @IN to_file log to special file ? LOG_TO_FILE - yes, NO_LOG_TO_FILE no

    RETURN
    0 iff success
    non-zero value iff failure
*/
int log_init(const FILE *fd, int to_file);

/*
    deinit log system

    PARAMS
    NO PARAMS

    RETURN
    This is a void function
*/
void log_deinit(void);

/*
    GET FD connected with logger

    PARAMS
    @NO PARAMS

    RETURN
    FD connected with logger or NULL
*/
FILE *log_get_fd(void);

/*
    GET FILE connected with logger

    PARAMS
    @NO PARAMS

    RETURN
    FILE connected with logger or NULL
*/
FILE *log_get_file(void);

#endif
