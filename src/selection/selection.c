#include <partition.h>
#include <selection.h>
#include <log.h>
#include <generic.h>
#include <sort.h>
#include <stdbool.h>
#include <common.h>
#include <string.h>
#include <search.h>

ssize_t select_kth(const void *t, size_t len, int size_of, size_t k, int (*cmp)(const void *a, const void *b))
{
    size_t offset_left;
    size_t offset_right;
    size_t _size_of = (size_t)size_of;
    size_t left;
    size_t right;
    size_t p_index;
    size_t range;
    size_t ith;

    BYTE *_t;
    BYTE var[size_of];

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of <= 0)
        ERROR("size_of <= 0\n", -1);

    if (cmp == NULL)
        ERROR("cmp is needed\n", -1);

    if (k >= len)
        ERROR("k must be >= 0 and < len\n", -1);

    _t = (BYTE *)malloc(len * _size_of);
    if (_t == NULL)
        ERROR("malloc error\n", -1);

    (void)memcpy(_t, t, len * _size_of);

    offset_left = 0;
    offset_right = (len - 1) * _size_of;

    while ((ssize_t)offset_left < (ssize_t)offset_right)
    {
        range = (offset_right - offset_left) / _size_of + 1;
        p_index = partition_get_pivot_median(_t + offset_left, range, size_of, cmp) *_size_of + offset_left;
        __SWAP__(_t[p_index], _t[offset_left], _size_of);

        if (partition_hoare(_t, offset_left / _size_of, offset_right / _size_of, cmp, size_of, &left, &right))
            ERROR("partition_hoare error\n", 1);

        p_index = left + 1;
        ith = p_index - (offset_left / _size_of);

        if(k < ith)
            offset_right = (p_index - 1) * _size_of;
        else
        {
            k -= ith;
            offset_left = (p_index) * _size_of;
        }
    }

    __ASSIGN__(var[0], _t[offset_left], _size_of);
    FREE(_t);

    return find_first_unsorted(&var[0], t, len, cmp, size_of);
}