#include <sort.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <compiler.h>
#include <common.h>
#include <assert.h>

#define MERGESORT_CUTOFF 8

#define EPSILON(N, S) ((((N / S) >> 3) * S) * S)
#define QUICKSORT_CUTOFF 8
#define TUKEY_MIN       41
#define MEDIAN_MIN      16

#define INSORT_TRESHOLD      32
#define BINSORT_TRESHOLD     128

/*
    Merge 2 subarrays of t array and write it to buffer so in buffer we got merged array

    PARAMS
    @IN t - array to merge
    @IN offset_left - begining of 1st subarray ( byte offset )
    @IN offset_middle - end of 1st subarray (byte offset )
    @IN offset_right - end of 2nd subarray ( byte offset )
    @IN cmp - function to compare elements of array
    @IN size_of - size of element
    @IN buffer - temp array ( in this array will be merged t array )

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
___inline___ static int merge(    void        *t,
                                size_t      offset_left,
                                size_t      offset_middle,
                                size_t      offset_right,
                                int         (*cmp)(void *a, void *b),
                                int         size_of,
                                void        *buffer);

/*
    Count medians of 3 element of array t using cmp function to compare elements

    PARAMS
    @IN t - array
    @IN offset_index1 - 1st index ( byte_offset )
    @IN offset_index2 - 2nd index (byte_offset )
    @IN offset_index3 - 3rd index ( byte_offset )
    @IN cmp - compare funciton
    @OUT median_pos - position of median in array ( one of tree index )

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
___inline___ static int medians_of_3(     void        *t,
                                        size_t      offset_index1,
                                        size_t      offset_index2,
                                        size_t      offset_index3,
                                        int         (*cmp)(void *a, void *b),
                                        size_t      *median_pos);


/*
    Count tukey median using cmp funcion to compare

    PARAMS
    @IN t - array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN epsilon - eplsion use to count tukey median
    @IN cmp - compare funciton
    @IN size_of - size of elemrnt in array
    @OUT median_pos - position of median

    RETURN:
    %0 if success
    %Non-zero value if failure

*/
___inline___ static int tukey_medians(    void        *t,
                                        size_t      offset_left,
                                        size_t      offset_right,
                                        size_t      epsilon,
                                        int         (*cmp)(void *a, void *b),
                                        int         size_of,
                                        size_t      *median_pos);


/*
    Bentley McLorin partition

    PARAMS
    @IN t - array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN cmp - compare function
    @IN size_of - sizeof type
    @OUT offset_left_index - last index of left part of unsorted array
    @OUT offset_right_index - first index of right part of unsroted array

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
static int partition_bentley(   void        *t,
                                size_t      offset_left,
                                size_t      offset_right,
                                int         (*cmp)(void *a, void *b),
                                int         size_of,
                                size_t      *offset_left_index,
                                size_t      *offset_right_index);

/*
    Main body of quicksort
    Use a Tukey Median, Cutoff to insort and tail-recursive

    PARAMS
    @IN t- array
    @IN offset_left - 1st index of array
    @IN offset_right - last index of array
    @IN cmp - compare function
    @IN size_of - size of element

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
static int _quicksort(  void        *t,
                        size_t      offset_left,
                        size_t      offset_right,
                        int         (*cmp)(void *a, void *b),
                        int         size_of);


___inline___ static int merge(    void        *t,
                                size_t      offset_left,
                                size_t      offset_middle,
                                size_t      offset_right,
                                int         (*cmp)(void *a, void *b),
                                int         size_of,
                                void        *buffer)
{

    BYTE *_t;
    BYTE *_buffer;

    /* 1st subarray t[left;middle] */
    /* 2nd subarray t[middle + 1;right] */
    ssize_t offset_i; /* counter of 1st subarray */
    ssize_t offset_j; /* counter of 2nd subarray */
    ssize_t offset_k; /* counter of buffer */

    TRACE();

    assert(t != NULL);
    assert(buffer != NULL);
    assert(cmp != NULL);
    assert(size_of >= 1);
    assert(offset_middle >= offset_left && offset_right >= offset_middle
            && offset_right >= offset_left);

    _t = (BYTE *)t;
    _buffer = (BYTE *)buffer;

    offset_i = (ssize_t)offset_left;
    offset_j = (ssize_t)(offset_middle + (size_t)size_of);
    offset_k = (ssize_t)offset_left;

    /* merge 2 subarray until both have entries to merge */
    while (offset_i <= (ssize_t)offset_middle && offset_j <= (ssize_t)offset_right)
    {
        if (cmp((void *)&_t[offset_i], (void *)&_t[offset_j]) < 0)
        {
            __ASSIGN__(_buffer[offset_k], _t[offset_i], size_of);
            offset_k += size_of;
            offset_i += size_of;
        }
        else
        {
            __ASSIGN__(_buffer[offset_k], _t[offset_j], size_of);
            offset_k += size_of;
            offset_j += size_of;
        }

    }

    if (offset_i <= (ssize_t)offset_middle)
        if (memcpy((void *)(_buffer + offset_k), (void *)(_t + offset_i),
        offset_middle - (size_t)offset_i + (size_t)size_of) == NULL)
            ERROR("memcpy error\n", 1);


    if (offset_j <= (ssize_t)offset_right)
        if (memcpy((void *)(_buffer + offset_k), (void *)(_t + offset_j),
        offset_right - (size_t)offset_j + (size_t)size_of) == NULL)
            ERROR("memcpy error\n", 1);

    return 0;
}

