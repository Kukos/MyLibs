#include <array.h>
#include <common.h>
#include <stdlib.h>
#include <search.h>
#include <sort.h>
#include <selection.h>
#include <log.h>
#include <string.h>
#include <generic.h>

/*
    Delete element at @pos in @array, if @destructor is not null, call before deletetion

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - element size of
    @IN pos - pos to delete
    @IN destructor - destructor or NULL

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __array_delete_pos(void *array, size_t len, size_t size_of, size_t pos, void (*destructor)(void *element));

/*
    Insert @data at @pos in @array

    PARAMS
    @IN array - array
    @IN len - array len
    @IN size_of - element size of
    @IN pos - pos to insert
    @IN data - data to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __array_insert_pos(void *array, size_t len, size_t size_of, size_t pos, const void *data);

static int __array_delete_pos(void *array, size_t len, size_t size_of, size_t pos, void (*destructor)(void *element))
{
    BYTE *_t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (len == 0)
        ERROR("len == 0\n", 1);

    if (size_of == 0)
        ERROR("size_of == 0\n", 1);

    if (pos >= len)
        ERROR("pos >= len\n", 1);

    _t = (BYTE *)array;
    if (destructor != NULL)
        destructor(&_t[pos * size_of]);

    /* move entries and "delete" pos */
    if ((memmove((void *)(_t + (pos * size_of)), (void *)(_t + ((pos + 1) * size_of)), (len - pos - 1) * size_of )) == NULL)
        ERROR("memmove error\n", 1);

    return 0;
}

static int __array_insert_pos(void *array, size_t len, size_t size_of, size_t pos, const void *data)
{
    BYTE *_t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (len == 0)
        ERROR("len == 0\n", 1);

    if (size_of == 0)
        ERROR("size_of == 0\n", 1);

    if (pos > len)
        ERROR("pos > len\n", 1);

    _t = (BYTE *)array;

    /* make empty slot in pos and insert an entry */
    if ((memmove((void *)(_t + ((pos + 1) * size_of)), (void *)(_t + (pos * size_of)), (len - pos - 1) * size_of )) == NULL)
        ERROR("memmove error\n", 1);

    __ASSIGN__(_t[pos * size_of], *(BYTE *)data, size_of);

    return 0;
}

void *array_create(size_t len, size_t size_of)
{
    void *t;

    TRACE();

    if (len == 0)
        ERROR("len == 0\n", NULL);

    if (size_of == 0)
        ERROR("size_of == 0\n", NULL);

    t = calloc(len, size_of);
    if (t == NULL)
        ERROR("calloc error\n", NULL);

    return t;
}

bool array_equal(const void * array1, const void *array2, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b))
{
    const BYTE *_t1;
    const BYTE *_t2;

    size_t i;

    TRACE();

    if (array1 == NULL)
        ERROR("array1 == NULL\n", false);

    if (array2 == NULL)
        ERROR("array2 == NULL\n", false);

    if (len == 0)
        ERROR("len == 0\n", false);

    if (size_of == 0)
        ERROR("size_of == 0\n", false);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", false);

    _t1 = (const BYTE *)array1;
    _t2 = (const BYTE *)array2;

    for (i = 0; i < len * size_of; i += size_of)
        if (cmp(&_t1[i], &_t2[i]))
            return false;

    return true;
}

void array_shuffle(void *array, size_t len, size_t size_of)
{
    BYTE *t;
    size_t i;
    size_t r;

    TRACE();

    if (array == NULL || len == 0 || size_of == 0)
        return;

    t = (BYTE *)array;
    for (i = (len - 1); i > 0; --i)
    {
        r = (size_t)rand() % (i + 1);
        __SWAP__(t[i * size_of], t[r * size_of], size_of);
    }
}

void array_copy(void * ___restrict___ dst, const void * ___restrict___ src, size_t len, size_t size_of)
{
    TRACE();

    if (dst == NULL || src == NULL || len == 0 || size_of == 0)
        return;

    (void)memcpy(dst, src, len * (size_of));
}

