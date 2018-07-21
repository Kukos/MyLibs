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


struct Darray
{
    void    *array;         /* main array */

    size_t  size_of;        /* size of element */
    size_t  size;           /* current allocated size ( num of entries ) */
    size_t  num_entries;    /* num of entries in array */
    size_t  init_size;      /* minimum size using on init ( num of entries ) */

    destructor_f destroy;
    cmp_f cmp; /* pointer to compare function */
    DARRAY_TYPE type;        /* type of array ( sorted or unsorted ) */
};

struct Darray_iterator
{
    void        *array;         /* pointer to array */
    ssize_t     index;          /* index of array */
    size_t      size_of;        /* sizeof element in array */
    size_t      array_length;   /* array length */

};

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
static int _darray_insert_pos(Darray * ___restrict___ darray, const void * ___restrict___ entry, size_t pos, bool check);

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

static int _darray_insert_pos(Darray * ___restrict___ darray, const void * ___restrict___ entry, size_t pos, bool check)
{
    TRACE();

    assert(darray != NULL);
    assert(darray->array != NULL);
    assert(pos <= darray->num_entries);

    if (check)
        if (darray->type == DARRAY_SORTED)
            return 1;

    /* resize array */
    if (darray->num_entries + 1 >= darray->size)
    {
        darray->size <<= 1;

        darray->array = realloc(darray->array, darray->size * (darray->size_of));
        if (darray->array == NULL)
            ERROR("realloc error\n", 1);
    }

    if (array_unsorted_insert_pos(darray->array, darray->num_entries + 1, darray->size_of, pos, entry))
        ERROR("Insert error\n", 1);

    ++darray->num_entries;

    return 0;
}

static int __darray_delete_pos(Darray *darray, size_t pos, bool destroy)
{
    int ret;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (pos >= darray->num_entries)
        ERROR("pos >= num_entries\n", 1);

	if (darray->num_entries == 0)
		ERROR("Nothing to delete\n", 1);

    if (destroy && darray->destroy != NULL)
        ret = array_delete_pos_with_entry(darray->array, darray->num_entries, darray->size_of, pos, darray->destroy);
    else
        ret = array_delete_pos(darray->array, darray->num_entries, darray->size_of, pos);

    if (ret)
        ERROR("Deleting error\n", ret);

    --darray->num_entries;

    /* resize array */
    if ((double)darray->num_entries <= ((double)darray->size * 0.4) && ((darray->size >> 1) >= darray->init_size))
    {
        darray->size >>= 1;

        darray->array = realloc(darray->array, darray->size * (darray->size_of));
        if (darray->array == NULL)
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

    t = (BYTE *)darray->array;

    if (destroy && darray->destroy != NULL)
        for (i = 0; i < darray->num_entries; ++i)
            darray->destroy((void *)(t + (i * darray->size_of)));


    FREE(darray->array);
    FREE(darray);
}

Darray *darray_create(DARRAY_TYPE type, size_t size, size_t size_of,
        cmp_f cmp,
        destructor_f destroy)
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
        da->size = INIT_SIZE;
    else
        da->size = size;

    da->init_size = da->size;
    da->size_of = size_of;
    da->type = type;
    da->cmp = cmp;
    da->destroy = destroy;

    da->array = malloc(da->size * da->size_of);

    if (da->array == NULL)
    {
        FREE(da);
        ERROR("malloc error\n", NULL);
    }

    da->num_entries = 0;

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

int darray_insert(Darray * ___restrict___ darray, const void * ___restrict___ entry)
{
    int status;
    ssize_t pos;

    TRACE();

    if (darray == NULL || entry == NULL)
        ERROR("darray == NULL || entry == NULL\n", 1);

    if (darray->type == DARRAY_UNSORTED)
        status = _darray_insert_pos(darray, entry, darray->num_entries, false);
    else
    {
        if (darray->cmp == NULL)
            ERROR("CMP is needed\n", 1);

        if (darray->num_entries == 0)
            pos = 0;
        else
            pos = array_upper_bound(darray->array, darray->num_entries, darray->size_of, entry, darray->cmp);

        if (pos == -1)
            ERROR("Upper bound error\n", 1);

        status = _darray_insert_pos(darray, entry, (size_t)pos, false);
    }

    if (status)
        ERROR("darray_insert_pos error\n", 1);

    return 0;
}

int darray_insert_pos(Darray * ___restrict___ darray, const void * ___restrict___ entry, size_t pos)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (pos > darray->num_entries)
        ERROR("pos > num_entries\n", 1);

    return _darray_insert_pos(darray, entry, pos, true);
}

int darray_delete(Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (__darray_delete_pos(darray, darray->num_entries - 1, false))
        ERROR("darray_delete_pos error\n",1);

    return 0;
}

