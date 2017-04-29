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

/* private simple strlen to avoid including string.h */
__inline__ long ______test_strlen______(const char *str)
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
        printf(COLOR "==========" \
                     " TEST SUMMARY " \
                     "==========\n"); \
        printf("TESTS EXECUTED:\t%ld\n", ______passed_counter + ______failed_counter); \
        printf("PASSED:\t\t%ld / %ld\n", ______passed_counter, ______passed_counter + ______failed_counter); \
        printf("FAILED:\t\t%ld / %ld\n", ______failed_counter, ______passed_counter + ______failed_counter); \
        printf(STR_RESULT); \
        printf("==================================\n" RESET); \
    } while(0);

/* private type */
typedef unsigned long ____test_counter_t;

/* PRIVATE counters for testing, needed in summary */
____test_counter_t ______passed_counter;
____test_counter_t ______failed_counter;

#define __TEST_MAX_STRING_LENGTH__ 40

/* to fill gap with ' ' */
const char const *______spaces = "                                              ";

/* use this type for test functions */
typedef unsigned long long test_t;

#define PASSED 0
#define FAILED 1

/*
    USe this macro before call test functions
*/
#define TEST_INIT \
    ______passed_counter = 0; \
    ______failed_counter = 0;

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
        { \
            printf(CYAN "[TEST]\t%s%.*s" RED "FAILED" RESET "\n", \
                        tostring(func), \
                        (int)(__TEST_MAX_STRING_LENGTH__ - \
                            ______test_strlen______(tostring(func))), \
                        ______spaces); \
            ++______failed_counter; \
        } \
        else \
        { \
            printf(CYAN "[TEST]\t%s%.*s" GREEN "PASSED" RESET "\n", \
                        tostring(func), \
                        (int)( __TEST_MAX_STRING_LENGTH__ - \
                            ______test_strlen______(tostring(func))), \
                        ______spaces); \
            ++______passed_counter; \
        } \
    } while(0);

/*
    Use this macro at the end of your tests to get tests summary
*/
#define TEST_SUMMARY \
    do { \
        if (______failed_counter) \
        { \
            ____TEST_SUMMARY(RED, "TESTS FAILED\n"); \
        } \
        else \
        { \
            ____TEST_SUMMARY(GREEN, "TESTS PASSED\n"); \
        } \
    } while(0);

/*
    Use this macro to call funtion @func and cmp ret value with @val
*/
#define T_EXPECT(func, val) \
    __extension__ ({ \
            typeof(func) ______ret = func; \
            (______ret != (val)) ? \
            (CAST_TO_BOOL(printf( "[TEXPECT]\t%s return bad value\n" \
                                  "RETURN %#llx\tEXPECTED %#llx\n", \
                                  tostring(func), \
                                  (test_t)(______ret), \
                                  (test_t)(val) \
                                ))) : 0; \
    })

/*
    Use this macro to compare val and expected val
*/
#define T_ASSERT(val, expt) \
    __extension__ ({ \
        ((val) != (expt)) ? \
        (CAST_TO_BOOL(printf( "[TASSERT]\t%s != %s\n" \
                              "RETURN %#llx\tEXPECTED %#llx\n", \
                              tostring(val), \
                              tostring(expt), \
                              (test_t)(val), \
                              (test_t)(expt) \
                            ))) : 0; \
    })

/*
    Use this macro to mock funtion
*/
#define T_MOCK(func, val) \
    typeof(val) ____MOCK_F____(func)(__unused__ int guard, ...) { return (val); }

/*
    Use this macro to mock void funtion
*/
#define T_MOCK_V(func) \
    void ____MOCK_F____(func)(__unused__ int guard, ...) { }

/*
    Use this macro to call mock fuction
*/
#define T_CALL(func, ...) \
    ____MOCK_F____(func)(0, ##__VA_ARGS__)


#endif
