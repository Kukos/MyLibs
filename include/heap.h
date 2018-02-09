#ifndef HEAP_H
#define HEAP_H

/*
    Simple D-ary heap
    Please note that heap is "generic" and storing only pointers to structures
    ( if data is not simple type )
    So you must deallocate your data manually, heap only dealoate own memory,
    or you can call heap_destroy_with_entries and pass destructor function

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <darray.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */
#include <sys/types.h>

typedef char heap_type;

#define HEAP_MIN        0
#define HEAP_MAX        1
#define OUT_OF_HEAP    -1

typedef struct Heap_entry
{
    void        *____data;
    ssize_t      ____pos;    /* index in Heap */

}Heap_entry;

typedef struct Heap
{
    Darray      *____darray;                     /* dynamic array for heap */
    int         (*____cmp)(const void *a, const void *b);   /* compare function */
    void        (*____destroy)(void *entry); /* data destructor */

    int         ____size_of;                    /* size of data */
    int         ____ary;                        /* heap ary */

    heap_type   ____type;
}Heap;

/*
    Create Heap entry

    PARAMS
    @IN data - data
    @IN size_of - size of data

    RETURN:
    %NULL iff failure
    %pointer iff success
*/
Heap_entry *heap_entry_create(const void *data, int size_of);

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
    Destroy heap entry

    PARAMS
    @IN entry - heap entry
    @IN destructor - data destructor

    RETURN:
    This is a void function
*/
void heap_entry_destroy_with_data(Heap_entry *entry, void (*destructor)(void *entry));

/*
    Get pos in Heap

    PARAMS
    @IN entry - heap entry

    RETURN
    Pos of entry iff success
    -2 iff failure
*/
ssize_t heap_entry_get_pos(const Heap_entry *entry);

/*
    Get heap data

    PARAMS
    @IN entry - heap entry

    RETURN
    data iff success
    NULL iff failure
*/
void *heap_entry_get_data(const Heap_entry *entry);

/*
    Create heap

    PARAMS
    @IN type - type of heap
    @IN size_of - size_of entry
    @IN ary - heap ary >= 2
    @IN cmp - compare function
    @IN destroy - data destructor or NULL

    RETURN:
    %NULL iff failure
    %pointer iff success
*/
Heap *heap_create(heap_type type, int size_of, int ary,
     int (*cmp)(const void *a, const void *b),
     void (*destroy)(void *entry));

#define HEAP_CREATE(HEAP, HTYPE, DTYPE, ARY, CMP, DESTROY) \
    do { \
        HEAP = heap_create(HTYPE, sizeof(DTYPE), ARY, CMP, DESTROY); \
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
    Destroy Heap and all entries

    PARAMS
    @IN heap - pointer to heap

    RETURN:
    This is a void function
*/
void heap_destroy_with_entries(Heap *heap);
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
Heap_entry *heap_get_top(const Heap *heap);

/*
    Change the key value, create before new entry with changed,
    Old entry will be destroy


    PARAMS
    @IN heap - pointer to heap
    @IN index - entry index in heap
    @IN new_data - new data with new key

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int heap_change_key(Heap *heap, size_t index, const void *new_data);

/*
    Check heap capacity

    PARAMS
    @IN heap - pointer to heap

    RETURN:
    %false iff not empty
    %true iff empty or heap is null
*/
bool heap_is_empty(const Heap *heap);

/*
    Get Num entries in heap

    PARAMS
    @IN heap - pointer to heap

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t heap_get_num_entries(const Heap *heap);

/*
    Get heap data size

    PARAMS
    @IN heap - pointer to heap

    RETURN
    -1 iff failure
    Data size iff success
*/
int heap_get_data_size(const Heap *heap);

#endif
