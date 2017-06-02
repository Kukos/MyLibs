#ifndef ASSERT_H
#define ASSERT_H

#include <compiler.h>   /* tostring */
#include <stdlib.h>     /* exit */

/*
    Print stack trace on stderr

    PARAMS
    NO PARAMS

    RETURN
    This is a void function

*/
void stack_trace(void);

/* private simple strlen to avoid including string.h */
___inline___ long ______assert_strlen______(const char *str);

___inline___ long ______assert_strlen______(const char *str)
{
    long len = 0;
    while (*str++ != '\0')
        ++len;

    return len;
}

#define ASSERT_MAX_FUNC_SPACE   20

extern void __assert_msg(const char *msg, ...);

#include <string.h>

#define ASSERT_MSG \
    "[ASSERT]\tFILE: %s\n\tFUNC: %s%*.sLINE: %d\n", __FILE__, __func__, \
             (int)(ASSERT_MAX_FUNC_SPACE - ______assert_strlen______(__func__)), "", __LINE__

#ifdef ASSERT
    #define assert(expr) \
        do { \
            if ((expr) == 0) \
            { \
                __assert_msg(ASSERT_MSG); \
                __assert_msg("FAILED:\t" tostring(expr) "\n", ""); \
                stack_trace(); \
                exit(1); \
            } \
        } while (0);
#else
    #define assert(expr)
#endif

#endif
