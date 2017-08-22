#include <bst.h>
#include <log.h>
#include <generic.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <stdbool.h>
#include <assert.h>

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
static ___inline___ Bst_node *bst_node_create(void *data, int size_of, Bst_node *parent);

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
static ___inline___  Bst_node* bst_min_node(Bst_node *node);

/*
    Search for node with max key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___  Bst_node *bst_max_node(Bst_node *node);

/*
    Search for node with key equals @data_key ( using cmp )

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___ Bst_node *bst_node_search(Bst *tree, void *data_key);

/*
    Get successor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
static ___inline___ Bst_node *bst_successor(Bst_node *node);

/*
    Get predecessor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/

static ___inline___ Bst_node *bst_predecessor(Bst_node *node);

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
static int __bst_rek_get_hight(Bst_node *node);

static ___inline___ Bst_node *bst_node_create(void *data, int size_of, Bst_node *parent)
{
    Bst_node *node;

    TRACE("");

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Bst_node *)malloc(sizeof(Bst_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->____data = malloc((size_t)size_of);
    if (node->____data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    __ASSIGN__(*(BYTE *)node->____data, *(BYTE *)data, size_of);

    node->____parent    = parent;
    node->____left_son  = NULL;
    node->____right_son = NULL;

    return node;
}

static ___inline___ void bst_node_destroy(Bst_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->____data);
    FREE(node);
}

static ___inline___ Bst_node *bst_successor(Bst_node *node)
{
    Bst_node *parent;

    TRACE("");

    assert(node != NULL);

    if (node->____right_son != NULL)
        return bst_min_node(node->____right_son);

    parent = node->____parent;
    while (parent != NULL && node == parent->____right_son)
    {
        node = parent;
        parent = parent->____parent;
    }

    return parent;
}

static ___inline___ Bst_node *bst_predecessor(Bst_node *node)
{
    Bst_node *parent;

    TRACE("");

    assert(node != NULL);

    if (node->____left_son != NULL)
        return bst_max_node(node->____left_son);

    parent = node->____parent;
    while (parent != NULL && node == parent->____left_son)
    {
        node = parent;
        parent = parent->____parent;
    }

    return parent;
}

static ___inline___ Bst_node *bst_node_search(Bst *tree, void *data_key)
{
    Bst_node *node;

    TRACE("");

    assert(tree != NULL);
    assert(data_key != NULL);

    node = tree->____root;

    while (node != NULL)
    {
        if (tree->____cmp(node->____data,data_key) == 0)
            return node;
        else if (tree->____cmp(node->____data,data_key) > 0)
            node = node->____left_son;
        else
            node = node->____right_son;
    }

    /* now node is NULL */
    return NULL;
}

static ___inline___ Bst_node *bst_max_node(Bst_node *node)
{
    Bst_node *parent;

    TRACE("");

    assert(node != NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = node;
        node = node->____right_son;
    }

    return parent;
}

static ___inline___ Bst_node *bst_min_node(Bst_node *node)
{
    Bst_node *parent;

    TRACE("");

    assert(node != NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = node;
        node = node->____left_son;
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

    TRACE("");

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->____left_son != NULL);

    left_son = node->____left_son;

    /* update X ptr */
    node->____left_son = left_son->____right_son;

    if( node->____left_son != NULL)
        node->____left_son->____parent = node;

    left_son->____right_son = node;
    left_son->____parent = node->____parent;
    node->____parent = left_son;

    /* update his child ptr */
    if (left_son->____parent != NULL)
    {
        if (left_son->____parent->____left_son == node)
            left_son->____parent->____left_son = left_son;
        else
            left_son->____parent->____right_son = left_son;
    }
    else
        tree->____root = left_son;
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

    TRACE("");

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->____right_son != NULL);

    right_son = node->____right_son;

    /* update X ptr */
    node->____right_son = right_son->____left_son;

    if (node->____right_son != NULL)
        node->____right_son->____parent = node;

    right_son->____left_son = node;
    right_son->____parent = node->____parent;
    node->____parent = right_son;

    /* update his child ptr */
    if (right_son->____parent != NULL)
    {
        if (right_son->____parent->____left_son == node)
            right_son->____parent->____left_son = right_son;
        else
            right_son->____parent->____right_son = right_son;
    }
    else
        tree->____root = right_son;
}

