#include <stdlib.h>
#include <string.h>
#include <darray.h>
#include <generic.h>
#include <sort.h>
#include <log.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define INIT_SIZE 1024

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val - searched val

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
__inline__ static int darray_unsorted_search_first(Darray *darray, void *val);

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val - searched val

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
__inline__ static int darray_unsorted_search_last(Darray *darray, void *val);

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val - searched val

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
__inline__ static int darray_sorted_search_first(Darray *darray, void *val);

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val - searched val

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
__inline__ static int darray_sorted_search_last(Darray *darray, void *val);

/*
    Find pos where val i MAX ( <= val )

    PARAMS
    @IN darray - pointer to darray
    @IN val - searched val

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
__inline__ static int upper_bound(Darray *darray, void *val);

__inline__ static int darray_unsorted_search_first(Darray *darray, void *val)
{
    BYTE *_t;
    size_t offset_i;
    int max;

    TRACE("");

    assert( darray == NULL || darray->array == NULL || val == NULL
            || darray->cmp == NULL);

    assert(darray->num_entries < 0);

    _t = (BYTE *)darray->array;
    max = darray->num_entries * darray->size_of;

    for (offset_i = 0; offset_i < max; offset_i += darray->size_of)
        if (darray->cmp((void *)&_t[offset_i], val) == 0)
            return offset_i / darray->size_of;

    return -1;
}

__inline__ static int darray_unsorted_search_last(Darray *darray, void *val)
{
    BYTE *_t;
    size_t offset_i;

    TRACE("");

    assert( darray == NULL || darray->array == NULL || val == NULL
            || darray->cmp == NULL);

    assert(darray->num_entries < 0);

    _t = (BYTE *)darray->array;
    offset_i = darray->num_entries * darray->size_of - darray->size_of;

    for (offset_i = darray->num_entries * darray->size_of - darray->size_of;
         offset_i >= 0; offset_i -= darray->size_of)
        if (darray->cmp((void *)&_t[offset_i], val) == 0)
            return offset_i / darray->size_of;

    return -1;
}

__inline__ static int darray_sorted_search_first(Darray *darray, void *val)
{
    BYTE *_t;

    size_t offset_left;
    size_t offset_right;
    size_t offset_middle;

    TRACE("");

    assert( darray == NULL || darray->array == NULL || val == NULL
            || darray->cmp == NULL);

    assert(darray->num_entries < 0);

    _t = (BYTE *)darray->array;

    offset_left = 0;
    offset_right = (darray->num_entries - 1) * darray->size_of;

    while (offset_left < offset_right)
    {
        offset_middle = ((offset_left / darray->size_of
             + offset_right / darray->size_of) >> 1) * darray->size_of;

        if (darray->cmp((void *)&_t[offset_middle], val) < 0)
            offset_left = offset_middle + darray->size_of;
        else
            offset_right = offset_middle;
    }

    if (darray->cmp((void *)&_t[offset_left], val) == 0)
         return offset_left / darray->size_of;
     else
         return -1;
}

__inline__ static int darray_sorted_search_last(Darray *darray, void *val)
{
    BYTE *_t;

    size_t offset_left;
    size_t offset_right;
    size_t offset_middle;

    TRACE("");

    assert( darray == NULL || darray->array == NULL || val == NULL
            || darray->cmp == NULL);

    assert(darray->num_entries < 0);

    _t = (BYTE *)darray->array;

    offset_left = 0;
    offset_right = (darray->num_entries - 1) * darray->size_of;

    while (offset_left < offset_right)
    {
        offset_middle = ((offset_left / darray->size_of
             + offset_right / darray->size_of + 1) >> 1) * darray->size_of;

        if (darray->cmp((void *)&_t[offset_middle], val) > 0)
            offset_right = offset_middle - darray->size_of;
        else
            offset_left = offset_middle;
    }

    if (darray->cmp((void *)&_t[offset_left], val) == 0)
         return offset_left / darray->size_of;
     else
         return -1;
}

__inline__ static int upper_bound(Darray *darray, void *val)
{
    size_t offset_left;
    size_t offset_right;
    size_t offset_middle;

    BYTE *_t;

    TRACE("");

    assert( darray == NULL || darray->array == NULL || val == NULL
            || darray->cmp == NULL);

    offset_left = - darray->size_of;
    offset_right = (darray->num_entries - 1) * darray->size_of;
    offset_middle = 0;

    _t = (BYTE *)darray->array;

    while (offset_left <= offset_right)
    {
        offset_middle = ((offset_left / darray->size_of
             + offset_right / darray->size_of) >> 1) * darray->size_of;

        if (darray->cmp((void* )&_t[offset_middle], val) > 0)
            offset_right = offset_middle - darray->size_of;
        else
        {
            if (offset_middle + darray->size_of
            >(darray->num_entries - 1) * darray->size_of ||
            darray->cmp((void *)&_t[offset_middle + darray->size_of], val) > 0)
                break;
            else
                offset_left = offset_middle + darray->size_of;
        }
    }

    return offset_middle / darray->size_of;
}

Darray_iterator *darray_iterator_create(Darray *darray, ITI_MODE mode)
{
    Darray_iterator *iterator;

    TRACE("");

    assert(darray == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator = (Darray_iterator *)malloc(sizeof(Darray_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    iterator->array = darray->array;
    iterator->array_length = darray->num_entries;

	if (mode == ITI_BEGIN)
    	iterator->index = 0;
	else
		iterator->index = darray->num_entries - 1;

    iterator->size_of = darray->size_of;

    return iterator;
}

void darray_iterator_destroy(Darray_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int darray_iterator_init(Darray *darray, Darray_iterator *iterator,ITI_MODE mode)
{
    TRACE("");

    assert(darray == NULL || iterator == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator->array = darray->array;
    iterator->array_length = darray->num_entries;

   	if (mode == ITI_BEGIN)
    	iterator->index = 0;
	else
		iterator->index = darray->num_entries - 1;

    iterator->size_of = darray->size_of;

    return 0;
}

int darray_iterator_next(Darray_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    ++iterator->index;

    return 0;
}

int darray_iterator_prev(Darray_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    --iterator->index;

    return 0;
}

int darray_iterator_get_data(Darray_iterator *iterator, void *val)
{
    BYTE *t;

    TRACE("");

    assert(iterator == NULL || val == NULL);

    t = (BYTE *)iterator->array;
    __ASSIGN__(*(BYTE *)val, t[iterator->index * iterator->size_of],
        iterator->size_of);

    return 0;
}

bool darray_iterator_end(Darray_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    return (iterator->index >= iterator->array_length || iterator->index < 0);
}

Darray *darray_create(ARRAY_TYPE type, size_t size, int size_of,
        int (*cmp)(void *a, void*b))
{
    Darray *da;

    TRACE("");

    assert(type != SORTED && type != UNSORTED);
    assert(size < 0 || size_of < 0);

    if (type == SORTED && cmp == NULL)
        ERROR("cmp is needed for sorted array\n", NULL, "");

    da = (Darray *)malloc(sizeof(Darray));
    if (da == NULL)
        ERROR("malloc error on da\n", NULL, "");

    if(size == 0)
        da->size = INIT_SIZE;
    else
        da->size = size;

    da->init_size = da->size;
    da->size_of = size_of;
    da->type = type;
    da->cmp = cmp;

    da->array = malloc(da->size * da->size_of);

    if (da->array == NULL)
    {
        FREE(da);
        ERROR("malloc error\n", NULL, "");
    }

    da->num_entries = 0;

    return da;
}

void darray_destroy(Darray *darray)
{
    TRACE("");

    if (darray == NULL)
        return;

    FREE(darray->array);
    FREE(darray);
}

int darray_insert(Darray *darray, void *entry)
{
    int status;
    int pos;

    TRACE("");

    assert(darray == NULL || darray->array == NULL || entry == NULL);

    if(darray->type == UNSORTED)
        status = darray_insert_pos(darray, entry, darray->num_entries);
    else
    {
        if (darray->cmp == NULL)
            ERROR("CMP is needed\n", 1, "");

        if (darray->num_entries == 0)
            pos = 0;
        else
            pos = upper_bound(darray, entry) + 1;

        status = darray_insert_pos(darray, entry, pos);
    }

    if (status)
        ERROR("darray_insert_pos error\n", 1, "");

    return 0;
}

int darray_insert_pos(Darray *darray, void *entry, size_t pos)
{
    BYTE *_t;
    BYTE *_entry;

    TRACE("");

    assert(darray == NULL || darray->array == NULL);
    assert(pos < 0 || pos > darray->num_entries);

    /* resize array */
    if (darray->num_entries + 1 >= darray->size)
    {
        darray->size <<= 1;

        darray->array = realloc(darray->array, darray->size * (darray->size_of));
        if (darray->array == NULL)
            ERROR("realloc error\n", 1, "");
    }

    _t = (BYTE *)darray->array;

    /* make empty slot in pos and insert an entry */
    if ((memmove(   (void *)(_t + ((pos + 1) * darray->size_of)),
                    (void *)(_t + (pos * darray->size_of)),
                    (darray->num_entries - pos) * darray->size_of )) == NULL)
        ERROR("memmove error\n", 1, "");

    _entry = (BYTE *)entry;

    __ASSIGN__(_t[pos * darray->size_of], *_entry, darray->size_of);

    ++darray->num_entries;

    return 0;
}

