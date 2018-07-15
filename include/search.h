#ifndef SEARCH_H
#define SEARCH_H

/*
    Implementation of basic search algorithms

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

#include <stddef.h>
#include <sys/types.h>
#include <common.h>

/*
    Find First occurrence of @key in array @t
    Array is unsorted so linear algorithm will be use

    PARAMS
    @IN key - data with key to find
    @IN t - array
    @IN num_elements - number of elements
    @IN cmp - compare func
    @IN size_of - data size of

    RETURN
    -1 iff key doesn't exist in array
    Index in array where is key
*/
ssize_t find_first_unsorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, int(*cmp)(const void *a, const void *b), size_t size_of);

/*
    Find Last occurrence of @key in array @t
    Array is unsorted so linear algorithm will be use

    PARAMS
    @IN key - data with key to find
    @IN t - array
    @IN num_elements - number of elements
    @IN cmp - compare func
    @IN size_of - data size of

    RETURN
    -1 iff key doesn't exist in array
    Index in array where is key
*/
ssize_t find_last_unsorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, int(*cmp)(const void *a, const void *b), size_t size_of);

/*
    Find First occurrence of @key in array @t
    Assume that array is sorted so logarithmic time will be use

    PARAMS
    @IN key - data with key to find
    @IN t - array
    @IN num_elements - number of elements
    @IN cmp - compare func
    @IN size_of - data size of

    RETURN
    -1 iff key doesn't exist in array
    Index in array where is key
*/
ssize_t find_first_sorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, int(*cmp)(const void *a, const void *b), size_t size_of);

/*
    Find Last occurrence of @key in array @t
    Assume that array is sorted so logarithmic time will be use

    PARAMS
    @IN key - data with key to find
    @IN t - array
    @IN num_elements - number of elements
    @IN cmp - compare func
    @IN size_of - data size of

    RETURN
    -1 iff key doesn't exist in array
    Index in array where is key
*/
ssize_t find_last_sorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, int(*cmp)(const void *a, const void *b), size_t size_of);


#endif