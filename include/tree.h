#ifndef TREE_H
#define TREE_H

/*
    Tree Container Framework

    To use this framework:

    1. Use Macro in TREE_WRAPPERS_CREATE your .c file
    2. Declare your constructor with prefix tree and sufix _create i.e tree_bst_create
    3. In Constructor use macro TREE_WRAPPERS_ASSIGN
    4. Use Tree instead of your struct but create Tree by your custom constructor i.e Tree *tree = tree_bst_create
    5. Use Tree inline function instead of your struct function i.e tree_insert instead of bst_insert

    (6.) IF you have custom struct function instead of default names and parameters for tree
    do not use TREE macros, you have to create wrappers and fill hooks by yourself

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <stdbool.h>
#include <stddef.h>
#include <compiler.h>
#include <sys/types.h>
#include <iterators.h>
#include <common.h>
#include <stdlib.h>
#include <log.h>

typedef struct Tree_iterator
{
    void    *____iterator;

    void    (*____destroy)(void *iterator);
    int     (*____next)(void *iterator);
    int     (*____prev)(void *iterator);
    int     (*____get_data)(void *iterator, void *data);
    int     (*____get_node)(void *iterator, void *node);
    bool    (*____end)(void *iterator);
} Tree_iterator;

typedef struct Tree
{
    void        *____tree; /* pointer to tree */

    /* private functions */
    void        (*____destroy)(void *tree);
    void        (*____destroy_with_entries)(void *tree, void (*destructor)(void *data));
    int         (*____insert)(void *tree, void *data);
    int         (*____delete)(void *tree, void *data);
    int         (*____min)(void *tree, void *data);
    int         (*____max)(void *tree, void *data);
    int         (*____search)(void *tree, void *data_in, void *data_out);
    bool        (*____key_exist)(void *tree, void *data);
    int         (*____balance)(void *tree);
    int         (*____to_array)(void *tree, void *array, size_t *size);
    ssize_t     (*____get_num_entries)(void *tree);
    int         (*____get_data_size)(void *tree);

    /* to create iterator */
    void*       (*____it_create)(void *tree, ITI_MODE mode);
    int         (*____it_init)(void *tree, void *it, ITI_MODE mode);
    void        (*____it_destroy)(void *iterator);
    int         (*____it_next)(void *iterator);
    int         (*____it_prev)(void *iterator);
    int         (*____it_get_data)(void *iterator, void *data);
    int         (*____it_get_node)(void *iterator, void *node);
    bool        (*____it_end)(void *iterator);
}Tree;


/*
    Create Tree iterator

    PARAMS
    @IN tree - pointer to Tree
    @IN mode - iterator mode

    RETURN
    NULL iff failure
    Pointer to Tree_iterator iff success
*/
___inline___ Tree_iterator *tree_iterator_create(Tree *tree, ITI_MODE mode);

/*
    Init Tree iterator

    PARAMS
    @IN tree - pointer to Tree
    @IN it - pointer to iterator
    @IN mode - Iterator mode

    RETURN
    0 iff success
    Non-zero value iff failure
*/
___inline___ int tree_iterator_init(Tree *tree, Tree_iterator *it, ITI_MODE mode);

/* Macro to create wrappers to your struct to provide assignment to framework functions */
#define TREE_WRAPPERS_CREATE(type, prefix) \
    TREE_ITERATOR_WRAPPERS_CREATE(concat(type, _iterator), concat(prefix, _iterator)) \
    static ___unused___ void ____destroy(void *tree); \
    static ___unused___ void ____destroy_with_entries(void *tree, void (*destructor)(void *data)); \
    static ___unused___ int ____insert(void *tree, void *data); \
    static ___unused___ int ____delete(void *tree, void *data); \
    static ___unused___ int ____min(void *tree, void *data); \
    static ___unused___ int ____max(void *tree, void *data); \
    static ___unused___ int ____search(void *tree, void *data_in, void *data_out); \
    static ___unused___ bool ____key_exist(void *tree, void *data); \
    static ___unused___ int ____balance(void *tree); \
    static ___unused___ int ____to_array(void *tree, void *array, size_t *size); \
    static ___unused___ ssize_t ____get_num_entries(void *tree); \
    static ___unused___ int ____get_data_size(void *tree); \
    static ___unused___ void ____destroy(void *tree) \
    { \
        concat(prefix, _destroy)((type *)tree); \
    } \
    \
    static ___unused___ void ____destroy_with_entries(void *tree, void (*destructor)(void *data)) \
    { \
        concat(prefix, _destroy_with_entries)((type *)tree, destructor); \
    } \
    \
    static ___unused___ int ____insert(void *tree, void *data) \
    { \
        return concat(prefix, _insert)((type *)tree, data); \
    } \
    \
    static ___unused___ int ____delete(void *tree, void *data) \
    { \
        return concat(prefix, _delete)((type *)tree, data); \
    } \
    \
    static ___unused___ int ____min(void *tree, void *data) \
    { \
        return concat(prefix, _min)((type *)tree, data); \
    } \
    \
    static ___unused___ int ____max(void *tree, void *data) \
    { \
        return concat(prefix, _max)((type *)tree, data); \
    } \
    \
    static ___unused___ int ____search(void *tree, void *data_in, void* data_out) \
    { \
        return concat(prefix, _search)((type *)tree, data_in, data_out); \
    } \
    \
    static ___unused___ bool ____key_exist(void *tree, void *data) \
    { \
        return concat(prefix, _key_exist)((type *)tree, data); \
    } \
    \
    static ___unused___ int ____balance(void *tree) \
    { \
        return concat(prefix, _balance)((type *)tree); \
    } \
    \
    static ___unused___ int ____to_array(void *tree, void *array, size_t *size) \
    { \
        return concat(prefix, _to_array)((type *)tree, array, size); \
    } \
    \
    static ___unused___ int ____get_data_size(void *tree) \
    { \
        return concat(prefix, _get_data_size)((type *)tree); \
    } \
    \
    static ___unused___ ssize_t ____get_num_entries(void *tree) \
    { \
        return concat(prefix, _get_num_entries)((type *)tree); \
    }

