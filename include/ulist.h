#ifndef ULIST_H
#define ULIST_H

/*
    Unsorted List Container Framework

    To use this framework, plase provide constructor function for your Unsorted list
    i. e. ulist_arraylist_create( ... )
    and in function assign pointers to functions

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <stdbool.h>
#include <stddef.h>
#include <compiler.h>
#include <sys/types.h>
#include <stdlib.h>
#include <common.h>

typedef struct UList
{
    void        *____list; /* pointer to list */

    /* private functions */
    void        (*____destroy)(void *list);
    void        (*____destroy_with_entries)(void *list, void (*destructor)(void *data));
    int         (*____insert_first)(void *list, void *data);
    int         (*____insert_last)(void *list, void *data);
    int         (*____insert_pos)(void *list, size_t pos, void *data);
    int         (*____delete_first)(void *list);
    int         (*____delete_last)(void *list);
    int         (*____delete_pos)(void *list, size_t pos);
    int         (*____get_pos)(void *list, size_t pos, void *data);
    void*       (*____merge)(void *list1, void *list2);
    int         (*____to_array)(void *list, void *array, size_t *size);
    int         (*____get_size_of)(void *list);
    ssize_t     (*____get_num_entries)(void *list);

}UList;

/* Macro to create wrappers to your struct to provide assignment to framework functions */
#define ULIST_WRAPPERS_CREATE(type, prefix) \
    static ___unused___ void ____destroy(void *list) \
    { \
        concat(prefix, _destroy)((type *)list); \
    } \
    \
    static ___unused___ void ____destroy_with_entries(void *list, void (*destructor)(void *data)) \
    { \
        concat(prefix, _destroy_with_entries)((type *)list, destructor); \
    } \
    \
    static ___unused___ int ____insert_first(void *list, void *data) \
    { \
        return concat(prefix, _insert_first)((type *)list, data); \
    } \
    \
    static ___unused___ int ____insert_last(void *list, void *data) \
    { \
        return concat(prefix, _insert_last)((type *)list, data); \
    } \
    \
    static ___unused___ int ____insert_pos(void *list, size_t pos, void *data) \
    { \
        return concat(prefix, _insert_pos)((type *)list, pos, data); \
    } \
    \
    static ___unused___ int ____delete_first(void *list) \
    { \
        return concat(prefix, _delete_first)((type *)list); \
    } \
    \
    static ___unused___ int ____delete_last(void *list) \
    { \
        return concat(prefix, _delete_last)((type *)list); \
    } \
    \
    static ___unused___ int ____delete_pos(void *list, size_t pos) \
    { \
        return concat(prefix, _delete_pos)((type *)list, pos); \
    } \
    \
    static ___unused___ int ____get_pos(void *list, size_t pos, void *data) \
    { \
        return concat(prefix, _get_pos)((type *)list, pos, data); \
    } \
    \
    static ___unused___ void *____merge(void *list1, void *list2) \
    { \
        return (void *)concat(prefix, _merge)((type *)list1, (type *)list2); \
    } \
    \
    static ___unused___ int ____to_array(void *list, void *array, size_t *size) \
    { \
        return concat(prefix, _to_array)((type *)list, array, size); \
    } \
    \
    static ___unused___ int ____get_size_of(void *list) \
    { \
        return concat(prefix, _get_size_of)((type *)list); \
    } \
    \
    static ___unused___ ssize_t ____get_num_entries(void *list) \
    { \
        return concat(prefix, _get_num_entries)((type *)list); \
    }


#define ULIST_WRAPPER_ASSIGN(list) \
    do { \
        list->____destroy               = ____destroy; \
        list->____destroy_with_entries  = ____destroy_with_entries; \
        list->____insert_first          = ____insert_first; \
        list->____insert_last           = ____insert_first; \
        list->____insert_pos            = ____insert_pos; \
        list->____delete_first          = ____delete_first; \
        list->____delete_last           = ____delete_last; \
        list->____delete_pos            = ____delete_pos; \
        list->____get_pos               = ____get_pos; \
        list->____merge                 = ____merge; \
        list->____to_array              = ____to_array; \
        list->____get_size_of           = ____get_size_of; \
        list->____get_num_entries       = ____get_num_entries; \
    } while (0);

/*
    Check types of lists

    PARAMS
    @IN list1 - pointer to 1. UList
    @IN list2 - pointer to 2. UList

    RETURN
    false iff lists have different types
    true iff lists have the same type

*/
___inline___ bool ulisy_the_same_type(UList *list1, UList *list2)
{
    if (list1 == NULL || list2 == NULL)
        return false;

    return !(list1->____destroy                 != list2->____destroy               ||
             list1->____destroy_with_entries    != list2->____destroy_with_entries  ||
             list1->____insert_first            != list2->____insert_first          ||
             list1->____insert_last             != list2->____insert_last           ||
             list1->____insert_pos              != list2->____insert_pos            ||
             list1->____delete_first            != list2->____delete_first          ||
             list1->____delete_last             != list2->____delete_last           ||
             list1->____delete_pos              != list2->____delete_pos            ||
             list1->____get_pos                 != list2->____get_pos               ||
             list1->____merge                   != list2->____merge                 ||
             list1->____to_array                != list2->____to_array              ||
             list1->____get_size_of             != list2->____get_size_of           ||
             list1->____get_num_entries         != list2->____get_num_entries);
}