static int __bst_rek_get_hight(Bst_node *node)
{
    int left;
    int right;

    if (node == NULL)
        return 0;

    left = __bst_rek_get_hight(node->____left_son);
    right = __bst_rek_get_hight(node->____right_son);

    return MAX(left, right) + 1;
}

Bst* bst_create(int size_of, int (*cmp)(void* a,void *b))
{
    Bst *tree;

    TRACE("");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    tree = (Bst *)malloc(sizeof(Bst));
    if (tree == NULL)
       ERROR("malloc error\n", NULL, "");

    tree->____root = NULL;

    tree->____cmp = cmp;
    tree->____size_of = (size_t)size_of;

    tree->____nodes = 0;

    return tree;
}

void bst_destroy(Bst *tree)
{
    Bst_node *node;
    Bst_node *temp;

    TRACE("");

    if (tree == NULL)
        return;

    if (tree->____root == NULL)
    {
        FREE(tree);
        return;
    }

    /* Destroy tree using inorder */
    node = bst_min_node(tree->____root);
    while (node != NULL)
    {
        temp = node;
        node = bst_successor(node);
        bst_node_destroy(temp);
    }

    FREE(tree);
}

void bst_destroy_with_entries(Bst *tree, void (*destructor)(void *data))
{
    Bst_node *node;
    Bst_node *temp;

    TRACE("");

    if (tree == NULL)
        return;

    if (destructor == NULL)
        return;

    if (tree->____root == NULL)
    {
        FREE(tree);
        return;
    }

    /* Destroy tree using inorder */
    node = bst_min_node(tree->____root);
    while (node != NULL)
    {
        temp = node;
        node = bst_successor(node);
        destructor(temp->____data);
        bst_node_destroy(temp);
    }

    FREE(tree);
}

int bst_insert(Bst *tree, void *data)
{
    Bst_node *node;
    Bst_node *parent;
    Bst_node *new_node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    /* special case - empty tree */
    if (tree->____root == NULL)
    {
        node = bst_node_create(data, (int)tree->____size_of, NULL);
		if (node == NULL)
			ERROR("bst_node_create\n", 1, "");

        tree->____root = node;
    }
    else
    {
        parent = NULL;
        node = tree->____root;

        /* find correct place to insert */
        while (node != NULL)
        {
            parent = node;
            if (tree->____cmp(node->____data, data) == 0) /* data exists in tree */
            {
                return 1;
            }
            else if (tree->____cmp(node->____data, data) > 0)
                node = node->____left_son;
            else
                node = node->____right_son;
        }

        new_node = bst_node_create(data, (int)tree->____size_of, parent);
		if (new_node == NULL)
			ERROR("bst_node_create\n", 1, "");

        /* new node is the right son */
        if (tree->____cmp(new_node->____data, parent->____data) > 0)
            parent->____right_son = new_node;
        else /* new_node is the left_node */
            parent->____left_son = new_node;
    }

    ++tree->____nodes;

    return 0;
}


