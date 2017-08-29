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
static BPTree_node *bptree_node_create(size_t size_of, int fanout, BPTree_node *parent, bool leaf);

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
static BPTree_node *bptree_node_get_node_ptr_with_key(BPTree_node *node, size_t size_of, int (*cmp)(void *a, void *b), void *key);

/*
     Check if node is full

     PARAMS
     @IN tree - pointer to tree
     @IN node - pointer to node

     RETURN
     false iff node is not full
     true iff node is full
*/
static bool bptree_node_is_full(BPTree *tree, BPTree_node *node);

/*
    Insert key & ptr related to this key to node

    PARAMS
    @IN tree - pointer to B+
    @IN node - pointer to node where key will be inserted
    @IN ptr - pointer to child of this key or NULL iff node is leaf
    @IN key - pointer to key

    RETURN
    Non-zero value iff failure
    0 iff success
*/
static int  bptree_node_insert_into_node(BPTree *tree, BPTree_node *node, BPTree_node *ptr, void *key);

/*
    Set all parents of node children to node

    PARAMS
    @IN node - pointer to B+Tree node

    RETURN
    This is a void function
*/
static void bptree_node_set_parent_to_all(BPTree_node *node);

/*
    Split full node

    PARAMS
    @IN tree - pointer to B+ tree
    @IN node - pointer to node that will be split

    RETURN
    NULL iff failure
    Pointer to new node (sibling) iff success
*/
static BPTree_node *bptree_split_node(BPTree *tree, BPTree_node *node);

