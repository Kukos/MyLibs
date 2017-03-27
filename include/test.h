#ifndef TEST_H
#define TEST_H

/*
    Simple test framework

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <common.h>
#include <stdio.h>
#include <compiler.h>

/* PRIVATE MACRO */
#define ____MOCK_F____(func) concat(___mock___, func)

typedef int test_t;

#define PASSED 0
#define FAILED 1

/*
    Use this macro to call your test func,

    example:
    test_t test1(void)
    {
        ...
        return PASSED;
    }

    TEST(test1());

*/
#define TEST(func) \
    do { \
        if (func == FAILED) \
            printf("[TEST]\t%s\t" RED "FAILED" RESET "\n", #func); \
        else \
            printf("[TEST]\t%s\t" GREEN "PASSED" RESET "\n", #func); \
    } while(0);


/*
    Use this macro to call funtion @func and cmp ret value with @val
*/
#define EXPECT(func, val) \
    __extension__ ({ \
            (func != val) ? \
            (CAST_TO_BOOL(printf("[TEXPECT]\t%s return bad value\n", #func))) : 0; \
    })

/*
    Use this macro to compare val and expected val
*/
#define ASSERT(val, expt) \
    __extension__ ({ \
        (val != expt) ? \
        (CAST_TO_BOOL(printf("[TASSERT]\t%s bad value\n", __func__))) : 0; \
    })

/*
    Use this macro to mock funtion
*/
#define MOCK(func, val) \
    typeof(val) ____MOCK_F____(func)(__unused__ int guard, ...) { return val; }

/*
    Use this macro to mock void funtion
*/
#define MOCK_V(func) \
    void ____MOCK_F____(func)(__unused__ int guard, ...) { }

/*
    Use this macro to call mock fuction
*/
#define CALL(func, ...) \
    ____MOCK_F____(func)(0, ##__VA_ARGS__)


#endif
