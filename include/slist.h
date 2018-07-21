#ifndef SLIST_H
#define SLIST_H

/*
    Sorted List Container Framework

    To use this framework:

    1. Use Macro in SLIST_WRAPPERS_CREATE your .c file
    2. Declare your constructor with prefix slist and sufix _create i.e slist_list_create
    3. In Constructor use macro SLIST_WRAPPERS_ASSIGN
    4. Use SList instead of your struct but create SList by your custom constructor i.e SList *list = slist_list_create
    5. Use Slist inline function instead of your struct function i.e slist_first(list) instead of list_insert

    (6.) IF you have custom struct function instead of default names and parameters for list
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
#include <log.h>

typedef struct SList_iterator
{
    void    *____iterator;

    void    (*____destroy)(void *iterator);
    int     (*____next)(void *iterator);
    int     (*____prev)(void *iterator);
    int     (*____get_data)(const void * ___restrict___ iterator, void * ___restrict___ data);
    int     (*____get_node)(const void * ___restrict___ iterator, void * ___restrict___ node);
    bool    (*____end)(const void *iterator);

}SList_iterator;

typedef struct SList
{
    void        *____list; /* pointer to list */

   /* private functions */
    void        (*____destroy)(void *list);
    void        (*____destroy_with_entries)(void *list);
    int         (*____insert)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____delete)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____delete_with_entry)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____delete_all)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____delete_all_with_entry)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____search)(const void * ___restrict___ list, const void *in, void *out);
    void*       (*____merge)(const void * ___restrict___ list1, const void * ___restrict___ list2);
    int         (*____to_array)(const void *list, void *array, size_t *size);
    ssize_t     (*____get_data_size)(const void *list);
    ssize_t     (*____get_num_entries)(const void *list);

    /* to create iterator */
    void*       (*____it_create)(const void *list, iti_mode_t mode);
    void        (*____it_destroy)(void *iterator);
    int         (*____it_next)(void *iterator);
    int         (*____it_prev)(void *iterator);
    int         (*____it_get_data)(const void * ___restrict___ iterator, void * ___restrict___ data);
    int         (*____it_get_node)(const void * ___restrict___ iterator, void * ___restrict___ node);
    bool        (*____it_end)(const void *iterator);

}SList;

/*
    Create SList iterator

    PARAMS
    @IN list - pointer to SList
    @IN mode - iterator mode

    RETURN
    NULL iff failure
    Pointer to SList_iterator iff success
*/
___inline___ SList_iterator *slist_iterator_create(const SList *list, iti_mode_t mode);

/* Macro to create wrappers to your struct to provide assignment to framework functions */
#define SLIST_WRAPPERS_CREATE(type, prefix) \
    SLIST_ITERATOR_WRAPPERS_CREATE(concat(type, _iterator), concat(prefix, _iterator)) \
    static ___unused___ void ____destroy(void *list); \
    static ___unused___ void ____destroy_with_entries(void *list); \
    static ___unused___ int ____insert(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____delete(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____delete_with_entry(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____delete_all(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____delete_all_with_entry(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____search(const void * ___restrict___ list, const void *in, void *out); \
    static ___unused___ void *____merge(const void * ___restrict___ list1, const void * ___restrict___ list2); \
    static ___unused___ int ____to_array(const void * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size); \
    static ___unused___ ssize_t ____get_data_size(const void *list); \
    static ___unused___ ssize_t ____get_num_entries(const void *list); \
    static ___unused___ void ____destroy(void *list) \
    { \
        concat(prefix, _destroy)((type *)list); \
    } \
    \
    static ___unused___ void ____destroy_with_entries(void *list) \
    { \
        concat(prefix, _destroy_with_entries)((type *)list); \
    } \
    \
    static ___unused___ int ____insert(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _insert)((type *)list, data); \
    } \
    \
    static ___unused___ int ____delete(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _delete)((type *)list, data); \
    } \
    \
    static ___unused___ int ____delete_with_entry(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _delete_with_entry)((type *)list, data); \
    } \
    \
    static ___unused___ int ____delete_all(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _delete_all)((type *)list, data); \
    } \
    \
    static ___unused___ int ____delete_all_with_entry(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _delete_all_with_entry)((type *)list, data); \
    } \
    \
    static ___unused___ int ____search(const void * ___restrict___ list, const void *in, void *out) \
    { \
        return concat(prefix, _search)((const type *)list, in, out); \
    } \
    \
    static ___unused___ void *____merge(const void * ___restrict___ list1, const void * ___restrict___ list2) \
    { \
        return (void *)concat(prefix, _merge)((const type * ___restrict___)list1, (const type *___restrict___)list2); \
    } \
    \
    static ___unused___ int ____to_array(const void * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size) \
    { \
        return concat(prefix, _to_array)((const type *)list, array, size); \
    } \
    \
    static ___unused___ ssize_t ____get_data_size(const void *list) \
    { \
        return concat(prefix, _get_data_size)((const type *)list); \
    } \
    \
    static ___unused___ ssize_t ____get_num_entries(const void *list) \
    { \
        return concat(prefix, _get_num_entries)((const type *)list); \
    }


