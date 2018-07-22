#ifndef ARRAY_H
#define ARRAY_H

/*
    Implementation of basic array functions

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h>
#include <sys/types.h>
#include <common.h>
#include <generic.h>
#include <stdbool.h>

/* PRIVATE MACRO DO NOT USE DIRECTLY */
#define __ARRAY_ASSIGN__(A, B, S) \
__extension__ \
    ({ \
        __ASSIGN__(A, B, S); \
        1; \
    })

#define ARRAY_IT_NAME concat(__i, __LINE__)

/*
    FOR_EACH like iterators

    PARAMS
    @IN t - array type [] or type * or void *
    @IN len - array len
    @IN size_of - members size of
    @OUT ptr - ptr to next element
    @OUT data - value of next element
 */
#define for_each_array(t, len, size_of, ptr, data)  for (size_t ARRAY_IT_NAME = 0, _size_of = size_of; \
                                                         ARRAY_IT_NAME < len && \
                                                         (__ARRAY_ASSIGN__(*(BYTE *)&data, ((BYTE *)t)[ARRAY_IT_NAME * _size_of], _size_of) || true) && \
                                                         ((ptr = (typeof(ptr))&((BYTE *)t)[ARRAY_IT_NAME * _size_of]) || true); \
                                                         ++ARRAY_IT_NAME)

#define for_each_data_array(t, len, size_of, data)  for (size_t ARRAY_IT_NAME = 0, _size_of = size_of; \
                                                         ARRAY_IT_NAME < len && \
                                                         (__ARRAY_ASSIGN__(*(BYTE *)&data, ((BYTE *)t)[ARRAY_IT_NAME * _size_of], _size_of) || true);\
                                                         ++ARRAY_IT_NAME)


#define for_each_ptr_array(t, len, size_of, ptr)    for (size_t ARRAY_IT_NAME = 0, _size_of = size_of; \
                                                         ARRAY_IT_NAME < len && \
                                                         ((ptr = (typeof(ptr))&((BYTE *)t)[ARRAY_IT_NAME * _size_of]) || true); \
                                                         ++ARRAY_IT_NAME)


#define for_each_prev_array(t, len, size_of, ptr, data) for (size_t ARRAY_IT_NAME = 1, _size_of = size_of; \
                                                             ARRAY_IT_NAME <= len && \
                                                             (__ARRAY_ASSIGN__(*(BYTE *)&data, ((BYTE *)t)[(len - ARRAY_IT_NAME) * _size_of], _size_of) || true) && \
                                                             ((ptr = (typeof(ptr))&((BYTE *)t)[(len - ARRAY_IT_NAME) * _size_of]) || true); \
                                                             ++ARRAY_IT_NAME)

#define for_each_data_prev_array(t, len, size_of, data) for (size_t ARRAY_IT_NAME = 1, _size_of = size_of; \
                                                             ARRAY_IT_NAME <= len && \
                                                             (__ARRAY_ASSIGN__(*(BYTE *)&data, ((BYTE *)t)[(len - ARRAY_IT_NAME) * _size_of], _size_of) || true); \
                                                             ++ARRAY_IT_NAME)

#define for_each_ptr_prev_array(t, len, size_of, ptr)   for (size_t ARRAY_IT_NAME = 1, _size_of = size_of; \
                                                             ARRAY_IT_NAME <= len && \
                                                             ((ptr = (typeof(ptr))&((BYTE *)t)[(len - ARRAY_IT_NAME) * _size_of]) || true); \
                                                             ++ARRAY_IT_NAME)


/*
    Create new array with @len of members with size @size_of

    PARAMS
    @IN len - array len
    @IN size_of - members size_of

    RETURN
    NULL iff failure
    Pointer to new array iff success
*/
void *array_create(size_t len, size_t size_of);

