#include <bst.h>
#include <log.h>
#include <generic.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <stdbool.h>
#include <assert.h>

struct Bst_node
{
    struct Bst_node     *left_son;
    struct Bst_node     *right_son;
    struct Bst_node     *parent;
    size_t              size_of;
    __extension__ BYTE  data[]; /* placeholder for data */

};

struct Bst_iterator
{
    size_t      size_of;
    Bst_node    *node;
};

struct Bst
{
    Bst_node    *root;
    size_t      nodes;
    size_t      size_of;

    cmp_f cmp;
    destructor_f destroy;
};

/*
    Create new node with data and parent

    PARAMS
    @IN data - addr to data
    @IN size_of - size of data in bytes
    @IN parent - node parent

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___ Bst_node *bst_node_create(const void * ___restrict___ data, size_t size_of, const Bst_node * ___restrict___ parent);

/*
    Deallocate bst node

    PARAMS
    @IN node - pointer to bst node

    RETURN:
    This is a void function
*/
static ___inline___ void bst_node_destroy(Bst_node *node);


/*
    Search for node with min key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___  Bst_node* bst_min_node(const Bst_node *node);

/*
    Search for node with max key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___  Bst_node *bst_max_node(const Bst_node *node);

/*
    Search for node with key equals @data_key ( using cmp )

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___ Bst_node *bst_node_search(const Bst * ___restrict___ tree, const void * ___restrict___ data_key);

/*
    Get successor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___ Bst_node *bst_successor(const Bst_node *node);

/*
    Get predecessor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/

static ___inline___ Bst_node *bst_predecessor(const Bst_node *node);

/*
    Right tree rotate

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node wchich we rotate

    RETURN:
    This is a void function
*/
static ___inline___  void bst_rotate_right(Bst *tree, Bst_node *node);

/*
    Left tree rotate

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node wchich we rotate

    RETURN:
    This is a void function
*/
static ___inline___ void bst_rotate_left(Bst *tree, Bst_node *node);

/*
    Recursive helper for bst_get_hight

    PARAMS
    @IN node - bst node

    RETURN
    hight of subtree with root @node
*/
static int __bst_rek_get_hight(const Bst_node *node);

/*
    Destroy whole BST

    PARAMS
    @IN tree - pointer to BST
    @IN destroy - call destructor ?

    RETURN
    This is a void function
*/
static void __bst_destroy(Bst *tree, bool destroy);

/*
    Delete node with key equal to @data_key

    PARAMS
    @IN tree - ptr to Bst
    @IN data_key - fake data with real key
    @IN destroy - call destructor ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __bst_delete(Bst * ___restrict___ tree, const void * ___restrict___ data_key, bool destroy);

static ___inline___ Bst_node *bst_node_create(const void * ___restrict___ data, size_t size_of, const Bst_node * ___restrict___ parent)
{
    Bst_node *node;

    TRACE();

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Bst_node *)malloc(sizeof(Bst_node) + size_of);
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    node->parent    = (Bst_node *)parent;
    node->left_son  = NULL;
    node->right_son = NULL;
    node->size_of   = size_of;

    return node;
}

static ___inline___ void bst_node_destroy(Bst_node *node)
{
    TRACE();

    if (node == NULL)
        return;

    FREE(node);
}

static ___inline___ Bst_node *bst_successor(const Bst_node *node)
{
    Bst_node *parent;

    TRACE();

    assert(node != NULL);

    if (node->right_son != NULL)
        return bst_min_node(node->right_son);

    parent = node->parent;
    while (parent != NULL && node == parent->right_son)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

static ___inline___ Bst_node *bst_predecessor(const Bst_node *node)
{
    Bst_node *parent;

    TRACE();

    assert(node != NULL);

    if (node->left_son != NULL)
        return bst_max_node(node->left_son);

    parent = node->parent;
    while (parent != NULL && node == parent->left_son)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

static ___inline___ Bst_node *bst_node_search(const Bst * ___restrict___ tree, const void * ___restrict___ data_key)
{
    Bst_node *node;

    TRACE();

    assert(tree != NULL);
    assert(data_key != NULL);

    node = tree->root;

    while (node != NULL)
    {
        if (tree->cmp(node->data,data_key) == 0)
            return node;
        else if (tree->cmp(node->data,data_key) > 0)
            node = node->left_son;
        else
            node = node->right_son;
    }

    /* now node is NULL */
    return NULL;
}

