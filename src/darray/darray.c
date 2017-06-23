#include <stdlib.h>
#include <string.h>
#include <darray.h>
#include <generic.h>
#include <sort.h>
#include <log.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define INIT_SIZE (size_t)1024

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val_in - val with key
    @IN val_out - val from array or NULL

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
___inline___ static ssize_t darray_unsorted_search_first(Darray *darray,
                                                void *val_in, void *val_out);

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val_in - val with key
    @IN val_out - val from array or NULL

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
___inline___ static ssize_t darray_unsorted_search_last(Darray *darray,
                                                void *val_in, void *val_out);

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val_in - val with key
    @IN val_out - val from array or NULL

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
___inline___ static ssize_t darray_sorted_search_first(Darray *darray,
                                                void *val_in, void *val_out);

/*
    PARAMS
    @IN darray - pointer to darray
    @IN val_in - val with key
    @IN val_out - val from array or NULL

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
___inline___ static ssize_t darray_sorted_search_last(Darray *darray,
                                                void *val_in, void *val_out);

/*
    Find pos where val i MAX ( <= val )

    PARAMS
    @IN darray - pointer to darray
    @IN val - searched val

    RETURN:
    %-1 iff failure
    %Pos iff success
*/
___inline___ static ssize_t upper_bound(Darray *darray, void *val);


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
static int _darray_insert_pos(Darray *darray, void *entry, size_t pos, bool check);

___inline___ static ssize_t darray_unsorted_search_first(Darray *darray,
                                                    void *val_in, void *val_out)
{
    BYTE *_t;
    size_t offset_i;
    size_t max;

    TRACE("");

    assert(darray != NULL);
    assert(darray->____array != NULL);
    assert(val_in != NULL);
    assert(darray->____cmp != NULL);
    assert(darray->____num_entries != 0);

    _t = (BYTE *)darray->____array;
    max = darray->____num_entries * darray->____size_of;

    for (offset_i = 0; offset_i < max; offset_i += darray->____size_of)
        if (darray->____cmp((void *)&_t[offset_i], val_in) == 0)
        {
            if (val_out != NULL)
                __ASSIGN__(*(BYTE *)val_out, _t[offset_i], darray->____size_of);
            return (ssize_t)offset_i / (ssize_t)darray->____size_of;
        }

    return (ssize_t)-1;
}

___inline___ static ssize_t darray_unsorted_search_last(Darray *darray,
                                                    void *val_in, void *val_out)
{
    BYTE *_t;
    ssize_t offset_i;

    TRACE("");

    assert(darray != NULL);
    assert(darray->____array != NULL);
    assert(val_in != NULL);
    assert(darray->____cmp != NULL);
    assert(darray->____num_entries != 0);

    _t = (BYTE *)darray->____array;
    offset_i = (ssize_t)darray->____num_entries * (ssize_t)darray->____size_of - (ssize_t)darray->____size_of;

    for (offset_i = (ssize_t)darray->____num_entries * (ssize_t)darray->____size_of - (ssize_t)darray->____size_of;
         offset_i >= 0; offset_i -= (ssize_t)darray->____size_of)
        if (darray->____cmp((void *)&_t[offset_i], val_in) == 0)
        {
            if (val_out != NULL)
                __ASSIGN__(*(BYTE *)val_out, _t[offset_i], darray->____size_of);
            return offset_i / (ssize_t)darray->____size_of;
        }

    return (ssize_t)-1;
}

___inline___ static ssize_t darray_sorted_search_first(Darray *darray,
                                                    void *val_in, void *val_out)
{
    BYTE *_t;

    size_t offset_left;
    size_t offset_right;
    size_t offset_middle;

    TRACE("");

    assert(darray != NULL);
    assert(darray->____array != NULL);
    assert(val_in != NULL);
    assert(darray->____cmp != NULL);
    assert(darray->____num_entries != 0);

    _t = (BYTE *)darray->____array;

    offset_left = 0;
    offset_right = (darray->____num_entries - 1) * darray->____size_of;

    while (offset_left < offset_right)
    {
        offset_middle = ((offset_left / darray->____size_of
             + offset_right / darray->____size_of) >> 1) * darray->____size_of;

        if (darray->____cmp((void *)&_t[offset_middle], val_in) < 0)
            offset_left = offset_middle + darray->____size_of;
        else
            offset_right = offset_middle;
    }

    if (darray->____cmp((void *)&_t[offset_left], val_in) == 0)
    {
        if (val_out != NULL)
            __ASSIGN__(*(BYTE *)val_out, _t[offset_left], darray->____size_of);

        return (ssize_t)offset_left / (ssize_t)darray->____size_of;
    }
    else
        return (ssize_t)-1;
}

