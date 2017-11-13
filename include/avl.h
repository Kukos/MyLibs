#ifndef AVL_H
#define AVL_H

/*
    AVL tree implementation without recursive function[^1], but with parent pointer

    ^1 -> only 1 function is recursive: bst_get_hight, because function is rarely use
    it's better to have O(n) recursive getter instead of additional O(log(n)) operation
    during insert and delete

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <iterators.h>
#include <sys/types.h>
#include <tree.h>

typedef struct Avl_node
{
    void            *____data;
    struct Avl_node *____left_son;
    struct Avl_node *____right_son;
    struct Avl_node *____parent;

    uint8_t         ____bf; /* balanced factor */

}Avl_node;

typedef struct Avl_iterator
{
    size_t   ____size_of;
    Avl_node *____node;
}Avl_iterator;

typedef struct Avl
{
    Avl_node    *____root;
    size_t      ____nodes;
    size_t      ____size_of;

    int (*____cmp)(void *a, void *b);
}Avl;

IT_FUNC(Avl, avl)

/*
    Create AVL as TREE

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to Tree iff success
*/
Tree *tree_avl_create(int size_of, int (*cmp)(void* a,void *b));

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
    Destroy AVL with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN tree - pointer to AVL
    @IN destructor -  your object destructor

    RETURN:
    This is a void function
*/
void avl_destroy_with_entries(Avl *tree, void (*destructor)(void *data));

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

/*
    Get Num entries of AVL

    PARAMS
    @IN tree - pointer to AVL

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t avl_get_num_entries(Avl *tree);

/*
    Get Size of AVL data

    PARAMS
    @IN tree - pointer to AVL

    RETURN
    -1 iff failure
    Size of data
*/
int avl_get_data_size(Avl *tree);

/*
    RECURSIVE getter with O(n) complexity

    PARAMS
    @IN tree

    RETURN
    -1 iff failure
    Hight iff success
*/
int avl_get_hight(Avl *tree);

#endif
