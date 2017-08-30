#ifndef B_PLUS_TREE_H
#define B_PLUS_TREE_H

/*
    Simple implementation of B+Tree

    Only destroy function is recursive due the fact that B+Tree high is so small

    To make Tree more balanced, place for sentinel is added to all nodes,
    always before splitting, insert is calling, so keys are more balanced.

    Also to make structure more cache friendly, KList is used instead of List2D

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <stddef.h>
#include <klist.h>
#include <tree.h>
#include <iterators.h>

typedef struct BPTree_node
{
    struct BPTree_node  **____ptrs;
    void                *____keys;
    size_t              ____keys_c;

    struct BPTree_node  *____parent;

    KList               ____list;
    bool                ____is_leaf;
}BPTree_node;

typedef struct BPTree
{
    BPTree_node     *____root;
    KList_master    ____leaves;

    int             ____hight;
    int             ____fanout;
    size_t          ____num_entries;
    size_t          ____size_of;

    int (*____cmp)(void *a, void *b);
    void (*____destructor)(void *data);
}BPTree;

typedef struct BPTree_iterator
{
    BPTree_node *____node;
    size_t      ____index;
    size_t      ____size_of;
    bool        ____first_time;
}BPTree_iterator;

IT_FUNC(BPTree, bptree)


/*
    Create BPTree as TREE

    PARAMS
    @IN fanout - number of max entries in one node
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to Tree iff success
*/
Tree *tree_bptree_create(int fanout, int size_of, int (*cmp)(void *a, void *b));

/*
    Create BPTree

    PARAMS
    @IN fanout - number of max entries in one node
    @IN size_of - size_of data in tree
    @IN cmp - cmp function

    RETURN:
    NULL iff failure
    Pointer to bptree iff success
*/
BPTree* bptree_create(int fanout, int size_of, int (*cmp)(void* a,void *b));

#define BPTREE_CREATE(PTR, FAN, TYPE, CMP) \
    do { \
        PTR = bptree_create(FAN, sizeof(TYPE), CMP); \
    } while (0);

/*
    Destroy all nodes in tree

    PARAMS
    @IN tree - pointer to BPTree

    RETURN:
    This is a void function
*/
void bptree_destroy(BPTree *tree);

/*
    Destroy BPTree with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN tree - pointer to BPTree
    @IN destructor - your destructor

    RETURN:
    This is a void function
*/
void bptree_destroy_with_entries(BPTree *tree, void (*destructor)(void *data));

/*
    Insert data to BPTree IFF data with key ( using cmp ) is not in tree

    PARAMS
    @IN tree - pointer to tree
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bptree_insert(BPTree *tree, void *data);

/*
    Getter of min value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bptree_min(BPTree *tree, void *data);

/*
    Getter of max value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bptree_max(BPTree *tree, void *data);

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
int bptree_search(BPTree *tree, void *data_key, void *data_out);

 /*
 	Check existing of key in tree

  	PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key

    RETURN:
 	false iff key doesn't exist in tree
 	true iff key exists in tree
 */
bool bptree_key_exist(BPTree *tree, void *data_key);

/*
    Delete data with key equals @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bptree_delete(BPTree *tree, void *data_key);

/*
    Convert bptree to sorted array

    PARAMS
    @IN tree - pointer to tree
    @OUT addr of array
    @OUT size - size of array

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int bptree_to_array(BPTree *tree, void *array, size_t *size);

/*
    Get Num entries of BPTree

    PARAMS
    @IN tree - pointer to BPTree

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t bptree_get_num_entries(BPTree *tree);

/*
    Get Size of BPTree data

    PARAMS
    @IN tree - pointer to BPTree

    RETURN
    -1 iff failure
    Size of data
*/
int bptree_get_data_size(BPTree *tree);

/*
    Get Tree Hight

    PARAMS
    @IN tree

    RETURN
    -1 iff failure
    Hight iff success
*/
int bptree_get_hight(BPTree *tree);

#endif