int darray_delete(Darray *darray)
{
    TRACE("");

    assert(darray == NULL || darray->array == NULL);

    if (darray_delete_pos(darray, darray->num_entries))
        ERROR("darray_delete_pos error\n",1,"");

    return 0;
}

int darray_delete_pos(Darray *darray, size_t pos)
{
    BYTE *_t;

    TRACE("");

    assert(darray == NULL || darray->array == NULL);
    assert(pos < 0 || pos > darray->num_entries);

	if (darray->num_entries == 0)
		ERROR("Nothing to delete\n", 1, "");

    _t = (BYTE *)darray->array;

    if ((memmove( (void *)(_t + (pos * darray->size_of)),
                  (void *)(_t + ((pos + 1) * darray->size_of)),
                  (darray->num_entries - pos) * darray->size_of )) == NULL)
        ERROR("memmove error\n", 1, "");

    --darray->num_entries;

    /* resize array */
    if (darray->num_entries <= (darray->size * 0.4)
    && ((darray->size >> 1) >= darray->init_size))
    {
        darray->size >>= 1;

        darray->array = realloc(darray->array, darray->size * (darray->size_of));
        if (darray->array == NULL)
            ERROR("realloc error\n",1,"");
    }

    return 0;
}

int darray_search_first(Darray *darray, void *val)
{
     TRACE("");

     assert(darray == NULL || darray->array == NULL || val == NULL);
     assert(darray->num_entries <= 0);

     if (darray->type == UNSORTED)
        return darray_unsorted_search_first(darray, val);
     else
     {
         if (darray->cmp == NULL)
             ERROR("CMP is needed\n", -1, "");

         return darray_sorted_search_first(darray, val);
     }
}

