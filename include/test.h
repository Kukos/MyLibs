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

#define __TEST_COLOR_INFO__     COLOR_CYAN
#define __TEST_COLOR_PASSED__   COLOR_GREEN
#define __TEST_COLOR_FAILED__   COLOR_RED
#define __TEST_SUMMARY_TEXT__   " TEST SUMMARY "

/* private simple strlen to avoid including string.h */
___inline___ long ______test_strlen______(const char *str)
{
    long len = 0;
    while (*str++ != '\0')
        ++len;

    return len;
}

/* PRIVATE MACRO */
#define ____MOCK_F____(func) concat(___mock___, func)

/* PRIVATE MACRO */
#define ____TEST_SUMMARY(COLOR, STR_RESULT) \
    do { \
        printf( COLOR "%.*s" __TEST_SUMMARY_TEXT__ "%.*s\n", \
                ((int)(__TEST_MAX_STRING_LENGTH__ - \
                    ______test_strlen______(__TEST_SUMMARY_TEXT__)) >> 1), \
                ________chars, \
                ((int)(__TEST_MAX_STRING_LENGTH__ - \
                    ______test_strlen______(__TEST_SUMMARY_TEXT__) + 1) >> 1), \
                ________chars \
               ); \
        printf("TESTS EXECUTED:\t%ld\n", ________passed_counter + ________failed_counter); \
        printf("PASSED:\t\t%ld / %ld\n", ________passed_counter, ________passed_counter + ________failed_counter); \
        printf("FAILED:\t\t%ld / %ld\n", ________failed_counter, ________passed_counter + ________failed_counter); \
        printf(STR_RESULT); \
        printf("%.*s\n" COLOR_RESET, __TEST_MAX_STRING_LENGTH__, ________chars); \
    } while(0);

/* private type */
typedef unsigned long ____test_counter_t;

/* PRIVATE counters for testing, needed in summary */
____test_counter_t ________passed_counter;
____test_counter_t ________failed_counter;

#define __TEST_MAX_STRING_LENGTH__ 40

/* to fill gap with ' ' */
const char const *________spaces = "                                              ";
const char const *________chars = "===============================================";

/* use this type for internal test variables */
typedef unsigned long long test_t;

/* use this type for test function type */
typedef void test_f;

test_t ________ret;

#define PASSED 0
#define FAILED 1

/*
    USe this macro before call test functions
*/
#define TEST_INIT(fmt) \
    do { \
        ________passed_counter = 0; \
        ________failed_counter = 0; \
        printf( __TEST_COLOR_INFO__ "%.*s %s %.*s\n" COLOR_RESET, \
                ((int)(__TEST_MAX_STRING_LENGTH__ - \
                    ______test_strlen______(fmt) - 2) >> 1), \
                ________chars, \
                fmt, \
                ((int)(__TEST_MAX_STRING_LENGTH__ - \
                    ______test_strlen______(fmt) - 1) >> 1), \
                ________chars \
               ); \
    } while (0);

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
        ________ret = 0; \
        func; \
        test_t ______ret = ________ret ? FAILED : PASSED; \
        if (______ret == FAILED) \
        { \
            printf( __TEST_COLOR_INFO__ "[TEST]\t%s%.*s" \
                    __TEST_COLOR_FAILED__ "FAILED" COLOR_RESET "\n", \
                        tostring(func), \
                        (int)(__TEST_MAX_STRING_LENGTH__ - \
                            ______test_strlen______(tostring(func))), \
                        ________spaces); \
            ++________failed_counter; \
        } \
        else \
        { \
            printf( __TEST_COLOR_INFO__ "[TEST]\t%s%.*s" \
                    __TEST_COLOR_PASSED__ "PASSED" COLOR_RESET "\n", \
                        tostring(func), \
                        (int)( __TEST_MAX_STRING_LENGTH__ - \
                            ______test_strlen______(tostring(func))), \
                        ________spaces); \
            ++________passed_counter; \
        } \
    } while(0);

/*
    Use this macro at the end of your tests to get tests summary
*/
#define TEST_SUMMARY \
    do { \
        if (________failed_counter) \
        { \
            ____TEST_SUMMARY(__TEST_COLOR_FAILED__, "TESTS FAILED\n"); \
            return ________failed_counter; \
        } \
        else \
        { \
            ____TEST_SUMMARY(__TEST_COLOR_PASSED__, "TESTS PASSED\n"); \
            return 0; \
        } \
    } while(0);

/*
    Use this macro to call funtion @func and cmp ret value with @val
*/
#define T_EXPECT(func, val) \
    __extension__ \
    ({ \
            typeof(func) ______ret = func; \
            typeof(val)  ______val = (val); \
            if (______ret != ______val) \
                ++________ret; \
            (______ret != ______val) ? \
            (CAST_TO_BOOL(printf( "[TEXPECT]\t%s return bad value\n" \
                                  "RETURN %#llx\tEXPECTED %#llx\t\tLINE = %d\n", \
                                  tostring(func), \
                                  (test_t)(______ret), \
                                  (test_t)(______val), \
                                  __LINE__ \
                                ))) : 0; \
    })

/*
    Use this macro to compare val and expected val
*/
#define T_ASSERT(val, expt) \
    __extension__ \
    ({ \
        typeof(val) ______val = (val); \
        typeof(expt) ______expt = (expt); \
        if (______val != ______expt) \
            ++________ret; \
        (______val != ______expt) ? \
        (CAST_TO_BOOL(printf( "[TASSERT]\t%s != %s\n" \
                              "RETURN %#llx\tEXPECTED %#llx\t\tLINE = %d\n", \
                              tostring(val), \
                              tostring(expt), \
                              (test_t)(______val), \
                              (test_t)(______expt), \
                              __LINE__ \
                            ))) : 0; \
    })

/*
    Use this macro like assert, if expr is true then test failed
*/
#define T_CHECK(expr) \
    do { \
        if (!(expr)) \
        { \
            printf("[T_CHECK]\tNOT\t%s\nLINE = %d\n", tostring(expr), __LINE__); \
            ++________ret; \
        } \
    } while (0);

/*
    Use this macro like assert, if expr is true then test failed and exit test
*/
#define T_ERROR(expr) \
    do { \
        if (expr) \
        { \
            printf("[T_ERROR]\t%s\nLINE = %d\n", tostring(expr), __LINE__); \
            ++________ret; \
            return; \
        } \
    } while (0);

/*
    Use this macro to mock funtion
*/
#define T_MOCK(func, val) \
    typeof(val) ____MOCK_F____(func)(___unused___ int guard, ...) { return (val); }

/*
    Use this macro to mock void funtion
*/
#define T_MOCK_V(func) \
    void ____MOCK_F____(func)(___unused___ int guard, ...) { }

/*
    Use this macro to call mock fuction
*/
#define T_CALL(func, ...) \
    ____MOCK_F____(func)(0, ##__VA_ARGS__)


#endif