void array_move(void *dst, void *src,  size_t len, size_t size_of)
{
    TRACE();

    if (dst == NULL || src == NULL || len == 0 || size_of == 0)
        return;

    (void)memmove(dst, src, len * (size_of));
}

void *array_clone(const void *array, size_t len, size_t size_of)
{
    void *t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULL\n", NULL);

    if (len == 0)
        ERROR("len == 0\n", NULL);

    if (size_of == 0)
        ERROR("size_of == 0\n", NULL);

    t = array_create(len, size_of);
    if (t == NULL)
        ERROR("array_create error\n", NULL);

    array_copy(t, array, len, size_of);

    return t;
}

void array_zeros(void *array, size_t len, size_t size_of)
{
    TRACE();

    if (array == NULL || len == 0 || size_of == 0)
        return;

    (void)memset(array, 0, len * size_of);
}

void array_set_all(void *array, size_t len, size_t size_of, const void *data)
{
    size_t i;
    BYTE *t;

    TRACE();

    if (array == NULL || len == 0 || size_of == 0 || data == NULL)
        return;

    t = (BYTE *)array;
    for (i = 0; i < len * size_of; i += size_of)
        __ASSIGN__(t[i], *(BYTE *)data, size_of);
}

void array_destroy(void *array)
{
    TRACE();

    if (array == NULL)
        return;

    FREE(array);
}

void array_destroy_with_entries(void *array, size_t len, size_t size_of, void (*destructor)(void *element))
{
    void *ptr;
    TRACE();

    if (array == NULL || len == 0 || size_of == 0 || destructor == NULL)
        return;

    for_each_ptr_array(array, len, size_of, ptr)
        destructor(ptr);

    FREE(array);
}

int array_unsorted_insert_first(void *array, size_t len, size_t size_of, const void *data)
{
    TRACE();
    return __array_insert_pos(array, len, size_of, 0, data);
}

int array_unsorted_insert_last(void *array, size_t len, size_t size_of, const void *data)
{
    TRACE();
    return __array_insert_pos(array, len, size_of, len - 1, data);
}

int array_unsorted_insert_pos(void *array, size_t len, size_t size_of, size_t pos, const void *data)
{
    TRACE();
    return __array_insert_pos(array, len, size_of, pos, data);
}

int array_sorted_insert(void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b), const void *data)
{
    ssize_t index;
    TRACE();

    if (len == 1)
        return __array_insert_pos(array, len, size_of, 0, data);

    index = array_upper_bound(array, len - 1, size_of, data, cmp);
    if (index == -1)
        ERROR("array_upper_bound error\n", 1);

    return __array_insert_pos(array, len, size_of, (size_t)index, data);
}

int array_delete_first(void *array, size_t len, size_t size_of)
{
    TRACE();
    return __array_delete_pos(array, len, size_of, 0, NULL);
}

int array_delete_last(void *array, size_t len, size_t size_of)
{
    TRACE();
    return __array_delete_pos(array, len, size_of, len - 1, NULL);
}

int array_delete_pos(void *array, size_t len, size_t size_of, size_t pos)
{
    TRACE();
    return __array_delete_pos(array, len, size_of, pos, NULL);
}

int array_delete_first_with_entry(void *array, size_t len, size_t size_of, void (*destructor)(void *element))
{
    TRACE();
    if (destructor == NULL)
        ERROR("destructor == NULL\n", 1);

    return __array_delete_pos(array, len, size_of, 0, destructor);
}

int array_delete_last_with_entry(void *array, size_t len, size_t size_of, void (*destructor)(void *element))
{
    TRACE();
    if (destructor == NULL)
        ERROR("destructor == NULL\n", 1);

    return __array_delete_pos(array, len, size_of, len - 1, destructor);
}

