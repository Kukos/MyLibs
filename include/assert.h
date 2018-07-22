#ifndef ASSERT_H
#define ASSERT_H

#include <common.h>   /* tostring */
#include <stdlib.h>     /* exit */
#include <string.h>

/*
    Print stack trace on stderr

    PARAMS
    NO PARAMS

    RETURN
    This is a void function

*/
void stack_trace(void);

#define ASSERT_MAX_FUNC_SPACE   20

extern void __assert_msg(const char *msg, ...);

#define ASSERT_MSG \
    "[ASSERT]\tFILE: %s\n\tFUNC: %s%*.sLINE: %d\n", __FILE__, __func__, \
             (int)(ASSERT_MAX_FUNC_SPACE - strlen(__func__)), "", __LINE__

#ifdef ASSERT
    #define assert(expr) \
        do { \
            if ((expr) == 0) \
            { \
                __assert_msg(ASSERT_MSG); \
                __assert_msg("FAILED:\t" tostring(expr) "\n" "%s", ""); \
                stack_trace(); \
                exit(1); \
            } \
        } while (0);
#else
    #define assert(expr)
#endif

#endif