___inline___ static int medians_of_3(     void        *t,
                                        size_t      offset_index1,
                                        size_t      offset_index2,
                                        size_t      offset_index3,
                                        int         (*cmp)(void *a, void *b),
                                        size_t      *median_pos)
{
    BYTE *_t;

    TRACE();

    assert(t != NULL);
    assert(median_pos != NULL);
    assert(cmp != NULL);

    _t = (BYTE *)t;

    if (cmp((void *)&_t[offset_index1], (void *)&_t[offset_index2]) < 0)
    {
        if (cmp((void *)&_t[offset_index2], (void *)&_t[offset_index3]) < 0)
        {
            *median_pos = offset_index2;
            return 0;
        }
        else
        {
            if (cmp((void *)&_t[offset_index1], (void *)&_t[offset_index3]) < 0)
            {
                *median_pos = offset_index3;
                return 0;
            }
            else
            {
                *median_pos = offset_index1;
                return 0;
            }
        }
    }
    else
    {
        if (cmp((void *)&_t[offset_index3], (void *)&_t[offset_index2]) < 0)
        {
            *median_pos = offset_index2;
            return 0;
        }
        else
        {
            if (cmp((void *)&_t[offset_index3], (void *)&_t[offset_index1]) < 0)
            {
                *median_pos = offset_index3;
                return 0;
            }
            else
            {
                *median_pos = offset_index1;
                return 0;
            }
        }
    }
}

___inline___ static int tukey_medians(    void        *t,
                                        size_t      offset_left,
                                        size_t      offset_right,
                                        size_t      epsilon,
                                        int         (*cmp)(void *a, void *b),
                                        int         size_of,
                                        size_t      *median_pos)
{
    size_t offset_middle;
    size_t m1;
    size_t m2;
    size_t m3;
    size_t median;
    size_t _size_of = (size_t)size_of;

    TRACE();

    assert(t != NULL);
    assert(median_pos != NULL);
    assert(cmp != NULL);
    assert(offset_left <= offset_right);

    offset_middle = ((offset_left / _size_of + offset_right / _size_of) >> 1 ) * _size_of;

    if (medians_of_3(t, offset_left, offset_left + epsilon,
    offset_left + (((epsilon / _size_of ) << 1) * _size_of), cmp, &m1))
        ERROR("medians_of_3 error\n", 1);


    if (medians_of_3(t, offset_middle - epsilon, offset_middle,
    offset_middle + epsilon, cmp, &m2))
        ERROR("medians_of_3 error\n", 1);

    if (medians_of_3(t, offset_right - (((epsilon / _size_of ) << 1) * _size_of),
    offset_right - epsilon, offset_right, cmp, &m3 ))
        ERROR("medians_of_3 error\n", 1);

    if (medians_of_3(t, m1 ,m2, m3, cmp, &median))
        ERROR("medians_of_3 error\n",1);

    *median_pos = median;

    return 0;
}

