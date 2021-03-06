#ifndef LIST2D_H
#define LIST2D_H

/*
    Two Directional Circual Linked List

    If you want to use 2D list property: start from node with less diffrence on keys
    You have to pass diff function, which return: how much A is diffrent of B

    i.e

    #define TYPE int
    unsigned int diff (void *a, void* b)
    {
        if( *(TYPE*)a >= *(TYPE*)b )
            return (unsigned int)(*(TYPE*)a - *(TYPE*)b);
        else
            return (unsigned int)(*(TYPE*)b - *(TYPE*)a);
    }

    for double:

    #define TYPE double
    unsigned int diff (void *a, void* b)
    {
        if( *(TYPE*)a >= *(TYPE*)b )
            return (unsigned int)ceil(*(TYPE*a) - *(TYPE*)b);
        else
            return (unsigned int)ceil(*(TYPE*b) - *(TYPE*)a);
    }

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

#include <slist.h>
#include <iterators.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */
#include <generic.h>
#include <common.h>

typedef struct List2D_node List2D_node;
typedef struct List2D List2D;
typedef struct List2D_iterator List2D_iterator;

IT_FUNC(List2D, list2d)

/*
    Macro for create a list, please see function description

    PARAMS
	@OUT PTR - pointer to list
    @IN TYPE - type of element of list
    @IN CMP - compare function
    @IN DIFF - diff function
    @IN DESTROY - your data destructor
*/
#define LIST2D_CREATE(PTR, TYPE, CMP, DIFF, DESTROY) \
    do { \
        PTR = list2d_create(sizeof(TYPE), CMP, DIFF, DESTROY); \
    } while (0)

/*
    Create list as SList

    PARAMS
    @IN size_of - size of element in list
    @IN cmp - compare function
    @IN diff - diff function
    @IN destroy - your data destructor

    RETURN:
    NULL iff failure
    Pointer iff success
*/
SList *slist_list2d_create(size_t size_of, cmp_f cmp, diff_f diff, destructor_f destroy);

/*
    Create list

    PARAMS
    @IN size_of - size of element in list
    @IN cmp - compare function
    @IN diff - diff function
    @IN destroy - your data destructor


    RETURN:
    NULL iff failure
    Pointer iff success
*/
List2D *list2d_create(size_t size_of, cmp_f cmp, diff_f diff, destructor_f destroy);

/*
    Destroy list

    PARAMS
    @IN list - pointer to list

	RETURN:
	This is a void function
*/
void list2d_destroy(List2D *list);

/*
    Destroy list with all entries ( call destructor for each entries)

    PARAMS
    @IN list - pointer to list

    RETURN:
    This is a void function
*/
void list2d_destroy_with_entries(List2D *list);

/*
    Insert an element to sorted list

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
int list2d_insert(List2D * ___restrict___ list, const void * ___restrict___ entry);

/*
    Delete the first entry for which cmp(list->entry,entry) == 0

    PARAMS
    @IN list - pointer to list
    @IN entry -  entry

    RETURN:
	0 iff success
	Non-zero value iff failure ( i.e entry doesn't exist in list )
*/
int list2d_delete(List2D * ___restrict___ list, const void * ___restrict___ entry);

/*
    Delete the first entry for which cmp(list->entry,entry) == 0
    and call data destructor

    PARAMS
    @IN list - pointer to list
    @IN entry -  entry

    RETURN:
	0 iff success
	Non-zero value iff failure ( i.e entry doesn't exist in list )
*/
int list2d_delete_with_entry(List2D * ___restrict___ list, const void * ___restrict___ entry);

/*
    Delete all entries for which cmp(list->entry,entry) == 0

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    -1 iff failure ( i.e entry doesn't exist in list )
    Number of delete entries iff success
*/
int list2d_delete_all(List2D * ___restrict___ list, const void * ___restrict___ entry);


/*
    Delete all entries for which cmp(list->entry,entry) == 0
    and call destructor

    PARAMS
    @IN list - pointer to list
    @IN entry - entry

    RETURN:
    -1 iff failure ( i.e entry doesn't exist in list )
    Number of delete entries iff success
*/
int list2d_delete_all_with_entry(List2D * ___restrict___ list, const void * ___restrict___ entry);

/*
    Allocate new list and merge list1 & list2 to the new list

    PARAMS
    @IN list1 - pointer to first list
    @IN list2 - pointer to second list

    RETURN:
    NULL iff failure
    Pointer to list iff success
*/
List2D *list2d_merge(const List2D * ___restrict___ list1, const List2D * ___restrict___ list2);

/*
    Search for entry which cmp(list.entry,val) == 0,
    so if you have your own struct, val must be fake struct with correct key
    i.e
    struct Entry
    {
        int key;
        char* name;
        Data *data;
    }

    please as val set
    struct Entry fake_entry = { correct_key,NULL,NULL)
    struct Entry out_entry;

    and search
    list2d_search(list,(void*)&fake_entry,(void)&out_entry);

    PARAMS
    @IN list - pointer to list
    @IN val - value to search ( or fake entry )
    @OUT entry - entry with val == val

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
int list2d_search(const List2D * ___restrict___ list, const void *val, void *entry);

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
int list2d_to_array(const List2D * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size);

/*
    Get size of data List

    PARAMS
    @IN list - pointer to List

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t list2d_get_data_size(const List2D *list);

/*
    Get number of entries in List

    PARAMS
    @IN list - pointer to List

    RETURN
    -1 iff failure
    sizeof iff success
*/
ssize_t list2d_get_num_entries(const List2D *list);

/*
    Get Node data

    PARAMS
    @IN node - pointer to list2d_node
    @OUT data - node data

    RETURN
    Non-zero iff failure
    0 iff success
*/
int list2d_node_get_data(const List2D_node * ___restrict___ node, void * ___restrict___ data);

#endif
