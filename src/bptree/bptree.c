#include <bptree.h>
#include <compiler.h>
#include <log.h>
#include <stdlib.h>
#include <assert.h>
#include <generic.h>
#include <search.h>
#include <sort.h>

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
static BPTree_node *bptree_node_create(size_t size_of, int fanout, const BPTree_node *parent, bool leaf);

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
    @IN size_of - size of data
    @IN destructor - destructor

    RETURN
    This is a void function
*/
static void bptree_node_destroy_with_entries(BPTree_node *node, size_t size_of, void (*destructor)(void *data));

/*
    Is BPTree empty ?

    PARAMS
    @IN tree - pointer to Tree

    RETURN
    true iff tree is empty
    false iff tree is not empty
*/
static bool bptree_is_empty(const BPTree *tree);

/*
    Get first leaf of BPTree

    PARAMS
    @IN tree - pointer to BPTree

    RETURN
    NULL iff failure
    Pointer to first leaf
*/
static BPTree_node *bptree_get_first_leaf(const BPTree *tree);

/*
    Get last leaf of BPTree

    PARAMS
    @IN tree - pointer to BPTree

    RETURN
    NULL iff failure
    Pointer to first leaf
*/
static BPTree_node *bptree_get_last_leaf(const BPTree *tree);

/*
    Find node that contains this key

    PARAMS
    @IN tree - pointer to BPTree
    @IN key - data with key

    RETURN
    NULL iff failure
    Pointer to node iff success
*/
static BPTree_node *bptree_find_node_with_key(const BPTree *tree, const void *key);

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
static BPTree_node *bptree_node_get_node_ptr_with_key(const BPTree_node *node, size_t size_of, int (*cmp)(const void *a, const void *b), const void *key);

/*
     Check if node is full

     PARAMS
     @IN tree - pointer to tree
     @IN node - pointer to node

     RETURN
     false iff node is not full
     true iff node is full
*/
static bool bptree_node_is_full(const BPTree *tree, const BPTree_node *node);

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
static int  bptree_node_insert_into_node(BPTree *tree, BPTree_node *node, const BPTree_node *ptr, const void *key);

/*
    Set all parents of node children to node

    PARAMS
    @IN node - pointer to B+Tree node

    RETURN
    This is a void function
*/
static void bptree_node_set_parent_to_all(const BPTree_node *node);

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

/*
    Helper to recursive calling

    PARAMS
    @IN node - pointer to node to destroy
    @IN size_of - size of data
    @IN destructor - pointer to destructor func or NULL

    RETURN
    This is a void function
*/
static void bptree_destroy_helper(BPTree_node *node, size_t size_of, void (*destructor)(void *data));

/*
    Get index of Lower bound of Key

    PARAMS
    @IN node - pointer to BPTree node
    @IN size_of - size of data
    @IN key - key
    @IN cmp - cmp function

    RETURN
    Index in @node where is lower bound
*/
static size_t bptree_lower_bound(const BPTree_node *node, size_t size_of, const void *key, int (*cmp)(const void *a, const void *b));

/*
    Do nothing

    Needed by Tree Framework
*/
___unused___ static int bptree_balance(BPTree *tree);

/*
    Update first key in path from node to root

    PARAMS
    @IN tree - pointer to BPTree
    @IN node - pointer to start node
    @IN key - new key

    RETURN
    This is a void function
*/
static void bptree_node_update_first_key(BPTree *tree, BPTree_node *node, const void *key);

static int bptree_balance(BPTree *tree)
{
    TRACE();
    LOG("BPTree is self balanced, balance no needed\n");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (bptree_is_empty(tree))
        ERROR("Tree is empty\n", 1);

    return 0;
}

