#ifndef CCACHE_H
#define CCACHE_H

/*
    Contiguous Cache in memory full RAM or RAM + file

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL3.0
*/

#include <stddef.h>
#include <common.h>
#include <stdbool.h>
#include <sys/types.h>

typedef struct CCache CCache;

/*
    Create CCache

    PARAMS
    @IN size - ccache size

    RETURN
    NULL iff failure
    Pointer to new CCache iff success
*/
CCache *ccache_create(size_t size);

/*
    Destroy CCache

    PARAMS
    @IN cache - pointer to CCache

    RETURN
    This is a void function
*/
void ccache_destroy(CCache *cache);

/*
    Get size of Cache

    PARAMS
    @IN cache - pointer to CCache

    RETURN
    -1 iff failure
    Cache size iff success
*/
ssize_t ccache_get_size(CCache *cache);

/*
    Get Cache array

    PARAMS
    @IN cache - pointer to CCache
    @OUT size - array size

    RETURN
    NULL iff failure
    Pointer to array iff success
*/
void *ccache_get_array(CCache *cache, size_t *size);

#endif