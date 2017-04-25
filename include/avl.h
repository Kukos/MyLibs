#ifndef AVL_H
#define AVL_H

/*
    AVL tree implementation without recursive function, but with parent pointer

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <iterators.h>

typedef struct Avl_node
{
    void            *data;
    struct Avl_node *left_son;
    struct Avl_node *right_son;
    struct Avl_node *parent;

    uint8_t         bf; /* balanced factor */

}Avl_node;

typedef struct Avl_iterator
{
    int      size_of;
    Avl_node *node;
}Avl_iterator;

typedef struct Avl
{
    Avl_node    *root;
    size_t      nodes;
    int         size_of;

    int (*cmp)(void *a, void *b);
}Avl;

IT_FUNC(Avl, avl)

/*
    Create AVL

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to Avl iff success
*/
Avl *avl_create(int size_of, int (*cmp)(void *a, void *b));

#define AVL_CREATE(PTR,TYPE,CMP) \
    do { \
        PTR = avl_create(sizeof(TYPE), CMP); \
    } while (0)

/*
    Destroy all nodes in tree

    PARAMS
    @IN tree - pointer to AVL

    RETURN:
    This is a void function
*/
void avl_destroy(Avl *tree);

/*
    Insert data to AVL IFF data with key ( using cmp ) is not in tree

    PARAMS
    @IN tree - pointer to tree
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int avl_insert(Avl *tree, void *data);

/*
    Getter of min value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int avl_min(Avl *tree, void *data);

/*
    Getter of max value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int avl_max(Avl *tree, void *data);

/*
    Search for data with key equals key @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key
    @OUT data_out - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
 int avl_search(Avl *tree, void *data_key, void *data_out);

 /*
 	Check existing of key in tree

  	PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key

    RETURN:
 	false iff key doesn't exist in tree
 	true iff key exists in tree
 */
 bool avl_key_exist(Avl *tree, void *data_key);

/*
    Delete data with key equals @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int avl_delete(Avl *tree, void *data_key);

/*
    Convert bst to sorted array

    PARAMS
    @IN tree - pointer to tree
    @OUT addr of array
    @IN / OUT - size - size of array

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int avl_to_array(Avl *tree, void *array, size_t *size);

#endif
