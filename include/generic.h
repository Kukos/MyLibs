#ifndef GENERIC_H
#define GENERIC_H

/*
	Framework to make libs generic for every primitive types

	Author: Michal Kukowski
	email: michalkukowski10@gmail.com

	LICENCE: GPL 3.0
*/

#include <string.h> /* SSE instruction */
#include <stdint.h>
#include <compiler.h>

/* get void* size to get architecture */
#if __SIZEOF_POINTER__ == 8
    #define __ARCH_64__
#else
    #define __ARCH_32__
#endif

/* define types like "asm" */
#ifdef __ARCH_64__

    #define BYTE			uint8_t
    #define HALF_WORD 		uint16_t
    #define WORD 			uint32_t
    #define DWORD 			uint64_t
    #define QWORD 			__uint128_t /* gcc extensions */

    #define BYTE_SIZE 		sizeof(BYTE)
    #define HALF_WORD_SIZE	sizeof(HALF_WORD)
    #define WORD_SIZE		sizeof(WORD)
    #define DWORD_SIZE		sizeof(DWORD)
    #define QWORD_SIZE		sizeof(QWORD)

    #define MAXWORD			sizeof(QWORD)

#elif defined(__ARCH_32__)

    #define BYTE 			uint8_t
    #define WORD 			uint16_t
    #define DWORD 			uint32_t
    #define QWORD 			uint64_t

    #define BYTE_SIZE		sizeof(BYTE)
    #define WORD_SIZE		sizeof(WORD)
    #define DWORD_SIZE		sizeof(DWORD)
    #define QWORD_SIZE		sizeof(QWORD)

    #define MAXWORD			sizeof(QWORD)

#endif

/* A = B, A, B has size S, use SSE if possible */
#define __ASSIGN__(A, B, S) \
    do { \
        _Pragma("GCC diagnostic push"); \
        _Pragma("GCC diagnostic ignored \"-Wstrict-aliasing\""); \
        (void)type_var_check(A, B); \
        switch (S) \
        { \
            case 1: *(uint8_t *)&A = *(uint8_t *)&B; break; \
            case 2: *(uint16_t *)&A = *(uint16_t *)&B; break; \
            case 4: *(uint32_t *)&A = *(uint32_t *)&B; break; \
            case 8: *(uint64_t *)&A = *(uint64_t *)&B; break; \
            default: (void)memcpy((void *)&A, (void *)&B, (size_t)(S)); \
        } \
        _Pragma("GCC diagnostic pop"); \
    } while (0)

/* swap A and B with size = S, use SSE if possible */
#define __SWAP__(A, B, S) \
    do { \
        (void)type_var_check(A, B); \
        if (&(A) != &(B)) \
        { \
            BYTE __buffer__[S]; \
            __ASSIGN__(__buffer__[0], A, S); \
            __ASSIGN__(A, B, S); \
            __ASSIGN__(B, __buffer__[0], S); \
        } \
    } while (0)

#endif
