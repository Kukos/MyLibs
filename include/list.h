#ifndef LIST_H
#define LIST_H

/*
    Linked List

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <iterators.h>
#include <stddef.h> /* size_t */

typedef struct List_node
{
    struct List_node    *next;
    void                *data;

}List_node;

typedef struct List
{
    int             size_of;    /* size of element */
    size_t          length;     /* length of list */

    List_node       *head;
    List_node       *tail;      /* needed for insert guardian */

    int (*cmp)(void* a, void *b);

}List;

/*
   List_iterator
*/
typedef struct List_iterator
{
    List_node       *node;
    int             size_of;

}List_iterator;

IT_FUNC(List, list)


/*
    Macro for create a list, please see function description

    PARAMS
	@OUT PTR - pointer to list
    @IN TYPE - type of element of list
    @IN CMP - compare function
*/
#define LIST_CREATE(PTR, TYPE, CMP) \
    do { \
        PTR = list_create(sizeof(TYPE), CMP); \
    } while (0)

/*
    Create list

    PARAMS
    @IN size_of - size of element in list
    @IN cmp - compare function

    RETURN:
    NULL iff failure
    Pointer iff success
*/
List *list_create(int size_of, int (*cmp)(void* a, void *b));

/*
    Destroy list

    PARAMS
    @IN list - pointer to list

	RETURN:
	This is a void function
*/
void list_destroy(List *list);

/*
    Destroy list with all entries ( call destructor for each entries)

    PARAMS
    @IN list - pointer to list
    @IN desturctor -  your object destructor

    RETURN:
    This is a void function
*/
void list_destroy_with_entries(List *list, void (*destructor)(void *data));

/*
    Insert an element  to sorted list

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
int list_insert(List *list, void *entry);

/*
    Delete the first entry which cmp(list->entry, entry) == 0

    PARAMS
    @IN list - pointer to list
    @IN entry -  entry

    RETURN:
	0 iff success
	Non-zero value iff failure (e.i entry doesn't exist in list )
*/
int list_delete(List *list,void *entry);

/*
    Delete all entries which cmp(list->entry,entry) == 0

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    -1 iff failure ( e.i entry doesn't exist in list )
    Number of delete entries iff success
*/
int list_delete_all(List *list, void *entry);

/*
    Allocate new list and merge list1 & list2 to the new list

    PARAMS
    @IN list1 - pointer to first list
    @IN list2 - pointer to second list

    RETURN:
    NULL iff failure
    Pointer to list iff success
*/
List *list_merge(List *list1, List *list2);

/*
    Search for entry which cmp(list.entry,val) == 0,
    so if you have your own struct, val must be fake struct with corect key
    e.i
    struct Entry
    {
        int key;
        char* name;
        Data *data;
    }

    plase as val set
    struct Entry fake_entry = { correct_key, NULL, NULL }
    struct Entry out_entry;

    and search
    list_search(list,(void*)&fake_entry,(void)&out_entry);

    PARAMS
    @IN list - pointer to list
    @IN val - value to search ( or fake entry )
    @OUT entry - entry with val == val

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
int list_search(List *list, void *val, void *entry);

/*
    Create array from list

    PARAMS
    @IN list - pointer to list
    @OUT array - pointer to array
    @OUT size - size of  returned array

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
int list_to_array(List *list, void *array, size_t *size);

#endif
