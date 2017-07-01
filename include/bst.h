#ifndef BST_H
#define BST_H

/*
    Binary Search Tree without recursive function, but with parent pointer

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <iterators.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <tree.h>

typedef struct Bst_node
{
    void                *____data;
    struct Bst_node     *____left_son;
    struct Bst_node     *____right_son;
    struct Bst_node     *____parent;

}Bst_node;

typedef struct Bst_iterator
{
    int         ____size_of;
    Bst_node    *____node;
}Bst_iterator;

typedef struct Bst
{
    Bst_node    *____root;
    size_t      ____nodes;
    int         ____size_of;

    int (*____cmp)(void* a,void *b);
}Bst;

IT_FUNC(Bst, bst)

/*
    Create Bst as TREE

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to Tree iff success
*/
Tree *tree_bst_create(int size_of, int (*cmp)(void* a,void *b));

/*
    Create BST

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to bst iff success
*/
Bst* bst_create(int size_of, int (*cmp)(void* a,void *b));

#define BST_CREATE(PTR, TYPE, CMP) \
    do { \
        PTR = bst_create(sizeof(TYPE), CMP); \
    } while (0);

/*
    Destroy all nodes in tree

    PARAMS
    @IN tree - pointer to BST

    RETURN:
    This is a void function
*/
void bst_destroy(Bst *tree);

/*
    Destroy BST with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN tree - pointer to BST
    @IN destructor -  your object destructor

    RETURN:
    This is a void function
*/
void bst_destroy_with_entries(Bst *tree, void (*destructor)(void *data));

/*
    Insert data to BST IFF data with key ( using cmp ) is not in tree

    PARAMS
    @IN tree - pointer to tree
    @IN data - addr of data

    RETURN:
    0 iff success
    Positive value iff failure
*/
int bst_insert(Bst *tree, void *data);

/*
    Getter of min value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Positive value iff failure
*/
int bst_min(Bst *tree, void *data);

/*
    Getter of max value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Positive value iff failure
*/
int bst_max(Bst *tree, void *data);

/*
    Search for data with key equals key @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key
    @OUT data_out - returned data by addr

    RETURN:
    0 iff success
    Positive value iff failure
*/
 int bst_search(Bst *tree, void *data_key, void *data_out);

 /*
 	Check existing of key in tree

  	PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key

    RETURN:
 	false iff key doesn't exist in tree
 	true iff key exists in tree
 */
 bool bst_key_exist(Bst *tree, void *data_key);

/*
    Delete data with key equals @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Positive value iff failure
*/
int bst_delete(Bst *tree, void *data_key);

/*
    Balance tree using DSW algorithm

    PARAMS
    @IN tree - pointer to tree

    RETURN:
    0 iff success
    Positive value iff failure
*/
int bst_balance(Bst *tree);

/*
    Convert bst to sorted array

    PARAMS
    @IN tree - pointer to tree
    @OUT addr of array
    @OUT size - size of array

    RETURN:
    0 iff success
    Positive value iff failure
*/
int bst_to_array(Bst *tree, void *array, size_t *size);

/*
    Get Num entries of BST

    PARAMS
    @IN tree - pointer to BST

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t bst_get_num_entries(Bst *tree);

/*
    Get Size of BST data

    PARAMS
    @IN tree - pointer to BST

    RETURN
    -1 iff failure
    Size of data
*/
int bst_get_data_size(Bst *tree);

#endif