___inline___ static ssize_t darray_sorted_search_last(Darray *darray,
                                            void *val_in, void *val_out)
{
    BYTE *_t;

    size_t offset_left;
    size_t offset_right;
    size_t offset_middle;

    TRACE("");

    assert(darray != NULL);
    assert(darray->____array != NULL);
    assert(val_in != NULL);
    assert(darray->____cmp != NULL);
    assert(darray->____num_entries != 0);

    _t = (BYTE *)darray->____array;

    offset_left = 0;
    offset_right = (darray->____num_entries - 1) * darray->____size_of;

    while (offset_left < offset_right)
    {
        offset_middle = ((offset_left / darray->____size_of
             + offset_right / darray->____size_of + 1) >> 1) * darray->____size_of;

        if (darray->____cmp((void *)&_t[offset_middle], val_in) > 0)
            offset_right = offset_middle - darray->____size_of;
        else
            offset_left = offset_middle;
    }

    if (darray->____cmp((void *)&_t[offset_left], val_in) == 0)
    {
        if (val_out != NULL)
            __ASSIGN__(*(BYTE *)val_out, _t[offset_left], darray->____size_of);

        return (ssize_t)offset_left / (ssize_t)darray->____size_of;
    }
    else
        return (ssize_t)-1;
}

___inline___ static ssize_t upper_bound(Darray *darray, void *val)
{
    ssize_t offset_left;
    ssize_t offset_right;
    ssize_t offset_middle;

    BYTE *_t;

    TRACE("");

    assert(darray != NULL);
    assert(darray->____array != NULL);
    assert(val != NULL);
    assert(darray->____cmp != NULL);

    offset_left = 0;
    offset_right = ((ssize_t)darray->____num_entries - 1) * (ssize_t)darray->____size_of;
    offset_middle = 0;

    _t = (BYTE *)darray->____array;

    /* normal upper bound */
    while (offset_left <= offset_right)
    {
        offset_middle = ((offset_left / (ssize_t)darray->____size_of
             + offset_right / (ssize_t)darray->____size_of) >> 1) * (ssize_t)darray->____size_of;

        if (darray->____cmp((void *)&_t[offset_middle], val) >= 0)
            offset_right = offset_middle - (ssize_t)darray->____size_of;
        else
            offset_left = offset_middle + (ssize_t)darray->____size_of;
    }

    if (darray->____cmp((void *)&_t[offset_middle], val) > 0)
        offset_middle -= (ssize_t)darray->____size_of;

    return offset_middle / (ssize_t)darray->____size_of;
}

static int _darray_insert_pos(Darray *darray, void *entry, size_t pos, bool check)
{
    BYTE *_t;
    BYTE *_entry;

    TRACE("");

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
            ERROR("realloc error\n", 1, "");
    }

    _t = (BYTE *)darray->____array;

    /* make empty slot in pos and insert an entry */
    if ((memmove(   (void *)(_t + ((pos + 1) * darray->____size_of)),
                    (void *)(_t + (pos * darray->____size_of)),
                    (darray->____num_entries - pos) * darray->____size_of )) == NULL)
        ERROR("memmove error\n", 1, "");

    _entry = (BYTE *)entry;

    __ASSIGN__(_t[pos * darray->____size_of], *_entry, darray->____size_of);

    ++darray->____num_entries;

    return 0;
}

Darray *darray_create(DARRAY_TYPE type, size_t size, int size_of,
        int (*cmp)(void *a, void*b))
{
    Darray *da;

    TRACE("");

    if (type != DARRAY_SORTED && type != DARRAY_UNSORTED)
        ERROR("Incorrect type\n", NULL, "");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    if (type == DARRAY_SORTED && cmp == NULL)
        ERROR("cmp is needed for sorted array\n", NULL, "");

    da = (Darray *)malloc(sizeof(Darray));
    if (da == NULL)
        ERROR("malloc error on da\n", NULL, "");

    if(size == 0)
        da->____size = INIT_SIZE;
    else
        da->____size = size;

    da->____init_size = da->____size;
    da->____size_of = (size_t)size_of;
    da->____type = type;
    da->____cmp = cmp;

    da->____array = malloc(da->____size * da->____size_of);

    if (da->____array == NULL)
    {
        FREE(da);
        ERROR("malloc error\n", NULL, "");
    }

    da->____num_entries = 0;

    return da;
}

