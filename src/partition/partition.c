#include <partition.h>
#include <log.h>
#include <generic.h>
#include <stdlib.h>
#include <common.h>

#define EPSILON(N)      (N >> 3)

#define TUKEY_MIN       41
#define MEDIAN_MIN      16

/*
    Count medians of 3 element of array t using cmp function to compare elements

    PARAMS
    @IN t - array
    @IN offset_index1 - 1st index ( byte_offset )
    @IN offset_index2 - 2nd index (byte_offset )
    @IN offset_index3 - 3rd index ( byte_offset )
    @IN cmp - compare funciton
    @IN size_of - size of element in array

    RETURN:
    Index in pivot
*/
___inline___ static size_t medians_of_3(const void   *t,
                                        size_t       offset_index1,
                                        size_t       offset_index2,
                                        size_t       offset_index3,
                                        cmp_f        cmp,
                                        size_t       size_of);

/*
    Count tukey median using cmp funcion to compare

    PARAMS
    @IN t - array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN epsilon - eplsion use to count tukey median
    @IN cmp - compare funciton
    @IN size_of - size of element in array

    RETURN:
    Index of pivot

*/
___inline___ static size_t tukey_medians(const void        *t,
                                         size_t            offset_left,
                                         size_t            offset_right,
                                         size_t            epsilon,
                                         cmp_f             cmp,
                                         size_t            size_of);

___inline___ static size_t medians_of_3(const void        *t,
                                        size_t            offset_index1,
                                        size_t            offset_index2,
                                        size_t            offset_index3,
                                        cmp_f             cmp,
                                        size_t            size_of)
{
    BYTE *_t;

    TRACE();

    assert(t != NULL);
    assert(cmp != NULL);

    offset_index1 *= size_of;
    offset_index2 *= size_of;
    offset_index3 *= size_of;

    _t = (BYTE *)t;

    if (cmp((void *)&_t[offset_index1], (void *)&_t[offset_index2]) < 0)
    {
        if (cmp((void *)&_t[offset_index2], (void *)&_t[offset_index3]) < 0)
            return offset_index2 / size_of;
        else
        {
            if (cmp((void *)&_t[offset_index1], (void *)&_t[offset_index3]) < 0)
                return offset_index3 / size_of;
            else
                return offset_index1 / size_of;
        }
    }
    else
    {
        if (cmp((void *)&_t[offset_index3], (void *)&_t[offset_index2]) < 0)
            return offset_index2 / size_of;
        else
        {
            if (cmp((void *)&_t[offset_index3], (void *)&_t[offset_index1]) < 0)
                return offset_index3 / size_of;
            else
                return offset_index1 / size_of;
        }
    }
}

___inline___ static size_t tukey_medians(const void  *t,
                                         size_t      offset_left,
                                         size_t      offset_right,
                                         size_t      epsilon,
                                         cmp_f       cmp,
                                         size_t      size_of)
{
    size_t offset_middle;
    size_t m1;
    size_t m2;
    size_t m3;

    TRACE();

    assert(t != NULL);
    assert(cmp != NULL);
    assert(offset_left <= offset_right);

    offset_middle = (offset_left + offset_right) >> 1;

    m1 = medians_of_3(t, offset_left, offset_left + epsilon, offset_left + (epsilon << 1), cmp, size_of);
    m2 = medians_of_3(t, offset_middle - epsilon, offset_middle, offset_middle + epsilon, cmp, size_of);
    m3 = medians_of_3(t, offset_right - (epsilon << 1), offset_right - epsilon, offset_right, cmp, size_of);

    return medians_of_3(t, m1 ,m2, m3, cmp, size_of);
}

size_t partition_get_pivot_trivial(const void *t, size_t len, size_t size_of, cmp_f cmp)
{
    int r;
    (void)size_of;
    (void)cmp;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", 0);

    if (len == 0)
        ERROR("len == NULL\n", 0);

    r = rand() % 3;
    switch (r)
    {
        case 0: return 0;
        case 1: return len / 2;
        case 2: return len - 1;
        default: return 0;
    }
}

