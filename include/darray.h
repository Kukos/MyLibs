#ifndef DARRAY_H
#define DARRAY_H

/*
    Wrapper for simple dynamic allocated array

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

#include <iterators.h>

#ifndef BOOL
    #define BOOL    char
    #define TRUE    1
    #define FALSE   0
#endif

#define ARRAY_TYPE  char
#define SORTED      0
#define UNSORTED    1

/*
    Dynamic array, use binary realloc
*/
typedef struct Darray
{
    void    *array;         /* main array */

    int     size_of;        /* size of element */
    int     size;           /* current allocated size ( num of entries ) */
    int     num_entries;    /* num of entries in array */
    int     init_size;      /* minimum size using on init ( num of entries ) */

    int (*cmp)(void *a,void*b); /* pointer to compare function */
    ARRAY_TYPE type;        /* type of array ( sorted or unsorted ) */

}Darray;

/*
   Darray_iterator
*/
typedef struct Darray_iterator
{
    void    *array;         /* pointer to array */
    int     index;          /* index of array */
    int     size_of;        /* sizeof element in array */
    int     array_length;   /* array length */

}Darray_iterator;

IT_FUNC(Darray, darray)


/*
    MACRO to create an array of type TYPE

    PARAMS
    @OUT PTR - pointer to array wchich we allocate the structure
    @IN TYPE - type of element ( int, double ...)
    @IN ARRAY_TYPE - type of array ( sorted or unsorted )
    @IN SIZE - begining size, if 0 array will be create with default size
    @IN CMP - compare function, if TYPE == SORTED cmp is needed, else you can put NULL ( functions needed cmp wont work )
*/
#define DARRAY_CREATE(PTR, TYPE, ARRAYTYPE, SIZE, CMP) \
    do { \
        PTR = darray_create(ARRAYTYPE, SIZE, sizeof(TYPE), CMP); \
    } while (0)

/*
    Create new instance of dynamic array

    PARAMS
    @IN type - type of array ( sorted or unsorted )
    @IN size - begining size, if 0 array will be create with default size
    @IN size_of - size of element
    @IN cmp - compare function, if TYPE == SORTED cmp is needed,
        else you can put NULL ( functions needed cmp wont work )

    RETURN:
    %NULL iff failure
    %Pointer to Darray iff success
*/
Darray *darray_create(ARRAY_TYPE type, int size, int size_of,
                int (*cmp)(void *a, void*b));

/*
    Deallocate darray

    PARAMS
    @IN darray - pointer to darray

    RETURN:
    This is a void function
*/
void darray_destroy(Darray *darray);

/*
    Insert an entry at the end of array

    PARAMS
    @IN darray - pointer to Darray
    @IN entry - inserted entry (void *)&var

    RETURN:
  	%0 iff success
    %Non-zero value iff failure
*/
int darray_insert(Darray *darray, void *entry);

/*
    Insert an entry to array ( unsorted ) in array[pos]

    PARAMS
    @IN darray - pointer to Darray
    @IN entry - inserted entry (void *)&var
    @IN pos - index where insert

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int darray_insert_pos(Darray *darray, void *entry, int pos);

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
    Delete an entry on position pos from array

    PARAMS
    @IN darray - pointer to Darray
    @IN pos - position of deleted entry

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int darray_delete_pos(Darray *darray, int pos);

/*
    Search for val in array
    (First of all values)

    PARAMS
    @IN darray - pointer to Darray
    @IN val - value

    RETURN:
    %-1 iff failure or value doesn't exists
    %position of value iff success
*/
int darray_search_first(Darray *darray, void *val);


/*
    Search for val in array
    (Last of all values)

    PARAMS
    @IN darray - pointer to Darray
    @IN val - value

    RETURN:
    %-1 iff failure or value doesn't exists
    %position of value iff success
*/
int darray_search_last(Darray *darray, void *val);

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

    RETURN
    %-1 iff failure
    %Pos iff success
*/
int darray_min(Darray *darray);

/*
    Find Maximum and return pos

    PARAMS
    @IN darray - pointer to darray

    RETURN
    %-1 iff failure
    %Pos iff success
*/
int darray_max(Darray *darray);

#endif
