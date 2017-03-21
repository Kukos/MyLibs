#ifndef HEAP_H
#define HEAP_H

#include <darray.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */

/*
    Simple D-ary heap
    Please note that heap is "generic" and storing only pointers to structures
    ( if data is not simple type )
    So you must deallocate your data manually, heap only dealoate own memory

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#define HEAP_TYPE       char
#define HEAP_MIN        0
#define HEAP_MAX        1
#define OUT_OF_HEAP    -1

typedef struct Heap_entry
{
    void        *data;
    size_t      pos;    /* index in Heap */

}Heap_entry;

typedef struct Heap
{
    Darray      *darray;                     /* dynamic array for heap */
    int         (*cmp)(void *a, void *b);   /* compare function */

    int         size_of;                    /* size of data */
    int         ary;                        /* heap ary */

    HEAP_TYPE   type;
}Heap;

/*
    Because heap use dynamic array, iterate it using dynamic array iterator
*/
typedef struct Darray_iterator Heap_iterator;

IT_FUNC(Heap, heap)

/*
    Create Heap entry

    PARAMS
    @IN data - data
    @IN size_of - size of data

    RETURN:
    %NULL iff failure
    %pointer iff success
*/
Heap_entry *heap_entry_create(void *data, int size_of);

#define HEAP_ENTRY_CREATE(PTR, DATA, TYPE) \
    do { \
        PTR = dheap_entry_create(DATA, sizeof(TYPE)); \
    } while (0)

/*
    Destroy heap entry

    PARAMS
    @IN entry - heap entry

    RETURN:
    This is a void function
*/
void heap_entry_destroy(Heap_entry *entry);

/*
    Create heap

    PARAMS
    @IN type - type of heap
    @IN size_of - size_of entry
    @IN ary - heap ary >= 2
    @IN cmp - compare function

    RETURN:
    %NULL iff failure
    %pointer iff success
*/
Heap *heap_create(HEAP_TYPE type, int size_of, int ary,
     int (*cmp)(void *a, void *b));

#define HEAP_CREATE(HEAP, HTYPE, DTYPE, ARY, CMP) \
    do { \
        HEAP = heap_create(HTYPE, sizeof(DTYPE), ARY, CMP); \
    } while (0);

/*
    Destroy heap

    PARAMS
    @IN heap - pointer to heap

    RETURN:
    This is a void function
*/
void heap_destroy(Heap *heap);

/*
    Build heap from array

    PARAMS
    @IN heap - pointer to heap
    @IN array - pointer to array
    @IN n - size of array

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int heap_build(Heap *heap, Heap_entry **array, size_t n);

/*
    Insert entry to heap

    PARAMS
    @IN heap - pointer to heap
    @IN entry - pointer to heap entry

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int heap_insert(Heap *heap, Heap_entry *entry);

/*
    Extract top element from heap ( get end delete from heap )
    if heap is minheap extract min element
    if heap is maxheap extract max element

    PARAMS
    @IN heap - pointer to heap

    RETURN:
    %NULL iff failure
    %pointer iff success
*/
Heap_entry *heap_extract_top(Heap *heap);

/*
    Get top element from heap
    if heap is minheap get min element
    if heap is maxheap get max element

    PARAMS
    @IN heap - pointer to heap

    RETURN:
    %NULL iff failure
    %pointer iff success

*/
Heap_entry *heap_get_top(Heap *heap);

/*
    Change the key value, create before new entry with changed,
    in heap we deallocate only ptr to data, so free your old entry
    to avoid memory leak

    if heap is minheap only decreace key can be perform
    if heap is maxheap only increace key can be perform


    PARAMS
    @IN heap - pointer to heap
    @IN index - entry index in heap
    @IN new_data - new data with new key

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int heap_change_key(Heap *heap, size_t index, void *new_data);

/*
    Check heap capacity

    PARAMS
    @IN heap - pointer to heap

    RETURN:
    %FALSE iff not empty
    %TRUE iff empty or heap is null
*/
bool heap_is_empty(Heap *heap);

#endif
