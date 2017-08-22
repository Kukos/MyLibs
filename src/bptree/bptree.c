#include <bptree.h>
#include <compiler.h>
#include <log.h>
#include <stdlib.h>
#include <assert.h>
#include <generic.h>
#include <search.h>

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
static BPTree_node *bptree_node_create(int size_of, int fanout, BPTree_node *parent, bool leaf);

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
static void bptree_node_destroy_with_entries(BPTree_node *node, void (*destructor)(void *data));

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

/*
    Get last leaf of BPTree

    PARAMS
    @IN tree - pointer to BPTree

    RETURN
    NULL iff failure
    Pointer to first leaf
*/
static BPTree_node *bptree_get_last_leaf(BPTree *tree);

/*
    Find node that contains this key

    PARAMS
    @IN tree - pointer to BPTree
    @IN key - data with key

    RETURN
    NULL iff failure
    Pointer to node iff success
*/
static BPTree_node *bptree_find_node_with_key(BPTree *tree, void *key);

/*
    Perform Binary Search and find Node with @key

    PARAMS
    @IN t - array of BPTree_node
    @IN size_of - size of data
    @IN cmp - compare func
    @IN key - data with key

    RETURN
    NULL iff failure
    Pointer to node iff success
*/
static BPTree_node *bptree_node_get_node_ptr_with_key(BPTree_node *node, int size_of, int (*cmp)(void *a, void *b), void *key);

static BPTree_node *bptree_node_create(int size_of, int fanout, BPTree_node *parent, bool leaf)
{
    BPTree_node *node;

    TRACE("");

    assert(size_of > 1);
    assert(fanout > 2);

    node = (BPTree_node *)malloc(sizeof(BPTree_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->____keys = calloc((size_t)fanout, (size_t)size_of);
    if (node->____keys == NULL)
    {
        FREE(node);
        ERROR("calloc error\n", NULL, "");
    }

    node->____ptrs = (BPTree_node **)calloc((size_t)(fanout + 1), (size_t)(sizeof(BPTree_node *)));
    if (node->____ptrs == NULL)
    {
        FREE(node);
        FREE(node->____keys);

        ERROR("calloc error\n", NULL, "");
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
    for (i = 0; i < (size_t)node->____keys_c; ++i)
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
    KList *list;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    list = klist_get_head(&tree->____leaves);
    if (list == NULL)
        ERROR("Tree is empty\n", NULL, "");

    return klist_entry(list, BPTree_node, ____list);
}

static BPTree_node *bptree_get_last_leaf(BPTree *tree)
{
    KList *list;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    list = klist_get_tail(&tree->____leaves);
    if (list == NULL)
        ERROR("Tree is empty\n", NULL, "");

    return klist_entry(list, BPTree_node, ____list);
}

static BPTree_node *bptree_node_get_node_ptr_with_key(BPTree_node *node, int size_of, int (*cmp)(void *a, void *b), void *key)
{
    ssize_t index;

    TRACE("");

    if (node == NULL)
        ERROR("node == NULL\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    if (key == NULL)
        ERROR("key == NULL\n", NULL, "");

    index = find_first_sorted(key, node->____keys, node->____keys_c, cmp, size_of);
    if (index == -1)
        return NULL;

    return node->____ptrs[index + 1];
}

static BPTree_node *bptree_find_node_with_key(BPTree *tree, void *key)
{
    BPTree_node *ptr;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    if (key == NULL)
        ERROR("key == NULL\n", NULL, "");

    ptr = tree->____root;
    while (!ptr->____is_leaf)
        ptr = bptree_node_get_node_ptr_with_key(ptr, (int)tree->____size_of, tree->____cmp, key);

    return ptr;
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

    KLIST_MASTER_INIT(&tree->____leaves);

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
    BPTree_node *new_node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (bptree_is_empty(tree))
    {
        new_node = bptree_node_create((int)tree->____size_of, tree->____fanout, NULL, true);
        if (new_node)
            ERROR("bptree_node_create error\n", 1, "");

        tree->____root = new_node;
        tree->____hight = 1;
        tree->____num_entries = 1;
    }
    else
    {

    }

    return 0;
}

void bptree_destroy(BPTree *tree)
{
    BPTree_node *ptr;
    BPTree_node *temp;
    size_t index;

    TRACE("");

    if (tree == NULL)
        return;

    if (bptree_is_empty(tree))
        goto destroy_error;

    ptr = bptree_get_first_leaf(tree);
    if (ptr == NULL)
        goto destroy_error;

    while (ptr != NULL)
    {
        for (index = 0; index <= ptr->____keys_c; ++index)
            bptree_node_destroy(ptr->____ptrs[index]);

        temp = ptr->____parent;
        bptree_node_destroy(ptr);
        ptr = temp;
    }

destroy_error:
    FREE(tree);
}

void bptree_destroy_with_entries(BPTree *tree, void (*destructor)(void *data))
{
    BPTree_node *ptr;
    BPTree_node *temp;
    size_t index;

    TRACE("");

    if (tree == NULL)
        return;

    if (destructor == NULL)
        goto destroy_error;

    if (bptree_is_empty(tree))
        goto destroy_error;

    ptr = bptree_get_first_leaf(tree);
    if (ptr == NULL)
        goto destroy_error;

    while (ptr != NULL)
    {
        for (index = 0; index <= ptr->____keys_c; ++index)
            bptree_node_destroy_with_entries(ptr->____ptrs[index], destructor);

        temp = ptr->____parent;
        bptree_node_destroy(ptr);
        ptr = temp;
    }

destroy_error:
    FREE(tree);
}

int bptree_min(BPTree *tree, void *data)
{
    BPTree_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", 1, "");

    node = bptree_get_first_leaf(tree);

    __ASSIGN__(*(BYTE *)data, ((BYTE *)node->____keys)[0], tree->____size_of);

    return 0;
}

int bptree_max(BPTree *tree, void *data)
{
    BPTree_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", 1, "");

    node = bptree_get_last_leaf(tree);

    __ASSIGN__(*(BYTE *)data, ((BYTE *)node->____keys)[node->____keys_c - 1], tree->____size_of);

    return 0;
}

int bptree_search(BPTree *tree, void *data_key, void *data_out)
{
    BPTree_node *node;
    ssize_t index;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1, "");

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", 1, "");

    node = bptree_find_node_with_key(tree, data_key);
    if (node == NULL)
        return 1;

    index = find_first_sorted(data_key, node->____keys, node->____keys_c, tree->____cmp, (int)tree->____size_of);
    if (index == -1)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, ((BYTE *)node->____keys)[index], tree->____size_of);

    return 0;
}

int bptree_to_array(BPTree *tree, void *array, size_t *size)
{
    void *t;
    BPTree_node *node;
    KList *ptr;
    int counter;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (array == NULL)
        ERROR("array == NULL\n", 1, "");

    t = malloc(tree->____size_of * tree->____num_entries);
    if ()

    /* for each leaf */
    klist_for_each(ptr, &tree->____leaves, counter)
    {
        node = klist_entry(ptr, BPTree_node, ____list);
    }

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

