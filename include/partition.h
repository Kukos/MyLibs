#ifndef PARTITION_H
#define PARTITION_H

/*
    Partitioning algorithms + pivot selection heuristic
    bentley partitioning is used quicksort
    hoare partition is used by select_kth

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h>
#include <sys/types.h>

/*
    Get index of pivot using trivial heuristic (first, middle, last)

    PARAMS
    @IN t - array
    @IN len - len of array
    @IN size_of - size of element in arrary
    @IN cmp - compare array elements

    RETURN
    Pivot index in t
*/
size_t partition_get_pivot_trivial(const void *t, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b));

/*
    Get index of pivot using rand

    PARAMS
    @IN t - array
    @IN len - len of array
    @IN size_of - size of element in arrary
    @IN cmp - compare array elements

    RETURN
    Pivot index in t
*/
size_t partition_get_pivot_rand(const void *t, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b));

/*
    Get index of pivot using tukey heuristic (medians of 9)

    PARAMS
    @IN t - array
    @IN len - len of array
    @IN size_of - size of element in arrary
    @IN cmp - compare array elements

    RETURN
    Pivot index in t
*/
size_t partition_get_pivot_median(const void *t, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b));

/*
    Bentley McIlory partition

    PARAMS
    @IN t - array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN cmp - compare function
    @IN size_of - sizeof type
    @OUT offset_left_index - last index of left part of unsorted array
    @OUT offset_right_index - first index of right part of unsroted array

    RETURN:
    %Pivot position iff success
    %-1 iff failure
*/
ssize_t partition_bentley(void        *t,
                          size_t      offset_left,
                          size_t      offset_right,
                          int         (*cmp)(const void *a, const void *b),
                          size_t      size_of,
                          size_t      *offset_left_index,
                          size_t      *offset_right_index);

/*
    Hoare partition

    PARAMS
    @IN t - array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN cmp - compare function
    @IN size_of - sizeof type
    @OUT offset_left_index - last index of left part of unsorted array
    @OUT offset_right_index - first index of right part of unsroted array

    RETURN:
    %Pivot position iff success
    %-1 iff failure
*/
ssize_t partition_hoare(void        *t,
                        size_t      offset_left,
                        size_t      offset_right,
                        int         (*cmp)(const void *a, const void *b),
                        size_t      size_of,
                        size_t      *offset_left_index,
                        size_t      *offset_right_index);

/*
    Lomuto partition

    PARAMS
    @IN t - array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN cmp - compare function
    @IN size_of - sizeof type
    @OUT offset_left_index - last index of left part of unsorted array
    @OUT offset_right_index - first index of right part of unsroted array

    RETURN:
    %Pivot position iff success
    %-1 iff failure
*/
ssize_t partition_lomuto(void        *t,
                         size_t      offset_left,
                         size_t      offset_right,
                         int         (*cmp)(const void *a, const void *b),
                         size_t      size_of,
                         size_t      *offset_left_index,
                         size_t      *offset_right_index);


#endif