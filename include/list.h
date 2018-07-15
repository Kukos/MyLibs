#ifndef LIST_H
#define LIST_H

/*
    Linked List

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <slist.h>
#include <iterators.h>
#include <stddef.h> /* size_t */
#include <sys/types.h>
#include <generic.h>

typedef struct List_node
{
    struct List_node    *____next;
    __extension__ BYTE  ____data[]; /* placeholder for data */

}List_node;

typedef struct List
{
    size_t          ____size_of;    /* size of element */
    size_t          ____length;     /* length of list */

    List_node       *____head;
    List_node       *____tail;      /* needed for insert guardian */

    int (*____cmp)(const void* a, const void *b);
    void (*____destroy)(void *entry);

}List;

/*
   List_iterator
*/
typedef struct List_iterator
{
    List_node       *____node;
    size_t          ____size_of;

}List_iterator;

IT_FUNC(List, list)

/*
    Macro for create a list, please see function description

    PARAMS
	@OUT PTR - pointer to list
    @IN TYPE - type of element of list
    @IN CMP - compare function
    @IN DESTROY - your data destructor
*/
#define LIST_CREATE(PTR, TYPE, CMP, DESTROY) \
    do { \
        PTR = list_create(sizeof(TYPE), CMP, DESTROY); \
    } while (0)

/*
    Create list as SList

    PARAMS
    @IN size_of - size of element in list
    @IN cmp - compare function
    @IN destroy - your data destructor

    RETURN:
    NULL if failure
    Pointer if success
*/
SList *slist_list_create(size_t size_of, int (*cmp)(const void *a, const void *b), void (*destroy)(void *entry));


/*
    Create list

    PARAMS
    @IN size_of - size of element in list
    @IN cmp - compare function
    @IN destroy - your data destructor

    RETURN:
    NULL iff failure
    Pointer iff success
*/
List *list_create(size_t size_of, int (*cmp)(const void *a, const void *b), void (*destroy)(void *entry));

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
void list_destroy_with_entries(List *list);

/*
    Insert an element  to sorted list

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
int list_insert(List *list, const void *entry);

/*
    Delete the first entry which cmp(list->entry, entry) == 0

    PARAMS
    @IN list - pointer to list
    @IN entry -  entry

    RETURN:
	0 iff success
	Non-zero value iff failure (i.e entry doesn't exist in list )
*/
int list_delete(List *list, const void *entry);

/*
    Delete the first entry which cmp(list->entry, entry) ==
    and call destructor

    PARAMS
    @IN list - pointer to list
    @IN entry -  entry

    RETURN:
	0 iff success
	Non-zero value iff failure (i.e entry doesn't exist in list )
*/
int list_delete_with_entry(List *list, const void *entry);

/*
    Delete all entries which cmp(list->entry,entry) == 0

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    -1 iff failure (i.e entry doesn't exist in list )
    Number of delete entries iff success
*/
int list_delete_all(List *list, const void *entry);


/*
    Delete all entries which cmp(list->entry,entry) == 0
    and call destructor

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    -1 iff failure (i.e entry doesn't exist in list )
    Number of delete entries iff success
*/
int list_delete_all_with_entry(List *list, const void *entry);

/*
    Allocate new list and merge list1 & list2 to the new list

    PARAMS
    @IN list1 - pointer to first list
    @IN list2 - pointer to second list

    RETURN:
    NULL iff failure
    Pointer to list iff success
*/
List *list_merge(const List * ___restrict___ list1, const List * ___restrict___ list2);

/*
    Search for entry which cmp(list.entry,val) == 0,
    so if you have your own struct, val must be fake struct with corect key
    i.e
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
int list_search(const List *list, const void *val, void *entry);

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
int list_to_array(const List *list, void *array, size_t *size);

/*
    Get size of data List

    PARAMS
    @IN list - pointer to List

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t list_get_data_size(const List *list);

/*
    Get number of entries in List

    PARAMS
    @IN list - pointer to List

    RETURN
    -1 iff failure
    sizeof iff success
*/
ssize_t list_get_num_entries(const List *list);

#endif
