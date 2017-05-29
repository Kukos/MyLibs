#include <stdlib.h>
#include <log.h>
#include <heap.h>
#include <generic.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define PARENT(n, d) (((n) - 1) / (d))
#define SON(n, d, k) (((n) * (d) + (k)))

#define SWAP_ENTRY(a, b) \
    do { \
        SWAP(a, b); \
        SWAP(a->pos, b->pos); \
    } while (0)

/*
    cmp function for min heap property

    PARAMS
    @IN a - first heap entry
    @IN b - second heap entry
    @IN cmp - data cmp funcion

    RETURN
    1 - (a, b) fulfills min heap property
    0 - (a, b) don't fulfills min heap property
*/
static int __min_heap_cmp(Heap_entry *a, Heap_entry *b,
    int (*cmp)(void *a, void *b));

/*
    cmp function for max heap property

    PARAMS
    @IN a - first heap entry
    @IN b - second heap entry
    @IN cmp - data cmp funcion

    RETURN
    1 - (a, b) fulfills max heap property
    0 - (a, b) don't fulfills max heap property
*/
static int __max_heap_cmp(Heap_entry *a, Heap_entry *b,
    int (*cmp)(void *a, void *b));


static int __min_heap_cmp(Heap_entry *a, Heap_entry *b,
    int (*cmp)(void *a, void *b))
{
    return cmp(a->data, b->data) > 0;
}

static int __max_heap_cmp(Heap_entry *a, Heap_entry *b,
    int (*cmp)(void *a, void *b))
{
    return cmp(a->data, b->data) < 0;
}

/*
    Fix heap property, start from index i and go to leaf level

    PARAMS
    @IN heap - pointer to dheap
    @IN index - start index
    @IN cmp - cmp heap property

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
___inline___ static int __heap_heapify(Heap *heap, size_t index,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));

/*
    Build heap from array

    PARAMS
    @IN heap - pointer to heap
    @IN array - pointer to array
    @IN n - size of array
    @IN cmp - cmp heap property

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
___inline___ static int __heap_build(Heap *heap, Heap_entry **array, size_t n,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));


/*
    Insert entry to heap

    PARAMS
    @IN dheap - pointer to heap
    @IN entry - pointer to heap entry
    @IN cmp - cmp heap property

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
___inline___ static int __heap_insert(Heap *heap, Heap_entry *entry,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));

/*
    Extract first element

    PARAMS
    @IN heap - pointer to heap
    @IN cmp - cmp heap property

    RETURN:
    %NULL if failure
    %pointer if success
*/
___inline___ static Heap_entry *__heap_extract_top(Heap *heap,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));

/*
    Get first

    PARAMS
    @IN heap - pointer to dheap
    @IN cmp - cmp heap property

    RETURN:
    %NULL if failure
    %pointer if success

*/
___inline___ static Heap_entry *__heap_get_top(Heap *heap,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));

/*
    Change the key value, create before new entry with changed,
    in heap we deallocate only ptr to data, so free your old entry
    to avoid memory leak

    PARAMS
    @IN bheap - pointer to bheap
    @IN index - entry index in heao
    @IN new_data - new data with new key
    @IN cmp - cmp heap property

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
___inline___ static int __heap_change_key(Heap *heap, size_t index, void *new_data,
        int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));

___inline___ static int __heap_heapify(Heap *heap, size_t index,
        int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    Heap_entry **array;
    Heap_entry *temp;

    size_t size;
    size_t next;
    size_t son;
    int k;

    TRACE("");

    assert(heap != NULL);
    assert(index >= 0 && index < darray_get_num_entries(heap->darray));

    size = darray_get_num_entries(heap->darray);
    array = (Heap_entry **)darray_get_array(heap->darray);

    while (index < size)
    {
        temp = array[index];
        next = index;

        for (k = 1; k <= heap->ary; ++k)
        {
            son = SON(index, heap->ary, k);
            if (son < size && ! cmp(array[son], temp, heap->cmp))
            {
                temp = array[son];
                next = son;
            }
        }

        if (next != index)
            SWAP_ENTRY(array[index], array[next]);
        else
            break;

        index = next;
    }

    return 0;
}

___inline___ static int __heap_build(Heap *heap, Heap_entry **array, size_t n,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    size_t i;

    TRACE("");

    assert(heap != NULL);
    assert(array != NULL);
    assert( n >= 0);

    /* create heap */
    for (i = 0; i < n; ++i)
    {
        array[i]->pos = i;
        if (darray_insert_pos(heap->darray, (void *)&array[i], i))
            ERROR("darray_insert_pos error\n", 1, "");
    }

    /* fix heap property */
    for (i = n / heap->ary; i >= 0; --i)
        if (__heap_heapify(heap, i, cmp))
            ERROR("__heap_heapify error\n", 1, "");

    return 0;
}

