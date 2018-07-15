#ifndef BST_H
#define BST_H

/*
    Binary Search Tree without recursive function[^1], but with parent pointer

    ^1 -> only 1 function is recursive: bst_get_hight, because function is rarely use
    it's better to have O(n) recursive getter instead of additional O(log(n)) operation
    during insert and delete

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <iterators.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <tree.h>
#include <generic.h>

typedef struct Bst_node
{
    struct Bst_node     *____left_son;
    struct Bst_node     *____right_son;
    struct Bst_node     *____parent;
    __extension__ BYTE  ____data[]; /* placeholder for data */

}Bst_node;

typedef struct Bst_iterator
{
    size_t      ____size_of;
    Bst_node    *____node;
}Bst_iterator;

typedef struct Bst
{
    Bst_node    *____root;
    size_t      ____nodes;
    size_t      ____size_of;

    int (*____cmp)(const void* a, const void *b);
    void (*____destroy)(void *entry);
}Bst;

IT_FUNC(Bst, bst)

/*
    Create Bst as TREE

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function
    @IN destroy - your data destructor or NULL

    RETURN:
    NULL iff failure
    Pointer to Tree iff success
*/
Tree *tree_bst_create(size_t size_of, int (*cmp)(const void *a, const void *b), void (*destroy)(void *entry));

/*
    Create BST

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function
    @IN destroy - your data destructor or NULL

    RETURN:
    NULL iff failure
    Pointer to bst iff success
*/
Bst* bst_create(size_t size_of, int (*cmp)(const void *a, const void *b), void (*destroy)(void *entry));

#define BST_CREATE(PTR, TYPE, CMP, DESTROY) \
    do { \
        PTR = bst_create(sizeof(TYPE), CMP, DESTROY); \
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

    PARAMS
    @IN tree - pointer to BST

    RETURN:
    This is a void function
*/
void bst_destroy_with_entries(Bst *tree);

/*
    Insert data to BST IFF data with key ( using cmp ) is not in tree

    PARAMS
    @IN tree - pointer to tree
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bst_insert(Bst *tree, const void *data);

/*
    Getter of min value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bst_min(const Bst *tree, void *data);

/*
    Getter of max value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bst_max(const Bst *tree, void *data);

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
 int bst_search(const Bst *tree, const void *data_key, void *data_out);

 /*
 	Check existing of key in tree

  	PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key

    RETURN:
 	false iff key doesn't exist in tree
 	true iff key exists in tree
 */
 bool bst_key_exist(const Bst *tree, const void *data_key);

/*
    Delete data with key equals @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bst_delete(Bst *tree, const void *data_key);

/*
    Delete data with key equals @data_key ( using cmp )
    and call destructor

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bst_delete_with_entry(Bst *tree, const void *data_key);

/*
    Balance tree using DSW algorithm

    PARAMS
    @IN tree - pointer to tree

    RETURN:
    0 iff success
    Non-zero value iff failure
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
    Non-zero value iff failure
*/
int bst_to_array(const Bst *tree, void *array, size_t *size);

/*
    Get Num entries of BST

    PARAMS
    @IN tree - pointer to BST

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t bst_get_num_entries(const Bst *tree);

/*
    Get Size of BST data

    PARAMS
    @IN tree - pointer to BST

    RETURN
    -1 iff failure
    Size of data
*/
ssize_t bst_get_data_size(const Bst *tree);

/*
    RECURSIVE getter with O(n) complexity

    PARAMS
    @IN tree

    RETURN
    -1 iff failure
    Hight iff success
*/
int bst_get_hight(const Bst *tree);

#endif