#define SLIST_WRAPPERS_ASSIGN(list) \
    do { \
        (list)->____destroy               = ____destroy; \
        (list)->____destroy_with_entries  = ____destroy_with_entries; \
        (list)->____insert                = ____insert; \
        (list)->____delete                = ____delete; \
        (list)->____delete_with_entry     = ____delete_with_entry; \
        (list)->____delete_all            = ____delete_all; \
        (list)->____delete_all_with_entry = ____delete_all_with_entry; \
        (list)->____search                = ____search; \
        (list)->____merge                 = ____merge; \
        (list)->____to_array              = ____to_array; \
        (list)->____get_data_size         = ____get_data_size; \
        (list)->____get_num_entries       = ____get_num_entries; \
        (list)->____it_create             = ____it_create; \
        (list)->____it_destroy            = ____it_destroy; \
        (list)->____it_next               = ____it_next; \
        (list)->____it_prev               = ____it_prev; \
        (list)->____it_get_data           = ____it_get_data; \
        (list)->____it_get_node           = ____it_get_node; \
        (list)->____it_end                = ____it_end; \
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
___inline___ bool slist_the_same_type(const SList * ___restrict___ list1, const SList * ___restrict___ list2);

/*
    Get list from generic SList

    PARAMS
    @IN list - pointer to SList

    RETURN
    NULL iff failure
    Pointer to list iff success
*/
___inline___ void *slist_get_list(const SList *list);


/*
    Destroy the list

    PARAMS
    @IN list - pointer to SList

    RETURN
    This is a void function
*/
___inline___ void slist_destroy(SList *list);

/*
    Destroy slist with all entries

    PARAMS
    @IN list - pointer to SList

    RETURN
    This is a void function

*/
___inline___ void slist_destroy_with_entries(SList *list);

/*
    Insert data

    PARAMS
    @IN list - pointer to SList
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int slist_insert(SList * ___restrict___ list, const void * ___restrict___ data);

/*
    Delete the first data which cmp(list->data, data) == 0

    PARAMS
    @IN list - pointer to SList
    @IN data -  data

    RETURN:
	0 iff success
	Non-zero value iff failure (i.e data doesn't exist in list )
*/
___inline___ int slist_delete(SList * ___restrict___ list, const void * ___restrict___ data);

/*
    Delete the first data which cmp(list->data, data) == 0
    and call destructor

    PARAMS
    @IN list - pointer to SList
    @IN data -  data

    RETURN:
	0 iff success
	Non-zero value iff failure (i.e data doesn't exist in list )
*/
___inline___ int slist_delete_with_entry(SList * ___restrict___ list, const void * ___restrict___ data);

/*
    Delete all entries which cmp(list->data, data) == 0

    PARAMS
    @IN list - pointer to SList
    @IN data - data

    RETURN:
    -1 iff failure (i.e data doesn't exist in list )
    Number of delete entries iff success
*/
___inline___ int slist_delete_all(SList * ___restrict___ list, const void * ___restrict___ data);

/*
    Delete all entries which cmp(list->data, data) == 0
    and call destructor

    PARAMS
    @IN list - pointer to SList
    @IN data - data

    RETURN:
    -1 iff failure (i.e data doesn't exist in list )
    Number of delete entries iff success
*/
___inline___ int slist_delete_all_with_entry(SList * ___restrict___ list, const void * ___restrict___ data);

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
___inline___ int slist_search(const SList * ___restrict___ list, const void *in, void *out);

/*
    Merge 2 SList (IFF they have the same time)

    PARAMS
    @IN list1 - pointer to 1. SList
    @IN list2 - pointer to 2. SList

    RETURN
    NULL iff failure
    pointer to New SList iff success
*/
___inline___ SList *slist_merge(const SList * ___restrict___ list1, const SList * ___restrict___ list2);

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
___inline___ int slist_to_array(const SList * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size);

/*
    Get SList data size_of

    PARAMS
    @IN list - pointer to SList

    RETURN
    -1 iff failure
    SList data size of iff success
*/
___inline___ ssize_t slist_get_data_size(const SList *list);

/*
    Get SList num of entries

    PARAMS
    @IN list - pointer to SList

    RETURN
    -1 iff failure
    Num entries iff success
*/
___inline___ ssize_t slist_get_num_entries(const SList *list);

___inline___ bool slist_the_same_type(const SList * ___restrict___ list1, const SList * ___restrict___ list2)
{
    if (list1 == NULL || list2 == NULL)
        return false;

    return !(list1->____destroy                 != list2->____destroy               ||
             list1->____destroy_with_entries    != list2->____destroy_with_entries  ||
             list1->____insert                  != list2->____insert                ||
             list1->____delete                  != list2->____delete                ||
             list1->____delete_with_entry       != list2->____delete_with_entry     ||
             list1->____delete_all              != list2->____delete_all            ||
             list1->____delete_all_with_entry   != list2->____delete_all_with_entry ||
             list1->____search                  != list2->____search                ||
             list1->____merge                   != list2->____merge                 ||
             list1->____to_array                != list2->____to_array              ||
             list1->____get_data_size           != list2->____get_data_size         ||
             list1->____get_num_entries         != list2->____get_num_entries       ||
             list1->____it_create               != list2->____it_create             ||
             list1->____it_destroy              != list2->____it_destroy            ||
             list1->____it_next                 != list2->____it_next               ||
             list1->____it_prev                 != list2->____it_prev               ||
             list1->____it_get_data             != list2->____it_get_data           ||
             list1->____it_get_node             != list2->____it_get_node           ||
             list1->____it_end                  != list2->____it_end
             );
}

___inline___ void *slist_get_list(const SList *list)
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

___inline___ void slist_destroy_with_entries(SList *list)
{
    if (list == NULL)
        return;

    list->____destroy_with_entries(slist_get_list(list));
    FREE(list);
}

___inline___ int slist_insert(SList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____insert(slist_get_list(list), data);
}

___inline___ int slist_delete(SList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____delete(slist_get_list(list), data);
}


___inline___ int slist_delete_with_entry(SList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____delete_with_entry(slist_get_list(list), data);
}

___inline___ int slist_delete_all(SList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____delete_all(slist_get_list(list), data);
}

___inline___ int slist_delete_all_with_entry(SList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____delete_all_with_entry(slist_get_list(list), data);
}

___inline___ int slist_search(const SList * ___restrict___ list, const void *in, void *out)
{
    if (list == NULL)
        return 1;

    return list->____search(slist_get_list(list), in, out);
}

___inline___ SList *slist_merge(const SList * ___restrict___ list1, const SList * ___restrict___ list2)
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
    list3->____destroy               = list1->____destroy;
    list3->____destroy_with_entries  = list1->____destroy_with_entries;
    list3->____insert                = list1->____insert;
    list3->____delete                = list1->____delete;
    list3->____delete_with_entry     = list1->____delete_with_entry;
    list3->____delete_all            = list1->____delete_all;
    list3->____delete_all_with_entry = list1->____delete_all_with_entry;
    list3->____search                = list1->____search;
    list3->____merge                 = list1->____merge;
    list3->____to_array              = list1->____to_array;
    list3->____get_data_size         = list1->____get_data_size;
    list3->____get_num_entries       = list1->____get_num_entries;

    list3->____it_create             = list1->____it_create;
    list3->____it_destroy            = list1->____it_destroy;
    list3->____it_next               = list1->____it_next;
    list3->____it_prev               = list1->____it_prev;
    list3->____it_get_data           = list1->____it_get_data;
    list3->____it_get_node           = list1->____it_get_node;
    list3->____it_end                = list1->____it_end;

    return list3;
}

