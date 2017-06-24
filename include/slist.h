#ifndef SLIST_H
#define SLIST_H

/*
    Ssorted List Container Framework

    To use this framework:

    1. Use Macro in SLIST_WRAPPERS_CREATE your .c file
    2. Declare your constructor with prefix slist and sufix _create i.e slist_list_create
    3. In Constructor use macro SLIST_WRAPPERS_ASSIGN
    4. Use Macro SLIST_ITERATOR_WRAPPERS_CREATE in your .c file
    5. Declare your constructor with prefix slist and sufix _iterator_create i.e slist_list_iterator_create
    6. Declare your constructor with prefix slist and sufix _iterator_init i.e slist_list_iterator_init
    7. In Constructor use macro SLIST_ITERATOR_WRAPPERS_ASSIGN
    8. Use SList instead of your struct but create SList by your custom constructor i.e SList *list = slist_list_create
    9. Use Slist inline function instead of your struct function i.e slist_insert instead of list_insert
    10. Use iterator macros with container sufix, and as __internal_type pass your struct type

    (11.) IF you have custom struct function inetad of default names and parameters for list
    do not use SLIST macros, you have to create wrappers and fill hooks by yourself

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
#include <iterators.h>

typedef struct SList
{
    void        *____list; /* pointer to list */

    /* private functions */
    void        (*____destroy)(void *list);
    void        (*____destroy_with_entries)(void *list, void (*destructor)(void *data));
    int         (*____insert)(void *list, void *data);
    int         (*____delete)(void *list, void *data);
    int         (*____delete_all)(void *list, void *data);
    int         (*____search)(void *list, void *in, void *out);
    void*       (*____merge)(void *list1, void *list2);
    int         (*____to_array)(void *list, void *array, size_t *size);
    int         (*____get_size_of)(void *list);
    ssize_t     (*____get_num_entries)(void *list);

}SList;