___inline___ static int __heap_insert(Heap *heap, Heap_entry *entry,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    Heap_entry **array;
    size_t i;

    TRACE("");

    assert(heap != NULL);
    assert(entry != NULL);

    entry->pos = darray_get_num_entries(heap->darray);

    if (darray_insert_pos(heap->darray, (void *)&entry, entry->pos))
        ERROR("darray_insert_pos error\n", 1, "");

    i = entry->pos;
    array = (Heap_entry **)darray_get_array(heap->darray);

    while (i > 0 && cmp(array[PARENT(i, heap->ary)], array[i], heap->cmp))
    {
        SWAP_ENTRY(array[PARENT(i, heap->ary)], array[i]);
        i = PARENT(i, heap->ary);
    }

    return 0;
}

___inline___ static Heap_entry *__heap_get_top(Heap *heap,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    TRACE("");

    assert(heap != NULL);
    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL, "");

    return ((Heap_entry **)(darray_get_array(heap->darray)))[0];
}

___inline___ static Heap_entry *__heap_extract_top(Heap *heap,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))

{
    Heap_entry **array;
    Heap_entry *entry;

    TRACE("");

    assert(heap != NULL);
    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL, "");

    entry = ((Heap_entry **)(darray_get_array(heap->darray)))[0];
    entry->pos = OUT_OF_HEAP;

    array = (Heap_entry **)darray_get_array(heap->darray);
    SWAP_ENTRY(array[0], array[darray_get_num_entries(heap->darray) - 1]);

    if (darray_delete(heap->darray))
        ERROR("darray_delete error\n", NULL, "");

    if (! heap_is_empty(heap))
        if (__heap_heapify(heap, 0, cmp))
            ERROR("__heap_heapify error\n", NULL, "");

    return entry;
}

___inline___ static int __heap_change_key(Heap *heap, size_t index, void *new_data,
        int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    Heap_entry **array;
    size_t i;

    TRACE("");

    assert(heap != NULL);
    assert(index >= 0 && index < darray_get_num_entries(heap->darray));

    array = (Heap_entry **)darray_get_array(heap->darray);

    __ASSIGN__(*(BYTE *)array[index]->data, *(BYTE *)new_data, heap->size_of);

    i = index;
    while (i > 0 && cmp(array[PARENT(i, heap->ary)], array[i], heap->cmp))
    {
        SWAP_ENTRY(array[PARENT(i, heap->ary)], array[i]);
        i = PARENT(i, heap->ary);
    }

    return 0;
}

Heap_entry *heap_entry_create(void *data, int size_of)
{
    Heap_entry *entry;

    TRACE("");

    assert(data == NULL || size_of < 0);

    entry = (Heap_entry *)malloc(sizeof(Heap_entry));
    if (entry == NULL)
        ERROR("malloc error\n", NULL, "");

    entry->pos = OUT_OF_HEAP;

    entry->data = malloc(size_of);
    if (entry->data == NULL)
    {
        FREE(entry);

        ERROR("malloc error\n", NULL, "");
    }

    __ASSIGN__(*(BYTE *)entry->data, *(BYTE *)data, size_of);

    return entry;
}

void heap_entry_destroy(Heap_entry *entry)
{
    TRACE("");

    if (entry == NULL)
        return;

    FREE(entry->data);
    FREE(entry);
}


Heap* heap_create(HEAP_TYPE type, int size_of, int ary,
     int (*cmp)(void *a, void *b))
{
    Heap *heap;

    TRACE("");

    assert(type == HEAP_MIN || type == HEAP_MAX);
    assert(ary >= 2);
    assert(cmp != NULL);
    assert(size_of >= 1);

    heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL)
        ERROR("heap == NULL\n", NULL, "");

    heap->cmp       = cmp;
    heap->size_of   = size_of;
    heap->type      = type;
    heap->ary       = ary;
    heap->darray    = darray_create(DARRAY_UNSORTED, 0, sizeof(Heap_entry *), cmp);

    if (heap->darray == NULL)
        ERROR("darray_create error\n", NULL, "");

    return heap;
}

