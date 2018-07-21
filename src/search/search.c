#include <search.h>
#include <sys/types.h>
#include <log.h>
#include <generic.h>

ssize_t find_first_unsorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, cmp_f cmp, size_t size_of)
{
    BYTE *_t;

    size_t i;
    size_t max;

    TRACE();

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    if (num_elements == 0)
        ERROR("Empty array\n", -1);

    _t = (BYTE *)t;
    max = (size_t)(num_elements * size_of);
    for (i = 0; i < max; i += size_of)
        if (cmp((void *)(_t + i), key) == 0)
            return (ssize_t)(i / size_of);

    return -1;
}

ssize_t find_last_unsorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, cmp_f cmp, size_t size_of)
{
    BYTE *_t;

    ssize_t i;
    ssize_t max;

    TRACE();

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    if (num_elements == 0)
        ERROR("Empty array\n", -1);

    _t = (BYTE *)t;
    max = (ssize_t)((ssize_t)(num_elements - 1) * (ssize_t)size_of);
    for (i = max; i >= 0; i -= (ssize_t)size_of)
        if (cmp((void *)(_t + i), key) == 0)
            return i / (ssize_t)size_of;

    return -1;
}

ssize_t find_first_sorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, cmp_f cmp, size_t size_of)
{
    BYTE *_t;

    size_t left;
    size_t right;
    size_t middle;

    TRACE();

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    if (num_elements == 0)
        ERROR("Empty array\n", -1);

     _t = (BYTE *)t;

    left = 0;
    right = (num_elements - 1) * size_of;
    while (left < right)
    {
        middle = ((left / size_of + right / size_of) >> 1) * size_of;
        if (cmp((void *)(_t + middle), key) < 0)
            left = middle + size_of;
        else
            right = middle;
    }

    if (cmp((void *)(_t + left), key) == 0)
        return (ssize_t)left / (ssize_t)size_of;

    return -1;
}

ssize_t find_last_sorted(const void * ___restrict___ key, const void * ___restrict___ t, size_t num_elements, cmp_f cmp, size_t size_of)
{
    BYTE *_t;

    size_t left;
    size_t right;
    size_t middle;

    TRACE();

    if (key == NULL)
        ERROR("key == NULL\n", -1);

    if (t == NULL)
        ERROR("t == NULL\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    if (num_elements == 0)
        ERROR("Empty array\n", -1);

     _t = (BYTE *)t;

    left = 0;
    right = (num_elements - 1) * size_of;
    while (left < right)
    {
        middle = ((left / size_of + right / size_of + 1) >> 1) * size_of;
        if (cmp((void *)(_t + middle), key) > 0)
            right = middle - size_of;
        else
            left = middle;
    }

    if (cmp((void *)(_t + left), key) == 0)
        return (ssize_t)left / (ssize_t)size_of;

    return -1;
}