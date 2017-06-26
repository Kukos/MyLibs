#ifndef ARRAYLIST_H
#define ARRAYLIST_H


/*
    ArrayList [ Unsorted List ]

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <ulist.h>
#include <iterators.h>
#include <stddef.h> /* size_t */
#include <sys/types.h>

typedef struct Arraylist_node
{
    struct Arraylist_node *____next;
    struct Arraylist_node *____prev;
    void *____data;
    size_t ____size_of;

}Arraylist_node;


typedef struct Arraylist
{
    size_t              ____size_of;    /* size of element */
    size_t              ____length;     /* length of list */

    Arraylist_node      *____head;
    Arraylist_node      *____tail;

}Arraylist;

/*
   ArrayList_iterator
*/
typedef struct Arraylist_iterator
{
    Arraylist_node  *____node;
    size_t          ____size_of;

}Arraylist_iterator;

IT_FUNC(Arraylist, arraylist)

/*
    Macro for create a alist, please see function description

    PARAMS
	@OUT PTR - pointer to alist
    @IN TYPE - type of element of list
*/
#define LIST_CREATE(PTR, TYPE) \
    do { \
        PTR = arraylist_create(sizeof(TYPE)); \
    } while (0)

/*
    Create alist as UList

    PARAMS
    @IN size_of - size of element in list

    RETURN:
    NULL if failure
    Pointer if success
*/
UList *ulist_arraylist_create(int size_of);

/*
    Create alist

    PARAMS
    @IN size_of - size of element in list

    RETURN:
    NULL if failure
    Pointer if success
*/
Arraylist *arraylist_create(int size_of);

/*
    Destroy alist

    PARAMS
    @IN alist - pointer to list

	RETURN:
	This is a void function
*/
void arraylist_destroy(Arraylist *alist);

/*
    Destroy arraylist with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN alist - pointer to arraylist
    @IN desturctor -  your object destructor

    RETURN:
    This is a void function
*/
void arraylist_destroy_with_entries(Arraylist *alist,
        void (*destructor)(void *data));

/*
    Insert Data at the begining of the alist

    PARAMS
    @IN alist - pointer to alist
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_insert_first(Arraylist *alist, void *data);

/*
    Insert Data at the end of the alist

    PARAMS
    @IN alist - pointer to alist
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_insert_last(Arraylist *alist, void *data);

/*
    Insert Data at @pos

    PARAMS
    @IN alist - pointer to alist
    @IN pos - posision where we insert data
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_insert_pos(Arraylist *alist, size_t pos, void *data);

/*
    Delete first data od alist

    PARAMS
    @IN alist - pointer to alist

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_delete_first(Arraylist *alist);

/*
    Delete last data od alist

    PARAMS
    @IN alist - pointer to alist

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_delete_last(Arraylist *alist);


/*
    Delete data on @pos

    PARAMS
    @IN alist - pointer to alist
    @IN pos - pos from we delete data

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_delete_pos(Arraylist *alist, size_t pos);


/*
    Get data from node at @pos

    PARAMS
    @IN alist - pointer to alist
    @IN pos - posision
    @OUT data - addr of data

    RETURN:
    0 iff success
    Non-zero value of failure
*/
int arraylist_get_pos(Arraylist *alist, size_t pos, void *data);

/*
    Allocate new alist and merge alist1 & alist2 to the new alist

    PARAMS
    @IN alist1 - pointer to first list
    @IN alist2 - pointer to second list

    RETURN:
    NULL if failure
    Pointer to alist if success
*/
Arraylist *arraylist_merge(Arraylist *alist1, Arraylist *alist2);

/*
    Create array from alist

    PARAMS
    @IN alist - pointer to alist
    @OUT array - pointer to array
    @OUT size - size of  returned array

    RETURN:
    0 if success
	Non-zero value if failure
*/
int arraylist_to_array(Arraylist *alist, void *array, size_t *size);

/*
    Get size of

    PARAMS
    @IN alist - pointer to Arraylist

    RETURN
    -1 iff failure
    Num of entries iff success
*/
int arraylist_get_size_of(Arraylist *alist);

/*
    Get number of entries in arraylist

    PARAMS
    @IN alist - pointer to Arraylist

    RETURN
    -1 iff failure
    sizeof iff success
*/
ssize_t arraylist_get_num_entries(Arraylist *alist);

/*
    Get Node data

    PARAMS
    @IN node - pointer to Arraylist_node
    @OUT data - node data

    RETURN
    Non-zero iff failure
    0 iff success
*/
int arraylist_node_get_data(Arraylist_node *node, void *data);

#endif