void heap_destroy(Heap *heap)
{
    Heap_entry **array;
    int i;

    TRACE("");

    if (heap == NULL)
        return;

    array = (Heap_entry **)darray_get_array(heap->darray);

    for(i = 0; i < darray_get_num_entries(heap->darray); ++i)
        heap_entry_destroy(array[i]);

    darray_destroy(heap->darray);

    FREE(heap);
}

void heap_destroy_with_entries(Heap *heap, void (*destructor)(void *data))
{
    Heap_entry **array;
    int i;

    TRACE("");

    if (heap == NULL)
        return;

    array = (Heap_entry **)darray_get_array(heap->darray);

    for(i = 0; i < darray_get_num_entries(heap->darray); ++i)
    {
        destructor(array[i]->data);
        heap_entry_destroy(array[i]);
    }

    darray_destroy(heap->darray);

    FREE(heap);
}

int heap_build(Heap *heap, Heap_entry **array, size_t n)
{
    TRACE("");

    assert(heap != NULL);
    assert(array != NULL);
    assert(n >= 0);

    if(heap->type == HEAP_MIN)
        return __heap_build(heap, array, n, __min_heap_cmp);
    else
        return __heap_build(heap, array, n, __max_heap_cmp);
}

int heap_insert(Heap *heap, Heap_entry *entry)
{
    TRACE("");

    assert(heap != NULL);
    assert(entry != NULL);

    if (heap->type == HEAP_MIN)
        return __heap_insert(heap, entry, __min_heap_cmp);
    else
        return __heap_insert(heap, entry, __max_heap_cmp);
}

Heap_entry *heap_extract_top(Heap *heap)
{
    TRACE("");

    assert(heap != NULL);

    if(heap->type == HEAP_MIN)
        return __heap_extract_top(heap, __min_heap_cmp);
    else
        return __heap_extract_top(heap, __max_heap_cmp);
}

Heap_entry *heap_get_top(Heap *heap)
{
    TRACE("");

    assert(heap != NULL);

    if (heap->type == HEAP_MIN)
        return __heap_get_top(heap, __min_heap_cmp);
    else
        return __heap_get_top(heap, __max_heap_cmp);
}

int heap_change_key(Heap *heap, size_t index, void *new_data)
{
    Heap_entry **array;

    TRACE("");

    assert(heap != NULL);
    assert(new_data != NULL);
    assert(index >= 0 && index < darray_get_num_entries(heap->darray));

    array = (Heap_entry **)heap->darray;

    if (heap->type == HEAP_MIN)
    {
        if (heap->cmp(array[index]->data, new_data) <= 0)
            ERROR("bad key, new key must be less than old\n", 1, "");

        return __heap_change_key(heap, index, new_data, __min_heap_cmp);
    }
    else
    {
        if (heap->cmp(array[index]->data, new_data) >= 0)
            ERROR("bad key, new key must be grater than old\n", 1, "");

        return __heap_change_key(heap, index, new_data, __max_heap_cmp);
    }

    return 0;
}

bool heap_is_empty(Heap *heap)
{
    TRACE("");

    return (heap == NULL || heap->darray == NULL
                || darray_get_num_entries(heap->darray) == 0);
}

Heap_iterator *heap_iterator_create(Heap *heap, ITI_MODE mode)
{
    TRACE("");

    return (Heap_iterator *)darray_iterator_create(heap->darray, mode);
}

int heap_iterator_init(Heap *heap, Heap_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    return darray_iterator_init(heap->darray, (Darray_iterator *)iterator, mode);
}

void heap_iterator_destroy(Heap_iterator *iterator)
{
    TRACE("");

    darray_iterator_destroy(iterator);
}

int heap_iterator_next(Heap_iterator *iterator)
{
    TRACE("");

    return darray_iterator_next(iterator);
}

int heap_iterator_prev(Heap_iterator *iterator)
{
    TRACE("");

    return darray_iterator_prev(iterator);
}

int heap_iterator_get_data(Heap_iterator *iterator, void *data)
{
    TRACE("");

    return darray_iterator_get_data(iterator, data);
}

bool heap_iterator_end(Heap_iterator *iterator)
{
    TRACE("");

    return darray_iterator_end(iterator);
}