static ___inline___ Bst_node *bst_max_node(const Bst_node *node)
{
    Bst_node *parent;

    TRACE();

    assert(node != NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = (Bst_node *)node;
        node = node->right_son;
    }

    return parent;
}

static ___inline___ Bst_node *bst_min_node(const Bst_node *node)
{
    Bst_node *parent;

    TRACE();

    assert(node != NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = (Bst_node *)node;
        node = node->left_son;
    }

    return parent;
}

/*
        parent                        parent
          |                              |
         node                         left_son
       /      \          ---->       /         \
   left_son   right_son             X           node
   /    \                                      /     \
  X      Y                                    Y       right_son

*/
static ___inline___ void bst_rotate_right(Bst *tree, Bst_node *node)
{
    Bst_node *left_son;

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->left_son != NULL);

    left_son = node->left_son;

    /* update X ptr */
    node->left_son = left_son->right_son;

    if( node->left_son != NULL)
        node->left_son->parent = node;

    left_son->right_son = node;
    left_son->parent = node->parent;
    node->parent = left_son;

    /* update his child ptr */
    if (left_son->parent != NULL)
    {
        if (left_son->parent->left_son == node)
            left_son->parent->left_son = left_son;
        else
            left_son->parent->right_son = left_son;
    }
    else
        tree->root = left_son;
}

/*
    parent                           parent
       |                               |
      node                         right_son
    /      \                        /      \
left_son   right_son   ---->     node       Y
            /     \              /   \
           X       Y        left_son  X

*/
static ___inline___ void bst_rotate_left(Bst *tree, Bst_node *node)
{
    Bst_node *right_son;

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->right_son != NULL);

    right_son = node->right_son;

    /* update X ptr */
    node->right_son = right_son->left_son;

    if (node->right_son != NULL)
        node->right_son->parent = node;

    right_son->left_son = node;
    right_son->parent = node->parent;
    node->parent = right_son;

    /* update his child ptr */
    if (right_son->parent != NULL)
    {
        if (right_son->parent->left_son == node)
            right_son->parent->left_son = right_son;
        else
            right_son->parent->right_son = right_son;
    }
    else
        tree->root = right_son;
}

static int __bst_rek_get_hight(const Bst_node *node)
{
    int left;
    int right;

    if (node == NULL)
        return 0;

    left = __bst_rek_get_hight(node->left_son);
    right = __bst_rek_get_hight(node->right_son);

    return MAX(left, right) + 1;
}

static void __bst_destroy(Bst *tree, bool destroy)
{
    Bst_node *node;
    Bst_node *temp;

    TRACE();

    if (tree == NULL)
        return;

    if (tree->root == NULL)
    {
        FREE(tree);
        return;
    }

    /* Destroy tree using inorder */
    node = bst_min_node(tree->root);
    while (node != NULL)
    {
        temp = node;
        node = bst_successor(node);

        if (destroy && tree->destroy != NULL)
            tree->destroy((void *)temp->data);

        bst_node_destroy(temp);
    }

    FREE(tree);
}

static int __bst_delete(Bst * ___restrict___ tree, const void * ___restrict___ data_key, bool destroy)
{
    Bst_node *node;
    Bst_node *successor;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (tree->root == NULL)
        ERROR("Tree is empty\n", 1);

    node = bst_node_search(tree, data_key);
    if (node == NULL)
        ERROR("data with this key doesn't exist in tree, nothing to delete\n", 1);

    /* case 1, node is leaf_node */
    if (node->left_son == NULL && node->right_son == NULL)
    {
        /* if have parent */
        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = NULL;
            else
                node->parent->right_son = NULL;
        }
        else /* is root */
            tree->root = NULL;
    }
    else if (node->right_son == NULL) /* case 2 node has only left subtree */
    {
        /* update ptr to parent */
        node->left_son->parent = node->parent;
        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = node->left_son;
            else
                node->parent->right_son = node->left_son;
        }
        else
            tree->root = node->left_son;
    }
    else if (node->left_son == NULL) /* case 3 node has only right subtree */
    {
        /* update ptr to parent */
        node->right_son->parent = node->parent;
        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = node->right_son;
            else
                node->parent->right_son = node->right_son;
        }
        else
            tree->root = node->right_son;
    }
    else /* case 4 node has both childdren */
    {
        successor = bst_successor(node);

        /* delete successor */
        if (successor->right_son != NULL)
            successor->right_son->parent = successor->parent;

        if (successor->parent->left_son == successor)
            successor->parent->left_son = successor->right_son;
        else
            successor->parent->right_son = successor->right_son;

        /* update node children ptrs */
        successor->left_son = node->left_son;
        successor->right_son = node->right_son;

        /* successor is new parent */
        successor->left_son->parent = successor;
        if (successor->right_son != NULL)
            successor->right_son->parent = successor;

        /* successor is in node place, so update parent ptr */
        successor->parent = node->parent;

        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = successor;
            else
                node->parent->right_son = successor;
        }
        else
            tree->root = successor;
    }

    --tree->nodes;

    if (destroy && tree->destroy != NULL)
        tree->destroy((void *)node->data);

    bst_node_destroy(node);

    return 0;
}