/*
    Check iff arrays are equal (all members has the same keys)

    PARAMS
    @IN array1 - array1
    @IN array2 - array2
    @IN len - len of array1 and array2
    @IN size_of - size of members
    @IN cmp - cmp

    RETURN
    true iff arrays are equal
    false iff arrays are not equal
*/
bool array_equal(const void *array1, const void *array2, size_t len, size_t size_of, cmp_f cmp);

/*
    Shuffle array

    PARAMS
    @IN array - array to clone
    @IN len - array len
    @IN size_of - size of  member

    RETURN
    This is a void funciton
*/
void array_shuffle(void *array, size_t len, size_t size_of);

/*
    Copy array from @src to @dst

    PARAMS
    @IN dst - dest array
    @IN src - src array
    @IN len - len of array to copy
    @IN size_of - members size_of

    RETURN
    This is a void funciton
*/
void array_copy(void * ___restrict___ dst, const void * ___restrict___ src, size_t len, size_t size_of);

/*
    Move array from src pointer to dst pointer (Can overlap)

    PARAMS
    @IN dst - dest array
    @IN src - src array
    @IN len - len of array to copy
    @IN size_of - members size_of

    RETURN
    This is a void funciton
*/
void array_move(void *dst, void *src,  size_t len, size_t size_of);

/*
    memset to 0 whole array

    PARAMS
    @IN array - array to clone
    @IN len - array len
    @IN size_of - size of  member

    RETURN
    This is a void funciton
*/
void array_zeros(void *array, size_t len, size_t size_of);

/*
    Set data to all members

    PARAMS
    @IN array - array
    @IN len - len
    @IN size_of - size of  member
    @IN data - data to insert

    RETURN
    This is a void function
*/
void array_set_all(void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ data);

/*
    Clone @array to another array (this function allocate memory for new array)

    PARAMS
    @IN array - array to clone
    @IN len - array len
    @IN size_of - size of  member

    RETURN
    NULL iff failure
    Pointer to new cloned array iff success
*/
void *array_clone(const void *array, size_t len, size_t size_of);

/*
    Destroy array

    PARAMS
    @IN array - array

    RETURN
    This is a void function
*/
void array_destroy(void *array);

/*
    Destroy array with entries

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - member size
    @IN destructor - element destructor

    RETURN
    This is a void function
*/
void array_destroy_with_entries(void *array, size_t len, size_t size_of, destructor_f destructor);

/*
    Insert data at first pos

    PARAMS
    @IN array - array
    @IN len - len
    @IN size_of - members size
    @IN data - data to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_unsorted_insert_first(void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ data);

/*
    Insert data at last pos

    PARAMS
    @IN array - array
    @IN len - len
    @IN size_of - members size
    @IN data - data to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_unsorted_insert_last(void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ data);

/*
    Insert data at @pos

    PARAMS
    @IN array - array
    @IN len - len
    @IN size_of - members size
    @IN pos - pos to insert
    @IN data - data to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_unsorted_insert_pos(void * ___restrict___ array, size_t len, size_t size_of, size_t pos, const void * ___restrict___ data);

/*
    USE only iff array is sorted

    Insert data into correct place to sorted array

    PARAMS
    @IN array - array
    @IN len - len
    @IN size_of - members size
    @IN data - data to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_sorted_insert(void * ___restrict___ array, size_t len, size_t size_of, cmp_f cmp, const void * ___restrict___ data);

/*
    Delete first entry (move array, so at first pos will be 2nd entry etc (last will be empty))

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_delete_first(void *array, size_t len, size_t size_of);

/*
    Delete last entry

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_delete_last(void *array, size_t len, size_t size_of);

/*
    Delete  entry at @pos (move array, so at pos will be (pos + 1)th entry etc (last will be empty))

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_delete_pos(void *array, size_t len, size_t size_of, size_t pos);

/*
    Delete first entry (move array, so at first pos will be 2nd entry etc (last will be empty))
    Also call destructor and destroy member

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members
    @IN destructor - member destructor

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_delete_first_with_entry(void *array, size_t len, size_t size_of, destructor_f destructor);

/*
    Delete last entry Also call destructor and destroy member

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members
    @IN destructor - member destructor

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_delete_last_with_entry(void *array, size_t len, size_t size_of, destructor_f destructor);

/*
    Delete  entry at @pos (move array, so at pos will be (pos + 1)th entry etc (last will be empty))
    Also call destructor and destroy member

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members
    @IN destructor - member destructor

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_delete_pos_with_entry(void *array, size_t len, size_t size_of, size_t pos, destructor_f destructor);

/*
    Reverse array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size of members

    RETURN
    This is a void function
*/
void array_reverse(void *array, size_t len, size_t size_of);