size_t partition_get_pivot_rand(const void *t, size_t len, size_t size_of, cmp_f cmp)
{
    (void)size_of;
    (void)cmp;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", 0);

    if (len == 0)
        ERROR("len == NULL\n", 0);

    return (size_t)((size_t)rand() % len);
}

size_t partition_get_pivot_median(const void *t, size_t len, size_t size_of, cmp_f cmp)
{
    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", 0);

    if (len == 0)
        ERROR("len == NULL\n", 0);

    if (len >= TUKEY_MIN)
        return tukey_medians(t, 0, len - 1, EPSILON(len), cmp, size_of);
    else if (len >= MEDIAN_MIN)
        return medians_of_3(t, 0, len / 2, len - 1, cmp, size_of);
    else
        return partition_get_pivot_rand(t, len, size_of, cmp);
}

ssize_t partition_bentley(void        *t,
                          size_t      offset_left,
                          size_t      offset_right,
                          cmp_f       cmp,
                          size_t      size_of,
                          size_t      *offset_left_index,
                          size_t      *offset_right_index)
{

    /* normal indexes for main loop */
    ssize_t offset_index1;
    ssize_t offset_index2;

    /* indexes for element == to pivot */
    ssize_t offset_equal_left;
    ssize_t offset_equal_right;

    BYTE *_t;
    BYTE pivot[size_of];

    ssize_t i;
    ssize_t _size_of = (ssize_t)size_of;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp is needed\n", -1);

    if (offset_left_index == NULL)
        ERROR("offset_left_index == NULL\n", -1);

    if (offset_right_index == NULL)
        ERROR("offset_right_index == NULL\n", -1);

    if (offset_right < offset_left)
        ERROR("offset_right < offset_left\n", -1);

    offset_left *= size_of;
    offset_right *= size_of;

    offset_index1 = (ssize_t)offset_left + _size_of;
    offset_index2 = (ssize_t)offset_right;

    offset_equal_left = offset_index1;
    offset_equal_right = offset_index2;

    _t = (BYTE *)t;

    __ASSIGN__(pivot[0], _t[offset_left], _size_of);

    while (offset_index1 <= offset_index2)
    {
        while (offset_index1 < offset_index2 && cmp((void *)&_t[offset_index1], (void *)pivot ) < 0)
            offset_index1 += _size_of;

        while (cmp((void *)&_t[offset_index2], (void *)pivot) > 0)
            offset_index2 -= _size_of;

        /* move element to part of elements == pivot */
        if (offset_index1 == offset_index2 && !cmp((void *)& _t[offset_index1], (void *)pivot))
        {
            __SWAP__(_t[offset_index1], _t[offset_equal_left], _size_of);
            offset_equal_left += _size_of;
            break;
        }

        /* indexes are crossed so end loop */
        if (offset_index1 >= offset_index2)
            break;

        /* swap entries */
        __SWAP__(_t[offset_index1], _t[offset_index2], _size_of);

        /* move element to part of element == pivot ( we insert from left so inert in left part ) */
        if (!cmp((void *)&_t[offset_index1], (void *)pivot))
        {
            __SWAP__(_t[offset_index1], _t[offset_equal_left], _size_of);
            offset_equal_left += _size_of;
        }

        /* move element to part of element == pivot ( we insert from right so inert in right part ) */
        if (!cmp((void *)&_t[offset_index2], (void *)pivot))
        {
            __SWAP__(_t[offset_index2], _t[offset_equal_right], _size_of);
            offset_equal_right -= _size_of;
        }

        offset_index1 += _size_of;
        offset_index2 -= _size_of;
    }

    /* insert entries == pivot to correct part of array t */
    offset_index1 = offset_index2 + _size_of;

    for (i = (ssize_t)offset_left + _size_of; i < (ssize_t)offset_equal_left; i += _size_of )
    {
        __SWAP__(_t[i], _t[offset_index2], _size_of);
        offset_index2 -= _size_of;
    }

    for (i = (ssize_t)offset_right; i > (ssize_t)offset_equal_right; i -= _size_of)
    {
         __SWAP__(_t[i], _t[offset_index1], _size_of);
          offset_index1 += _size_of;
    }

    /* move pivot to correct place (offset_index2) */
    __SWAP__(_t[offset_index2], _t[offset_left], _size_of);

    *offset_left_index = (size_t)MAX((ssize_t)(offset_index2 - _size_of), (ssize_t)0) / (size_t)_size_of;
    *offset_right_index = (size_t)offset_index1 / size_of;

    return offset_index2 / (ssize_t)_size_of;
}