int array_delete_pos_with_entry(void *array, size_t len, size_t size_of, size_t pos, void (*destructor)(void *element))
{
    TRACE();
    if (destructor == NULL)
        ERROR("destructor == NULL\n", 1);

    return __array_delete_pos(array, len, size_of, pos, destructor);
}

int array_sort(void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b))
{
    TRACE();
    return sort(array, len, cmp, size_of);
}

bool array_is_sorted(const void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b))
{
    size_t i;
    size_t size;
    size_t _size_of = size_of;
    const BYTE *t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULLL\n", false);

    if (len == 0)
        ERROR("len == 0\n", false);

    if (size_of == 0)
        ERROR("size_of == 0\n", false);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", false);

    t = (const BYTE *)array;
    size = (len - 1) * _size_of;
    for (i = 0; i < size; i += _size_of)
        if (cmp(&t[i], &t[i + _size_of]) > 0)
            return false;

    return true;
}

bool array_is_reverse_sorted(const void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b))
{
    size_t i;
    size_t size;
    size_t _size_of = size_of;
    const BYTE *t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULLL\n", false);

    if (len == 0)
        ERROR("len == 0\n", false);

    if (size_of == 0)
        ERROR("size_of == 0\n", false);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", false);

    t = (const BYTE *)array;
    size = (len - 1) * _size_of;
    for (i = 0; i < size; i += _size_of)
        if (cmp(&t[i], &t[i + _size_of]) < 0)
            return false;

    return true;
}

void array_reverse(void *array, size_t len, size_t size_of)
{
    BYTE *t;
    size_t i;

    TRACE();

    if (array == NULL || len == 0 || size_of == 0)
        return;

    t = (BYTE *)array;
    for (i = 0; i < len / 2; ++i)
        __SWAP__(t[i * size_of], t[(len - i - 1) * size_of], size_of);
}

ssize_t array_lower_bound(const void *array, size_t len, size_t size_of, const void *data, int (*cmp)(const void *a, const void *b))
{
    size_t offset_left;
    size_t offset_right;
    size_t offset_mid;
    size_t _size_of = size_of;

    const BYTE *_t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULLL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    _t = (const BYTE *)array;
    offset_left = 0;
    offset_right = len * _size_of;

    while (offset_left < offset_right)
    {
        offset_mid = ((offset_left / _size_of + offset_right / _size_of) / 2) * _size_of;
        if (cmp(data, &_t[offset_mid]) <= 0)
            offset_right = offset_mid;
        else
            offset_left = offset_mid + _size_of;
    }

    return (ssize_t)(offset_left / _size_of);
}

ssize_t array_upper_bound(const void *array, size_t len, size_t size_of, const void *data, int (*cmp)(const void *a, const void *b))
{
    size_t offset_left;
    size_t offset_right;
    size_t offset_mid;
    size_t _size_of = size_of;

    const BYTE *_t;

    TRACE();

    if (array == NULL)
        ERROR("array == NULLL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    _t = (const BYTE *)array;
    offset_left = 0;
    offset_right = len * _size_of;

    while (offset_left < offset_right)
    {
        offset_mid = ((offset_left / _size_of + offset_right / _size_of) / 2) * _size_of;
        if (cmp(data, &_t[offset_mid]) >= 0)
            offset_left = offset_mid + _size_of;
        else
            offset_right = offset_mid;
    }

    return (ssize_t)(offset_left / _size_of);
}

ssize_t array_min(const void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b), void *min)
{
    size_t pos;

    BYTE *_min;
    const BYTE *_t;

    size_t offset_i;
    size_t offset_max;
    size_t _size_of = size_of;

    TRACE();

    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    _min = alloc_on_stack(_size_of);

    _t = (const BYTE *)array;

    pos = 0;
    __ASSIGN__(_min[0], _t[0], _size_of);
    offset_max = len * _size_of;
    for (offset_i = _size_of; offset_i < offset_max; offset_i += _size_of)
        if (cmp((void *)_min, (void *)&_t[offset_i]) > 0)
        {
            __ASSIGN__(_min[0], _t[offset_i], _size_of);
            pos = offset_i;
        }

    if (min != NULL)
        __ASSIGN__(*(BYTE *)min, _t[pos], _size_of);

    return (ssize_t)pos / (ssize_t)_size_of;
}

