#include <rbt.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <compiler.h>
#include <common.h>
#include <assert.h>

/* RBT COLORS */
#define RBT_BLACK   0
#define RBT_RED     1

#define SENTINEL_BUFFER_SIZE    16  /* provide allignment */
#define SENTINEL_VALUE          -1
#define SENTINEL_COLOR          RBT_BLACK

/* sentinel for all RBTs */
static ___unused___ char ____sentinel_buffer[SENTINEL_BUFFER_SIZE] = { SENTINEL_VALUE };

static ___unused___ Rbt_node ____sentinel =
{
    .data       = (void *)&____sentinel_buffer,
    .left_son   = &____sentinel,
    .right_son  = &____sentinel,
    .parent     = &____sentinel
};

static Rbt_node *sentinel = &____sentinel;

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
___inline___ static Rbt_node *rbt_node_create(void *data, int size_of, Rbt_node *parent);

/*
    Deallocate rbt node

    PARAMS
    @IN node - pointer to bst node

    RETURN:
    This is a void function
*/
___inline___ static void rbt_node_destroy(Rbt_node *node);


/*
    Search for node with min key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node* rbt_min_node(Rbt_node *node);

/*
    Search for node with max key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node *rbt_max_node(Rbt_node *node);

/*
    Search for node with key equals @data_key ( using cmp )

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node *rbt_node_search(Rbt *tree, void *data_key);

/*
    Get successor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node *rbt_successor(Rbt_node *node);

/*
    Get predecessor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/

___inline___ static Rbt_node *rbt_predecessor(Rbt_node *node);

/*
    Right tree rotate

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node wchich we rotate

    RETURN:
    This is a void function
*/
___inline___ static void rbt_rotate_right(Rbt *tree, Rbt_node *node);

/*
    Left tree rotate

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node wchich we rotate

    RETURN:
    This is a void function
*/
___inline___ static void rbt_rotate_left(Rbt *tree, Rbt_node *node);

/*
    Fix RBT property after insert, start fixing from node @node

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node

    RETURN:
    0 iff failure
    Non-zero value iff success
*/
static int rbt_insert_fixup(Rbt *tree, Rbt_node *node);

/*
    Fix RBT property after delete, start fixing from node @node

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node

    RETURN:
    0 iff failure
    Non-zero value iff success
*/
static int rbt_delete_fixup(Rbt *tree, Rbt_node *node);

