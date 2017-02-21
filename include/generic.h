#ifndef GENERIC_H
#define GENERIC_H

/*
	Framework to make libs generic for every primitive types

	Author: Michal Kukowski
	email: michalkukowski10@gmail.com

	LICENCE: GPL 3.0+
*/

#include <string.h> /* SSE instruction */

/* get void* size to get architecture */
#if __SIZEOF_POINTER__ == 8
	#define __ARCH_64__
#else
	#define __ARCH_32__
#endif

/* define types like "asm" */
#ifdef __ARCH_64__

    #define BYTE			char
    #define HALF_WORD 		short
    #define WORD 			int
    #define DWORD 			long
    #define QWORD 			long double

    #define BYTE_SIZE 		sizeof(BYTE)
    #define HALF_WORD_SIZE	sizeof(HALF_WORD)
    #define WORD_SIZE		sizeof(WORD)
    #define DWORD_SIZE		sizeof(DWORD)
    #define QWORD_SIZE		sizeof(QWORD)

    #define MAXWORD			sizeof(QWORD)

#elif defined(__ARCH_32__)

    #define BYTE 			char
    #define WORD 			short
    #define DWORD 			long
    #define QWORD 			long long
    #define SIXWORD 		long double

    #define BYTE_SIZE		sizeof(BYTE)
    #define WORD_SIZE		sizeof(WORD)
    #define DWORD_SIZE		sizeof(DWORD)
    #define QWORD_SIZE		sizeof(QWORD)
    #define SIXWORD_SIZE	sizeof(SIXWORD)

    #define MAXWORD			sizeof(SIXWORD)

#endif

/* buffer for swapping */
__attribute__ ((unused)) static BYTE __buffer__[MAXWORD];

/* swap A and B with size = S, use SSE if possible */
#define __SWAP__(A, B, S) \
	do { \
    	if (&A != &B) \
        { \
            (void)memcpy(__buffer__, &A, S); \
            (void)memcpy(&A, &B, S); \
            (void)memcpy(&B, __buffer__, S); \
        } \
    } while (0)

/* A = B, A, B has size S, use SSE if possible */
#define __ASSIGN__(A, B, S) \
	do { \
        if (&A != &B) \
            (void)memcpy(&A, &B, S); \
    } while (0)

#endif
