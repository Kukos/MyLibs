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

/* swap A and B with size = S, use SSE if possible */
#define __SWAP__(A, B, S) \
    do { \
        (void)type_var_check(A, B); \
        if (&(A) != &(B)) \
        { \
            BYTE __buffer__[S]; \
            (void)memcpy((void *)__buffer__, (void *)&(A), (size_t)(S)); \
            (void)memcpy((void *)&(A), (void *)&(B), (size_t)(S)); \
            (void)memcpy((void *)&(B), (void *)__buffer__, (size_t)(S)); \
        } \
    } while (0)

/* A = B, A, B has size S, use SSE if possible */
#define __ASSIGN__(A, B, S) \
    do { \
        (void)type_var_check(A, B); \
        if (&(A) != &(B)) \
            (void)memcpy((void *)&(A), (void *)&(B), (size_t)(S)); \
    } while (0)

#endif
