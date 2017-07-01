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
#include <sys/types.h>
#include <tree.h>

typedef char rbt_color_t;

typedef struct Rbt_node
{
    void            *____data;
    struct Rbt_node *____left_son;
    struct Rbt_node *____right_son;
    struct Rbt_node *____parent;
    rbt_color_t     ____color;

}Rbt_node;

typedef struct Rbt_iterator
{
    size_t      ____size_of;
    Rbt_node    *____node;
}Rbt_iterator;

typedef struct Rbt
{
    Rbt_node    *____root;
    size_t      ____nodes;
    size_t      ____size_of;

    int (*____cmp)(void *a, void *b);
}Rbt;

IT_FUNC(Rbt, rbt)

/*
    Create RBT as TREE

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to RBT iff success
*/
Tree *tree_rbt_create(int size_of, int (*cmp)(void* a,void *b));

/*
    Create RBT

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to RBT iff success
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
    Destroy RBT with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN tree - pointer to RBT
    @IN destructor -  your object destructor

    RETURN:
    This is a void function
*/
void rbt_destroy_with_entries(Rbt *tree, void (*destructor)(void *data));

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

/*
    Get Num entries of RBT

    PARAMS
    @IN tree - pointer to RBT

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t rbt_get_num_entries(Rbt *tree);

/*
    Get Size of RBT data

    PARAMS
    @IN tree - pointer to RBT

    RETURN
    -1 iff failure
    Size of data
*/
int rbt_get_data_size(Rbt *tree);

#endif
