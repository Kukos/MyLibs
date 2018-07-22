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
#include <common.h>

typedef enum DARRAY_TYPE
{
    DARRAY_SORTED = 0,
    DARRAY_UNSORTED
} DARRAY_TYPE;

typedef struct Darray Darray;
typedef struct Darray_iterator Darray_iterator;

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
Darray *darray_create(DARRAY_TYPE type, size_t size, size_t size_of,
                cmp_f cmp,
                destructor_f destroy);

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
int darray_insert(Darray * ___restrict___ darray, const void * ___restrict___ entry);

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
int darray_insert_pos(Darray * ___restrict___ darray, const void * ___restrict___ entry, size_t pos);

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
    Get data from darray like data = darray[pos]

    PARAMS
    @IN darray - pointer to Darray
    @IN pos - position
    @OUT data - pointer where data will be copied

    RETURN
    %0 iff success
    %Non-zero value iff failure
*/
int darray_get_data(const Darray *darray, size_t pos, void *data);

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
ssize_t darray_search_first(const Darray * ___restrict___ darray, const void *val_in, void *val_out);


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
ssize_t darray_search_last(const Darray * ___restrict___ darray, const void *val_in, void *val_out);

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
ssize_t darray_min(const Darray * ___restrict___ darray, void * ___restrict___ val);

/*
    Find Maximum and return pos

    PARAMS
    @IN darray - pointer to darray
    @IN val - minimum value or NULL

    RETURN
    %-1 iff failure
    %Pos iff success
*/
ssize_t darray_max(const Darray * ___restrict___ darray, void * ___restrict___ val);

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
ssize_t darray_get_data_size(const Darray *darray);

#endif