int bst_delete(Bst *tree, void *data_key)
{
    Bst_node *node;
    Bst_node *successor;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1, "");

    node = bst_node_search(tree, data_key);
    if (node == NULL)
        ERROR("data with this key doesn't exist in tree, nothing to delete\n", 1, "");

    /* case 1, node is leaf_node */
    if (node->____left_son == NULL && node->____right_son == NULL)
    {
        /* if have parent */
        if (node->____parent != NULL)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = NULL;
            else
                node->____parent->____right_son = NULL;
        }
        else /* is root */
            tree->____root = NULL;
    }
    else if (node->____right_son == NULL) /* case 2 node has only left subtree */
    {
        /* update ptr to parent */
        node->____left_son->____parent = node->____parent;
        if (node->____parent != NULL)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = node->____left_son;
            else
                node->____parent->____right_son = node->____left_son;
        }
        else
            tree->____root = node->____left_son;
    }
    else if (node->____left_son == NULL) /* case 3 node has only right subtree */
    {
        /* update ptr to parent */
        node->____right_son->____parent = node->____parent;
        if (node->____parent != NULL)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = node->____right_son;
            else
                node->____parent->____right_son = node->____right_son;
        }
        else
            tree->____root = node->____right_son;
    }
    else /* case 4 node has both childdren */
    {
        successor = bst_successor(node);

        /* delete successor */
        if (successor->____right_son != NULL)
            successor->____right_son->____parent = successor->____parent;

        if (successor->____parent->____left_son == successor)
            successor->____parent->____left_son = successor->____right_son;
        else
            successor->____parent->____right_son = successor->____right_son;

        /* update node children ptrs */
        successor->____left_son = node->____left_son;
        successor->____right_son = node->____right_son;

        /* successor is new parent */
        successor->____left_son->____parent = successor;
        if (successor->____right_son != NULL)
            successor->____right_son->____parent = successor;

        /* successor is in node place, so update parent ptr */
        successor->____parent = node->____parent;

        if (node->____parent != NULL)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = successor;
            else
                node->____parent->____right_son = successor;
        }
        else
            tree->____root = successor;
    }

    --tree->____nodes;
    bst_node_destroy(node);

    return 0;
}

int bst_min(Bst *tree, void *data)
{
    Bst_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (tree->____root == NULL)
        ERROR("tree is empty\n", 1, "");

    node = bst_min_node(tree->____root);
    if (node == NULL)
        ERROR("bst_min_node error\n", 1, "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->____data, tree->____size_of);

    return 0;
}

int bst_max(Bst *tree,void *data)
{
    Bst_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (tree->____root == NULL)
        ERROR("tree is empty\n", 1, "");

    node = bst_max_node(tree->____root);
    if(node == NULL)
        ERROR("bst_max_node error\n", 1 , "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->____data, tree->____size_of);

    return 0;
}

 bool bst_key_exist(Bst *tree, void *data_key)
 {
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", false, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", false, "");

    if (tree->____root == NULL)
        ERROR("tree is empty\n", false, "");

    return bst_node_search(tree, data_key) != NULL;
 }

 int bst_search(Bst *tree, void *data_key, void *data_out)
{
    Bst_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1, "");

    if (data_out == NULL)
        ERROR("data_out == NULL\n", 1, "");

    if (tree->____root == NULL)
        ERROR("tree is empty\n", 1, "");

    node = bst_node_search(tree, data_key);
    if (node == NULL)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->____data, tree->____size_of);

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

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (tree->____root == NULL)
        ERROR("tree is empty\n", 1, "");

    ptr = tree->____root;

    /* make "spine" from tree */
    while (ptr != NULL)
    {
        if (ptr->____left_son != NULL)
        {
            bst_rotate_right(tree, ptr);
            ptr = ptr->____parent;
        }
        else
            ptr = ptr->____right_son;
    }

    n = (int)tree->____nodes;

    /* count k from formula k = n + 1 - 2^(floor(LOG2(n + 1))) */
    k = n + 1 - (1 << (int)LOG2_int((unsigned int)(n + 1)));

    /* start from root and make k rotations */
    ptr = tree->____root;
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
        ptr = ptr->____parent->____right_son;
    }

    /*  we made k rotations, now rotate n - k / 2 n - k / 4 .... times */
    n -= k;

    while (n > 1)
    {
        n >>= 1;
        ptr = tree->____root;
        for (i = 0; i < n; ++i)
        {
            bst_rotate_left(tree,ptr);
            ptr = ptr->____parent->____right_son;
        }
    }

    return 0;
}

