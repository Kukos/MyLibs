#ifndef RBT_H
#define RBT_H

/*
    Generic Red-Black Tree, without recursive functions, but with parent pointer

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdbool.h>
#include <stddef.h>
#include <iterators.h>

#define RBT_COLOR char

typedef struct Rbt_node
{
    void            *data;
    struct Rbt_node *left_son;
    struct Rbt_node *right_son;
    struct Rbt_node *parent;
    RBT_COLOR           color;

}Rbt_node;

typedef struct Rbt_iterator
{
    int         size_of;
    Rbt_node    *node;
}Rbt_iterator;

typedef struct Rbt
{
    Rbt_node    *root;
    size_t      nodes;

    int         size_of;

    int (*cmp)(void *a, void *b);
}Rbt;

IT_FUNC(Rbt, rbt)

/*
    Create RBT

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to bst iff success
*/
Rbt* rbt_create(int size_of,int (*cmp)(void *a, void *b));

#define RBT_CREATE(PTR, TYPE, CMP) \
    do { \
        PTR = rbt_create(sizeof(TYPE), CMP); \
    } while (0)

/*
    Destroy all nodes in tree

    PARAMS
    @IN tree - pointer to RBT

    RETURN:
    This is a void function
*/
void rbt_destroy(Rbt *tree);

/*
    Insert data to RBT IFF data with key ( using cmp ) is not in tree

    PARAMS
    @IN tree - pointer to tree
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_insert(Rbt *tree, void *data);

/*
    Getter of min value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Positive value if failure
*/
int rbt_min(Rbt *tree, void *data);

/*
    Getter of max value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_max(Rbt *tree, void *data);

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
 int rbt_search(Rbt *tree, void *data_key, void *data_out);

 /*
 	Check existing of key in tree

  	PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key

    RETURN:
 	false iff key doesn't exist in tree
 	true iff key exists in tree
 */
 bool rbt_key_exist(Rbt *tree, void *data_key);

/*
    Delete data with key equals @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_delete(Rbt *tree, void *data_key);

/*
    Convert rbt to sorted array

    PARAMS
    @IN tree - pointer to tree
    @OUT addr of array
    @OUT size - size of array

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_to_array(Rbt *tree, void *array, size_t *size);

#endif