int darray_search_last(Darray *darray, void *val)
{
    TRACE("");

    assert(darray == NULL || darray->array == NULL || val == NULL);
    assert(darray->num_entries <= 0);

    if (darray->type == UNSORTED)
        return darray_unsorted_search_last(darray, val);
    else
    {
        if (darray->cmp == NULL)
            ERROR("CMP is needed\n", -1, "");

       return darray_sorted_search_last(darray, val);
    }
}

int darray_sort(Darray *darray)
{
    TRACE("");

    assert(darray == NULL || darray->array == NULL);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1, "");

    if (darray->type == SORTED)
        return 0;

    return sort(darray->array, darray->num_entries, darray->cmp, darray->size_of);
}

int darray_min(Darray *darray)
{
    size_t pos;

    BYTE min[MAXWORD];
    BYTE *_t;

    size_t offset_i;
    size_t offset_max;

    TRACE("");

    assert(darray == NULL || darray->array == NULL);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1, "");

    pos = 0;
    _t = (BYTE *)darray->array;

    __ASSIGN__(min[0], _t[0], darray->size_of);

    offset_max = darray->num_entries * darray->size_of;

    for (offset_i = darray->size_of; offset_i < offset_max;
         offset_i += darray->size_of)
        if (darray->cmp((void *)min, (void *)&_t[offset_i]) > 0)
        {
            __ASSIGN__(min[0], _t[offset_i], darray->size_of);
            pos = offset_i;
        }

    return pos / darray->size_of;
}

int darray_max(Darray *darray)
{
    size_t pos;

    BYTE max[MAXWORD];
    BYTE *_t;

    size_t offset_i;
    size_t offset_max;

    TRACE("");

    assert(darray == NULL || darray->array == NULL);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1, "");

    pos = 0;
    _t = (BYTE *)darray->array;

    __ASSIGN__(max[0], _t[0], darray->size_of);

    offset_max = darray->num_entries * darray->size_of;

    for (offset_i = darray->size_of; offset_i < offset_max;
         offset_i += darray->size_of)
        if (darray->cmp((void *)max, (void *)&_t[offset_i]) < 0)
        {
            __ASSIGN__(max[0], _t[offset_i], darray->size_of);
            pos = offset_i;
        }

    return pos / darray->size_of;
}
