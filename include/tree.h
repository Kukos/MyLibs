#ifndef TREE_H
#define TREE_H

/*
    Tree Container Framework

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
    void        ____destroy(void *tree);
    int         ____insert(void *tree, void *data);
    int         ____delete(void *tree, void *data);
    int         ____min(void *tree);
    int         ____max(void *tree);
    int         ____search(void *tree, void *data_in, void *data_out);
    bool        ____key_exist(void *tree, void *data);
    int         ____balance(void *tree);
    int         ____toarray(void *tree, void **array, size_t *size);
    size_t      ____num_entries(void *tree);
    int         ____height(void *tree);
}Tree;

__inline__ void tree_destroy(Tree *tree)
{
    tree->____destroy(tree->tree);
}

__inline__ int tree_insert(Tree *tree, void *data)
{
    return tree->____insert(tree->tree, data);
}

__inline__ int tree_delete(Tree *tree, void *data)
{
    return tree->____delete(tree->tree, data);
}

__inline__ int tree_min(Tree *tree, void *data)
{
    return tree->____min(tree->tree, data);
}
__inline__ int tree_max(Tree *tree, void *data)
{
    return tree->____max(tree->tree, data);
}

__inline__ int tree_search(Tree *tree, void *data_in, void *data_out)
{
    return tree->____search(tree->tree, data_in, data_out);
}

__inline__ bool tree_key_exist(Tree *tree, void *data)
{
    return tree->____key_exist(tree->tree, data);
}

__inline__ int tree_balance(Tree *tree)
{
    return tree->____balance(tree->tree);
}

__inline__ int tree_to_array(Tree *tree, void **array, size_t *size)
{
    return tree->____toarray(tree->tree, array, size);
}

__inline__ size_t tree_num_enries(Tree *tree)
{
    return tree->____num_entries(tree->tree);
}

__inline__ int tree_height(Tree *tree)
{
    return tree->____height(tree->tree);
}

#endif