int bst_to_array(Bst *tree, void *array, size_t *size)
{
    void *t;
    BYTE *_t;

    Bst_node *node;

    size_t offset;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (array == NULL)
        ERROR("array == NULL\n", 1, "");

    if (size == NULL)
        ERROR("size == NULL\n", 1, "");

    if (tree->____root == NULL)
        ERROR("tree is empty\n", 1, "");

    t = malloc((size_t)tree->____size_of * tree->____nodes);
	if (t == NULL)
		ERROR("malloc error\n", 1, "");

    offset = 0;
    _t = (BYTE *)t;

    node = bst_min_node(tree->____root);
    while (node != NULL)
    {
        __ASSIGN__(_t[offset], *(BYTE *)node->____data, tree->____size_of);
        offset += tree->____size_of;

        node = bst_successor(node);
    }

    *(void **)array = t;
    *size = tree->____nodes;

    return 0;
}

ssize_t bst_get_num_entries(Bst *tree)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1, "");

    return (ssize_t)tree->____nodes;
}

int bst_get_data_size(Bst *tree)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", -1, "");

    return (int)tree->____size_of;
}

int bst_get_hight(Bst *tree)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", -1, "");

    if (tree->____root == NULL)
        return 0;

    return __bst_rek_get_hight(tree->____root);
}

Bst_iterator *bst_iterator_create(Bst *tree, iti_mode_t mode)
{
    Bst_iterator *iterator;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_END && mode != ITI_ROOT)
        ERROR("Incorrect node\n", NULL, "");

    iterator = (Bst_iterator *)malloc(sizeof(Bst_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    if (mode == ITI_BEGIN)
        iterator->____node = bst_min_node(tree->____root);
    else if ( mode == ITI_END)
        iterator->____node = bst_max_node(tree->____root);
    else
        iterator->____node = tree->____root;

    iterator->____size_of = tree->____size_of;

    return iterator;
}

void bst_iterator_destroy(Bst_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int bst_iterator_init(Bst *tree, Bst_iterator *iterator, iti_mode_t mode)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_END && mode != ITI_ROOT)
        ERROR("Incorrect node\n", 1, "");

    iterator->____size_of = tree->____size_of;

    if (mode == ITI_BEGIN)
        iterator->____node = bst_min_node(tree->____root);
    else if (mode == ITI_END)
        iterator->____node = bst_max_node(tree->____root);
    else
        iterator->____node = tree->____root;

    return 0;
}

int bst_iterator_next(Bst_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->____node = bst_successor(iterator->____node);

    return 0;
}

int bst_iterator_prev(Bst_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->____node = bst_predecessor(iterator->____node);

    return 0;
}

int bst_iterator_get_data(Bst_iterator *iterator, void *val)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (val == NULL)
        ERROR("iterator == NULL\n", 1, "");

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->____node->____data, iterator->____size_of);

    return 0;
}

int bst_iterator_get_node(Bst_iterator *iterator, void *node)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (node == NULL)
        ERROR("iterator == NULL\n", 1, "");

    *(void **)node = iterator->____node;

    return 0;
}

bool bst_iterator_end(Bst_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true, "");

    return iterator->____node == NULL;
}

TREE_WRAPPERS_CREATE(Bst, bst)

Tree *tree_bst_create(int size_of, int (*cmp)(void* a,void *b))
{
    Tree *tree;

    TRACE("");

    /* create Tree */
    tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL, "");

    /* create bst */
    tree->____tree = (void *)bst_create(size_of, cmp);
    if (tree->____tree == NULL)
    {
        FREE(tree);
        ERROR("bst_create error\n", NULL, "");
    }

    /* fill hooks */
    TREE_WRAPPERS_ASSIGN(tree);

    return tree;
}