Bst* bst_create(size_t size_of, cmp_f cmp, destructor_f destroy)
{
    Bst *tree;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    tree = (Bst *)malloc(sizeof(Bst));
    if (tree == NULL)
       ERROR("malloc error\n", NULL);

    tree->root = NULL;

    tree->cmp = cmp;
    tree->destroy = destroy;
    tree->size_of = size_of;

    tree->nodes = 0;

    return tree;
}

void bst_destroy(Bst *tree)
{
    TRACE();

    __bst_destroy(tree, false);
}

void bst_destroy_with_entries(Bst *tree)
{
    TRACE();

    __bst_destroy(tree, true);
}

int bst_insert(Bst * ___restrict___ tree, const void * ___restrict___ data)
{
    Bst_node *node;
    Bst_node *parent;
    Bst_node *new_node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    /* special case - empty tree */
    if (tree->root == NULL)
    {
        node = bst_node_create(data, tree->size_of, NULL);
		if (node == NULL)
			ERROR("bst_node_create\n", 1);

        tree->root = node;
    }
    else
    {
        parent = NULL;
        node = tree->root;

        /* find correct place to insert */
        while (node != NULL)
        {
            parent = node;
            if (tree->cmp(node->data, data) == 0) /* data exists in tree */
            {
                return 1;
            }
            else if (tree->cmp(node->data, data) > 0)
                node = node->left_son;
            else
                node = node->right_son;
        }

        new_node = bst_node_create(data, tree->size_of, parent);
		if (new_node == NULL)
			ERROR("bst_node_create\n", 1);

        /* new node is the right son */
        if (tree->cmp(new_node->data, parent->data) > 0)
            parent->right_son = new_node;
        else /* new_node is the left_node */
            parent->left_son = new_node;
    }

    ++tree->nodes;

    return 0;
}

int bst_delete(Bst * ___restrict___ tree, const void * ___restrict___ data_key)
{
    TRACE();

    return __bst_delete(tree, data_key, false);
}

int bst_delete_with_entry(Bst * ___restrict___ tree, const void * ___restrict___ data_key)
{
    TRACE();

    return __bst_delete(tree, data_key, true);
}