static int partition_bentley(   void        *t,
                                size_t      offset_left,
                                size_t      offset_right,
                                int         (*cmp)(void *a,void *b),
                                int         size_of,
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
    BYTE pivot[MAXWORD];

    ssize_t i;
    ssize_t _size_of = (ssize_t)size_of;

    TRACE();

    assert(t != NULL);
    assert(cmp != NULL);
    assert(offset_left_index != NULL);
    assert(offset_right_index != NULL);
    assert(offset_right >= offset_left);

    offset_index1 = (ssize_t)(offset_left + (size_t)_size_of);
    offset_index2 = (ssize_t)offset_right;

    offset_equal_left = (ssize_t)offset_index1;
    offset_equal_right = (ssize_t)offset_index2;

    _t = (BYTE *)t;

    __ASSIGN__(pivot[0], _t[offset_left], _size_of);

    while (offset_index1 <= offset_index2)
    {
        while (offset_index1 < offset_index2
                && cmp((void *)&_t[offset_index1], (void *)pivot ) < 0)
            offset_index1 += _size_of;

        while (cmp((void *)&_t[offset_index2], (void *)pivot) > 0)
            offset_index2 -= _size_of;

        /* move element to part of elements == pivot */
        if (offset_index1 == offset_index2
        && !cmp((void *)& _t[offset_index1], (void *)pivot))
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

    __SWAP__(_t[offset_index2], _t[offset_left], _size_of);

    *offset_left_index = (size_t)MAX((ssize_t)(offset_index2 - size_of), (ssize_t)0);
    *offset_right_index = (size_t)offset_index1;

    return 0;
}

static int _quicksort(  void        *t,
                        size_t      offset_left,
                        size_t      offset_right,
                        int         (*cmp)(void *a, void *b),
                        int         size_of)
{
    BYTE *_t;
    size_t m;

    size_t offset_left_index;
    size_t offset_right_index;
    size_t range;

    size_t _size_of = (size_t)size_of;

    TRACE();

    assert(t != NULL);
    assert(cmp != NULL);
    assert(offset_left < offset_right);
    assert(size_of >= 1);

    _t = (BYTE *)t;

    while ((ssize_t)offset_right - (ssize_t)offset_left >= QUICKSORT_CUTOFF)
    {
        range = (offset_right - offset_left) / _size_of + 1;

        if ((ssize_t)range < TUKEY_MIN)
        {
            if ((ssize_t)range > MEDIAN_MIN)
            {
                if (medians_of_3( _t, offset_left,
                ((offset_left / _size_of + offset_right / _size_of) >> 1)
                * _size_of, offset_right, cmp, &m) )
                    ERROR("medians_of_3 error\n", 1);

                __SWAP__(_t[m], _t[offset_left], _size_of);
            }
        }
        else
        {
            if (tukey_medians(_t, offset_left, offset_right,
            EPSILON(range, _size_of), cmp, size_of, &m))
                ERROR("tukey_medians error\n", 1);

            __SWAP__(_t[m], _t[offset_left], _size_of);
        }

        if (partition_bentley(_t, offset_left, offset_right, cmp, size_of,
        &offset_left_index, &offset_right_index))
            ERROR("partition_bentley error\n", 1);

        /* left part is smalller */
        if ((offset_left_index - offset_left) <
        (offset_right - offset_right_index))
        {
            if (offset_right > offset_right_index)
                _quicksort(_t, offset_right_index, offset_right, cmp, size_of);

            offset_right = offset_left_index;
        }
        else
        {
            if (offset_left_index > offset_left)
                _quicksort(_t, offset_left, offset_left_index, cmp, size_of);

            offset_left = offset_right_index;
        }
    }

    if ( (ssize_t)offset_right - (ssize_t)offset_left > 0 &&
        insort(_t + offset_left, (offset_right - offset_left) / _size_of + 1, cmp, size_of))
        ERROR("insort error\n", 1);

    return 0;
}

int insort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of)
{
    /* we use buffer instead of temporary variable */
    BYTE buffer[MAXWORD];
    BYTE *_t;

    ssize_t offset_i;
    ssize_t offset_j;

    size_t max;
    ssize_t _size_of = (ssize_t)size_of;

    TRACE();

    if (t == NULL || cmp == NULL)
        ERROR("t == NULL || cmp == NULL\n", 1);

    if (size_of < 1)
        ERROR("size_of < 1\n", 1);

    if (num_elements == 0)
        ERROR("Nothing to sort\n", 1);

    /* cast TYPE *t on BYTE*, so we can pick 1 byte */
    _t = (BYTE *)t;

    offset_i = size_of;
    max = num_elements * (size_t)_size_of;

    for (offset_i = _size_of; offset_i < (ssize_t)max; offset_i += _size_of)
    {
        /* temp = t[i] */
        __ASSIGN__(buffer[0], _t[offset_i], _size_of);

        offset_j = offset_i - _size_of;

        /* while j >= 0 && temp  <= t[j] */
        while (offset_j >= 0 && cmp((void *)&_t[offset_j], (void *)buffer) > 0)
            offset_j -= _size_of;

        /* if we move someone */
        if (offset_j + _size_of < offset_i)
        {
            /* make gap for temp */
            if ((memmove((void *)(_t + offset_j + (_size_of << 1)),
            (void *)(_t + offset_j + _size_of),
            (size_t)(offset_i - offset_j - _size_of))) == NULL)
                ERROR("memmove error\n", 1);

            /* fill gap */
            __ASSIGN__(_t[offset_j + _size_of], buffer[0], _size_of);
        }
    }

    return 0;
}

