#include <stdlib.h>
#include <string.h>
#include <darray.h>
#include <generic.h>
#include <array.h>
#include <log.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define INIT_SIZE (size_t)1024

/*
    Insert an entry to array ( unsorted ) in array[pos]

    PARAMS
    @IN darray - pointer to Darray
    @IN entry - inserted entry (void *)&var
    @IN pos - index where insert
    @IN check - check type or not

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
static int _darray_insert_pos(Darray *darray, const void *entry, size_t pos, bool check);

/*
    Delete entry from array

    PARAMS
    @IN darray - pointer to Darray
    @IN pos - pos from delete entry
    @INdestroy - destroy during deleting ?

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
static int __darray_delete_pos(Darray *darray, size_t pos, bool destroy);

/*
    Destroy whole darray

    PARAMS
    @IN darray - pointer to darray
    @IN destroy - call destructor ?

    RETURN:
    This is a void function
*/
static void __darray_destroy_with_entries(Darray *darray, bool destroy);

static int _darray_insert_pos(Darray *darray, const void *entry, size_t pos, bool check)
{
    TRACE();

    assert(darray != NULL);
    assert(darray->____array != NULL);
    assert(pos <= darray->____num_entries);

    if (check)
        if (darray->____type == DARRAY_SORTED)
            return 1;

    /* resize array */
    if (darray->____num_entries + 1 >= darray->____size)
    {
        darray->____size <<= 1;

        darray->____array = realloc(darray->____array, darray->____size * (darray->____size_of));
        if (darray->____array == NULL)
            ERROR("realloc error\n", 1);
    }

    if (array_unsorted_insert_pos(darray->____array, darray->____num_entries + 1, darray->____size_of, pos, entry))
        ERROR("Insert error\n", 1);

    ++darray->____num_entries;

    return 0;
}

static int __darray_delete_pos(Darray *darray, size_t pos, bool destroy)
{
    int ret;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (pos >= darray->____num_entries)
        ERROR("pos >= num_entries\n", 1);

	if (darray->____num_entries == 0)
		ERROR("Nothing to delete\n", 1);

    if (destroy && darray->____destroy != NULL)
        ret = array_delete_pos_with_entry(darray->____array, darray->____num_entries, darray->____size_of, pos, darray->____destroy);
    else
        ret = array_delete_pos(darray->____array, darray->____num_entries, darray->____size_of, pos);

    if (ret)
        ERROR("Deleting error\n", ret);

    --darray->____num_entries;

    /* resize array */
    if ((double)darray->____num_entries <= ((double)darray->____size * 0.4) && ((darray->____size >> 1) >= darray->____init_size))
    {
        darray->____size >>= 1;

        darray->____array = realloc(darray->____array, darray->____size * (darray->____size_of));
        if (darray->____array == NULL)
            ERROR("realloc error\n", 1);
    }

    return 0;
}

static void __darray_destroy_with_entries(Darray *darray, bool destroy)
{
    TRACE();

    size_t i;
    BYTE *t;

    if (darray == NULL)
        return;

    t = (BYTE *)darray->____array;

    if (destroy && darray->____destroy != NULL)
        for (i = 0; i < darray->____num_entries; ++i)
            darray->____destroy((void *)(t + (i * darray->____size_of)));


    FREE(darray->____array);
    FREE(darray);
}

Darray *darray_create(DARRAY_TYPE type, size_t size, size_t size_of,
        int (*cmp)(const void *a, const void *b),
        void (*destroy)(void *entry))
{
    Darray *da;

    TRACE();

    if (type != DARRAY_SORTED && type != DARRAY_UNSORTED)
        ERROR("Incorrect type\n", NULL);

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (type == DARRAY_SORTED && cmp == NULL)
        ERROR("cmp is needed for sorted array\n", NULL);

    da = (Darray *)malloc(sizeof(Darray));
    if (da == NULL)
        ERROR("malloc error on da\n", NULL);

    if(size == 0)
        da->____size = INIT_SIZE;
    else
        da->____size = size;

    da->____init_size = da->____size;
    da->____size_of = size_of;
    da->____type = type;
    da->____cmp = cmp;
    da->____destroy = destroy;

    da->____array = malloc(da->____size * da->____size_of);

    if (da->____array == NULL)
    {
        FREE(da);
        ERROR("malloc error\n", NULL);
    }

    da->____num_entries = 0;

    return da;
}

void darray_destroy(Darray *darray)
{
    TRACE();

    __darray_destroy_with_entries(darray, false);
}

void darray_destroy_with_entries(Darray *darray)
{
    TRACE();

    __darray_destroy_with_entries(darray, true);
}

int darray_insert(Darray *darray, const void *entry)
{
    int status;
    ssize_t pos;

    TRACE();

    if (darray == NULL || entry == NULL)
        ERROR("darray == NULL || entry == NULL\n", 1);

    if (darray->____type == DARRAY_UNSORTED)
        status = _darray_insert_pos(darray, entry, darray->____num_entries, false);
    else
    {
        if (darray->____cmp == NULL)
            ERROR("CMP is needed\n", 1);

        if (darray->____num_entries == 0)
            pos = 0;
        else
            pos = array_upper_bound(darray->____array, darray->____num_entries, darray->____size_of, entry, darray->____cmp);

        if (pos == -1)
            ERROR("Upper bound error\n", 1);

        status = _darray_insert_pos(darray, entry, (size_t)pos, false);
    }

    if (status)
        ERROR("darray_insert_pos error\n", 1);

    return 0;
}

int darray_insert_pos(Darray *darray, const void *entry, size_t pos)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (pos > darray->____num_entries)
        ERROR("pos > num_entries\n", 1);

    return _darray_insert_pos(darray, entry, pos, true);
}