/*
    Get list from generic UList

    PARAMS
    @IN list - pointer to UList

    RETURN
    NULL iff failure
    Pointer to list iff success
*/
___inline___ void *ulist_get_list(UList *list)
{
    if (list == NULL)
        return NULL;

    return list->____list;
}

/*
    Destroy the list

    PARAMS
    @IN list - pointer to UList

    RETURN
    This is a void function
*/
___inline___ void ulist_destroy(UList *list)
{
    if (list == NULL)
        return;

    list->____destroy(list->____list);
    FREE(list);
}

/*
    PARAMS
    @IN list - pointer to UList
    @IN destructor - pointer to destructor function

    RETURN
    This is a void function

*/
___inline___ void ulist_destroy_with_entries(UList *list, void (*destructor)(void *data))
{
    if (list == NULL)
        return;

    list->____destroy_with_entries(list->____list, destructor);
    FREE(list);
}

/*
    Insert data at the begining of list (this is useful for stack)

    PARAMS
    @IN list - pointer to UList
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_insert_first(UList *list, void *data)
{
    if (list == NULL)
        return 1;

    return list->____insert_first(list->____list, data);
}

/*
    Insert data at the end of list (this is useful for queue)

    PARAMS
    @IN list - pointer to UList
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_insert_last(UList *list, void *data)
{
    if (list == NULL)
        return 1;

    return list->____insert_last(list->____list, data);
}

/*
    Insert data on posision @pos

    PARAMS
    @IN list - pointer to UList
    @IN pos - posision where we insert data
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_insert_pos(UList *list, size_t pos, void *data)
{
    if (list == NULL)
        return 1;

    return list->____insert_pos(list->____list, pos, data);
}

/*
    Delete first data

    PARAMS
    @IN list - pointer to UList

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_first(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____delete_first(list->____list);
}

/*
    Delete last data

    PARAMS
    @IN list - pointer to UList

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_last(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____delete_last(list->____list);
}

/*
    Delete data from posision @pos

    PARAMS
    @IN list - pointer to UList
    @IN pos - posision from we delete data(first is 0 )

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_pos(UList *list, size_t pos)
{
    if (list == NULL)
        return 1;

    return list->____delete_pos(list->____list, pos);
}

/*
    PARAMS
    @IN list - pointer to UList
    @IN pos - posision from we get data(first is 0)
    @OUT data - (void *)&out_data

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_get_pos(UList *list, size_t pos, void *data)
{
    if (list == NULL)
        return 1;

    return list->____get_pos(list->____list, pos, data);
}

/*
    Merge 2 UList (IFF they have the same time)

    PARAMS
    @IN list1 - pointer to 1. UList
    @IN list2 - pointer to 2. UList

    RETURN
    NULL iff failure
    pointer to New UList iff success
*/
___inline___ UList *ulist_merge(UList *list1, UList *list2)
{
    UList *list3;

    if (list1 == NULL || list2 == NULL)
        return NULL;

    /* we can only merge IFF have the same type */
    if (!ulisy_the_same_type(list1, list2))
        return NULL;

    /* Create new instance */
    list3 = (UList *)malloc(sizeof(UList));
    if (list3 == NULL)
        return NULL;

    /* merge lists */
    list3->____list = list1->____merge(list1->____list, list2->____list);
    if (list3->____list == NULL)
        return NULL;

    /* fill hooks */
    list3->____destroy              = list1->____destroy;
    list3->____destroy_with_entries = list1->____destroy_with_entries;
    list3->____insert_first         = list1->____insert_first;
    list3->____insert_last          = list1->____insert_last;
    list3->____insert_pos           = list1->____insert_pos;
    list3->____delete_first         = list1->____delete_first;
    list3->____delete_last          = list1->____delete_last;
    list3->____delete_pos           = list1->____delete_pos;
    list3->____get_pos              = list1->____get_pos;
    list3->____merge                = list1->____merge;
    list3->____to_array             = list1->____to_array;
    list3->____get_size_of          = list1->____get_size_of;
    list3->____get_num_entries      = list1->____get_num_entries;

    return list3;
}

/*
    Convert UList to array

    PARAMS
    @IN list - pointer to UList
    @OUT array - (void *)&array
    @OUT size - array size

    RETURN
    0 iff success
    Non-zero iff failure
*/
___inline___ int ulist_to_array(UList *list, void *array, size_t *size)
{
    if (list == NULL)
        return 1;

    return list->____to_array(list->____list, array, size);
}

/*
    Get UList data size_of

    PARAMS
    @IN list - pointer to UList

    RETURN
    -1 iff failure
    UList data size of iff success
*/
___inline___ int ulist_get_size_of(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_size_of(list->____list);
}

/*
    Get UList num of entries

    PARAMS
    @IN list - pointer to UList

    RETURN
    -1 iff failure
    Num entries iff success
*/
___inline___ int ulist_get_num_entries(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_num_entries(list->____list);
}

#endif