static BPTree_node *bptree_node_create(size_t size_of, int fanout, BPTree_node *parent, bool leaf)
{
    BPTree_node *node;

    TRACE("");

    assert(size_of > 1);
    assert(fanout >= 2);

    node = (BPTree_node *)malloc(sizeof(BPTree_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    /* fanout -> keys, 1 -> place for insert before splitting (tree is more optimized if we insert before splitting) */
    node->____keys = calloc((size_t)(fanout + 1), size_of);
    if (node->____keys == NULL)
    {
        FREE(node);
        ERROR("calloc error\n", NULL, "");
    }

    /* fanout + 1 -> ptrs, 1 -> place for insert before splitting (tree is more optimized if we insert before splitting) */
    node->____ptrs = (BPTree_node **)calloc((size_t)(fanout + 2), (size_t)(sizeof(BPTree_node *)));
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

    return tree->____num_entries == 0;
}

static bool bptree_node_is_full(BPTree *tree, BPTree_node *node)
{
    TRACE("");

    return node->____keys_c == ((size_t)tree->____fanout + 1);
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

static BPTree_node *bptree_node_get_node_ptr_with_key(BPTree_node *node, size_t size_of, int (*cmp)(void *a, void *b), void *key)
{
    size_t i;

    TRACE("");

    if (node == NULL)
        ERROR("node == NULL\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    if (key == NULL)
        ERROR("key == NULL\n", NULL, "");

    /* temporary solution */
    i = 0;
    while (i < node->____keys_c && cmp(((BYTE *)node->____keys) + (i * size_of), key) < 0)
        ++i;

    return node->____ptrs[i];
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
        ptr = bptree_node_get_node_ptr_with_key(ptr, tree->____size_of, tree->____cmp, key);

    return ptr;
}

static void bptree_node_set_parent_to_all(BPTree_node *node)
{
    size_t i;

    TRACE("");

    for (i = 0; i <= node->____keys_c; ++i)
        if (node->____ptrs[i] != NULL)
            node->____ptrs[i]->____parent = node;
}

static BPTree_node *bptree_split_node(BPTree *tree, BPTree_node *node)
{
    BPTree_node *parent;
    BPTree_node *new_node;

    size_t entries_to_move;

    TRACE("");

    /* create parent iff needed */
    if (node->____parent == NULL)
    {
        parent = bptree_node_create(tree->____size_of, tree->____fanout, NULL, false);
        if (parent == NULL)
            ERROR("bptree_node_create error\n", NULL, "");

        node->____parent = parent;
        node->____parent->____ptrs[0] = node;

        tree->____root = parent;
    }

    /* create new node */
    new_node = bptree_node_create(tree->____size_of, tree->____fanout, node->____parent, node->____is_leaf);
    if (new_node == NULL)
        ERROR("bptree_node_create error\n", NULL, "");

    if (node->____is_leaf)
        klist_insert_after(&node->____list, &new_node->____list);

    entries_to_move = (node->____keys_c + 1) >> 1;

    /* copy half of entries to new node */
    (void)memcpy(new_node->____keys, ((BYTE *)node->____keys) + (((node->____keys_c ) >> 1) * tree->____size_of),
        entries_to_move * tree->____size_of);
    (void)memcpy(new_node->____ptrs + 1, node->____ptrs + ((node->____keys_c >> 1) + 1), entries_to_move * sizeof(BPTree_node *));

    node->____keys_c -= entries_to_move;
    new_node->____keys_c += entries_to_move;

    /* insert ptr to new_node to parent */
    if (bptree_node_insert_into_node(tree, new_node->____parent, new_node, new_node->____keys))
        ERROR("bptree_node_insert_into_node error\n", NULL, "");

    /* ptr copied to new node, so fix parent pointer */
    bptree_node_set_parent_to_all(new_node);

    return new_node;
}

static int bptree_node_insert_into_node(BPTree *tree, BPTree_node *node, BPTree_node *ptr, void *key)
{
    BPTree_node *new_node;
    size_t i;

    TRACE("");

    /* temporary solution */
    i = 0;
    while (i < node->____keys_c && tree->____cmp(((BYTE *)node->____keys) + (i * tree->____size_of), key) < 0)
        ++i;

    if (node->____keys_c > 0 && i < node->____keys_c)
    {
        /* make gap for key */
        (void)memcpy(((BYTE *)node->____keys) + (i * tree->____size_of),
            ((BYTE *)node->____keys) + ((i + 1) * tree->____size_of), (node->____keys_c - i) * tree->____size_of);

        /* make gap for ptr */
        (void)memcpy(node->____ptrs[i + 1], node->____ptrs[i + 2], (node->____keys_c - i - 1) * sizeof(BPTree_node *));
    }
    /* insert key */
    __ASSIGN__(((BYTE *)node->____keys)[i * tree->____size_of], *(BYTE *)key, tree->____size_of);

    /* insert ptr */
    node->____ptrs[i + 1] = ptr;

    ++node->____keys_c;
    if (bptree_node_is_full(tree, node))
    {
        /* node is full now, so split node */
        new_node = bptree_split_node(tree, node);
        if (new_node == NULL)
            ERROR("bptree_split_node error\n", 1, "");
    }

    return 0;
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
    BPTree_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    /* case if empty */
    if (bptree_is_empty(tree))
    {
        new_node = bptree_node_create(tree->____size_of, tree->____fanout, NULL, true);
        if (new_node == NULL)
            ERROR("bptree_node_create error\n", 1, "");

        bptree_node_insert_into_node(tree, new_node, NULL, data);

        tree->____root = new_node;
        tree->____hight = 1;
        tree->____num_entries = 1;
    }
    else
    {
        /* get correct node */
        node = bptree_find_node_with_key(tree, data);

        /* insert new key to node */
        bptree_node_insert_into_node(tree, node, NULL, data);
        ++tree->____num_entries;
    }

    return 0;
}

static void bptree_destroy_helper(BPTree_node *node, void (*destructor)(void *data))
{
    size_t i;

    TRACE("");

    if (node == NULL)
        return;

    for (i = 0; i <= node->____keys_c; ++i)
        if (node->____ptrs[i] != NULL)
            bptree_destroy_helper(node->____ptrs[i], destructor);

    if (destructor)
        bptree_node_destroy_with_entries(node, destructor);
    else
        bptree_node_destroy(node);
}

void bptree_destroy(BPTree *tree)
{
    TRACE("");

    if (tree == NULL)
        return;

    if (!bptree_is_empty(tree))
        bptree_destroy_helper(tree->____root, NULL);

    FREE(tree);
}

void bptree_destroy_with_entries(BPTree *tree, void (*destructor)(void *data))
{
    TRACE("");

    if (tree == NULL)
        return;

    if (destructor == NULL)
        return;

    if (!bptree_is_empty(tree))
        bptree_destroy_helper(tree->____root, destructor);

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

    __ASSIGN__(*(BYTE *)data, ((BYTE *)node->____keys)[(node->____keys_c - 1) * tree->____size_of], tree->____size_of);

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

    __ASSIGN__(*(BYTE *)data_out, ((BYTE *)node->____keys)[(size_t)index * tree->____size_of], tree->____size_of);

    return 0;
}

bool bptree_key_exist(BPTree *tree, void *data_key)
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
        return false;

    index = find_first_sorted(data_key, node->____keys, node->____keys_c, tree->____cmp, (int)tree->____size_of);
    return index == -1 ? false : true;
}

int bptree_to_array(BPTree *tree, void *array, size_t *size)
{
    void *t;
    BYTE *_t;
    BPTree_node *node;
    KList *ptr;
    int counter;
    size_t i;
    size_t offset = 0;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (array == NULL)
        ERROR("array == NULL\n", 1, "");

    if (bptree_is_empty(tree))
        ERROR("Tree is empty\n", 1, "");

    t = malloc(tree->____size_of * tree->____num_entries);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    _t = (BYTE *)t;

    /* for each leaf */
    klist_for_each(ptr, &tree->____leaves, counter)
    {
        node = klist_entry(ptr, BPTree_node, ____list);

        /* for each key in node */
        for (i = 0; i < node->____keys_c; ++i)
        {
            __ASSIGN__(_t[offset], ((BYTE *)node->____keys)[i * tree->____size_of], tree->____size_of);
            offset += tree->____size_of;
        }
    }

    *size = tree->____num_entries;

    return 0;
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

BPTree_iterator *bptree_iterator_create(BPTree *tree, iti_mode_t mode)
{
    BPTree_iterator *iterator;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect node\n", NULL, "");

    iterator = (BPTree_iterator *)malloc(sizeof(BPTree_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    if (mode == ITI_BEGIN)
        iterator->____node = bptree_get_first_leaf(tree);
    else
        iterator->____node = bptree_get_last_leaf(tree);

    iterator->____size_of = tree->____size_of;
    iterator->____index = 0;
    iterator->____first_time = true;

    return iterator;
}

int bptree_iterator_init(BPTree *tree, BPTree_iterator *iterator, iti_mode_t mode)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect node\n", 1, "");

    iterator = (BPTree_iterator *)malloc(sizeof(BPTree_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", 1, "");

    if (mode == ITI_BEGIN)
        iterator->____node = bptree_get_first_leaf(tree);
    else
        iterator->____node = bptree_get_last_leaf(tree);

    iterator->____size_of = tree->____size_of;
    iterator->____index = 0;
    iterator->____first_time = true;


    return 0;
}

void bptree_iterator_destroy(BPTree_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int bptree_iterator_next(BPTree_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (iterator->____first_time)
        iterator->____first_time = false;

    /* go to the next element */
    ++iterator->____index;
    if (iterator->____index >= iterator->____node->____keys_c)
    {
        /* go to the next node */
        iterator->____node = klist_entry(iterator->____node->____list.____next, BPTree_node, ____list);
        iterator->____index = 0;
    }

    return 0;
}

int bptree_iterator_prev(BPTree_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (iterator->____first_time)
        iterator->____first_time = false;

    /* go to the prev element */
    --iterator->____index;
    if ((ssize_t)iterator->____index < 0)
    {
        /* go to the prev node */
        iterator->____node = klist_entry(iterator->____node->____list.____prev, BPTree_node, ____list);
        iterator->____index = iterator->____node->____keys_c - 1;
    }

    return 0;
}