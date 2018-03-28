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

/* bits operations */
#define BIT(n)          (1ull << (n))
#define GET_BIT(n ,k)   (((n) & BIT(k)) >> (k))
#define SET_BIT(n, k)   ((n) |= BIT(k))
#define CLEAR_BIT(n, k) ((n) &= ~BIT(k))

#define SET_BIT_VALUE(n, k, val) \
    __extension__ \
    ({ \
        if (val) \
            SET_BIT(n, k); \
        else \
            CLEAR_BIT(n, k); \
    })

#define FLIP_BIT(n, k) (SET_BIT_VALUE(n, k, !(GET_BIT(n, k))))

#define GET_FLAG(n, flag)   CAST_TO_BOOL((n) & (flag))
#define SET_FLAG(n, flag)   ((n) |= (flag))
#define CLEAR_FLAG(n, flag) ((n) &= ~(flag))

#define REVERSE_BITS(x) \
    __extension__ \
    ({ \
        const typeof(x) ____fake = (x); \
        uint64_t        ____x = (uint64_t)____fake; \
        const uint64_t  ____mask1 = 0x5555555555555555ULL; \
        const uint64_t  ____mask2 = 0x3333333333333333ULL; \
        const uint64_t  ____mask3 = 0x0F0F0F0F0F0F0F0FULL; \
        const uint64_t  ____mask4 = 0x00FF00FF00FF00FFULL; \
        const uint64_t  ____mask5 = 0x0000FFFF0000FFFFULL; \
        const uint64_t  ____mask6 = 0x00000000FFFFFFFFULL; \
        const size_t    ____size = sizeof(____fake) << 3; \
        const uint64_t  ____guard = (sizeof(____fake) == sizeof(uint64_t) ? ((uint64_t)-1) : (BIT(____size) - 1)); \
        \
        switch(____size) \
        { \
            case 64: \
                ____x = ((____x >> 32) & (____mask6 & ____guard)) | ((____x & (____mask6 & ____guard)) << 32); \
            case 32: \
                ____x = ((____x >> 16) & (____mask5 & ____guard)) | ((____x & (____mask5 & ____guard)) << 16); \
            case 16: \
                ____x = ((____x >>  8) & (____mask4 & ____guard)) | ((____x & (____mask4 & ____guard)) <<  8); \
            case 8: \
            { \
                ____x = ((____x >>  4) & (____mask3 & ____guard)) | ((____x & (____mask3 & ____guard)) <<  4); \
                ____x = ((____x >>  2) & (____mask2 & ____guard)) | ((____x & (____mask2 & ____guard)) <<  2); \
                ____x = ((____x >>  1) & (____mask1 & ____guard)) | ((____x & (____mask1 & ____guard)) <<  1); \
                break; \
            } \
            default: \
                break; \
        } \
        (typeof(x))____x; \
    })

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
            ________x = HAMM_WEIGHT_int((unsigned int)x); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = HAMM_WEIGHT_long((unsigned long)x); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = HAMM_WEIGHT_longlong((unsigned long long)x); \
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
            ________x = HAMM_DIST_int((unsigned int)x, (unsigned int)y); \
        else if (sizeof(x) == sizeof(unsigned long)) \
            ________x = HAMM_DIST_long((unsigned long)x, (unsigned long)y); \
        else if (sizeof(x) == sizeof(unsigned long long)) \
            ________x = HAMM_DIST_longlong((unsigned long long)x, (unsigned long long)y); \
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

/*
    Macro create static function for cmp whole array (type must by simple type)
*/
#define ARRAY_EQUAL(type) \
    static ___unused___ bool concat(array_equal_, type)(const type *t1, const type *t2, size_t size) \
    { \
        size_t i; \
        \
        if (t1 == NULL || t2 == NULL) \
            return false; \
        \
        for (i = 0; i < size; ++i) \
            if (t1[i] != t2[i]) \
                return false; \
        \
        return true; \
    }

/*
    Macro create static function for reversing whole array (type must by simple type)
*/
#define ARRAY_REVERSE(type) \
    static ___unused___ void concat(array_reverse_, type)(type *t, size_t size) \
    { \
        size_t i; \
        \
        if (t == NULL) \
            return; \
        \
        for (i = 0; i < (size >> 1); ++i) \
            SWAP(t[i], t[size - i - 1]); \
    }

/*
    Macro create static function for cmp 2 vars (type must by simple type)
*/
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
