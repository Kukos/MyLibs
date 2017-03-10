#include  <stdlib.h>
#include  <log.h>
#include  <heap.h>
#include  <generic.h>

#define PARENT(n, d) (((n) - 1) / (d))
#define SON(n, d, k) (((n) *(d) + (k)))

#define FREE(T) do { free(T); T = NULL; } while (0)
#define SWAP(a, b) \
    do{ \
        typeof(a) __temp = a; \
        a = b; \
        b = __temp; \
    } while (0)

#define SWAP_ENTRY(a,b) \
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
static int __heap_heapify(Heap *heap, int index,
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
static int __heap_build(Heap *heap, Heap_entry **array, int n,
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
static int __heap_insert(Heap *heap, Heap_entry *entry,
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
static Heap_entry *__heap_extract_top(Heap *heap,
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
static Heap_entry *__heap_get_top(Heap *heap,
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
static int __heap_change_key(Heap *heap, int index, void *new_data,
        int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)));

static int __heap_heapify(Heap *heap, int index,
        int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    Heap_entry **array;
    Heap_entry *temp;

    int size;
    int next;
    int k;
    int son;

    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", 1, "");

    if (index < 0 || index >= heap->darray->num_entries)
        ERROR("index < 0 || index >= heap->heap->num_entries\n", 1, "");

    size = heap->darray->num_entries;
    array = (Heap_entry **)heap->darray->array;

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

static int __heap_build(Heap *heap, Heap_entry **array, int n,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    int i;

    TRACE("");

    if (heap == NULL || array == NULL || n < 0 )
        ERROR("heap == NULL || array == NULL || n < 0\n", 1, "");

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

static int __heap_insert(Heap *heap, Heap_entry *entry,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    Heap_entry **array;
    int i;

    TRACE("");

    if (heap == NULL || entry == NULL)
        ERROR("heap == NULL || entry == NULL\n", 1, "");

    entry->pos = heap->darray->num_entries;

    if (darray_insert_pos(heap->darray, (void *)&entry, entry->pos))
        ERROR("darray_insert_pos error\n", 1, "");

    i = entry->pos;
    array = (Heap_entry **)heap->darray->array;

    while (i > 0 && cmp(array[PARENT(i, heap->ary)], array[i], heap->cmp))
    {
        SWAP_ENTRY(array[PARENT(i, heap->ary)], array[i]);
        i = PARENT(i, heap->ary);
    }

    return 0;
}

static Heap_entry *__heap_get_top(Heap *heap,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    TRACE("");

    if (heap == NULL || heap_is_empty(heap))
        ERROR("heap is empty\n", NULL, "");

    return ((Heap_entry **)(heap->darray->array))[0];
}

static Heap_entry *__heap_extract_top(Heap *heap,
    int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))

{
    Heap_entry **array;
    Heap_entry *entry;

    TRACE("");

    if (heap == NULL || heap_is_empty(heap))
        ERROR("heap is empty\n", NULL, "");

    entry = ((Heap_entry **)(heap->darray->array))[0];
    entry->pos = OUT_OF_HEAP;

    array = (Heap_entry **)heap->darray->array;
    SWAP_ENTRY(array[0], array[heap->darray->num_entries - 1]);

    if (darray_delete(heap->darray))
        ERROR("darray_delete error\n", NULL, "");

    if (! heap_is_empty(heap))
        if (__heap_heapify(heap, 0, cmp))
            ERROR("__heap_heapify error\n", NULL, "");

    return entry;
}

static int __heap_change_key(Heap *heap, int index, void *new_data,
        int (*cmp)(Heap_entry *a, Heap_entry *b, int (*cmp)(void *a, void *b)))
{
    Heap_entry **array;
    int i;

    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL\n", 1, "");

    if (index < 0 || index >= heap->darray->num_entries)
        ERROR("index < 0 || index >= heap->heap->num_entries\n", 1 ,"");

    array = (Heap_entry **)heap->darray->array;

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

    if (data == NULL || size_of < 0)
        ERROR("data == NULL || size_of < 0\n", NULL, "");

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
    {
        LOG("entry == NULL\n", "");

        return ;
    }

    FREE(entry->data);
    FREE(entry);
}


Heap* heap_create(HEAP_TYPE type, int size_of, int ary,
     int (*cmp)(void *a, void *b))
{
    Heap *heap;

    TRACE("");

    if ((type != HEAP_MIN && type != HEAP_MAX))
        ERROR("type != HEAP_MIN && type != HEAP_MAX\n", NULL, "");

    if (ary < 2)
        ERROR("ary < 2\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    if (size_of < 0)
        ERROR("size_of < 0\n", NULL, "");

    heap = (Heap *)malloc(sizeof(Heap));
    if (heap == NULL)
        ERROR("heap == NULL\n", NULL, "");

    heap->cmp       = cmp;
    heap->size_of   = size_of;
    heap->type      = type;
    heap->ary       = ary;
    heap->darray    = darray_create(UNSORTED, 0, sizeof(Heap_entry *), cmp);

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
    {
        LOG("heap == NULL\n", "");

        return;
    }

    array = (Heap_entry **)heap->darray->array;

    for(i = 0; i < heap->darray->num_entries; ++i)
        heap_entry_destroy(array[i]);

    darray_destroy(heap->darray);

    FREE(heap);
}

int heap_build(Heap *heap, Heap_entry **array, int n)
{
    TRACE("");

    if (heap == NULL || array == NULL || n < 0)
        ERROR("heap == NULL || array == NULL || n < 0", 1, "");

    if(heap->type == HEAP_MIN)
        return __heap_build(heap, array, n, __min_heap_cmp);
    else
        return __heap_build(heap, array, n, __max_heap_cmp);
}

int heap_insert(Heap *heap, Heap_entry *entry)
{
    TRACE("");

    if (heap == NULL || entry == NULL)
        ERROR("heap == NULL || entry == NULL", 1, "");

    if (heap->type == HEAP_MIN)
        return __heap_insert(heap, entry, __min_heap_cmp);
    else
        return __heap_insert(heap, entry, __max_heap_cmp);
}

Heap_entry *heap_extract_top(Heap *heap)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL", NULL, "");

    if(heap->type == HEAP_MIN)
        return __heap_extract_top(heap, __min_heap_cmp);
    else
        return __heap_extract_top(heap, __max_heap_cmp);
}

Heap_entry *heap_get_top(Heap *heap)
{
    TRACE("");

    if (heap == NULL)
        ERROR("heap == NULL", NULL, "");

    if (heap->type == HEAP_MIN)
        return __heap_get_top(heap, __min_heap_cmp);
    else
        return __heap_get_top(heap, __max_heap_cmp);
}

int heap_change_key(Heap *heap, int index, void *new_data)
{
    Heap_entry **array;

    TRACE("");

    if (heap == NULL || new_data == NULL)
        ERROR("heap == NULL || new_data == NULL\n", 1, "");

    if (index < 0 || index >= heap->darray->num_entries)
        ERROR("index < 0 || index >= heap->heap->num_entries\n", 1, "");

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

BOOL heap_is_empty(Heap *heap)
{
    TRACE("");

    return (heap == NULL || heap->darray == NULL
                || heap->darray->num_entries == 0);
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

BOOL heap_iterator_end(Heap_iterator *iterator)
{
    TRACE("");

    return darray_iterator_end(iterator);
}