void darray_destroy(Darray *darray)
{
    TRACE("");

    if (darray == NULL)
        return;

    FREE(darray->____array);
    FREE(darray);
}

void darray_destroy_with_entries(Darray *darray, void (*destructor)(void *data))
{
    TRACE("");

    size_t i;
    BYTE *t;

    if (darray == NULL || destructor == NULL)
        return;

    t = (BYTE *)darray->____array;

    for (i = 0; i < darray->____num_entries; ++i)
       destructor((void *)(t + (i * darray->____size_of)));


    FREE(darray->____array);
    FREE(darray);
}


int darray_insert(Darray *darray, void *entry)
{
    int status;
    ssize_t pos;

    TRACE("");

    if (darray == NULL || entry == NULL)
        ERROR("darray == NULL || entry == NULL\n", 1, "");

    if (darray->____type == DARRAY_UNSORTED)
        status = _darray_insert_pos(darray, entry, darray->____num_entries, false);
    else
    {
        if (darray->____cmp == NULL)
            ERROR("CMP is needed\n", 1, "");

        if (darray->____num_entries == 0)
            pos = 0;
        else
            pos = upper_bound(darray, entry) + 1;

        status = _darray_insert_pos(darray, entry, (size_t)pos, false);

    }

    if (status)
        ERROR("darray_insert_pos error\n", 1, "");

    return 0;
}

int darray_insert_pos(Darray *darray, void *entry, size_t pos)
{
    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", 1, "");

    if (pos > darray->____num_entries)
        ERROR("pos > num_entries\n", 1, "");

    return _darray_insert_pos(darray, entry, pos, true);
}

int darray_delete(Darray *darray)
{
    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", 1, "");

    if (darray_delete_pos(darray, darray->____num_entries - 1))
        ERROR("darray_delete_pos error\n",1,"");

    return 0;
}

int darray_delete_pos(Darray *darray, size_t pos)
{
    BYTE *_t;

    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", 1, "");

    if (pos >= darray->____num_entries)
        ERROR("pos >= num_entries\n", 1, "");

	if (darray->____num_entries == 0)
		ERROR("Nothing to delete\n", 1, "");

    _t = (BYTE *)darray->____array;

    if ((memmove( (void *)(_t + (pos * darray->____size_of)),
                  (void *)(_t + ((pos + 1) * darray->____size_of)),
                  (darray->____num_entries - pos - 1) * darray->____size_of )) == NULL)
        ERROR("memmove error\n", 1, "");

    --darray->____num_entries;

    /* resize array */
    if ((double)darray->____num_entries <= ((double)darray->____size * 0.4)
    && ((darray->____size >> 1) >= darray->____init_size))
    {
        darray->____size >>= 1;

        darray->____array = realloc(darray->____array, darray->____size * (darray->____size_of));
        if (darray->____array == NULL)
            ERROR("realloc error\n",1,"");
    }

    return 0;
}

ssize_t darray_search_first(Darray *darray, void *val_in, void *val_out)
{
    TRACE("");

    if (darray == NULL || val_in == NULL)
        ERROR("darray == NULL || val_in == NULL\n", -1, "");

    if (darray->____num_entries == 0)
        ERROR("num_entries == 0\n", -1, "");

    if (darray->____type == DARRAY_UNSORTED)
        return darray_unsorted_search_first(darray, val_in, val_out);
    else
    {
        if (darray->____cmp == NULL)
            ERROR("CMP is needed\n", -1, "");

        return darray_sorted_search_first(darray, val_in, val_out);
    }
}

ssize_t darray_search_last(Darray *darray, void *val_in, void *val_out)
{
    TRACE("");

    if (darray == NULL || val_in == NULL)
        ERROR("darray == NULL || val_in == NULL\n", -1, "");

    if (darray->____num_entries == 0)
        ERROR("num_entries == 0\n", -1, "");

    if (darray->____type == DARRAY_UNSORTED)
        return darray_unsorted_search_last(darray, val_in, val_out);
    else
    {
        if (darray->____cmp == NULL)
            ERROR("CMP is needed\n", -1, "");

       return darray_sorted_search_last(darray, val_in, val_out);
    }
}

int darray_sort(Darray *darray)
{
    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", 1, "");

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1, "");

    if (darray->____num_entries == 0)
        ERROR("Darray is empty, nothing to sort\n", 1, "");

    if (darray->____type == DARRAY_SORTED)
        return 0;

    return sort(darray->____array, darray->____num_entries, darray->____cmp, (int)darray->____size_of);
}