#define TREE_WRAPPERS_ASSIGN(tree) \
    do { \
        (tree)->____destroy               = ____destroy; \
        (tree)->____destroy_with_entries  = ____destroy_with_entries; \
        (tree)->____insert                = ____insert; \
        (tree)->____delete                = ____delete; \
        (tree)->____min                   = ____min; \
        (tree)->____max                   = ____max; \
        (tree)->____search                = ____search; \
        (tree)->____key_exist             = ____key_exist; \
        (tree)->____balance               = ____balance; \
        (tree)->____to_array              = ____to_array; \
        (tree)->____get_data_size         = ____get_data_size; \
        (tree)->____get_num_entries       = ____get_num_entries; \
        (tree)->____it_create             = ____it_create; \
        (tree)->____it_init               = ____it_init; \
        (tree)->____it_destroy            = ____it_destroy; \
        (tree)->____it_next               = ____it_next; \
        (tree)->____it_prev               = ____it_prev; \
        (tree)->____it_get_data           = ____it_get_data; \
        (tree)->____it_get_node           = ____it_get_node; \
        (tree)->____it_end                = ____it_end; \
    } while (0);

/*
    Get pointer to tree from Tree

    PARAMS
    @IN tree - pointer to Tree

    RETURN
    pointer to tree member
*/
___inline___ void *tree_get_tree(Tree *tree);

/*
    Destroy Tree

    PARAMS
    @IN tree - pointer to Tree

    RETURN:
    This is a void function
*/
___inline___ void tree_destroy(Tree *tree);

/*
    Destroy Tree with data

    PARAMS
    @IN tree - pointer to Tree
    @IN destructor - destructor

    RETURN:
    This is a void function
*/
___inline___ void tree_destroy_with_entries(Tree *tree, void (*destructor)(void *data));

/*
    Insert new data to Tree

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_insert(Tree *tree, void *data);

/*
    Delete data from Tree

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_delete(Tree *tree, void *data);

/*
    Get data with min key

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of min data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_min(Tree *tree, void *data);

/*
    Get data with max key

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of max data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_max(Tree *tree, void *data);

/*
    Get data with key equal to fake @data

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of fake data with correct key
    @IN data_out - address of data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_search(Tree *tree, void *data_in, void *data_out);

/*
    Check existing of data with key in @data

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of fake data with correct key

    RETURN:
    true - iff data exist
    false iff data not exists
*/
___inline___ bool tree_key_exist(Tree *tree, void *data);

/*
    Balance Tree ( iff not balanced )

    PARAMS
    @IN tree - pointer to Tree

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_balance(Tree *tree);

/*
    Convert Tree to Array

    PARAMS
    @IN tree - pointer to Tree
    @IN array - address of array
    @IN size - array size

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
___inline___ int tree_to_array(Tree *tree, void *array, size_t *size);

/*
    Get Num of entries in Tree

    PARAMS
    @IN tree - pointer to tree

    RETURN:
    -1 iff failure
    %Num of Entries iff success

*/
___inline___ ssize_t tree_get_num_entries(Tree *tree);

/*
    Get Tree data size

    PARAMS
    @IN tree - pointer to tree

    RETURN:
    -1 iff failure
    %Data size iff success
*/
___inline___ int tree_get_data_size(Tree *tree);

___inline___ void *tree_get_tree(Tree *tree)
{
    if (tree == NULL)
        return NULL;

    return tree->____tree;
}

___inline___ void tree_destroy(Tree *tree)
{
    if (tree == NULL)
        return;

    tree->____destroy(tree_get_tree(tree));
    FREE(tree);
}

