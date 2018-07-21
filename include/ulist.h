#ifndef ULIST_H
#define ULIST_H

/*
    Unsorted List Container Framework

    To use this framework:

    1. Use Macro in ULIST_WRAPPERS_CREATE your .c file
    2. Declare your constructor with prefix ulist and sufix _create i.e ulist_arraylist_create
    3. In Constructor use macro ULIST_WRAPPERS_ASSIGN
    4. Use UList instead of your struct but create UList by your custom constructor i.e UList *list = ulist_arraylist_create
    5. Use Ulist inline function instead of your struct function i.e ulist_insert_first(list) instead of arraylist_insert_first

    (6.) IF you have custom struct function instead of default names and parameters for list
    do not use ULIST macros, you have to create wrappers and fill hooks by yourself

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

typedef struct UList_iterator
{
    void    *____iterator;

    void    (*____destroy)(void *iterator);
    int     (*____next)(void *iterator);
    int     (*____prev)(void *iterator);
    int     (*____get_data)(const void * ___restrict___ iterator, void * ___restrict___ data);
    int     (*____get_node)(const void * ___restrict___ iterator, void * ___restrict___ node);
    bool    (*____end)(const void *iterator);

}UList_iterator;

typedef struct UList
{
    void        *____list; /* pointer to list */

    /* private functions */
    void        (*____destroy)(void *list);
    void        (*____destroy_with_entries)(void *list);
    int         (*____insert_first)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____insert_last)(void * ___restrict___ list, const void * ___restrict___ data);
    int         (*____insert_pos)(void * ___restrict___ list, size_t pos, const void * ___restrict___ data);
    int         (*____delete_first)(void *list);
    int         (*____delete_last)(void *list);
    int         (*____delete_pos)(void *list, size_t pos);
    int         (*____delete_first_with_entry)(void *list);
    int         (*____delete_last_with_entry)(void *list);
    int         (*____delete_pos_with_entry)(void *list, size_t pos);
    int         (*____get_pos)(const void * ___restrict___ list, size_t pos, void * ___restrict___ data);
    void*       (*____merge)(const void * ___restrict___ list1, const void * ___restrict___ list2);
    int         (*____to_array)(const void * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size);
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

}UList;

/*
    Create UList iterator

    PARAMS
    @IN list - pointer to UList
    @IN mode - iterator mode

    RETURN
    NULL iff failure
    Pointer to UList_iterator iff success
*/
___inline___ UList_iterator *ulist_iterator_create(const UList *list, iti_mode_t mode);