int darray_delete_with_entry(Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (__darray_delete_pos(darray, darray->num_entries - 1, true))
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

int darray_get_data(const Darray *darray, size_t pos, void *data)
{
    BYTE *t;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (pos >= darray->num_entries)
        ERROR("Invalid pos\n", 1);

    t = (BYTE *)darray->array;
    __ASSIGN__(*(BYTE *)data, t[pos * darray->size_of], darray->size_of);

    return 0;
}

ssize_t darray_search_first(const Darray * ___restrict___ darray, const void *val_in, void *val_out)
{
    TRACE();

    if (darray == NULL || val_in == NULL)
        ERROR("darray == NULL || val_in == NULL\n", -1);

    if (darray->num_entries == 0)
        ERROR("num_entries == 0\n", -1);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1);

    if (darray->type == DARRAY_UNSORTED)
        return array_unsorted_find_first(darray->array, darray->num_entries, darray->size_of, val_in, darray->cmp, val_out);
    else
        return array_sorted_find_first(darray->array, darray->num_entries, darray->size_of, val_in, darray->cmp, val_out);
}

ssize_t darray_search_last(const Darray * ___restrict___ darray, const void *val_in, void *val_out)
{
    TRACE();

    if (darray == NULL || val_in == NULL)
        ERROR("darray == NULL || val_in == NULL\n", -1);

    if (darray->num_entries == 0)
        ERROR("num_entries == 0\n", -1);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1);

    if (darray->type == DARRAY_UNSORTED)
        return array_unsorted_find_last(darray->array, darray->num_entries, darray->size_of, val_in, darray->cmp, val_out);
    else
        return array_sorted_find_last(darray->array, darray->num_entries, darray->size_of, val_in, darray->cmp, val_out);
}

int darray_sort(Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", 1);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1);

    if (darray->num_entries == 0)
        ERROR("Darray is empty, nothing to sort\n", 1);

    if (darray->type == DARRAY_SORTED)
        return 0;

    return array_sort(darray->array, darray->num_entries, darray->size_of, darray->cmp);
}

ssize_t darray_min(const Darray * ___restrict___ darray, void * ___restrict___ val)
{
    BYTE *_t;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1);

    _t = (BYTE *)darray->array;
    if (darray->type == DARRAY_SORTED)
    {
        if (darray->num_entries == 0)
            return (ssize_t)-1;

        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, _t[0], darray->size_of);

        return 0;
    }

    return array_min(darray->array, darray->num_entries, darray->size_of, darray->cmp, val);
}

ssize_t darray_max(const Darray * ___restrict___ darray, void * ___restrict___ val)
{
    BYTE *_t;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    if (darray->cmp == NULL)
        ERROR("CMP is needed\n", -1);

    _t = (BYTE *)darray->array;
    if (darray->type == DARRAY_SORTED)
    {
        if (darray->num_entries == 0)
            return (ssize_t)-1;

        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, _t[(darray->num_entries - 1) * darray->size_of], darray->size_of);

        return (ssize_t)darray->num_entries - 1;
    }

    return array_max(darray->array, darray->num_entries, darray->size_of, darray->cmp, val);
}

void *darray_get_array(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", NULL);

    return darray->array;
}

ssize_t darray_get_num_entries(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    return (ssize_t)darray->num_entries;
}

DARRAY_TYPE darray_get_type(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    return darray->type;
}

ssize_t darray_get_data_size(const Darray *darray)
{
    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", -1);

    return (ssize_t)darray->size_of;
}

Darray_iterator *darray_iterator_create(const Darray *darray, iti_mode_t mode)
{
    Darray_iterator *iterator;

    TRACE();

    if (darray == NULL)
        ERROR("darray == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    if (darray->num_entries == 0)
        return NULL;

    iterator = (Darray_iterator *)malloc(sizeof(Darray_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    iterator->array = darray->array;
    iterator->array_length = darray->num_entries;

	if (mode == ITI_BEGIN)
    	iterator->index = 0;
	else
		iterator->index = (ssize_t)darray->num_entries - 1;

    iterator->size_of = darray->size_of;

    return iterator;
}

void darray_iterator_destroy(Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int darray_iterator_next(Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    ++iterator->index;

    return 0;
}

int darray_iterator_prev(Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    --iterator->index;

    return 0;
}

int darray_iterator_get_data(const Darray_iterator * ___restrict___ iterator, void * ___restrict___ val)
{
    BYTE *t;

    TRACE();

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1);

    t = (BYTE *)iterator->array;
    __ASSIGN__(*(BYTE *)val, t[iterator->index * (ssize_t)iterator->size_of], iterator->size_of);

    return 0;
}

int darray_iterator_get_node(const Darray_iterator * ___restrict___ iterator, void * ___restrict___ node)
{
    TRACE();

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || node == NULL\n", 1);

    *(void **)node = ((BYTE *)iterator->array) + iterator->index * (ssize_t)iterator->size_of;

    return 0;
}

bool darray_iterator_end(const Darray_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return (iterator->index < 0 || (size_t)iterator->index >= iterator->array_length);
}