ssize_t partition_hoare(void        *t,
                        size_t      offset_left,
                        size_t      offset_right,
                        cmp_f       cmp,
                        size_t      size_of,
                        size_t      *offset_left_index,
                        size_t      *offset_right_index)
{

    /* normal indexes for main loop */
    ssize_t offset_index1;
    ssize_t offset_index2;

    BYTE *_t;
    BYTE pivot[size_of];

    ssize_t _size_of = (ssize_t)size_of;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp is needed\n", -1);

    if (offset_left_index == NULL)
        ERROR("offset_left_index == NULL\n", -1);

    if (offset_right_index == NULL)
        ERROR("offset_right_index == NULL\n", -1);

    if (offset_right < offset_left)
        ERROR("offset_right < offset_left\n", -1);

    offset_left *= size_of;
    offset_right *= size_of;

    offset_index1 = (ssize_t)offset_left + _size_of;
    offset_index2 = (ssize_t)offset_right;

    _t = (BYTE *)t;

    __ASSIGN__(pivot[0], _t[offset_left], _size_of);

    while (offset_index1 <= offset_index2)
    {
        while (offset_index1 < offset_index2 && cmp((void *)&_t[offset_index1], (void *)pivot ) < 0)
            offset_index1 += _size_of;

        while (cmp((void *)&_t[offset_index2], (void *)pivot) > 0)
            offset_index2 -= _size_of;

        /* indexes are crossed so end loop */
        if (offset_index1 >= offset_index2)
            break;

        /* swap entries */
        __SWAP__(_t[offset_index1], _t[offset_index2], _size_of);

        offset_index1 += _size_of;
        offset_index2 -= _size_of;
    }

    /* move pivot to correct place (offset_index2) */
    __SWAP__(_t[offset_index2], _t[offset_left], _size_of);

    *offset_left_index = (size_t)MAX((ssize_t)(offset_index2 - _size_of), (ssize_t)0) / (size_t)_size_of;
    *offset_right_index = (size_t)offset_index1 / (size_t)_size_of;

    return offset_index2 / (ssize_t)_size_of;
}

ssize_t partition_lomuto(void        *t,
                         size_t      offset_left,
                         size_t      offset_right,
                         cmp_f       cmp,
                         size_t      size_of,
                         size_t      *offset_left_index,
                         size_t      *offset_right_index)
{
    size_t i;
    size_t j;
    size_t _size_of = size_of;

    BYTE *_t;
    BYTE pivot[_size_of];

    offset_left *= _size_of;
    offset_right *= _size_of;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp is needed\n", -1);

    if (offset_left_index == NULL)
        ERROR("offset_left_index == NULL\n", -1);

    if (offset_right_index == NULL)
        ERROR("offset_right_index == NULL\n", -1);

    if (offset_right < offset_left)
        ERROR("offset_right < offset_left\n", -1);

    _t = (BYTE *)t;
    __ASSIGN__(pivot[0], _t[offset_left], _size_of);

    __SWAP__(_t[offset_left], _t[offset_right], _size_of);
    j = offset_left;
    for (i = offset_left; i < offset_right; i += _size_of)
        if (cmp(&_t[i], pivot) < 0)
        {
            __SWAP__(_t[i], _t[j], _size_of);
            j += _size_of;
        }

    /* move pivot to correct place (j) */
    __SWAP__(_t[offset_right], _t[j], _size_of);
    *offset_left_index = (size_t)MAX((ssize_t)(j - _size_of), (ssize_t)0) / _size_of;
    *offset_right_index = MIN(j + _size_of, offset_right) / _size_of;

    return (ssize_t)(j / _size_of);
}