/* Macro to create wrappers to your struct to provide assignment to framework functions */
#define ULIST_WRAPPERS_CREATE(type, prefix) \
    ULIST_ITERATOR_WRAPPERS_CREATE(concat(type, _iterator), concat(prefix, _iterator)) \
    static ___unused___ void ____destroy(void *list); \
    static ___unused___ void ____destroy_with_entries(void *list); \
    static ___unused___ int ____insert_first(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____insert_last(void * ___restrict___ list, const void * ___restrict___ data); \
    static ___unused___ int ____insert_pos(void * ___restrict___ list, size_t pos, const void * ___restrict___ data); \
    static ___unused___ int ____delete_first(void *list); \
    static ___unused___ int ____delete_last(void *list); \
    static ___unused___ int ____delete_pos(void *list, size_t pos); \
    static ___unused___ int ____delete_first_with_entry(void *list); \
    static ___unused___ int ____delete_last_with_entry(void *list); \
    static ___unused___ int ____delete_pos_with_entry(void *list, size_t pos); \
    static ___unused___ int ____get_pos(const void * ___restrict___ list, size_t pos, void * ___restrict___ data); \
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
    static ___unused___ int ____insert_first(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _insert_first)((type *)list, data); \
    } \
    \
    static ___unused___ int ____insert_last(void * ___restrict___ list, const void * ___restrict___ data) \
    { \
        return concat(prefix, _insert_last)((type *)list, data); \
    } \
    \
    static ___unused___ int ____insert_pos(void * ___restrict___ list, size_t pos, const void * ___restrict___ data) \
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
    static ___unused___ int ____delete_first_with_entry(void *list) \
    { \
        return concat(prefix, _delete_first_with_entry)((type *)list); \
    } \
    \
    static ___unused___ int ____delete_last_with_entry(void *list) \
    { \
        return concat(prefix, _delete_last_with_entry)((type *)list); \
    } \
    \
    static ___unused___ int ____delete_pos_with_entry(void *list, size_t pos) \
    { \
        return concat(prefix, _delete_pos_with_entry)((type *)list, pos); \
    } \
    \
    static ___unused___ int ____get_pos(const void * ___restrict___ list, size_t pos, void * ___restrict___ data) \
    { \
        return concat(prefix, _get_pos)((const type *)list, pos, data); \
    } \
    \
    static ___unused___ void *____merge(const void * ___restrict___ list1, const void * ___restrict___ list2) \
    { \
        return (void *)concat(prefix, _merge)((const type * ___restrict___)list1, (const type * ___restrict___)list2); \
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


#define ULIST_WRAPPERS_ASSIGN(list) \
    do { \
        (list)->____destroy                 = ____destroy; \
        (list)->____destroy_with_entries    = ____destroy_with_entries; \
        (list)->____insert_first            = ____insert_first; \
        (list)->____insert_last             = ____insert_last; \
        (list)->____insert_pos              = ____insert_pos; \
        (list)->____delete_first            = ____delete_first; \
        (list)->____delete_last             = ____delete_last; \
        (list)->____delete_pos              = ____delete_pos; \
        (list)->____delete_first_with_entry = ____delete_first_with_entry; \
        (list)->____delete_last_with_entry  = ____delete_last_with_entry; \
        (list)->____delete_pos_with_entry   = ____delete_pos_with_entry; \
        (list)->____get_pos                 = ____get_pos; \
        (list)->____merge                   = ____merge; \
        (list)->____to_array                = ____to_array; \
        (list)->____get_data_size           = ____get_data_size; \
        (list)->____get_num_entries         = ____get_num_entries; \
        (list)->____it_create               = ____it_create; \
        (list)->____it_destroy              = ____it_destroy; \
        (list)->____it_next                 = ____it_next; \
        (list)->____it_prev                 = ____it_prev; \
        (list)->____it_get_data             = ____it_get_data; \
        (list)->____it_get_node             = ____it_get_node; \
        (list)->____it_end                  = ____it_end; \
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
___inline___ bool ulist_the_same_type(const UList * ___restrict___ list1, const UList * ___restrict___ list2);

/*
    Get list from generic UList

    PARAMS
    @IN list - pointer to UList

    RETURN
    NULL iff failure
    Pointer to list iff success
*/
___inline___ void *ulist_get_list(const UList *list);


/*
    Destroy the list

    PARAMS
    @IN list - pointer to UList

    RETURN
    This is a void function
*/
___inline___ void ulist_destroy(UList *list);

/*
    Destroy ulist with all entries ( call destructor for each entries )

    PARAMS
    @IN list - pointer to UList

    RETURN
    This is a void function

*/
___inline___ void ulist_destroy_with_entries(UList *list);

/*
    Insert data at the begining of list (this is useful for stack)

    PARAMS
    @IN list - pointer to UList
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_insert_first(UList * ___restrict___ list, const void * ___restrict___ data);

/*
    Insert data at the end of list (this is useful for queue)

    PARAMS
    @IN list - pointer to UList
    @IN data - (void *)&val

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_insert_last(UList * ___restrict___ list, const void * ___restrict___ data);


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
___inline___ int ulist_insert_pos(UList * ___restrict___ list, size_t pos, const void * ___restrict___ data);

/*
    Delete first data

    PARAMS
    @IN list - pointer to UList

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_first(UList *list);

/*
    Delete last data

    PARAMS
    @IN list - pointer to UList

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_last(UList *list);

/*
    Delete data from posision @pos

    PARAMS
    @IN list - pointer to UList
    @IN pos - posision from we delete data(first is 0 )

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_pos(UList * ___restrict___ list, size_t pos);

/*
    Delete first data with data

    PARAMS
    @IN list - pointer to UList

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_first_with_entry(UList * ___restrict___ list);

/*
    Delete last data with data

    PARAMS
    @IN list - pointer to UList

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_last_with_entry(UList * ___restrict___ list);

/*
    Delete data from posision @pos with data

    PARAMS
    @IN list - pointer to UList
    @IN pos - posision from we delete data(first is 0 )

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_delete_pos_with_entry(UList * ___restrict___ list, size_t pos);

/*
    Get data from node at @pos

    PARAMS
    @IN list - pointer to UList
    @IN pos - posision from we get data(first is 0)
    @OUT data - (void *)&out_data

    RETURN
    0 - iff success
    Non-zero iff failure
*/
___inline___ int ulist_get_pos(const UList * ___restrict___ list, size_t pos, void *  ___restrict___data);

/*
    Merge 2 UList (IFF they have the same time)

    PARAMS
    @IN list1 - pointer to 1. UList
    @IN list2 - pointer to 2. UList

    RETURN
    NULL iff failure
    pointer to New UList iff success
*/
___inline___ UList *ulist_merge(const UList * ___restrict___ list1, const UList * ___restrict___ list2);

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
___inline___ int ulist_to_array(const UList * ___restrict___ list, void * ___restrict___ array, size_t *  ___restrict___ size);

/*
    Get UList data size_of

    PARAMS
    @IN list - pointer to UList

    RETURN
    -1 iff failure
    UList data size of iff success
*/
___inline___ ssize_t ulist_get_data_size(const UList *list);

/*
    Get UList num of entries

    PARAMS
    @IN list - pointer to UList

    RETURN
    -1 iff failure
    Num entries iff success
*/
___inline___ ssize_t ulist_get_num_entries(const UList *list);

___inline___ bool ulist_the_same_type(const UList * ___restrict___ list1, const UList  * ___restrict___ list2)
{
    if (list1 == NULL || list2 == NULL)
        return false;

    return !(list1->____destroy                 != list2->____destroy                   ||
             list1->____destroy_with_entries    != list2->____destroy_with_entries      ||
             list1->____insert_first            != list2->____insert_first              ||
             list1->____insert_last             != list2->____insert_last               ||
             list1->____insert_pos              != list2->____insert_pos                ||
             list1->____delete_first            != list2->____delete_first              ||
             list1->____delete_last             != list2->____delete_last               ||
             list1->____delete_pos              != list2->____delete_pos                ||
             list1->____delete_first_with_entry != list2->____delete_first_with_entry   ||
             list1->____delete_last_with_entry  != list2->____delete_last_with_entry    ||
             list1->____delete_pos_with_entry   != list2->____delete_pos_with_entry     ||
             list1->____get_pos                 != list2->____get_pos                   ||
             list1->____merge                   != list2->____merge                     ||
             list1->____to_array                != list2->____to_array                  ||
             list1->____get_data_size           != list2->____get_data_size             ||
             list1->____get_num_entries         != list2->____get_num_entries           ||
             list1->____it_create               != list2->____it_create                 ||
             list1->____it_destroy              != list2->____it_destroy                ||
             list1->____it_next                 != list2->____it_next                   ||
             list1->____it_prev                 != list2->____it_prev                   ||
             list1->____it_get_data             != list2->____it_get_data               ||
             list1->____it_get_node             != list2->____it_get_node               ||
             list1->____it_end                  != list2->____it_end
             );
}

___inline___ void *ulist_get_list(const UList *list)
{
    if (list == NULL)
        return NULL;

    return list->____list;
}

___inline___ void ulist_destroy(UList *list)
{
    if (list == NULL)
        return;

    list->____destroy(ulist_get_list(list));
    FREE(list);
}

___inline___ void ulist_destroy_with_entries(UList *list)
{
    if (list == NULL)
        return;

    list->____destroy_with_entries(ulist_get_list(list));
    FREE(list);
}

___inline___ int ulist_insert_first(UList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____insert_first(ulist_get_list(list), data);
}

___inline___ int ulist_insert_last(UList * ___restrict___ list, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____insert_last(ulist_get_list(list), data);
}

___inline___ int ulist_insert_pos(UList * ___restrict___ list, size_t pos, const void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____insert_pos(ulist_get_list(list), pos, data);
}

___inline___ int ulist_delete_first(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____delete_first(ulist_get_list(list));
}

___inline___ int ulist_delete_last(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____delete_last(ulist_get_list(list));
}

___inline___ int ulist_delete_pos(UList *list, size_t pos)
{
    if (list == NULL)
        return 1;

    return list->____delete_pos(ulist_get_list(list), pos);
}

___inline___ int ulist_delete_first_with_entry(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____delete_first_with_entry(ulist_get_list(list));
}

___inline___ int ulist_delete_last_with_entry(UList *list)
{
    if (list == NULL)
        return 1;

    return list->____delete_last_with_entry(ulist_get_list(list));
}

___inline___ int ulist_delete_pos_with_entry(UList *list, size_t pos)
{
    if (list == NULL)
        return 1;

    return list->____delete_pos_with_entry(ulist_get_list(list), pos);
}

___inline___ int ulist_get_pos(const UList * ___restrict___ list, size_t pos, void * ___restrict___ data)
{
    if (list == NULL)
        return 1;

    return list->____get_pos(ulist_get_list(list), pos, data);
}

___inline___ UList *ulist_merge(const UList * ___restrict___ list1, const  UList * ___restrict___ list2)
{
    UList *list3;

    if (list1 == NULL || list2 == NULL)
        return NULL;

    /* we can only merge IFF have the same type */
    if (!ulist_the_same_type(list1, list2))
        return NULL;

    /* Create new instance */
    list3 = (UList *)malloc(sizeof(UList));
    if (list3 == NULL)
        return NULL;

    /* merge lists */
    list3->____list = list1->____merge(ulist_get_list(list1), ulist_get_list(list2));
    if (list3->____list == NULL)
    {
        FREE(list3);
        return NULL;
    }

    /* fill hooks */
    list3->____destroy                  = list1->____destroy;
    list3->____destroy_with_entries     = list1->____destroy_with_entries;
    list3->____insert_first             = list1->____insert_first;
    list3->____insert_last              = list1->____insert_last;
    list3->____insert_pos               = list1->____insert_pos;
    list3->____delete_first             = list1->____delete_first;
    list3->____delete_last              = list1->____delete_last;
    list3->____delete_pos               = list1->____delete_pos;
    list3->____delete_first_with_entry  = list1->____delete_first_with_entry;
    list3->____delete_last_with_entry   = list1->____delete_last_with_entry;
    list3->____delete_pos_with_entry    = list1->____delete_pos_with_entry;
    list3->____get_pos                  = list1->____get_pos;
    list3->____merge                    = list1->____merge;
    list3->____to_array                 = list1->____to_array;
    list3->____get_data_size            = list1->____get_data_size;
    list3->____get_num_entries          = list1->____get_num_entries;

    list3->____it_create                = list1->____it_create;
    list3->____it_destroy               = list1->____it_destroy;
    list3->____it_next                  = list1->____it_next;
    list3->____it_prev                  = list1->____it_prev;
    list3->____it_get_data              = list1->____it_get_data;
    list3->____it_get_node              = list1->____it_get_node;
    list3->____it_end                   = list1->____it_end;

    return list3;
}

___inline___ int ulist_to_array(const UList * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size)
{
    if (list == NULL)
        return 1;

    return list->____to_array(ulist_get_list(list), array, size);
}

___inline___ ssize_t ulist_get_data_size(const UList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_data_size(ulist_get_list(list));
}

___inline___ ssize_t ulist_get_num_entries(const UList *list)
{
    if (list == NULL)
        return 1;

    return list->____get_num_entries(ulist_get_list(list));
}

___inline___ UList_iterator *ulist_iterator_create(const UList *list, iti_mode_t mode)
{
    UList_iterator *it;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    it = (UList_iterator *)malloc(sizeof(UList_iterator));
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

IT_FUNC_CONTAINER(UList, ulist)

/* use this macro to create wrappers for iterator */
#define ULIST_ITERATOR_WRAPPERS_CREATE(type, prefix) \
    static ___unused___ void* ____it_create(const void *list, iti_mode_t mode); \
    static ___unused___ void ____it_destroy(void *it); \
    static ___unused___ int ____it_next(void *it); \
    static ___unused___ int ____it_prev(void *it); \
    static ___unused___ int ____it_get_data(const void * ___restrict___ it, void * ___restrict___ data); \
    static ___unused___ int ____it_get_node(const void *it, void *node); \
    static ___unused___ bool ____it_end(const void *it); \
    static ___unused___ void* ____it_create(const void *list, iti_mode_t mode) \
    { \
        return concat(prefix, _create)(ulist_get_list(list), mode); \
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