int bst_min(const Bst * ___restrict___ tree, void * ___restrict___ data)
{
    Bst_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (tree->root == NULL)
        ERROR("tree is empty\n", 1);

    node = bst_min_node(tree->root);
    if (node == NULL)
        ERROR("bst_min_node error\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

int bst_max(const Bst * ___restrict___ tree, void * ___restrict___ data)
{
    Bst_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (tree->root == NULL)
        ERROR("tree is empty\n", 1);

    node = bst_max_node(tree->root);
    if(node == NULL)
        ERROR("bst_max_node error\n", 1 );

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

 bool bst_key_exist(const Bst * ___restrict___ tree, const void * ___restrict___ data_key)
 {
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", false);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", false);

    if (tree->root == NULL)
        ERROR("tree is empty\n", false);

    return bst_node_search(tree, data_key) != NULL;
 }

 int bst_search(const Bst * ___restrict___ tree, const void *data_key, void *data_out)
{
    Bst_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (data_out == NULL)
        ERROR("data_out == NULL\n", 1);

    if (tree->root == NULL)
        ERROR("tree is empty\n", 1);

    node = bst_node_search(tree, data_key);
    if (node == NULL)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->data, tree->size_of);

    return 0;
}

/*
    Algorithm DSW (Day - Stout - Warren)
    O(n)
*/
int bst_balance(Bst *tree)
{
    Bst_node *ptr;

    int n;
    int k;
    int i;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (tree->root == NULL)
        ERROR("tree is empty\n", 1);

    ptr = tree->root;

    /* make "spine" from tree */
    while (ptr != NULL)
    {
        if (ptr->left_son != NULL)
        {
            bst_rotate_right(tree, ptr);
            ptr = ptr->parent;
        }
        else
            ptr = ptr->right_son;
    }

    n = (int)tree->nodes;

    /* count k from formula k = n + 1 - 2^(floor(LOG2(n + 1))) */
    k = n + 1 - (1 << (int)LOG2((unsigned int)(n + 1)));

    /* start from root and make k rotations */
    ptr = tree->root;
    for (i = 0; i < k; ++i)
    {
        /*rotate evry second node
            A
              \
                B
                  \
                    C   ( make left rotate on A and go to C)

             B
            /  \
           A    C

        */

        bst_rotate_left(tree, ptr);
        ptr = ptr->parent->right_son;
    }

    /*  we made k rotations, now rotate n - k / 2 n - k / 4 .... times */
    n -= k;

    while (n > 1)
    {
        n >>= 1;
        ptr = tree->root;
        for (i = 0; i < n; ++i)
        {
            bst_rotate_left(tree,ptr);
            ptr = ptr->parent->right_son;
        }
    }

    return 0;
}

int bst_to_array(const Bst * ___restrict___ tree, void * ___restrict___ array, size_t * ___restrict___ size)
{
    void *t;
    BYTE *_t;

    Bst_node *node;

    size_t offset;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (size == NULL)
        ERROR("size == NULL\n", 1);

    if (tree->root == NULL)
        ERROR("tree is empty\n", 1);

    t = malloc((size_t)tree->size_of * tree->nodes);
	if (t == NULL)
		ERROR("malloc error\n", 1);

    offset = 0;
    _t = (BYTE *)t;

    node = bst_min_node(tree->root);
    while (node != NULL)
    {
        __ASSIGN__(_t[offset], *(BYTE *)node->data, tree->size_of);
        offset += tree->size_of;

        node = bst_successor(node);
    }

    *(void **)array = t;
    *size = tree->nodes;

    return 0;
}

ssize_t bst_get_num_entries(const Bst *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1);

    return (ssize_t)tree->nodes;
}

ssize_t bst_get_data_size(const Bst *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    return (ssize_t)tree->size_of;
}

int bst_get_hight(const Bst *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (tree->root == NULL)
        return 0;

    return __bst_rek_get_hight(tree->root);
}

int bst_node_get_data(const Bst_node * ___restrict___ node, void * ___restrict___ data)
{
    TRACE();

    if (node == NULL || data == NULL)
        ERROR("node == NULL || data == NULL\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, node->size_of);

    return 0;
}


Bst_iterator *bst_iterator_create(const Bst *tree, iti_mode_t mode)
{
    Bst_iterator *iterator;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END && mode != ITI_ROOT)
        ERROR("Incorrect node\n", NULL);

    iterator = (Bst_iterator *)malloc(sizeof(Bst_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    if (tree->root == NULL)
    {
        FREE(iterator);
        return NULL;
    }

    if (mode == ITI_BEGIN)
        iterator->node = bst_min_node(tree->root);
    else if ( mode == ITI_END)
        iterator->node = bst_max_node(tree->root);
    else
        iterator->node = tree->root;

    iterator->size_of = tree->size_of;

    return iterator;
}

void bst_iterator_destroy(Bst_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int bst_iterator_next(Bst_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = bst_successor(iterator->node);

    return 0;
}

int bst_iterator_prev(Bst_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = bst_predecessor(iterator->node);

    return 0;
}

int bst_iterator_get_data(const Bst_iterator * ___restrict___ iterator, void * ___restrict___ val)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (val == NULL)
        ERROR("iterator == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

int bst_iterator_get_node(const Bst_iterator * ___restrict___ iterator, void * ___restrict___ node)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (node == NULL)
        ERROR("iterator == NULL\n", 1);

    *(void **)node = iterator->node;

    return 0;
}

bool bst_iterator_end(const Bst_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return iterator->node == NULL;
}

TREE_WRAPPERS_CREATE(Bst, bst)

Tree *tree_bst_create(size_t size_of, cmp_f cmp, destructor_f destroy)
{
    Tree *tree;

    TRACE();

    /* create Tree */
    tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL);

    /* create bst */
    tree->____tree = (void *)bst_create(size_of, cmp, destroy);
    if (tree->____tree == NULL)
    {
        FREE(tree);
        ERROR("bst_create error\n", NULL);
    }

    /* fill hooks */
    TREE_WRAPPERS_ASSIGN(tree);

    return tree;
}