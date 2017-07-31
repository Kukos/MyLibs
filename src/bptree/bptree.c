#include <bptree.h>
#include <compiler.h>
#include <log.h>
#include <stdlib.h>
#include <assert.h>
#include <generic.h>

/*
    Create B+ Tree Node

    PARAMS
    @IN size_of - data size of
    @IN fanout - max number in
    @IN parent - pointer to parent
    @IN leaf - is leaf ?

    RETURN
    NULL iff failure
    Pointer to new node
*/
___unused___ static BPTree_node *bptree_node_create(int size_of, int fanout, BPTree_node *parent, bool leaf);

/*
    Destroy node (without entries)

    PARAMS
    @IN node - pointer to BPTree node

    RETURN
    This is a void function
*/
static void bptree_node_destroy(BPTree_node *node);

/*
    Destroy node with entries

    PARAMS
    @IN node - pointer to BPTree node
    @IN destructor - destructor

    RETURN
    This is a void function
*/
___unused___ static void bptree_node_destroy_with_entries(BPTree_node *node, void (*destructor)(void *data));

/*
    Is BPTree empty ?

    PARAMS
    @IN tree - pointer to Tree

    RETURN
    true iff tree is empty
    false iff tree is not empty
*/
static bool bptree_is_empty(BPTree *tree);

/*
    Get first leaf of BPTree

    PARAMS
    @IN tree - pointer to BPTree

    RETURN
    NULL iff failure
    Pointer to first leaf
*/
static BPTree_node *bptree_get_first_leaf(BPTree *tree);

static BPTree_node *bptree_node_create(int size_of, int fanout, BPTree_node *parent, bool leaf)
{
    BPTree_node *node;

    TRACE("");

    assert(size_of > 1);
    assert(fanout > 2);

    node = (BPTree_node *)malloc(sizeof(BPTree_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->____keys = malloc((size_t)(size_of * fanout));
    if (node->____keys == NULL)
    {
        FREE(node);
        ERROR("malloc error\n", NULL, "");
    }

    node->____ptrs = (BPTree_node **)malloc((size_t)(sizeof(BPTree_node *) * (size_t)(fanout + 1)));
    if (node->____ptrs == NULL)
    {
        FREE(node);
        FREE(node->____keys);

        ERROR("malloc error\n", NULL, "");
    }

    node->____keys_c = 0;
    node->____parent = parent;
    node->____is_leaf = leaf;

    KLIST_INIT(&node->____list);

    return node;
}

static void bptree_node_destroy(BPTree_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->____keys);
    FREE(node->____ptrs);

    FREE(node);
}

static void bptree_node_destroy_with_entries(BPTree_node *node, void (*destructor)(void *data))
{
    size_t i;
    BYTE *t;

    TRACE("");

    if (node == NULL)
        return;

    if (destructor == NULL)
        return;

    t = (BYTE *)node->____keys;

    /* destroy entries */
    for (i = 0; i < node->____keys_c; ++i)
        destructor((void *)(t + i));

    /* destroy normal node */
    bptree_node_destroy(node);
}

static bool bptree_is_empty(BPTree *tree)
{
    TRACE("");

    return !!tree->____num_entries;
}

static BPTree_node *bptree_get_first_leaf(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    if (klist_is)

}

BPTree* bptree_create(int fanout, int size_of, int (*cmp)(void* a,void *b))
{
    BPTree *tree;

    TRACE("");

    if (fanout < 2)
        ERROR("fanout < 2\n", NULL, "");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    tree = (BPTree *)malloc(sizeof(BPTree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL, "");

    tree->____leaves = klist_master_create();
    if (tree->____leaves == NULL)
    {
        FREE(tree);
        ERROR("klist_master_create error\n", NULL, "");
    }

    tree->____root          = NULL;
    tree->____hight         = 0;
    tree->____fanout        = fanout;
    tree->____size_of       = (size_t)size_of;
    tree->____num_entries   = 0;
    tree->____cmp           = cmp;

    return tree;
}

int bptree_insert(BPTree *tree, void *data)
{
    return 0;
}

void bptree_destroy(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        return;

    if (bptree_is_empty(tree))
    {
        goto destroy_tree;
        return;
    }

    /* TODO: free all nodes */


destroy_tree:
    klist_master_destroy(tree->____leaves);
    FREE(tree);
}

ssize_t bptree_get_num_entries(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        return -1;

    return (ssize_t)tree->____num_entries;
}

int bptree_get_data_size(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        return -1;


    return (int)tree->____size_of;
}

int bptree_get_hight(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        return -1;


    return tree->____hight;
}

