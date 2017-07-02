#include <stdlib.h>
#include <log.h>
#include <heap.h>
#include <generic.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define PARENT(n, d) (size_t)((((size_t)n) - 1) / ((size_t)d))
#define SON(n, d, k) (size_t)((((size_t)n) * ((size_t)d) + ((size_t)k)))

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
    Wrapper to heap entry destroy
*/
static void heap_entry_destructor(void *entry);

static void heap_entry_destructor(void *entry)
{
    Heap_entry *e = *(Heap_entry **)entry;

    heap_entry_destroy(e);
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
    assert(index < (size_t)darray_get_num_entries(heap->darray));

    size = (size_t)darray_get_num_entries(heap->darray);
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
    ssize_t i;

    TRACE("");

    assert(heap != NULL);
    assert(array != NULL);

    /* create heap */
    for (i = 0; i < (ssize_t)n; ++i)
    {
        array[i]->pos = i;
        if (darray_insert_pos(heap->darray, (void *)&array[i], (size_t)i))
            ERROR("darray_insert_pos error\n", 1, "");
    }

    /* fix heap property */
    for (i = (ssize_t)((size_t)n / (size_t)heap->ary); i >= 0; --i)
        if (__heap_heapify(heap, (size_t)i, cmp))
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

    if (darray_insert_pos(heap->darray, (void *)&entry, (size_t)entry->pos))
        ERROR("darray_insert_pos error\n", 1, "");

    i = (size_t)entry->pos;
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
    (void)cmp;

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

    if (!heap_is_empty(heap))
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
    assert(index < (size_t)darray_get_num_entries(heap->darray));

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

    if (data == NULL)
        ERROR("data == NULL\n", NULL, "");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    entry = (Heap_entry *)malloc(sizeof(Heap_entry));
    if (entry == NULL)
        ERROR("malloc error\n", NULL, "");

    entry->pos = OUT_OF_HEAP;

    entry->data = malloc((size_t)size_of);
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

ssize_t heap_entry_get_pos(Heap_entry *entry)
{
    TRACE("");

    if (entry == NULL)
        ERROR("entry == NULL\n", -2 ,"");

    return entry->pos;
}

void *heap_entry_get_data(Heap_entry *entry)
{
    TRACE("");

    if (entry == NULL)
        ERROR("entry == NULL\n", NULL ,"");

    return entry->data;
}
Heap* heap_create(heap_type type, int size_of, int ary,
     int (*cmp)(void *a, void *b))
{
    Heap *heap;

    TRACE("");

    if (type != HEAP_MIN && type != HEAP_MAX)
        ERROR("Incorrect type\n", NULL, "");

    if (ary < 2)
        ERROR("Incorrect heap ary\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

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
    TRACE("");

    if (heap == NULL)
        return;

    darray_destroy_with_entries(heap->darray, heap_entry_destructor);
    FREE(heap);
}

void heap_destroy_with_entries(Heap *heap, void (*destructor)(void *data))
{
    Heap_entry *entry;

    TRACE("");

    if (heap == NULL)
        return;

    if (destructor == NULL)
        return;

    for_each_data(heap->darray, Darray, entry)
        destructor(entry->data);

    darray_destroy_with_entries(heap->darray, heap_entry_destructor);
    FREE(heap);
}

int heap_build(Heap *heap, Heap_entry **array, size_t n)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", 1, "");

    if (array == NULL)
        ERROR("array == NULL\n", 1, "");

    if(heap->type == HEAP_MIN)
        return __heap_build(heap, array, n, __min_heap_cmp);
    else
        return __heap_build(heap, array, n, __max_heap_cmp);
}

int heap_insert(Heap *heap, Heap_entry *entry)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", 1, "");

    if (entry == NULL)
        ERROR("entry == NULL\n", 1, "");

    if (heap->type == HEAP_MIN)
        return __heap_insert(heap, entry, __min_heap_cmp);
    else
        return __heap_insert(heap, entry, __max_heap_cmp);
}

Heap_entry *heap_extract_top(Heap *heap)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", NULL, "");

    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL, "");

    if(heap->type == HEAP_MIN)
        return __heap_extract_top(heap, __min_heap_cmp);
    else
        return __heap_extract_top(heap, __max_heap_cmp);
}

Heap_entry *heap_get_top(Heap *heap)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", NULL, "");

    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL, "");

    if (heap->type == HEAP_MIN)
        return __heap_get_top(heap, __min_heap_cmp);
    else
        return __heap_get_top(heap, __max_heap_cmp);
}

int heap_change_key(Heap *heap, size_t index, void *new_data)
{
    Heap_entry **array;

    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", 1, "");

    if (new_data == NULL)
        ERROR("new_data == NULL\n", 1, "");

    if (index >= (size_t)darray_get_num_entries(heap->darray))
        ERROR("Incorrect index\n", 1, "");


    array = (Heap_entry **)darray_get_array(heap->darray);

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

    if (heap == NULL)
        ERROR("heap == NULL\n", true, "");

    return darray_get_num_entries(heap->darray) == 0;
}

ssize_t heap_get_num_entries(Heap *heap)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", -1, "");

    return darray_get_num_entries(heap->darray);
}

int heap_get_data_size(Heap *heap)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", -1, "");

    return heap->size_of;
}