___inline___ void tree_destroy_with_entries(Tree *tree, void (*destructor)(void *data))
{
    if (tree == NULL)
        return;

    tree->____destroy_with_entries(tree_get_tree(tree), destructor);
    FREE(tree);
}


___inline___ int tree_insert(Tree *tree, void *data)
{
    if (tree == NULL || data == NULL)
        return 1;

    return tree->____insert(tree_get_tree(tree), data);
}

___inline___ int tree_delete(Tree *tree, void *data)
{
    if (tree == NULL || data == NULL)
        return 1;

    return tree->____delete(tree_get_tree(tree), data);
}

___inline___ int tree_min(Tree *tree, void *data)
{
    if (tree == NULL || data == NULL)
        return 1;

    return tree->____min(tree_get_tree(tree), data);
}

___inline___ int tree_max(Tree *tree, void *data)
{
    if (tree == NULL || data == NULL)
        return 1;

    return tree->____max(tree_get_tree(tree), data);
}

___inline___ int tree_search(Tree *tree, void *data_in, void *data_out)
{
    if (tree == NULL || data_in == NULL || data_out == NULL)
        return 1;

    return tree->____search(tree_get_tree(tree), data_in, data_out);
}

___inline___ bool tree_key_exist(Tree *tree, void *data)
{
    if (tree == NULL || data == NULL)
        return false;

    return tree->____key_exist(tree_get_tree(tree), data);
}

___inline___ int tree_balance(Tree *tree)
{
    if (tree == NULL)
        return 1;

    return tree->____balance(tree_get_tree(tree));
}

___inline___ int tree_to_array(Tree *tree, void *array, size_t *size)
{
    if (tree == NULL || array == NULL || size == NULL)
        return 1;

    return tree->____to_array(tree_get_tree(tree), array, size);
}

___inline___ ssize_t tree_get_num_entries(Tree *tree)
{
    if (tree == NULL)
        return -1;

    return tree->____get_num_entries(tree_get_tree(tree));
}

___inline___ int tree_get_data_size(Tree *tree)
{
    if (tree == NULL)
        return -1;

    return tree->____get_data_size(tree_get_tree(tree));
}

___inline___ Tree_iterator *tree_iterator_create(Tree *tree, ITI_MODE mode)
{
    Tree_iterator *it;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_END && mode != ITI_ROOT)
        ERROR("Incorrect mode\n", NULL, "");

    it = (Tree_iterator *)malloc(sizeof(Tree_iterator));
    if (it == NULL)
        ERROR("malloc error\n", NULL, "");

    it->____iterator = tree->____it_create(tree, mode);
    if (it->____iterator == NULL)
        ERROR("iterator create error\n", NULL, "");

    /* FILL HOOKS */
    it->____destroy    = tree->____it_destroy;
    it->____next       = tree->____it_next;
    it->____prev       = tree->____it_prev;
    it->____get_data   = tree->____it_get_data;
    it->____get_node   = tree->____it_get_node;
    it->____end        = tree->____it_end;

    return it;
}

___inline___ int tree_iterator_init(Tree *tree, Tree_iterator *it, ITI_MODE mode)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (it == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_END && mode != ITI_ROOT)
        ERROR("Incorrect mode\n", 1, "");

    it->____iterator = tree->____it_create(tree, mode);
    if (it->____iterator == NULL)
        ERROR("iterator create error\n", 1, "");

    /* FILL HOOKS */
    it->____destroy    = tree->____it_destroy;
    it->____next       = tree->____it_next;
    it->____prev       = tree->____it_prev;
    it->____get_data   = tree->____it_get_data;
    it->____get_node   = tree->____it_get_node;
    it->____end        = tree->____it_end;

    return 0;
}

IT_FUNC_CONTAINER(Tree, tree)

/* use this macro to create wrappers for iterator */
#define TREE_ITERATOR_WRAPPERS_CREATE(type, prefix) \
    static ___unused___ void* ____it_create(void *tree, ITI_MODE mode); \
    static ___unused___ int ____it_init(void *tree, void *it, ITI_MODE mode); \
    static ___unused___ void ____it_destroy(void *it); \
    static ___unused___ int ____it_next(void *it); \
    static ___unused___ int ____it_prev(void *it); \
    static ___unused___ int ____it_get_data(void *it, void *data); \
    static ___unused___ int ____it_get_node(void *it, void *node); \
    static ___unused___ bool ____it_end(void *it); \
    static ___unused___ void* ____it_create(void *tree, ITI_MODE mode) \
    { \
        return concat(prefix, _create)(tree_get_tree(tree), mode); \
    } \
    static ___unused___ int ____it_init(void *tree, void *it, ITI_MODE mode) \
    { \
        return concat(prefix, _init)(tree, (type *)it, mode); \
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

#endif