static size_t bptree_lower_bound(const BPTree_node *node, size_t size_of, const void *key, int (*cmp)(const void *a, const void *b))
{
    ssize_t offset_left;
    ssize_t offset_right;
    ssize_t offset_middle;

    BYTE *_t;

    TRACE();

    offset_left = 0;
    offset_right = ((ssize_t)node->____keys_c) * (ssize_t)size_of;
    offset_middle = 0;

    _t = (BYTE *)node->____keys;

    /* normal lower bound */
    while (offset_left < offset_right)
    {
        offset_middle = ((offset_left / (ssize_t)size_of + offset_right / (ssize_t)size_of) >> 1) * (ssize_t)size_of;
        if (cmp(key, (void *)&_t[offset_middle]) < 0)
            offset_right = offset_middle;
        else
            offset_left = offset_middle + (ssize_t)size_of;
    }

    return (size_t)offset_left / size_of;
}


static BPTree_node *bptree_node_create(size_t size_of, int fanout, const BPTree_node *parent, bool leaf)
{
    BPTree_node *node;

    TRACE();

    assert(size_of > 1);
    assert(fanout >= 2);

    node = (BPTree_node *)malloc(sizeof(BPTree_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    /* fanout -> keys, 1 -> place for insert before splitting (tree is more optimized if we insert before splitting) */
    node->____keys = calloc((size_t)(fanout + 1), size_of);
    if (node->____keys == NULL)
    {
        FREE(node);
        ERROR("calloc error\n", NULL);
    }

    /* fanout + 1 -> ptrs, 1 -> place for insert before splitting (tree is more optimized if we insert before splitting) */
    node->____ptrs = (BPTree_node **)calloc((size_t)(fanout + 2), (size_t)(sizeof(BPTree_node *)));
    if (node->____ptrs == NULL)
    {
        FREE(node);
        FREE(node->____keys);

        ERROR("calloc error\n", NULL);
    }

    node->____keys_c = 0;
    node->____parent = (BPTree_node *)parent;
    node->____is_leaf = leaf;

    KLIST_INIT(&node->____list);

    return node;
}

static void bptree_node_destroy(BPTree_node *node)
{
    TRACE();

    if (node == NULL)
        return;

    FREE(node->____keys);
    FREE(node->____ptrs);

    FREE(node);
}

static void bptree_node_destroy_with_entries(BPTree_node *node, size_t size_of, void (*destructor)(void *data))
{
    size_t i;
    BYTE *t;

    TRACE();

    if (node == NULL)
        return;

    if (destructor == NULL)
        return;

    t = (BYTE *)node->____keys;

    /* destroy entries */
    for (i = 0; i < (size_t)node->____keys_c; ++i)
        destructor((void *)(t + (i * size_of)));

    /* destroy normal node */
    bptree_node_destroy(node);
}

static bool bptree_is_empty(const BPTree *tree)
{
    TRACE();

    return tree->____num_entries == 0;
}

static bool bptree_node_is_full(const BPTree *tree, const BPTree_node *node)
{
    TRACE();

    return node->____keys_c == ((size_t)tree->____fanout + 1);
}

static BPTree_node *bptree_get_first_leaf(const BPTree *tree)
{
    KList *list;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    list = klist_get_head(&tree->____leaves);
    if (list == NULL)
        ERROR("Tree is empty\n", NULL);

    return klist_entry(list, BPTree_node, ____list);
}

static BPTree_node *bptree_get_last_leaf(const BPTree *tree)
{
    KList *list;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    list = klist_get_tail(&tree->____leaves);
    if (list == NULL)
        ERROR("Tree is empty\n", NULL);

    return klist_entry(list, BPTree_node, ____list);
}

static BPTree_node *bptree_node_get_node_ptr_with_key(const BPTree_node *node, size_t size_of, int (*cmp)(const void *a, const void *b), const void *key)
{
    size_t i;

    TRACE();

    if (node == NULL)
        ERROR("node == NULL\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    if (key == NULL)
        ERROR("key == NULL\n", NULL);

    i = bptree_lower_bound(node, size_of, key, cmp);
    return node->____ptrs[i];
}

static BPTree_node *bptree_find_node_with_key(const BPTree *tree, const void *key)
{
    BPTree_node *ptr;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    if (key == NULL)
        ERROR("key == NULL\n", NULL);

    ptr = tree->____root;
    while (!ptr->____is_leaf)
        ptr = bptree_node_get_node_ptr_with_key(ptr, tree->____size_of, tree->____cmp, key);

    return ptr;
}

static void bptree_node_set_parent_to_all(const BPTree_node *node)
{
    size_t i;

    TRACE();

    for (i = 0; i <= node->____keys_c; ++i)
        if (node->____ptrs[i] != NULL)
            node->____ptrs[i]->____parent = (BPTree_node *)node;
}

static BPTree_node *bptree_split_node(BPTree *tree, BPTree_node *node)
{
    BPTree_node *parent;
    BPTree_node *new_node;

    size_t entries_to_move;

    TRACE();

    /* create parent iff needed */
    if (node->____parent == NULL)
    {
        parent = bptree_node_create(tree->____size_of, tree->____fanout, NULL, false);
        if (parent == NULL)
            ERROR("bptree_node_create error\n", NULL);

        node->____parent = parent;
        node->____parent->____ptrs[0] = node;

        tree->____root = parent;
        ++tree->____hight;
    }

    /* create new node */
    new_node = bptree_node_create(tree->____size_of, tree->____fanout, node->____parent, node->____is_leaf);
    if (new_node == NULL)
        ERROR("bptree_node_create error\n", NULL);

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
        ERROR("bptree_node_insert_into_node error\n", NULL);

    /* ptr copied to new node, so fix parent pointer */
    bptree_node_set_parent_to_all(new_node);

    return new_node;
}

static void bptree_node_update_first_key(BPTree *tree, BPTree_node *node, const void *key)
{
    BPTree_node *ptr;

    TRACE();

    ptr = node->____parent;
    while (ptr != NULL && tree->____cmp(key, ptr->____keys) == 0)
    {
        /* update key */
        __ASSIGN__(*(BYTE *)ptr->____keys, *(BYTE *)key, tree->____size_of);

        ptr = ptr->____parent;
    }
}

static int bptree_node_insert_into_node(BPTree *tree, BPTree_node *node, const BPTree_node *ptr, const void *key)
{
    BPTree_node *new_node;
    size_t i;

    TRACE();

    i = bptree_lower_bound(node, tree->____size_of, key, tree->____cmp);

    if (node->____keys_c > 0 && i < node->____keys_c)
    {
        /* make gap for key */
        (void)memmove(((BYTE *)node->____keys) + ((i + 1) * tree->____size_of),
            ((BYTE *)node->____keys) + (i * tree->____size_of), (node->____keys_c - i) * tree->____size_of);

        /* make gap for ptr */
        (void)memmove(node->____ptrs + i + 2, node->____ptrs + i + 1, (node->____keys_c - i) * sizeof(BPTree_node *));
    }
    /* insert key */
    __ASSIGN__(((BYTE *)node->____keys)[i * tree->____size_of], *(BYTE *)key, tree->____size_of);

    /* insert ptr */
    node->____ptrs[i + 1] = (BPTree_node *)ptr;

    ++node->____keys_c;

    /* first key updated, so update parent */
    if (i == 0)
        bptree_node_update_first_key(tree, node, key);

    if (bptree_node_is_full(tree, node))
    {
        /* node is full now, so split node */
        new_node = bptree_split_node(tree, node);
        if (new_node == NULL)
            ERROR("bptree_split_node error\n", 1);
    }

    return 0;
}

static void bptree_destroy_helper(BPTree_node *node, size_t size_of, void (*destructor)(void *data))
{
    size_t i;

    TRACE();

    if (node == NULL)
        return;

    for (i = 0; i <= node->____keys_c; ++i)
        if (node->____ptrs[i] != NULL)
            bptree_destroy_helper(node->____ptrs[i], size_of, destructor);

    if (destructor != NULL && node->____is_leaf)
        bptree_node_destroy_with_entries(node, size_of, destructor);
    else
        bptree_node_destroy(node);
}

BPTree* bptree_create(int fanout, int size_of, int (*cmp)(const void *a, const void *b))
{
    BPTree *tree;

    TRACE();

    if (fanout < 2)
        ERROR("fanout < 2\n", NULL);

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    tree = (BPTree *)malloc(sizeof(BPTree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL);

    KLIST_MASTER_INIT(&tree->____leaves);

    tree->____root          = NULL;
    tree->____hight         = 0;
    tree->____fanout        = fanout;
    tree->____size_of       = (size_t)size_of;
    tree->____num_entries   = 0;
    tree->____cmp           = cmp;

    return tree;
}

int bptree_insert(BPTree *tree, const void *data)
{
    BPTree_node *new_node;
    BPTree_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    /* case if empty */
    if (bptree_is_empty(tree))
    {
        new_node = bptree_node_create(tree->____size_of, tree->____fanout, NULL, true);
        if (new_node == NULL)
            ERROR("bptree_node_create error\n", 1);

        bptree_node_insert_into_node(tree, new_node, NULL, data);
        klist_insert_first(&tree->____leaves, &new_node->____list);

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

int bptree_delete(BPTree *tree, const void *data_key)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (bptree_is_empty(tree))
        ERROR("Tree is empty, nothing to delete\n", 1);

    return 0;
}

void bptree_destroy(BPTree *tree)
{
    TRACE();

    if (tree == NULL)
        return;

    if (!bptree_is_empty(tree))
        bptree_destroy_helper(tree->____root, tree->____size_of, NULL);

    FREE(tree);
}

void bptree_destroy_with_entries(BPTree *tree, void (*destructor)(void *data))
{
    TRACE();

    if (tree == NULL)
        return;

    if (!bptree_is_empty(tree))
        bptree_destroy_helper(tree->____root, tree->____size_of, destructor);

    FREE(tree);
}

int bptree_min(const BPTree *tree, void *data)
{
    BPTree_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", 1);

    node = bptree_get_first_leaf(tree);

    __ASSIGN__(*(BYTE *)data, ((BYTE *)node->____keys)[0], tree->____size_of);

    return 0;
}

int bptree_max(const BPTree *tree, void *data)
{
    BPTree_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", 1);

    node = bptree_get_last_leaf(tree);

    __ASSIGN__(*(BYTE *)data, ((BYTE *)node->____keys)[(node->____keys_c - 1) * tree->____size_of], tree->____size_of);

    return 0;
}

int bptree_search(const BPTree *tree, const void *data_key, void *data_out)
{
    BPTree_node *node;
    ssize_t index;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", 1);

    node = bptree_find_node_with_key(tree, data_key);
    if (node == NULL)
        return 1;

    index = find_first_sorted(data_key, node->____keys, node->____keys_c, tree->____cmp, (int)tree->____size_of);
    if (index == -1)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, ((BYTE *)node->____keys)[(size_t)index * tree->____size_of], tree->____size_of);

    return 0;
}

bool bptree_key_exist(const BPTree *tree, const void *data_key)
{
    BPTree_node *node;
    ssize_t index;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", false);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", false);

    if (bptree_is_empty(tree))
        ERROR("BPTree is empty\n", false);

    node = bptree_find_node_with_key(tree, data_key);
    if (node == NULL)
        return false;

    index = find_first_sorted(data_key, node->____keys, node->____keys_c, tree->____cmp, (int)tree->____size_of);
    return index == -1 ? false : true;
}

int bptree_to_array(const BPTree *tree, void *array, size_t *size)
{
    void *t;
    BYTE *_t;
    BPTree_node *node;
    KList *ptr;
    int counter;
    size_t i;
    size_t offset = 0;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (bptree_is_empty(tree))
        ERROR("Tree is empty\n", 1);

    t = malloc(tree->____size_of * tree->____num_entries);
    if (t == NULL)
        ERROR("malloc error\n", 1);

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
    *(void **)array = _t;


    return 0;
}

ssize_t bptree_get_num_entries(const BPTree *tree)
{
    TRACE();

    if (tree == NULL)
        return -1;

    return (ssize_t)tree->____num_entries;
}

int bptree_get_data_size(const BPTree *tree)
{
    TRACE();

    if (tree == NULL)
        return -1;


    return (int)tree->____size_of;
}

int bptree_get_hight(const BPTree *tree)
{
    TRACE();

    if (tree == NULL)
        return -1;


    return tree->____hight;
}

int bptree_bulkload(BPTree *tree, void *keys, size_t n, double _load_factor)
{
    void *array = NULL;
    void *temp_array;
    size_t size;
    int load_factor = (int)(_load_factor * 100.0);

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (keys == NULL)
        ERROR("keys == NULL\n", 1);

    if (load_factor > 100 || load_factor < 50)
        ERROR("Incorrect load factor\n", 1);

    /* tree is not empty, so copy array and insert to again with keys */
    if (!bptree_is_empty(tree))
    {
        bptree_to_array(tree, (void *)&array, &size);
        if (array == NULL)
            ERROR("bptree to array error\n", 1);

        temp_array = malloc(tree->____size_of * (size + n));
        if (temp_array == NULL)
        {
            FREE(array);
            ERROR("malloc error\n", 1);
        }

        (void)memcpy(temp_array, array, size * tree->____size_of);
        (void)memcpy(((BYTE *)temp_array) + (size * tree->____size_of), keys, n * tree->____size_of);

        FREE(keys);
        keys = temp_array;
        n += size;

        (void)sort(keys, n, tree->____cmp, (int)tree->____size_of);
    }

    return 0;
}

BPTree_iterator *bptree_iterator_create(const BPTree *tree, iti_mode_t mode)
{
    BPTree_iterator *iterator;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect node\n", NULL);

    if (bptree_is_empty(tree))
        return NULL;

    iterator = (BPTree_iterator *)malloc(sizeof(BPTree_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    if (mode == ITI_BEGIN)
    {
        iterator->____node = bptree_get_first_leaf(tree);
        iterator->____index = 0;
    }
    else
    {
        iterator->____node = bptree_get_last_leaf(tree);
        iterator->____index = iterator->____node->____keys_c - 1;
    }

    iterator->____size_of = tree->____size_of;
    iterator->____first_time = true;
    iterator->____end_node = iterator->____node;

    return iterator;
}

int bptree_iterator_init(const BPTree *tree, BPTree_iterator *iterator, iti_mode_t mode)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect node\n", 1);

    if (bptree_is_empty(tree))
        return 1;

    iterator = (BPTree_iterator *)malloc(sizeof(BPTree_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", 1);

    if (mode == ITI_BEGIN)
    {
        iterator->____node = bptree_get_first_leaf(tree);
        iterator->____index = 0;
    }
    else
    {
        iterator->____node = bptree_get_last_leaf(tree);
        iterator->____index = iterator->____node->____keys_c - 1;
    }

    iterator->____size_of = tree->____size_of;
    iterator->____first_time = true;
    iterator->____end_node = iterator->____node;

    return 0;
}

void bptree_iterator_destroy(BPTree_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int bptree_iterator_next(BPTree_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

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
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

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

int bptree_iterator_get_data(const BPTree_iterator *iterator, void *val)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (val == NULL)
        ERROR("val == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, ((BYTE *)iterator->____node->____keys)[iterator->____index * iterator->____size_of], iterator->____size_of);

    return 0;
}

int bptree_iterator_get_node(const BPTree_iterator *iterator, void *node)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (node == NULL)
        ERROR("node == NULL\n", 1);

    *(void **)node = iterator->____node;

    return 0;
}

bool bptree_iterator_end(const BPTree_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return !iterator->____first_time && iterator->____node == iterator->____end_node;
}

TREE_WRAPPERS_CREATE(BPTree, bptree)

Tree *tree_bptree_create(int fanout, int size_of, int (*cmp)(const void *a, const void *b))
{
    Tree *tree;

    TRACE();

    /* create Tree */
    tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL);

    /* create bptree */
    tree->____tree = (void *)bptree_create(fanout, size_of, cmp);
    if (tree->____tree == NULL)
    {
        FREE(tree);
        ERROR("bptree_create error\n", NULL);
    }

    /* fill hooks */
    TREE_WRAPPERS_ASSIGN(tree);

    return tree;
}