int binsort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of)
{
    size_t size;

    /* temporary array for 2-directional binsort */
    BYTE *array;

    ssize_t offset_middle;
    ssize_t offset_left;
    ssize_t offset_right;

    /* binary search offsets */
    ssize_t bs_offset_left;
    ssize_t bs_offset_right;
    ssize_t bs_offset_middle;

    ssize_t offset_i;
    ssize_t _size_of = (ssize_t)size_of;

    BYTE *_t;
    BYTE buffer[MAXWORD];

    TRACE();

    if (t == NULL || cmp == NULL)
        ERROR("t == NULL || cmp == NULL\n", 1);

    if (size_of < 1)
        ERROR("size_of < 1\n", 1);

    if (num_elements == 0)
        ERROR("Nothing to sort\n", 1);

    if (num_elements < INSORT_TRESHOLD)
        return insort(t, num_elements, cmp, size_of);

    size = (size_t)_size_of * num_elements;

    array = (BYTE *)malloc(size);
    if (array == NULL)
        ERROR("malloc error\n", 1);

    offset_middle = (ssize_t)(size >> 1);
    offset_left = offset_middle;
    offset_right = offset_middle;

    _t = (BYTE *)t;

    /* insert first element in middle */
    __ASSIGN__(array[offset_middle], _t[0], _size_of);

    for (offset_i = _size_of; offset_i < (ssize_t)size; offset_i += _size_of)
    {
         __ASSIGN__(buffer[0], _t[offset_i], _size_of);

        /* perform standart BINARY SEARCH */
        bs_offset_left = offset_left - _size_of;
        bs_offset_right = offset_right + _size_of;

        while (bs_offset_right - bs_offset_left > _size_of)
        {
            bs_offset_middle = ((bs_offset_left / _size_of + bs_offset_right
                                / _size_of) >> 1) * _size_of;
            if (cmp((void *)&array[bs_offset_middle], (void *)buffer) < 0)
                bs_offset_left = bs_offset_middle;
            else
                bs_offset_right = bs_offset_middle;
        }

        /* move from left is better */
        if ((offset_left > 0 && bs_offset_right - offset_left <=
        offset_right - bs_offset_right) || offset_right == (ssize_t)size - _size_of)
        {
            if (memmove((void *)(array + offset_left - size_of),
            (void *)(array + offset_left), (size_t)(bs_offset_right - offset_left)) == NULL)
                ERROR("memmove error\n", 1);

            offset_left -= _size_of;

            __ASSIGN__(array[bs_offset_right - _size_of], buffer[0], _size_of);
        }
        else
        {
            if (memmove((void *)(array + bs_offset_right + _size_of),
            (void *)(array + bs_offset_right),
            (size_t)(offset_right - bs_offset_right + _size_of)) == NULL)
                ERROR("memmove error\n",1);

            offset_right += _size_of;

            __ASSIGN__(array[bs_offset_right], buffer[0], _size_of);
        }
    }

    /* now array is sorted, copy to t */
    if (memcpy(t, (void *)array, size) == NULL )
        ERROR("memcpy error\n", 1);

    FREE(array);

    return 0;
}

