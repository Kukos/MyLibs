#include <bptree.h>
#include <compiler.h>
#include <log.h>
#include <stdlib.h>
#include <assert.h>

/*
    Create B+ Tree Node

    PARAMS
    @IN size_of - data size of
    @IN fanout - max number in
*/
static BPTree_node *bptree_node_create(int size_of, int fanout);

static BPTree_node *bptree_node_create(int size_of, int fanout)
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

static bool bptree_is_empty(BPTree *tree)
{
    TRACE("");

    return !!tree->____num_entries;
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

void bptree_destroy(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        return;

    if (bptree_is_empty(tree))
    {
        klist_master_destroy(tree->____leaves);
        return;
    }

    /* TODO: free all nodes */


    klist_master_destroy(tree->____leaves);
}