/* Macro to create wrappers to your struct to provide assignment to framework functions */
#define SLIST_WRAPPERS_CREATE(type, prefix) \
    static ___unused___ void ____destroy(void *list); \
    static ___unused___ void ____destroy_with_entries(void *list, void (*destructor)(void *data)); \
    static ___unused___ int ____insert(void *list, void *data); \
    static ___unused___ int ____delete(void *list, void *data); \
    static ___unused___ int ____delete_all(void *list, void *data); \
    static ___unused___ int ____search(void *list, void *in, void *out); \
    static ___unused___ void *____merge(void *list1, void *list2); \
    static ___unused___ int ____to_array(void *list, void *array, size_t *size); \
    static ___unused___ int ____get_size_of(void *list); \
    static ___unused___ ssize_t ____get_num_entries(void *list); \
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
    static ___unused___ int ____insert(void *list, void *data) \
    { \
        return concat(prefix, _insert)((type *)list, data); \
    } \
    \
    static ___unused___ int ____delete(void *list, void *data) \
    { \
        return concat(prefix, _delete)((type *)list, data); \
    } \
    \
    static ___unused___ int ____delete_all(void *list, void *data) \
    { \
        return concat(prefix, _delete_all)((type *)list, data); \
    } \
    \
    static ___unused___ int ____search(void *list, void *in, void *out) \
    { \
        return concat(prefix, _search)((type *)list, in, out); \
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


#define SLIST_WRAPPERS_ASSIGN(list) \
    do { \
        (list)->____destroy               = ____destroy; \
        (list)->____destroy_with_entries  = ____destroy_with_entries; \
        (list)->____insert                = ____insert; \
        (list)->____delete                = ____delete; \
        (list)->____delete_all            = ____delete_all; \
        (list)->____search                = ____search; \
        (list)->____merge                 = ____merge; \
        (list)->____to_array              = ____to_array; \
        (list)->____get_size_of           = ____get_size_of; \
        (list)->____get_num_entries       = ____get_num_entries; \
    } while (0);

/*
    Check types of lists

    PARAMS
    @IN list1 - pointer to 1. SList
    @IN list2 - pointer to 2. SList

    RETURN
    false iff lists have different types
    true iff lists have the same type

*/
___inline___ bool slist_the_same_type(SList *list1, SList *list2);

/*
    Get list from generic SList

    PARAMS
    @IN list - pointer to SList

    RETURN
    NULL iff failure
    Pointer to list iff success
*/
___inline___ void *slist_get_list(SList *list);


/*
    Destroy the list

    PARAMS
    @IN list - pointer to SList

    RETURN
    This is a void function
*/
___inline___ void slist_destroy(SList *list);

/*
    Destroy slist with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN list - pointer to SList
    @IN destructor - pointer to destructor function

    RETURN
    This is a void function

*/
___inline___ void slist_destroy_with_entries(SList *list, void (*destructor)(void *data));

/*
    Insert data

    PARAMS
    @IN list - pointer to SList
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int slist_insert(SList *list, void *data);

/*
    Delete the first data which cmp(list->data, data) == 0

    PARAMS
    @IN list - pointer to SList
    @IN data -  data

    RETURN:
	0 iff success
	Non-zero value iff failure (i.e data doesn't exist in list )
*/
___inline___ int slist_delete(SList *list, void *data);

/*
    Delete all entries which cmp(list->data, data) == 0

    PARAMS
    @IN list - pointer to SList
    @IN data - data

    RETURN:
    -1 iff failure (i.e data doesn't exist in list )
    Number of delete entries iff success
*/
___inline___ int slist_delete_all(SList *list, void *data);

/*
    Search for data which cmp(list->data, in) == 0,
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
    @IN in - value to search ( or fake entry )
    @OUT out - data with val == val

    RETURN:
    0 iff success
	Non-zero value iff failure
*/
___inline___ int slist_search(SList *list, void *in, void *out);

/*
    Merge 2 SList (IFF they have the same time)

    PARAMS
    @IN list1 - pointer to 1. SList
    @IN list2 - pointer to 2. SList

    RETURN
    NULL iff failure
    pointer to New SList iff success
*/
___inline___ SList *slist_merge(SList *list1, SList *list2);

/*
    Convert SList to array

    PARAMS
    @IN list - pointer to SList
    @OUT array - (void *)&array
    @OUT size - array size

    RETURN
    0 iff success
    Non-zero iff failure
*/
___inline___ int slist_to_array(SList *list, void *array, size_t *size);

/*
    Get SList data size_of

    PARAMS
    @IN list - pointer to SList

    RETURN
    -1 iff failure
    SList data size of iff success
*/
___inline___ int slist_get_size_of(SList *list);

/*
    Get SList num of entries

    PARAMS
    @IN list - pointer to SList

    RETURN
    -1 iff failure
    Num entries iff success
*/
___inline___ ssize_t slist_get_num_entries(SList *list);

___inline___ bool slist_the_same_type(SList *list1, SList *list2)
{
    if (list1 == NULL || list2 == NULL)
        return false;

    return !(list1->____destroy                 != list2->____destroy               ||
             list1->____destroy_with_entries    != list2->____destroy_with_entries  ||
             list1->____insert                  != list2->____insert                ||
             list1->____delete                  != list2->____delete                ||
             list1->____delete_all              != list2->____delete_all            ||
             list1->____search                  != list2->____search                ||
             list1->____merge                   != list2->____merge                 ||
             list1->____to_array                != list2->____to_array              ||
             list1->____get_size_of             != list2->____get_size_of           ||
             list1->____get_num_entries         != list2->____get_num_entries);
}


___inline___ void *slist_get_list(SList *list)
{
    if (list == NULL)
        return NULL;

    return list->____list;
}

___inline___ void slist_destroy(SList *list)
{
    if (list == NULL)
        return;

    list->____destroy(slist_get_list(list));
    FREE(list);
}

___inline___ void slist_destroy_with_entries(SList *list, void (*destructor)(void *data))
{
    if (list == NULL)
        return;

    list->____destroy_with_entries(slist_get_list(list), destructor);
    FREE(list);
}

___inline___ int slist_insert(SList *list, void *data)
{
    if (list == NULL)
        return 1;

    return list->____insert(slist_get_list(list), data);
}

___inline___ int slist_delete(SList *list, void *data)
{
    if (list == NULL)
        return 1;

    return list->____delete(slist_get_list(list), data);
}

___inline___ int slist_delete_all(SList *list, void *data)
{
    if (list == NULL)
        return 1;

    return list->____delete_all(slist_get_list(list), data);
}

___inline___ int slist_search(SList *list, void *in, void *out)
{
    if (list == NULL)
        return 1;

    return list->____search(slist_get_list(list), in, out);
}

___inline___ SList *slist_merge(SList *list1, SList *list2)
{
    SList *list3;

    if (list1 == NULL || list2 == NULL)
        return NULL;

    /* we can only merge IFF have the same type */
    if (!slist_the_same_type(list1, list2))
        return NULL;

    /* Create new instance */
    list3 = (SList *)malloc(sizeof(SList));
    if (list3 == NULL)
        return NULL;

    /* merge lists */
    list3->____list = list1->____merge(slist_get_list(list1), slist_get_list(list2));
    if (list3->____list == NULL)
    {
        FREE(list3);
        return NULL;
    }

    /* fill hooks */
    list3->____destroy              = list1->____destroy;
    list3->____destroy_with_entries = list1->____destroy_with_entries;
    list3->____insert               = list1->____insert;
    list3->____delete               = list1->____delete;
    list3->____delete_all           = list1->____delete_all;
    list3->____search               = list1->____search;
    list3->____merge                = list1->____merge;
    list3->____to_array             = list1->____to_array;
    list3->____get_size_of          = list1->____get_size_of;
    list3->____get_num_entries      = list1->____get_num_entries;

    return list3;
}

___inline___ int slist_to_array(SList *list, void *array, size_t *size)
{
    if (list == NULL)
        return 1;

    return list->____to_array(slist_get_list(list), array, size);
}

___inline___ int slist_get_size_of(SList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_size_of(slist_get_list(list));
}

___inline___ ssize_t slist_get_num_entries(SList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_num_entries(slist_get_list(list));
}

typedef struct SList_iterator
{
    void    *____iterator;

    void    (*____destroy)(void *iterator);
    int     (*____next)(void *iterator);
    int     (*____prev)(void *iterator);
    int     (*____get_data)(void *iterator, void *data);
    int     (*____get_node)(void *iterator, void *node);
    bool    (*____end)(void *iterator);

}SList_iterator;

/* use this macro to create wrappers for iterator */
#define SLIST_ITERATOR_WRAPPERS_CREATE(type, prefix) \
    static ___unused___ void ____it_destroy(void *it); \
    static ___unused___ int ____it_next(void *it); \
    static ___unused___ int ____it_prev(void *it); \
    static ___unused___ int ____it_get_data(void *it, void *data); \
    static ___unused___ int ____it_get_node(void *it, void *node); \
    static ___unused___ bool ____it_end(void *it); \
    static ___unused___ void ____it_destroy(void *it) \
    { \
        concat(prefix, _destroy)((type *)it); \
    } \
    \
    static ___unused___ int ____it_next(void *it) \
    { \
        return concat(prefix, _next)((type *)it); \
    } \
    \
    static ___unused___ int ____it_prev(void *it) \
    { \
        return concat(prefix, _prev)((type *)it); \
    } \
    \
    static ___unused___ int ____it_get_data(void *it, void *data) \
    { \
        return concat(prefix, _get_data)((type *)it, data); \
    } \
    \
    static ___unused___ int ____it_get_node(void *it, void *node) \
    { \
        return concat(prefix, _get_node)((type *)it, node); \
    } \
    \
    static ___unused___ bool ____it_end(void *it) \
    { \
        return concat(prefix, _end)((type *)it); \
    }

#define SLIST_ITERATOR_WRAPPERS_ASSIGN(it) \
    do { \
        (it)->____destroy     = ____it_destroy; \
        (it)->____next        = ____it_next; \
        (it)->____prev        = ____it_prev; \
        (it)->____get_data    = ____it_get_data; \
        (it)->____get_node    = ____it_get_node; \
        (it)->____end         = ____it_end; \
    } while (0);

#endif
