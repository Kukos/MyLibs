#ifndef ASSERT_H
#define ASSERT_H

#include <common.h>   /* tostring */
#include <stdlib.h>     /* exit */
#include <string.h>
#include <log.h>

/*
    Print stack trace on stderr

    PARAMS
    NO PARAMS

    RETURN
    This is a void function

*/
void stack_trace(void);

extern void __assert_msg(const char *msg, ...);

#ifdef ASSERT
    #define assert(expr) \
        do { \
            if ((expr) == 0) \
            { \
                __assert_msg("[ASSRT]\t%s:%s.%d\tFAILED: " tostring(expr) "\n" "%s", PATH_TO_FILE, __func__, __LINE__, ""); \
                stack_trace(); \
                exit(1); \
            } \
        } while (0)
#else
    #define assert(expr)
#endif

#endif
