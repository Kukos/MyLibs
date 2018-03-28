#ifndef BITS_H
#define BITS_H

/*
    Header for bits operation created as macros

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdint.h>

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

#endif
