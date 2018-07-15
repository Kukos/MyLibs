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
        SWAP(a->____pos, b->____pos); \
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
static int __min_heap_cmp(const Heap_entry *a, const Heap_entry *b,
    int (*cmp)(const void *a, const void *b));

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
static int __max_heap_cmp(const Heap_entry *a, const Heap_entry *b,
    int (*cmp)(const void *a, const void *b));


static int __min_heap_cmp(const Heap_entry *a, const Heap_entry *b,
    int (*cmp)(const void *a, const void *b))
{
    return cmp(a->____data, b->____data) > 0;
}

static int __max_heap_cmp(const Heap_entry *a, const Heap_entry *b,
    int (*cmp)(const void *a, const void *b))
{
    return cmp(a->____data, b->____data) < 0;
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
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)));

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
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)));


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
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)));

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
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)));

/*
    Get first

    PARAMS
    @IN heap - pointer to dheap
    @IN cmp - cmp heap property

    RETURN:
    %NULL if failure
    %pointer if success

*/
___inline___ static Heap_entry *__heap_get_top(const Heap *heap,
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)));

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
___inline___ static int __heap_change_key(Heap *heap, size_t index, const void *new_data,
        int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)));

___inline___ static int __heap_heapify(Heap *heap, size_t index,
        int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)))
{
    Heap_entry **array;
    Heap_entry *temp;

    size_t size;
    size_t next;
    size_t son;
    int k;

    TRACE();

    assert(heap != NULL);
    assert(index < (size_t)darray_get_num_entries(heap->____darray));

    size = (size_t)darray_get_num_entries(heap->____darray);
    array = (Heap_entry **)darray_get_array(heap->____darray);

    while (index < size)
    {
        temp = array[index];
        next = index;

        for (k = 1; k <= heap->____ary; ++k)
        {
            son = SON(index, heap->____ary, k);
            if (son < size && ! cmp(array[son], temp, heap->____cmp))
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
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)))
{
    ssize_t i;

    TRACE();

    assert(heap != NULL);
    assert(array != NULL);

    /* create heap */
    for (i = 0; i < (ssize_t)n; ++i)
    {
        array[i]->____pos = i;
        if (darray_insert_pos(heap->____darray, (void *)&array[i], (size_t)i))
            ERROR("darray_insert_pos error\n", 1);
    }

    /* fix heap property */
    for (i = (ssize_t)((size_t)n / (size_t)heap->____ary); i >= 0; --i)
        if (__heap_heapify(heap, (size_t)i, cmp))
            ERROR("__heap_heapify error\n", 1);

    return 0;
}

___inline___ static int __heap_insert(Heap *heap, Heap_entry *entry,
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)))
{
    Heap_entry **array;
    size_t i;

    TRACE();

    assert(heap != NULL);
    assert(entry != NULL);

    entry->____pos = darray_get_num_entries(heap->____darray);

    if (darray_insert_pos(heap->____darray, (void *)&entry, (size_t)entry->____pos))
        ERROR("darray_insert_pos error\n", 1);

    i = (size_t)entry->____pos;
    array = (Heap_entry **)darray_get_array(heap->____darray);

    while (i > 0 && cmp(array[PARENT(i, heap->____ary)], array[i], heap->____cmp))
    {
        SWAP_ENTRY(array[PARENT(i, heap->____ary)], array[i]);
        i = PARENT(i, heap->____ary);
    }

    return 0;
}

___inline___ static Heap_entry *__heap_get_top(const Heap *heap,
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)))
{
    TRACE();

    assert(heap != NULL);
    (void)cmp;

    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL);

    return ((Heap_entry **)(darray_get_array(heap->____darray)))[0];
}

___inline___ static Heap_entry *__heap_extract_top(Heap *heap,
    int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)))

{
    Heap_entry **array;
    Heap_entry *entry;

    TRACE();

    assert(heap != NULL);

    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL);

    entry = ((Heap_entry **)(darray_get_array(heap->____darray)))[0];
    entry->____pos = OUT_OF_HEAP;

    array = (Heap_entry **)darray_get_array(heap->____darray);
    SWAP_ENTRY(array[0], array[darray_get_num_entries(heap->____darray) - 1]);

    if (darray_delete(heap->____darray))
        ERROR("darray_delete error\n", NULL);

    if (!heap_is_empty(heap))
        if (__heap_heapify(heap, 0, cmp))
            ERROR("__heap_heapify error\n", NULL);

    return entry;
}

___inline___ static int __heap_change_key(Heap *heap, size_t index, const void *new_data,
        int (*cmp)(const Heap_entry *a, const Heap_entry *b, int (*cmp)(const void *a, const void *b)))
{
    Heap_entry **array;
    size_t i;

    TRACE();

    assert(heap != NULL);
    assert(index < (size_t)darray_get_num_entries(heap->____darray));

    array = (Heap_entry **)darray_get_array(heap->____darray);
    if (heap->____destroy != NULL)
        heap->____destroy(array[index]->____data);

    __ASSIGN__(*(BYTE *)array[index]->____data, *(BYTE *)new_data, heap->____size_of);

    i = index;
    while (i > 0 && cmp(array[PARENT(i, heap->____ary)], array[i], heap->____cmp))
    {
        SWAP_ENTRY(array[PARENT(i, heap->____ary)], array[i]);
        i = PARENT(i, heap->____ary);
    }

    return 0;
}