___inline___ int slist_to_array(const SList * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size)
{
    if (list == NULL)
        return 1;

    return list->____to_array(slist_get_list(list), array, size);
}

___inline___ ssize_t slist_get_data_size(const SList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_data_size(slist_get_list(list));
}

___inline___ ssize_t slist_get_num_entries(const SList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_num_entries(slist_get_list(list));
}
___inline___ SList_iterator *slist_iterator_create(const SList *list, iti_mode_t mode)
{
    SList_iterator *it;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    it = (SList_iterator *)malloc(sizeof(SList_iterator));
    if (it == NULL)
        ERROR("malloc error\n", NULL);

    it->____iterator = list->____it_create(list, mode);
    if (it->____iterator == NULL)
    {
        FREE(it);
        return NULL;
    }

    /* FILL HOOKS */
    it->____destroy    = list->____it_destroy;
    it->____next       = list->____it_next;
    it->____prev       = list->____it_prev;
    it->____get_data   = list->____it_get_data;
    it->____get_node   = list->____it_get_node;
    it->____end        = list->____it_end;

    return it;
}

IT_FUNC_CONTAINER(SList, slist)

/* use this macro to create wrappers for iterator */
#define SLIST_ITERATOR_WRAPPERS_CREATE(type, prefix) \
    static ___unused___ void* ____it_create(const void *list, iti_mode_t mode); \
    static ___unused___ void ____it_destroy(void *it); \
    static ___unused___ int ____it_next(void *it); \
    static ___unused___ int ____it_prev(void *it); \
    static ___unused___ int ____it_get_data(const void * ___restrict___ it, void * ___restrict___ data); \
    static ___unused___ int ____it_get_node(const void * ___restrict___ it, void * ___restrict___ node); \
    static ___unused___ bool ____it_end(const void *it); \
    static ___unused___ void* ____it_create(const void *list, iti_mode_t mode) \
    { \
        return concat(prefix, _create)(slist_get_list(list), mode); \
    } \
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
    static ___unused___ int ____it_get_data(const void * ___restrict___ it, void * ___restrict___ data) \
    { \
        return concat(prefix, _get_data)((const type *)it, data); \
    } \
    \
    static ___unused___ int ____it_get_node(const void * ___restrict___ it, void * ___restrict___ node) \
    { \
        return concat(prefix, _get_node)((const type *)it, node); \
    } \
    \
    static ___unused___ bool ____it_end(const void *it) \
    { \
        return concat(prefix, _end)((const type *)it); \
    }

#endif
