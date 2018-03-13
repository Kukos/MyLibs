#ifndef HASH_H
#define HASH_H

/*
    Implementation of non-crypto hash functions
    Please note that all functions are modyfied to fulfill my code style
    and my optimizations style

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL 3.0
*/

#include <stdint.h>
#include <stddef.h>

/*
    Each hash function has prototype

    uint32_t hash(const void *data, size_t size);

    PARAMS
    @IN data - pointer to data to hash
    @IN size - sizeof(data)

    RETURN
    H(data) , where H is mathematical hash function
*/

#define hash(data, size)            hash_murmur(data, size)
#define hash_big_data(data, size)   hash_xxhash(data, size)

/* Bob Jenkins's one-at-a-time */
uint32_t hash_jenkins_one_at_time(const void *data, size_t size);

/* A simple hash function from Robert Sedgwicks Algorithms in C book */
uint32_t hash_rs(const void *data, size_t size);

/* A bitwise hash function written by Justin Sobel */
uint32_t hash_js(const void *data, size_t size);

/* This hash algorithm is based on work by Peter J. Weinberger of AT&T Bell Labs. Tweaked for 32-bit processors */
uint32_t hash_elf(const void *data, size_t size);

/* This is the algorithm used in the open source SDBM project */
uint32_t hash_sdbm(const void *data, size_t size);

/* An algorithm produced by Professor Daniel J. Bernstein */
uint32_t hash_djb(const void *data, size_t size);

/* An algorithm proposed by Donald E. Knuth in The Art Of Computer Programming Volume 3 */
uint32_t hash_dek(const void *data, size_t size);

/* An algorithm created by Glenn Fowler, Landon Curt Noll, and Kiem-Phong Vo */
uint32_t hash_fnv(const void *data, size_t size);

/* An algorithm produced by Arash Partow */
uint32_t hash_ap(const void *data, size_t size);

/* created by Austin Appleby The name comes from two basic operations, multiply (MU) and rotate (R), used in its inner loop */
uint32_t hash_murmur(const void *data, size_t size);

/* An algorithm created by Paul Hsieh */
/* 
 * Copyright (c) 2010, Paul Hsieh
 *
 * All rights reserved.  Redistribution and use in source and binary forms,
 * with or without modification, are permitted provided that the following
 * conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither my name, Paul Hsieh, nor the names of any other contributors to
 *    the code use may not be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
uint32_t hash_super_fast_hash(const void *data, size_t size);

/*
*  xxHash - Fast Hash algorithm
*  Copyright (C) 2012-2016, Yann Collet
*
*  BSD 2-Clause License (http://www.opensource.org/licenses/bsd-license.php)
*
*  Redistribution and use in source and binary forms, with or without
*  modification, are permitted provided that the following conditions are
*  met:
*
*  * Redistributions of source code must retain the above copyright
*  notice, this list of conditions and the following disclaimer.
*  * Redistributions in binary form must reproduce the above
*  copyright notice, this list of conditions and the following disclaimer
*  in the documentation and/or other materials provided with the
*  distribution.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*  You can contact the author at :
*  - xxHash homepage: http://www.xxhash.com
*  - xxHash source repository : https://github.com/Cyan4973/xxHash
*/
uint32_t hash_xxhash(const void *data, size_t size);

#endif