___inline___ static Rbt_node *rbt_node_create(void *data, int size_of, Rbt_node *parent)
{
    Rbt_node *node;

    TRACE("");

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Rbt_node *)malloc(sizeof(Rbt_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->data = malloc(size_of);
    if (node->data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    node->parent = parent;
    node->left_son = sentinel;
    node->right_son = sentinel;

    node->color = RBT_RED; /* always we create red nodes */

    return node;
}

___inline___ static void rbt_node_destroy(Rbt_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->data);
    FREE(node);
}

___inline___ static Rbt_node *rbt_successor(Rbt_node *node)
{
    Rbt_node *parent;

    TRACE("");

    assert(node != NULL);
    assert(node != sentinel);

    if (node->right_son != sentinel)
        return rbt_min_node(node->right_son);

    parent = node->parent;
    while (parent != sentinel && node == parent->right_son)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

___inline___ static Rbt_node *rbt_predecessor(Rbt_node *node)
{
    Rbt_node *parent;

    TRACE("");

    assert(node != NULL);
    assert(node != sentinel);

    if (node->left_son != sentinel)
        return rbt_max_node(node->left_son);

    parent = node->parent;
    while (parent != sentinel && node == parent->left_son)
    {
        node = parent;
        parent = parent->parent;
    }
    return parent;
}

___inline___ static Rbt_node *rbt_node_search(Rbt *tree, void *data_key)
{
    Rbt_node *node;

    TRACE("");

    assert(tree != NULL);
    assert(data_key != NULL);

    node = tree->root;
    while (node != sentinel)
    {
        if (tree->cmp(node->data, data_key) == 0)
            return node;
        else if (tree->cmp(node->data, data_key) > 0)
            node = node->left_son;
        else
            node = node->right_son;
    }

    /* now node is a sentinel */
    return NULL;
}

___inline___ static Rbt_node *rbt_max_node(Rbt_node *node)
{
    Rbt_node *parent;

    TRACE("");

    assert(node != NULL);
    assert(node != sentinel);

    parent = NULL;
    while (node != sentinel)
    {
        parent = node;
        node = node->right_son;
    }

    return parent;
}

___inline___ static Rbt_node *rbt_min_node(Rbt_node *node)
{
    Rbt_node *parent;

    TRACE("");

    assert(node != NULL);
    assert(node != sentinel);

    parent = NULL;
    while (node != sentinel)
    {
        parent = node;
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
___inline___ static void rbt_rotate_right(Rbt *tree, Rbt_node *node)
{
    Rbt_node *left_son;

    TRACE("");

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->left_son != NULL);
    assert(node != sentinel);
    assert(node->left_son != sentinel);

    left_son = node->left_son;

    /* update X ptr */
    node->left_son = left_son->right_son;

    if (node->left_son != sentinel)
        node->left_son->parent = node;

    left_son->right_son = node;
    left_son->parent = node->parent;
    node->parent = left_son;

    /* update his child ptr */
    if (left_son->parent != sentinel)
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
___inline___ static void rbt_rotate_left(Rbt *tree, Rbt_node *node)
{
    Rbt_node *right_son;

    TRACE("");

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->right_son != NULL);
    assert(node != sentinel);
    assert(node->right_son != sentinel);

    right_son = node->right_son;

    /* update X ptr */
    node->right_son = right_son->left_son;

    if (node->right_son != sentinel)
        node->right_son->parent = node;

    right_son->left_son = node;
    right_son->parent = node->parent;
    node->parent = right_son;


    /* update his child ptr */
    if (right_son->parent != sentinel)
    {
        if (right_son->parent->left_son == node)
            right_son->parent->left_son = right_son;
        else
            right_son->parent->right_son = right_son;
    }
    else
        tree->root = right_son;
}

static int rbt_insert_fixup(Rbt *tree, Rbt_node *node)
{
    Rbt_node *uncle;

    TRACE("");

    assert(tree != NULL);
    assert(node != NULL);
    assert(node != sentinel);

    while (node->parent->color == RBT_RED)
        if (node->parent == node->parent->parent->left_son)
        {
            uncle = node->parent->parent->right_son;

            /* recolor */
            if (uncle->color == RBT_RED)
            {
                node->parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                node = node->parent->parent;
                node->color = RBT_RED;
            }
            else
            {
                if (node == node->parent->right_son)
                {
                    node = node->parent;
                    rbt_rotate_left(tree, node);
                }
                node->parent->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                rbt_rotate_right(tree, node->parent->parent);
            }
        }
        else
        {
            uncle = node->parent->parent->left_son;

            /* recolor */
            if (uncle->color == RBT_RED)
            {
                node->parent->color = RBT_BLACK;
                uncle->color = RBT_BLACK;
                node = node->parent->parent;
                node->color = RBT_RED;
            }
            else
            {
                if (node == node->parent->left_son)
                {
                    node = node->parent;
                    rbt_rotate_right(tree, node);
                }

                node->parent->color = RBT_BLACK;
                node->parent->parent->color = RBT_RED;
                rbt_rotate_left(tree, node->parent->parent);
            }
        }

    tree->root->color = RBT_BLACK;

    return 0;
}

static int rbt_delete_fixup(Rbt *tree, Rbt_node *node)
{
    Rbt_node *ptr;

    TRACE("");

    assert(tree != NULL);
    assert(node != NULL);
    assert(node != sentinel);

    while (node != tree->root && node->color == RBT_BLACK)
        if (node == node->parent->left_son)
        {
            ptr = node->parent->right_son;
            if (ptr->color == RBT_RED)
            {
                ptr->color = RBT_BLACK;
                node->parent->color = RBT_RED;
                rbt_rotate_left(tree, node->parent);
                ptr = node->parent->right_son;
            }

            if (ptr->left_son->color == RBT_BLACK && ptr->right_son->color == RBT_BLACK)
            {
                ptr->color = RBT_RED;
                node = node->parent;
            }
            else
            {
                if (ptr->right_son->color == RBT_BLACK)
                {
                    ptr->left_son->color = RBT_BLACK;
                    ptr->color = RBT_RED;
                    rbt_rotate_right(tree, ptr);
                    ptr = node->parent->right_son;
                }

                ptr->color = node->parent->color;

                node->parent->color = RBT_BLACK;
                ptr->right_son->color = RBT_BLACK;
                rbt_rotate_left(tree, node->parent);

                /* end fixup */
                node = tree->root;
            }
        }
        else
        {
            ptr = node->parent->left_son;

            if (ptr->color == RBT_RED)
            {
                ptr->color = RBT_BLACK;
                node->parent->color = RBT_RED;

                rbt_rotate_right(tree, node->parent);
                ptr = node->parent->left_son;
            }

            if (ptr->right_son->color == RBT_BLACK && ptr->left_son->color == RBT_BLACK)
            {
                ptr->color = RBT_RED;
                node = node->parent;
            }
            else
            {
                if (ptr->left_son->color == RBT_BLACK)
                {
                    ptr->right_son->color = RBT_BLACK;
                    ptr->color = RBT_RED;
                    rbt_rotate_left(tree, ptr);
                    ptr = node->parent->left_son;
                }

                ptr->color = node->parent->color;
                node->parent->color = RBT_BLACK;
                ptr->left_son->color = RBT_BLACK;
                rbt_rotate_right(tree, node->parent);

                /* end fixup */
                node = tree->root;
            }
        }

    node->color = RBT_BLACK;

    return 0;
}

Rbt* rbt_create(int size_of, int (*cmp)(void *a, void *b))
{
    Rbt *tree;

    TRACE("");

    assert(size_of >= 1);
    assert(cmp != NULL);

    tree = (Rbt *)malloc(sizeof(Rbt));
    if (tree == NULL)
       ERROR("malloc error\n", NULL, "");

    tree->root = sentinel;
    tree->size_of = size_of;
    tree->cmp = cmp;

    tree->nodes = 0;

    return tree;
}

void rbt_destroy(Rbt *tree)
{
    Rbt_node *node;
    Rbt_node *temp;

    TRACE("");

    assert(tree != NULL);

    /* destroy tree using inorder */
    node = rbt_min_node(tree->root);
    if (node == NULL)
        return;

    while (node != sentinel && node != NULL)
    {
        temp = node;
        node = rbt_successor(node);
        rbt_node_destroy(temp);
    }

    FREE(tree);
}

int rbt_insert(Rbt *tree, void *data)
{
    Rbt_node *node;
    Rbt_node *parent;
    Rbt_node *new_node;

    TRACE("");

    assert(tree != NULL);
    assert(data != NULL);

    /* special case - empty tree */
    if (tree->root == sentinel)
    {
        node = rbt_node_create(data, tree->size_of, sentinel);
		if (node == NULL)
			ERROR("rbt_node_create\n", 1, "");

        /* root is always black */
        node->color = RBT_BLACK;
        tree->root = node;
    }
    else
    {
         parent = sentinel;
         node = tree->root;

        /* find correct place to insert */
        while (node != sentinel)
        {
            parent = node;
            if (tree->cmp(node->data, data) == 0) /* data exists in tree */
            {
                ERROR("data with this key exist in tree\n", 1, "");
            }
            else if (tree->cmp(node->data, data) > 0)
                node = node->left_son;
            else
                node = node->right_son;
        }

        new_node = rbt_node_create(data, tree->size_of, parent);
		if (new_node == NULL)
			ERROR("rbt_node_create\n", 1, "");

        /* new node is the right son */
        if (tree->cmp(new_node->data, parent->data) > 0)
            parent->right_son = new_node;
        else /* new_node is the left_node */
            parent->left_son = new_node;

        if (rbt_insert_fixup(tree, new_node))
            ERROR("rbt_insert_fixup error\n", 1, "");
    }

    ++tree->nodes;
    return 0;
}

int rbt_delete(Rbt *tree, void *data_key)
{
    Rbt_node *node;
    Rbt_node *temp;
    Rbt_node *successor;

    RBT_COLOR color;

    TRACE("");

    assert(tree != NULL);
    assert(data_key != NULL);

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", 1, "");

    node = rbt_node_search(tree, data_key);
    if (node == NULL)
        ERROR("key doesn't exist in tree, nothing to delete\n", 1, "");

    temp = sentinel;
    color = node->color;

    /* case 1 node is a "leaf" node */
    if (node->left_son == sentinel && node->right_son == sentinel)
    {
        /* save ptr to parent ( need to fixup ) */
        temp->parent = node->parent;

        if (node->parent != sentinel)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = sentinel;
            else
                node->parent->right_son = sentinel;
        }
        else
            tree->root = sentinel;
    }
    /* case 2 node has only lef subtree */
    else if (node->right_son == sentinel)
    {
        node->left_son->parent = node->parent;

        /* we need it for fixup */
        temp = node->left_son;

        if (node->parent != sentinel)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = node->left_son;
            else
                node->parent->right_son = node->left_son;
        }
        else
            tree->root = node->left_son;
    }
    /* case 3 node has only right subtree */
    else if (node->left_son == sentinel)
    {
        node->right_son->parent = node->parent;

        /* we need it for fixup */
        temp = node->right_son;
        if (node->parent != sentinel)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = node->right_son;
            else
                node->parent->right_son = node->right_son;
        }
        else
            tree->root = node->right_son;
    }
    /* case 4 node has both children */
    else
    {
        successor = rbt_successor(node);

        /* we need it for fixup */
        temp = successor->right_son;
        color = successor->color;
        successor->color = node->color;

        /* delete successor */
        if (successor->parent == node)
            temp->parent = successor;
        else
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
        if (successor->right_son != successor)
            successor->right_son->parent = successor;

        /* successor is in node place, so update parent ptr */
        successor->parent = node->parent;

        if (node->parent != sentinel)
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

    /* if deleted node was black tree need fixup */
    if (color == RBT_BLACK)
        if (rbt_delete_fixup(tree,temp))
            ERROR("rbt_delete_fixup error\n", 1, "");

    rbt_node_destroy(node);

    return 0;
}

int rbt_min(Rbt *tree, void *data)
{
    Rbt_node *node;

    TRACE("");

    assert(tree != NULL);
    assert(data != NULL);

    node = rbt_min_node(tree->root);
    if(node == NULL)
        ERROR("rbt_min_node error\n", 1, "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

int rbt_max(Rbt *tree, void *data)
{
    Rbt_node *node;

    TRACE("");

    assert(tree != NULL);
    assert(data != NULL);

    node = rbt_max_node(tree->root);
    if(node == NULL)
        ERROR("rbt_max_node error\n", 1, "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

int rbt_search(Rbt *tree, void *data_key, void *data_out)
{
    Rbt_node *node;

    TRACE("");

    assert(tree != NULL);
    assert(data_key != NULL);
    assert(data_out != NULL);

    node = rbt_node_search(tree, data_key);
    if (node == NULL)
        ERROR("rbt_node_search error\n", 1, "");

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->data, tree->size_of);

    return 0;
}

bool rbt_key_exist(Rbt *tree, void *data_key)
{
   TRACE("");

   assert(tree != NULL);
   assert(data_key != NULL);

   return rbt_node_search(tree, data_key) != NULL;
}

int rbt_to_array(Rbt *tree, void *array, size_t *size)
{
    void *t;
    BYTE *_t;
    Rbt_node *node;
    size_t offset;

    TRACE("");

    assert(tree != NULL);
    assert(array != NULL);
    assert(size != NULL);

    t = malloc(tree->size_of * tree->nodes);
	if (t == NULL)
		ERROR("malloc error", 1, "");

    _t = (BYTE *)t;
    offset = 0;

    node = rbt_min_node(tree->root);
    if (node == NULL)
        ERROR("rbt_min_node error\n", 1, "");

    while(node != sentinel && node != NULL)
    {
        __ASSIGN__(_t[offset], *(BYTE *)node->data, tree->size_of);
        offset += tree->size_of;

        node = rbt_successor(node);
    }

    *(void **)array = t;
    *size = tree->nodes;

    return 0;
}

Rbt_iterator *rbt_iterator_create(Rbt *tree, ITI_MODE mode)
{
    Rbt_iterator *iterator;

    TRACE("");

    assert(tree != NULL);
    assert(mode == ITI_BEGIN || mode == ITI_ROOT || mode == ITI_END);

    iterator = (Rbt_iterator *)malloc(sizeof(Rbt_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    if (mode == ITI_BEGIN)
        iterator->node = rbt_min_node(tree->root);
    else if (mode == ITI_END)
        iterator->node = rbt_max_node(tree->root);
    else
        iterator->node = tree->root;

    iterator->size_of = tree->size_of;

    return iterator;
}

void rbt_iterator_destroy(Rbt_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int rbt_iterator_init(Rbt *tree, Rbt_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    assert(tree != NULL);
    assert(iterator != NULL);
    assert(mode == ITI_BEGIN || mode == ITI_ROOT || mode == ITI_END);

    iterator->size_of = tree->size_of;

    if (mode == ITI_BEGIN)
        iterator->node = rbt_min_node(tree->root);
    else if (mode == ITI_END)
        iterator->node = rbt_max_node(tree->root);
    else
        iterator->node = tree->root;

    iterator->size_of = tree->size_of;

    return 0;
}

int rbt_iterator_next(Rbt_iterator *iterator)
{
    TRACE("");

    assert(iterator != NULL);

    iterator->node = rbt_successor(iterator->node);

    return 0;
}

int rbt_iterator_prev(Rbt_iterator *iterator)
{
    TRACE("");

    assert(iterator != NULL);

    iterator->node = rbt_predecessor(iterator->node);

    return 0;
}

int rbt_iterator_get_data(Rbt_iterator *iterator, void *val)
{
    TRACE("");

    assert(iterator != NULL);
    assert(val != NULL);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

bool rbt_iterator_end(Rbt_iterator *iterator)
{
    TRACE("");

    assert(iterator != NULL);

    return iterator->node == sentinel || iterator->node == NULL;
}