Heap_entry *heap_entry_create(const void *data, size_t size_of)
{
    Heap_entry *entry;

    TRACE();

    if (data == NULL)
        ERROR("data == NULL\n", NULL);

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    entry = (Heap_entry *)malloc(sizeof(Heap_entry));
    if (entry == NULL)
        ERROR("malloc error\n", NULL);

    entry->____pos = OUT_OF_HEAP;

    entry->____data = malloc(size_of);
    if (entry->____data == NULL)
    {
        FREE(entry);

        ERROR("malloc error\n", NULL);
    }

    __ASSIGN__(*(BYTE *)entry->____data, *(BYTE *)data, size_of);

    return entry;
}

void heap_entry_destroy(Heap_entry *entry)
{
    TRACE();

    if (entry == NULL)
        return;

    FREE(entry->____data);
    FREE(entry);
}

void heap_entry_destroy_with_data(Heap_entry *entry, void (*destructor)(void *entry))
{
    TRACE();

    if (entry == NULL)
        return;

    if (destructor)
        destructor(entry->____data);

    FREE(entry->____data);
    FREE(entry);
}

ssize_t heap_entry_get_pos(const Heap_entry *entry)
{
    TRACE();

    if (entry == NULL)
        ERROR("entry == NULL\n", -2);

    return entry->____pos;
}

void *heap_entry_get_data(const Heap_entry *entry)
{
    TRACE();

    if (entry == NULL)
        ERROR("entry == NULL\n", NULL);

    return entry->____data;
}
Heap* heap_create(heap_type type, size_t size_of, int ary,
     int (*cmp)(const void *a, const void *b),
     void (*destroy)(void *entry))
{
    Heap *heap;

    TRACE();

    if (type != HEAP_MIN && type != HEAP_MAX)
        ERROR("Incorrect type\n", NULL);

    if (ary < 2)
        ERROR("Incorrect heap ary\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL)
        ERROR("heap == NULL\n", NULL);

    heap->____cmp       = cmp;
    heap->____size_of   = size_of;
    heap->____type      = type;
    heap->____ary       = ary;
    heap->____destroy   = destroy;
    heap->____darray    = darray_create(DARRAY_UNSORTED, 0, sizeof(Heap_entry *), cmp, heap_entry_destructor);

    if (heap->____darray == NULL)
        ERROR("darray_create error\n", NULL);

    return heap;
}

void heap_destroy(Heap *heap)
{
    TRACE();

    if (heap == NULL)
        return;

    darray_destroy_with_entries(heap->____darray);
    FREE(heap);
}

void heap_destroy_with_entries(Heap *heap)
{
    Heap_entry *entry;

    TRACE();

    if (heap == NULL)
        return;

    if (heap->____destroy != NULL)
        for_each_data(heap->____darray, Darray, entry)
            heap->____destroy(entry->____data);

    darray_destroy_with_entries(heap->____darray);
    FREE(heap);
}

int heap_build(Heap *heap, Heap_entry **array, size_t n)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", 1);

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if(heap->____type == HEAP_MIN)
        return __heap_build(heap, array, n, __min_heap_cmp);
    else
        return __heap_build(heap, array, n, __max_heap_cmp);
}

int heap_insert(Heap *heap, Heap_entry *entry)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", 1);

    if (entry == NULL)
        ERROR("entry == NULL\n", 1);

    if (heap->____type == HEAP_MIN)
        return __heap_insert(heap, entry, __min_heap_cmp);
    else
        return __heap_insert(heap, entry, __max_heap_cmp);
}

Heap_entry *heap_extract_top(Heap *heap)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", NULL);

    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL);

    if(heap->____type == HEAP_MIN)
        return __heap_extract_top(heap, __min_heap_cmp);
    else
        return __heap_extract_top(heap, __max_heap_cmp);
}

Heap_entry *heap_get_top(const Heap *heap)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", NULL);

    if (heap_is_empty(heap))
        ERROR("heap is empty\n", NULL);

    if (heap->____type == HEAP_MIN)
        return __heap_get_top(heap, __min_heap_cmp);
    else
        return __heap_get_top(heap, __max_heap_cmp);
}

int heap_change_key(Heap *heap, size_t index, const void *new_data)
{
    Heap_entry **array;

    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", 1);

    if (new_data == NULL)
        ERROR("new_data == NULL\n", 1);

    if (index >= (size_t)darray_get_num_entries(heap->____darray))
        ERROR("Incorrect index\n", 1);


    array = (Heap_entry **)darray_get_array(heap->____darray);

    if (heap->____type == HEAP_MIN)
    {
        if (heap->____cmp(array[index]->____data, new_data) <= 0)
            ERROR("bad key, new key must be less than old\n", 1);

        return __heap_change_key(heap, index, new_data, __min_heap_cmp);
    }
    else
    {
        if (heap->____cmp(array[index]->____data, new_data) >= 0)
            ERROR("bad key, new key must be grater than old\n", 1);

        return __heap_change_key(heap, index, new_data, __max_heap_cmp);
    }

    return 0;
}

bool heap_is_empty(const Heap *heap)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", true);

    return darray_get_num_entries(heap->____darray) == 0;
}

ssize_t heap_get_num_entries(const Heap *heap)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", -1);

    return darray_get_num_entries(heap->____darray);
}

ssize_t heap_get_data_size(const Heap *heap)
{
    TRACE();

    if (heap == NULL)
        ERROR("heap == NULL\n", -1);

    return (ssize_t)heap->____size_of;
}