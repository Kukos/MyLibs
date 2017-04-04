#ifndef TREE_H
#define TREE_H

/*
    Tree Container Framework

    To use this framework, plase provide constructor function for your tree
    i. e. tree_bst_create( ... )
    and in function assign pointers to functions

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <stdbool.h>
#include <stddef.h>
#include <compiler.h>

typedef struct Tree
{
    void    *tree; /* pointer to tree */

    /* private functions */
    void        (*____destroy)(void *tree);
    int         (*____insert)(void *tree, void *data);
    int         (*____delete)(void *tree, void *data);
    int         (*____min)(void *tree);
    int         (*____max)(void *tree);
    int         (*____search)(void *tree, void *data_in, void *data_out);
    bool        (*____key_exist)(void *tree, void *data);
    int         (*____balance)(void *tree);
    int         (*____toarray)(void *tree, void **array, size_t *size);
    size_t      (*____num_entries)(void *tree);
    int         (*____height)(void *tree);
}Tree;

/*
    Destroy Tree

    PARAMS
    @IN tree - pointer to Tree

    RETURN:
    This is a void function
*/
__inline__ void tree_destroy(Tree *tree)
{
    tree->____destroy(tree->tree);
}


/*
    Insert new data to Tree

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
__inline__ int tree_insert(Tree *tree, void *data)
{
    return tree->____insert(tree->tree, data);
}

/*
    Delet data to Tree

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
__inline__ int tree_delete(Tree *tree, void *data)
{
    return tree->____delete(tree->tree, data);
}

/*
    Get data with min key

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of min data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
__inline__ int tree_min(Tree *tree, void *data)
{
    return tree->____min(tree->tree, data);
}

/*
    Get data with max key

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of max data

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
__inline__ int tree_max(Tree *tree, void *data)
{
    return tree->____max(tree->tree, data);
}


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
__inline__ int tree_search(Tree *tree, void *data_in, void *data_out)
{
    return tree->____search(tree->tree, data_in, data_out);
}

/*
    Check existing of data with key in @data

    PARAMS
    @IN tree - pointer to Tree
    @IN data - address of fake data with correct key

    RETURN:
    true - iff data exist
    false iff data not exists
*/
__inline__ bool tree_key_exist(Tree *tree, void *data)
{
    return tree->____key_exist(tree->tree, data);
}


/*
    Balance Tree ( iff not balanced )

    PARAMS
    @IN tree - pointer to Tree

    RETURN:
    0 iff success
    Non-zero value iff failed
*/
__inline__ int tree_balance(Tree *tree)
{
    return tree->____balance(tree->tree);
}

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
__inline__ int tree_to_array(Tree *tree, void **array, size_t *size)
{
    return tree->____toarray(tree->tree, array, size);
}

/*
    Get Num of entries in Tree

    PARAMS
    @IN tree - pointer to tree

    RETURN:
    0 iff tree is empty or fail
    %Num of Entries iff success

*/
__inline__ size_t tree_num_enries(Tree *tree)
{
    return tree->____num_entries(tree->tree);
}

/*
    Get Tree Height

    PARAMS
    @IN tree

    RETURN:
    -1 iff failure
    %Height

*/
__inline__ int tree_height(Tree *tree)
{
    return tree->____height(tree->tree);
}

#endif
