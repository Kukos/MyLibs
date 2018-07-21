#ifndef RBT_H
#define RBT_H

/*
    Generic Red-Black Tree, without recursive functions[^1], but with parent pointer

    ^1 -> only 1 function is recursive: bst_get_hight, because function is rarely use
    it's better to have O(n) recursive getter instead of additional O(log(n)) operation
    during insert and delete

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stdbool.h>
#include <stddef.h>
#include <iterators.h>
#include <sys/types.h>
#include <tree.h>
#include <generic.h>
#include <common.h>

typedef struct Rbt_node Rbt_node;
typedef struct Rbt_iterator Rbt_iterator;
typedef struct Rbt Rbt;

IT_FUNC(Rbt, rbt)

/*
    Create RBT as TREE

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function
    @IN destroy - your data destructor

    RETURN:
    NULL iff failure
    Pointer to RBT iff success
*/
Tree *tree_rbt_create(size_t size_of, cmp_f cmp, destructor_f destroy);

/*
    Create RBT

    destructor by void * pass addr i.e in list we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN size_of - size_of data in tree
    @IN cmp - cmp function
    @IN destroy - your data destructor

    RETURN:
    NULL iff failure
    Pointer to RBT iff success
*/
Rbt* rbt_create(size_t size_of,cmp_f cmp, destructor_f destroy);

#define RBT_CREATE(PTR, TYPE, CMP, DESTROY) \
    do { \
        PTR = rbt_create(sizeof(TYPE), CMP, DESTROY); \
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

    PARAMS
    @IN tree - pointer to RBT

    RETURN:
    This is a void function
*/
void rbt_destroy_with_entries(Rbt *tree);

/*
    Insert data to RBT IFF data with key ( using cmp ) is not in tree

    PARAMS
    @IN tree - pointer to tree
    @IN data - addr of data

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_insert(Rbt * ___restrict___ tree, const void * ___restrict___ data);

/*
    Getter of min value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_min(const Rbt * ___restrict___ tree, void * ___restrict___ data);

/*
    Getter of max value ( using cmp ) in tree

    PARAMS
    @IN tree - pointer to tree
    @OUT data - returned data by addr

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_max(const Rbt * ___restrict___ tree, void * ___restrict___ data);

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
 int rbt_search(const Rbt * ___restrict___ tree, const void *data_key, void *data_out);

 /*
 	Check existing of key in tree

  	PARAMS
    @IN tree - pointer to tree
    @IN data_key - addr of data with search key

    RETURN:
 	false iff key doesn't exist in tree
 	true iff key exists in tree
 */
 bool rbt_key_exist(const Rbt * ___restrict___ tree, const void * ___restrict___ data_key);

/*
    Delete data with key equals @data_key ( using cmp )

    PARAMS
    @IN tree - pointer to tree
    @IN data_key = addr of data with key to delete

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int rbt_delete(Rbt * ___restrict___ tree, const void * ___restrict___ data_key);

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
int rbt_delete_with_entry(Rbt * ___restrict___ tree, const void * ___restrict___ data_key);

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
int rbt_to_array(const Rbt * ___restrict___ tree, void * ___restrict___ array, size_t * ___restrict___ size);

/*
    Get Num entries of RBT

    PARAMS
    @IN tree - pointer to RBT

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t rbt_get_num_entries(const Rbt *tree);

/*
    Get Size of RBT data

    PARAMS
    @IN tree - pointer to RBT

    RETURN
    -1 iff failure
    Size of data
*/
ssize_t rbt_get_data_size(const Rbt *tree);

/*
    RECURSIVE getter with O(n) complexity

    PARAMS
    @IN tree

    RETURN
    -1 iff failure
    Hight iff success
*/
int rbt_get_hight(const Rbt *tree);

/*
    Get Node data

    PARAMS
    @IN node - pointer to Rbt_node
    @OUT data - node data

    RETURN
    Non-zero iff failure
    0 iff success
*/
int rbt_node_get_data(const Rbt_node * ___restrict___ node, void * ___restrict___ data);

#endif
