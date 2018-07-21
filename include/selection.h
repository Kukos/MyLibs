#ifndef SELECTION_H
#define SELECTION_H

/*
    Implementation of selection algorithms like select_kth

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <stddef.h>
#include <sys/types.h>
#include <common.h>

/*
    Get index of kth positional statistic of array t

    PARAMS
    @IN t - array
    @IN len - array len
    @IN size_of - size_of array element
    @IN cmp - compare function
    @IN k - kth element to find (from 0 to len - 1)

    RETURN
    -1 iff failure
    Index of kth element in array iff success
*/
ssize_t select_kth(const void *t, size_t len, size_t size_of, size_t k, cmp_f cmp);

#endif