ssize_t darray_min(Darray *darray, void *val)
{
    size_t pos;

    BYTE min[MAXWORD];
    BYTE *_t;

    size_t offset_i;
    size_t offset_max;

    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", -1, "");

    _t = (BYTE *)darray->____array;
    if (darray->____type == DARRAY_SORTED)
    {
        if (darray->____num_entries == 0)
            return (ssize_t)-1;

        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, _t[0], darray->____size_of);

        return 0;
    }

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1, "");

    pos = 0;

    __ASSIGN__(min[0], _t[0], darray->____size_of);

    offset_max = darray->____num_entries * darray->____size_of;

    for (offset_i = darray->____size_of; offset_i < offset_max;
         offset_i += darray->____size_of)
        if (darray->____cmp((void *)min, (void *)&_t[offset_i]) > 0)
        {
            __ASSIGN__(min[0], _t[offset_i], darray->____size_of);
            pos = offset_i;
        }

    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, _t[pos], darray->____size_of);

    return (ssize_t)pos / (ssize_t)darray->____size_of;
}

ssize_t darray_max(Darray *darray, void *val)
{
    size_t pos;

    BYTE max[MAXWORD];
    BYTE *_t;

    size_t offset_i;
    size_t offset_max;

    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", -1, "");

    _t = (BYTE *)darray->____array;
    if (darray->____type == DARRAY_SORTED)
    {
        if (darray->____num_entries == 0)
            return (ssize_t)-1;

        if (val != NULL)
            __ASSIGN__(*(BYTE *)val, _t[(darray->____num_entries - 1) * darray->____size_of],
                                darray->____size_of);

        return (ssize_t)darray->____num_entries - 1;
    }

    if (darray->____cmp == NULL)
        ERROR("CMP is needed\n", -1, "");

    pos = 0;

    __ASSIGN__(max[0], _t[0], darray->____size_of);

    offset_max = darray->____num_entries * darray->____size_of;

    for (offset_i = darray->____size_of; offset_i < offset_max;
         offset_i += darray->____size_of)
        if (darray->____cmp((void *)max, (void *)&_t[offset_i]) <= 0)
        {
            __ASSIGN__(max[0], _t[offset_i], darray->____size_of);
            pos = offset_i;
        }

    if (val != NULL)
        __ASSIGN__(*(BYTE *)val, _t[pos], darray->____size_of);

    return (ssize_t)pos / (ssize_t)darray->____size_of;
}

void *darray_get_array(Darray *darray)
{
    if (darray == NULL)
        ERROR("darray == NULL\n", NULL, "");

    return darray->____array;
}

ssize_t darray_get_num_entries(Darray *darray)
{
    if (darray == NULL)
        ERROR("darray == NULL\n", -1, "");

    return (ssize_t)darray->____num_entries;
}

DARRAY_TYPE darray_get_type(Darray *darray)
{
    if (darray == NULL)
        ERROR("darray == NULL\n", -1, "");

    return darray->____type;
}

int darray_get_size_of(Darray *darray)
{
    if (darray == NULL)
        ERROR("darray == NULL\n", -1, "");

    return (int)darray->____size_of;
}

Darray_iterator *darray_iterator_create(Darray *darray, ITI_MODE mode)
{
    Darray_iterator *iterator;

    TRACE("");

    if (darray == NULL)
        ERROR("darray == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL, "");

    iterator = (Darray_iterator *)malloc(sizeof(Darray_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

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
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int darray_iterator_init(Darray *darray, Darray_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    if (darray == NULL || iterator == NULL)
        ERROR("darray == NULL || iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", 1, "");

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
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    ++iterator->____index;

    return 0;
}

int darray_iterator_prev(Darray_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    --iterator->____index;

    return 0;
}

int darray_iterator_get_data(Darray_iterator *iterator, void *val)
{
    BYTE *t;

    TRACE("");

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1, "");

    t = (BYTE *)iterator->____array;
    __ASSIGN__(*(BYTE *)val, t[iterator->____index * (ssize_t)iterator->____size_of],
        iterator->____size_of);

    return 0;
}

int darray_iterator_get_node(Darray_iterator *iterator, void *node)
{
    TRACE("");

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || node == NULL\n", 1, "");

    *(void **)node = ((BYTE *)iterator->____array) + iterator->____index * (ssize_t)iterator->____size_of;

    return 0;
}

bool darray_iterator_end(Darray_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true, "");

    return (iterator->____index < 0 || (size_t)iterator->____index >= iterator->____array_length);
}