/*
    Get upper bound of data in array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size_of
    @IN data - data to calculate upper bound
    @IN cmp - cmp

    RETURN
    -1 iff failure
    Index of upper bound iff success
*/
ssize_t array_upper_bound(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ data, cmp_f cmp);

/*
    Get lower bound of data in array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - size_of
    @IN data - data to calculate lower bound
    @IN cmp - cmp

    RETURN
    -1 iff failure
    Index of lower bound iff success
*/
ssize_t array_lower_bound(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ data, cmp_f cmp);

/*
    Get min in array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - member size_of
    @IN cmp - cmp
    @OUT min - min element

    RETURN
    -1 iff failure
    Index of min in array iff success
*/
ssize_t array_min(const void * ___restrict___ array, size_t len, size_t size_of, cmp_f cmp, void * ___restrict___ min);

/*
    Get max in array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - member size_of
    @IN cmp - cmp
    @OUT max - max element

    RETURN
    -1 iff failure
    Index of max in array iff success
*/
ssize_t array_max(const void * ___restrict___ array, size_t len, size_t size_of, cmp_f cmp, void * ___restrict___ max);

/*
    Get median in array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - member size_of
    @IN cmp - cmp
    @OUT median - median element

    RETURN
    -1 iff failure
    Index of median in array iff success
*/
ssize_t array_median(const void * ___restrict___ array, size_t len, size_t size_of, cmp_f cmp, void * ___restrict___ median);

/*
    Get kth smalest element in array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - member size_of
    @IN k - kth element to get
    @IN cmp - cmp
    @OUT kth - element

    RETURN
    -1 iff failure
    Index of kth element in array iff success
*/
ssize_t array_select_kth(const void * ___restrict___ array, size_t len, size_t size_of, size_t k, cmp_f cmp, void * ___restrict___ kth);

/*
    Sort array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int array_sort(void * ___restrict___ array, size_t len, size_t size_of, cmp_f cmp);

/*
    Check if array is sorted

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp

    RETURN
    false iff array is not sorted
    true iff array is sorted
*/
bool array_is_sorted(const void *array, size_t len, size_t size_of, cmp_f cmp);

/*
    Check if array is sorted in reverse order

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp

    RETURN
    false iff array is not sorted in reverse order
    true iff array is sorted in reverse order
*/
bool array_is_reverse_sorted(const void *array, size_t len, size_t size_of, cmp_f cmp);

/*
    Find first occurrence of key in unsorted array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp
    @OUT val - value iff finded

    RETURN
    Index of key iff success
    -1 iff failure
*/
ssize_t array_unsorted_find_first(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, cmp_f cmp,  void *val);

/*
    Find last occurrence of key in unsorted array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp
    @OUT val - value iff finded

    RETURN
    Index of key iff success
    -1 iff failure
*/
ssize_t array_unsorted_find_last(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, cmp_f cmp,  void *val);

/*
    Find first occurrence of key in sorted array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp
    @OUT val - value iff finded

    RETURN
    Index of key iff success
    -1 iff failure
*/
ssize_t array_sorted_find_first(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, cmp_f cmp, void *val);

/*
    Find last occurrence of key in sorted array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - members size_of
    @IN cmp - cmp
    @OUT val - value iff finded

    RETURN
    Index of key iff success
    -1 iff failure
*/
ssize_t array_sorted_find_last(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, cmp_f cmp, void *val);

#endif