ssize_t array_max(const void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b), void *max)
{
    size_t pos;

    BYTE *_max;
    const BYTE *_t;

    size_t offset_i;
    size_t offset_max;
    size_t _size_of = size_of;

    TRACE();

    if (array == NULL)
        ERROR("array == NULL\n", -1);

    if (len == 0)
        ERROR("len == 0\n", -1);

    if (size_of == 0)
        ERROR("size_of == 0\n", -1);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", -1);

    _max = alloc_on_stack(_size_of);

    _t = (const BYTE *)array;

    pos = 0;
    __ASSIGN__(_max[0], _t[0], _size_of);
    offset_max = len * _size_of;
    for (offset_i = _size_of; offset_i < offset_max; offset_i += _size_of)
        if (cmp((void *)_max, (void *)&_t[offset_i]) < 0)
        {
            __ASSIGN__(_max[0], _t[offset_i], _size_of);
            pos = offset_i;
        }

    if (max != NULL)
        __ASSIGN__(*(BYTE *)max, _t[pos], _size_of);

    return (ssize_t)pos / (ssize_t)_size_of;
}

ssize_t array_select_kth(const void *array, size_t len, size_t size_of, size_t k, int (*cmp)(const void *a, const void *b), void *kth)
{
    ssize_t index;
    const BYTE *_t;

    TRACE();

    index = select_kth(array, len, size_of, k, cmp);
    if (index == -1)
        ERROR("select_kth error\n", -1);

    _t = (const BYTE *)array;
    if (kth != NULL)
         __ASSIGN__(*(BYTE *)kth, _t[(size_t)index * size_of], size_of);

    return index;
}

ssize_t array_median(const void *array, size_t len, size_t size_of, int (*cmp)(const void *a, const void *b), void *median)
{
    TRACE();
    return array_select_kth(array, len, size_of, len >> 1, cmp, median);
}

ssize_t array_unsorted_find_first(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, int (*cmp)(const void *a, const void *b),  void *val)
{
    ssize_t index;
    BYTE *t;

    TRACE();

    index = find_first_unsorted(key, array, len, cmp, size_of);
    if (index == -1)
        return -1;

    t = (BYTE *)array;
    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, t[(size_t)index * size_of], size_of);

    return index;
}

ssize_t array_unsorted_find_last(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, int (*cmp)(const void *a, const void *b),  void *val)
{
    ssize_t index;
    BYTE *t;

    TRACE();

    index = find_last_unsorted(key, array, len, cmp, size_of);
    if (index == -1)
        return -1;

    t = (BYTE *)array;
    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, t[(size_t)index * size_of], size_of);

    return index;
}

ssize_t array_sorted_find_first(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, int (*cmp)(const void *a, const void *b),  void *val)
{
    ssize_t index;
    BYTE *t;

    TRACE();

    index = find_first_sorted(key, array, len, cmp, size_of);
    if (index == -1)
        return -1;

    t = (BYTE *)array;
    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, t[(size_t)index * size_of], size_of);

    return index;
}

ssize_t array_sorted_find_last(const void * ___restrict___ array, size_t len, size_t size_of, const void * ___restrict___ key, int (*cmp)(const void *a, const void *b),  void *val)
{
    ssize_t index;
    BYTE *t;

    TRACE();

    index = find_last_sorted(key, array, len, cmp, size_of);
    if (index == -1)
        return -1;

    t = (BYTE *)array;
    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, t[(size_t)index * size_of], size_of);

    return index;
}
