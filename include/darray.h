#ifndef DARRAY_H
#define DARRAY_H

/*
    Wrapper for simple dynamic allocated array

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <iterators.h>
#include <stddef.h> /* size_t */
#include <sys/types.h>

#define DARRAY_TYPE         char
#define DARRAY_SORTED       0
#define DARRAY_UNSORTED     1

/*
    Dynamic array, use binary realloc
*/
typedef struct Darray
{
    void    *____array;         /* main array */

    size_t  ____size_of;        /* size of element */
    size_t  ____size;           /* current allocated size ( num of entries ) */
    size_t  ____num_entries;    /* num of entries in array */
    size_t  ____init_size;      /* minimum size using on init ( num of entries ) */

    void (*____destroy)(void *entry);
    int (*____cmp)(const void *a, const void *b); /* pointer to compare function */
    DARRAY_TYPE ____type;        /* type of array ( sorted or unsorted ) */

}Darray;

/*
   Darray_iterator
*/
typedef struct Darray_iterator
{
    void        *____array;         /* pointer to array */
    ssize_t     ____index;          /* index of array */
    size_t      ____size_of;        /* sizeof element in array */
    size_t      ____array_length;   /* array length */

}Darray_iterator;

IT_FUNC(Darray, darray)


/*
    MACRO to create an array of type TYPE

    PARAMS
    @OUT PTR - pointer to array wchich we allocate the structure
    @IN TYPE - type of element ( int, double ...)
    @IN DARRAY_TYPE - type of array ( sorted or unsorted )
    @IN SIZE - begining size, if 0 array will be create with default size
    @IN CMP - compare function, if TYPE == DARRAY_SORTED cmp is needed, else you can put NULL ( functions needed cmp wont work )
    @IN DESTROY - pointer to your data destructor or NULL
*/
#define DARRAY_CREATE(PTR, TYPE, ARRAYTYPE, SIZE, CMP, DESTROY) \
    do { \
        PTR = darray_create(ARRAYTYPE, SIZE, sizeof(TYPE), CMP, DESTROY); \
    } while (0)

/*
    Create new instance of dynamic array

    destructor by void * pass addr i.e in array we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN type - type of array ( sorted or unsorted )
    @IN size - begining size, if 0 array will be create with default size
    @IN size_of - size of element
    @IN cmp - compare function, if TYPE == DARRAY_SORTED cmp is needed,
        else you can put NULL ( functions needed cmp wont work )
    @IN destroy - pointer to your data destructor or NULL

    RETURN:
    %NULL iff failure
    %Pointer to Darray iff success
*/
Darray *darray_create(DARRAY_TYPE type, size_t size, int size_of,
                int (*cmp)(const void *a, const void *b),
                void (*destroy)(void *entry));

/*
    Deallocate darray

    PARAMS
    @IN darray - pointer to darray

    RETURN:
    This is a void function
*/
void darray_destroy(Darray *darray);

/*
    Destroy Darray with all entries ( call destructor for each entries )

    PARAMS
    @IN darray - pointer darray

    RETURN:
    This is a void function
*/
void darray_destroy_with_entries(Darray *darray);

/*
    Insert an entry at the end of array or insert to the sorted array

    PARAMS
    @IN darray - pointer to Darray
    @IN entry - inserted entry (void *)&var

    RETURN:
  	%0 iff success
    %Non-zero value iff failure
*/
int darray_insert(Darray *darray, const void *entry);

/*
    Insert an entry to array ( only unsorted ) in array[pos]

    PARAMS
    @IN darray - pointer to Darray
    @IN entry - inserted entry (void *)&var
    @IN pos - index where insert

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int darray_insert_pos(Darray *darray, const void *entry, size_t pos);

/*
    Delete the last entry from array

    PARAMS
    @IN darray - pointer to Darray

    RETURN:
   	%0 iff success
    %Non-zero value iff failure
*/
int darray_delete(Darray *darray);

/*
    Delete the last entry from array
    and call destructor

    PARAMS
    @IN darray - pointer to Darray

    RETURN:
   	%0 iff success
    %Non-zero value iff failure
*/
int darray_delete_with_entry(Darray *darray);

/*
    Delete an entry on position pos from array

    PARAMS
    @IN darray - pointer to Darray
    @IN pos - position of deleted entry

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int darray_delete_pos(Darray *darray, size_t pos);

/*
    Delete an entry on position pos from array
    and call destructor

    PARAMS
    @IN darray - pointer to Darray
    @IN pos - position of deleted entry

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int darray_delete_pos_with_entry(Darray *darray, size_t pos);

/*
    Search for val in array
    (First of all values)

    PARAMS
    @IN darray - pointer to Darray
    @IN val_in - value with key
    @IN val_out - value from array or NULL

    RETURN:
    %-1 iff failure or value doesn't exists
    %position of value iff success
*/
ssize_t darray_search_first(const Darray *darray, const void *val_in, void *val_out);


/*
    Search for val in array
    (Last of all values)

    PARAMS
    @IN darray - pointer to Darray
    @IN val_in - value with key
    @IN val_out - value from array or NULL

    RETURN:
    %-1 iff failure or value doesn't exists
    %position of value iff success
*/
ssize_t darray_search_last(const Darray *darray, const void *val_in, void *val_out);

/*
    If array is unsorted sort the array

    PARAMS
    @IN darray -pointer to darray

    RETURN
    %0 iff success
    %Non-zero value iff failure
*/
int darray_sort(Darray *darray);

/*
    Find Minimum and return pos

    PARAMS
    @IN darray - pointer to darray
    @IN val - minimum value or NULL

    RETURN
    %-1 iff failure
    %Pos iff success
*/
ssize_t darray_min(const Darray *darray, const void *val);

/*
    Find Maximum and return pos

    PARAMS
    @IN darray - pointer to darray
    @IN val - minimum value or NULL

    RETURN
    %-1 iff failure
    %Pos iff success
*/
ssize_t darray_max(const Darray *darray, const void *val);

/*
    Get Array

    PARAMS
    @IN darray - pointer to darray

    RETURN
    %NULL iff failure
    %Pointer to array iff success
*/
void *darray_get_array(const Darray *darray);

/*
    Get number of entries in array

    PARAMS
    @IN darray - pointer to darray

    RETURN
    %-1 iff failure
    %Num of entries iff success
*/
ssize_t darray_get_num_entries(const Darray *darray);

/*
    Get Array Type (DARRAY_SORTED / DARRAY_UNSORTED)

    PARAMS
    @IN darray - pointer to darray

    RETURN
    %-1 iff failure
    %type iff success
*/
DARRAY_TYPE darray_get_type(const Darray *darray);

/*
    Get size of

    PARAMS
    @IN darray - pointer to darray

    RETURN
    %-1 iff failure
    %sizeof iff success
*/
int darray_get_data_size(const Darray *darray);

#endif