int mergesort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of)
{
    ssize_t i;
    ssize_t offset_i;

    int loop_counter;
    size_t size;

    ssize_t offset_k;
    ssize_t d_offset_k;

    int k;
    int dk;

    BYTE *_t;
    BYTE *buffer;

    TRACE();

    if (t == NULL || cmp == NULL)
        ERROR("t == NULL || cmp == NULL\n", 1);

    if (size_of < 1)
        ERROR("size_of < 1\n", 1);

    if (num_elements == 0)
        ERROR("Nothing to sort\n", 1);

    if (num_elements < INSORT_TRESHOLD)
        return insort(t, num_elements, cmp, size_of);

    size = num_elements * (size_t)size_of;
    _t = (BYTE *)t;

    /* sort small subarrays using insort */
    for (i = 0; i < (ssize_t)num_elements; i += MERGESORT_CUTOFF)
        if (insort(_t + (i * size_of),
        MIN((size_t)(MERGESORT_CUTOFF), (num_elements - (size_t)i)), cmp, size_of))
            ERROR("insort error\n", 1);

    buffer = (BYTE *)malloc(size);
    if (buffer == NULL)
        ERROR("malloc error on buffer\n", 1);

    loop_counter = 0;

    /* loop for every power of 2 less than size */
    for (offset_k = MERGESORT_CUTOFF * size_of; offset_k < (ssize_t)size; offset_k <<= 1)
    {
        d_offset_k = offset_k << 1;
        k = (int)(offset_k / (ssize_t)size_of);
        dk = k << 1;

        for (offset_i = 0; offset_i < (ssize_t)size; offset_i += d_offset_k)
            if (merge( _t, (size_t)offset_i,
            (size_t)MIN(((offset_i / size_of + k - 1) * size_of), (ssize_t)(size - (size_t)size_of)),
            (size_t)MIN(((offset_i / size_of + dk - 1) * size_of), (ssize_t)(size - (size_t)size_of)),
            cmp, size_of, buffer))
                ERROR("merge error\n", 1);

        ++loop_counter;

        SWAP(_t, buffer);
    }

    if (ODD(loop_counter))
    {
        SWAP(_t, buffer);

        if (memcpy((void *)_t, (void *)buffer, size) == NULL)
            ERROR("memcpy error\n", 1);
    }

    FREE(buffer);

    return 0;
}

int quicksort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of)
{
    TRACE();

    if (t == NULL || cmp == NULL)
        ERROR("t == NULL || cmp == NULL\n", 1);

    if (size_of < 1)
        ERROR("size_of < 1\n", 1);

    if (num_elements == 0)
        ERROR("Nothing to sort\n", 1);

    if (num_elements < INSORT_TRESHOLD)
        return insort(t, num_elements, cmp, size_of);

    return _quicksort(t, 0, (num_elements - 1) * (size_t)size_of, cmp, size_of);
}

int sort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of)
{
    TRACE();

    if (t == NULL || cmp == NULL)
        ERROR("t == NULL || cmp == NULL\n", 1);

    if (size_of < 1)
        ERROR("size_of < 1\n", 1);

    if (num_elements == 0)
        ERROR("Nothing to sort\n", 1);

	if (num_elements < INSORT_TRESHOLD)
		return insort(t, num_elements, cmp, size_of);
	else if (num_elements < BINSORT_TRESHOLD)
		return binsort(t, num_elements, cmp, size_of);
	else
		return quicksort(t, num_elements, cmp, size_of);
}
