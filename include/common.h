#ifndef COMMON_H
#define COMMON_H

/*
    Set of common useful functions

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <compiler.h>
#include <terminal_colors.h>
#include <stdlib.h> /* free */
#include <bits.h>
#include <template_array.h>
#include <stdint.h>
#include <inttypes.h>
#include <stddef.h>
#include <stdbool.h>

/* swap a with b IFF types are the same */
#define SWAP(a ,b) \
    do{ \
        (void)type_var_check(a, b); \
        typeof(a) ______temp = (a); \
        (a) = (b); \
        (b) = ______temp; \
    } while (0);

/* free & NULL */
#define FREE(PTR) \
    do { \
        free(PTR); \
        (PTR) = NULL; \
    } while (0);

/* get array size IFF is declared on stack */
#define ARRAY_SIZE(ARR) (sizeof(ARR) / sizeof(ARR[0]))

/* get min IFF types are the same */
#define MIN(a, b) \
    __extension__ \
     ({ \
        typeof(a) ______a = (a); \
        typeof(b) ______b = (b); \
        (void)type_var_check(______a, ______b); \
        ______a < ______b ? ______a : ______b; \
    })

/* get max IFF types are the same */
#define MAX(a, b) \
    __extension__ \
    ({ \
        typeof(a) ______a = (a); \
        typeof(b) ______b = (b); \
        (void)type_var_check(______a, ______b); \
        ______a > ______b ? ______a : ______b; \
    })

#define ABS(x) \
    __extension__ \
    ({ \
        typeof(x) ______x = (x); \
        ______x < 0 ? -______x : ______x; \
    })

#define ODD(x)          ((x) & 1)
#define EVEN(x)         (!((x) & 1))
#define CAST_TO_BOOL(x) (!!(x))

/* hamming weight */
#define HAMM_WEIGHT(x) \
    __extension__ \
    ({ \
        unsigned long ________x = (x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = (unsigned long)HAMM_WEIGHT_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = (unsigned long)HAMM_WEIGHT_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = (unsigned long)HAMM_WEIGHT_longlong((unsigned long long)x); \
        ________x; \
    })

#define HAMM_WEIGHT_int(n)          (number_1_int(n))
#define HAMM_WEIGHT_long(n)         (number_1_long(n))
#define HAMM_WEIGHT_longlong(n)     (number_1_longlong(n))

/* hamming distance */
#define HAMM_DIST(x, y) \
    __extension__ \
    ({ \
        unsigned long ________x = (x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = (unsigned long)HAMM_DIST_int((unsigned int)x, (unsigned int)y); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = (unsigned long)HAMM_DIST_long((unsigned long)x, (unsigned long)y); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = (unsigned long)HAMM_DIST_longlong((unsigned long long)x, (unsigned long long)y); \
        ________x; \
    })

#define HAMM_DIST_int(n, k)        (HAMM_WEIGHT_int((n) ^ (k)))
#define HAMM_DIST_long(n, k)       (HAMM_WEIGHT_long((n) ^ (k)))
#define HAMM_DIST_longlong(n, k)   (HAMM_WEIGHT_longlong((n) ^ (k)))

/* rly fast way to count log2 of n */
#define LOG2(x) \
    __extension__ \
    ({ \
        unsigned long ________x = (x); \
        if (sizeof(x) == sizeof(unsigned int)) \
            ________x = LOG2_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = LOG2_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = LOG2_longlong((unsigned long long)x); \
        ________x; \
    })

#define LOG2_int(n)             ((sizeof(typeof(n)) << 3) - (unsigned long)leading_0_int(n) - 1)
#define LOG2_long(n)            ((sizeof(typeof(n)) << 3) - (unsigned long)leading_0_long(n) - 1)
#define LOG2_longlong(n)        ((sizeof(typeof(n)) << 3) - (unsigned long)leading_0_longlong(n) - 1)

/* comparator  (takes addr of variable, so instead of int _a = (int)a, it will take int _a - *(int *)a) */
typedef int (*cmp_f)(const void *a, const void *b);

/* destructor (takes addr of variable, so instead of int *_a = (int *)a, it will take int *_a = *(int **)a) */
typedef void (*destructor_f)(void *a);

/* differ (takes addr of variable, so instead of int _a = (int)a, it will take int _a - *(int *)a) */
typedef long long (*diff_f)(const void *a, const void *b);

/*  Macro create static function for cmp 2 vars (type must by simple type) */
#define CMP(type) \
    static ___unused___ int concat(cmp_, type)(const void *a, const void *b) \
    { \
        if (a == NULL || b == NULL) \
            return -10000; \
        \
        if (*(type *)a < *(type *)b) \
            return -1; \
        \
        if (*(type *)a > *(type *)b) \
            return 1; \
        \
        return 0; \
    }

#endif