int darray_delete(Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (__darray_delete_pos(darray, darray->____num_entries - 1, false))
        ERROR("darray_delete_pos error\n",1);

    return 0;
}

int darray_delete_with_entry(Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (__darray_delete_pos(darray, darray->____num_entries - 1, true))
        ERROR("darray_delete_pos error\n",1);

    return 0;
}

int darray_delete_pos(Darray *darray, size_t pos)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (__darray_delete_pos(darray, pos, false))
        ERROR("darray_delete_pos error\n",1);

    return 0;
}

int darray_delete_pos_with_entry(Darray *darray, size_t pos)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (__darray_delete_pos(darray, pos, true))
        ERROR("darray_delete_pos error\n",1);

    return 0;
}

ssize_t darray_search_first(const Darray *darray, const void *val_in, void *val_out)
{
    TRACE();

    if (darray == NULL || val_in == NULL)
        ERROR("darray == NULL || val_in == NULL\n", -1);

    if (darray->____num_entries == 0)
        ERROR("num_entries == 0\n", -1);

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1);

    if (darray->____type == DARRAY_UNSORTED)
        return array_unsorted_find_first(darray->____array, darray->____num_entries, darray->____size_of, val_in, darray->____cmp, val_out);
    else
        return array_sorted_find_first(darray->____array, darray->____num_entries, darray->____size_of, val_in, darray->____cmp, val_out);
}

ssize_t darray_search_last(const Darray *darray, const void *val_in, void *val_out)
{
    TRACE();

    if (darray == NULL || val_in == NULL)
        ERROR("darray == NULL || val_in == NULL\n", -1);

    if (darray->____num_entries == 0)
        ERROR("num_entries == 0\n", -1);

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1);

    if (darray->____type == DARRAY_UNSORTED)
        return array_unsorted_find_last(darray->____array, darray->____num_entries, darray->____size_of, val_in, darray->____cmp, val_out);
    else
        return array_sorted_find_last(darray->____array, darray->____num_entries, darray->____size_of, val_in, darray->____cmp, val_out);
}

int darray_sort(Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1);

    if (darray->____num_entries == 0)
        ERROR("Darray is empty, nothing to sort\n", 1);

    if (darray->____type == DARRAY_SORTED)
        return 0;

    return array_sort(darray->____array, darray->____num_entries, darray->____size_of, darray->____cmp);
}

ssize_t darray_min(const Darray *darray, void *val)
{
    BYTE *_t;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1);

    _t = (BYTE *)darray->____array;
    if (darray->____type == DARRAY_SORTED)
    {
        if (darray->____num_entries == 0)
            return (ssize_t)-1;

        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, _t[0], darray->____size_of);

        return 0;
    }

    return array_min(darray->____array, darray->____num_entries, darray->____size_of, darray->____cmp, val);
}

ssize_t darray_max(const Darray *darray, void *val)
{
    BYTE *_t;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1);

    _t = (BYTE *)darray->____array;
    if (darray->____type == DARRAY_SORTED)
    {
        if (darray->____num_entries == 0)
            return (ssize_t)-1;

        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, _t[(darray->____num_entries - 1) * darray->____size_of], darray->____size_of);

        return (ssize_t)darray->____num_entries - 1;
    }

    return array_max(darray->____array, darray->____num_entries, darray->____size_of, darray->____cmp, val);
}

void *darray_get_array(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", NULL);

    return darray->____array;
}

ssize_t darray_get_num_entries(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    return (ssize_t)darray->____num_entries;
}

DARRAY_TYPE darray_get_type(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    return darray->____type;
}

ssize_t darray_get_data_size(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    return (ssize_t)darray->____size_of;
}

Darray_iterator *darray_iterator_create(const Darray *darray, iti_mode_t mode)
{
    Darray_iterator *iterator;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    if (darray->____num_entries == 0)
        return NULL;

    iterator = (Darray_iterator *)malloc(sizeof(Darray_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    iterator->____array = darray->____array;
    iterator->____array_length = darray->____num_entries;

	if (mode == ITI_BEGIN)
    	iterator->____index = 0;
	else
		iterator->____index = (ssize_t)darray->____num_entries - 1;

    iterator->____size_of = darray->____size_of;

    return iterator;
}

void darray_iterator_destroy(Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int darray_iterator_init(const Darray *darray, Darray_iterator *iterator, iti_mode_t mode)
{
    TRACE();

    if (darray == NULL || iterator == NULL)
        ERROR("darray == NULL || iterator == NULL\n", 1);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", 1);

    if (darray->____num_entries == 0)
        return 1;

    iterator->____array = darray->____array;
    iterator->____array_length = darray->____num_entries;

   	if (mode == ITI_BEGIN)
    	iterator->____index = 0;
	else
		iterator->____index = (ssize_t)darray->____num_entries - 1;

    iterator->____size_of = darray->____size_of;

    return 0;
}

int darray_iterator_next(Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    ++iterator->____index;

    return 0;
}

int darray_iterator_prev(Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    --iterator->____index;

    return 0;
}

int darray_iterator_get_data(const Darray_iterator *iterator, void *val)
{
    BYTE *t;

    TRACE();

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1);

    t = (BYTE *)iterator->____array;
    __ASSIGN__(*(BYTE *)val, t[iterator->____index * (ssize_t)iterator->____size_of], iterator->____size_of);

    return 0;
}

int darray_iterator_get_node(const Darray_iterator *iterator, void *node)
{
    TRACE();

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || node == NULL\n", 1);

    *(void **)node = ((BYTE *)iterator->____array) + iterator->____index * (ssize_t)iterator->____size_of;

    return 0;
}

bool darray_iterator_end(const Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return (iterator->____index < 0 || (size_t)iterator->____index >= iterator->____array_length);
}
