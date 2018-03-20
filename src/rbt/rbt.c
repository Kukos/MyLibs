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
#define SENTINEL_VALUE          (BYTE)-1
#define SENTINEL_COLOR          RBT_BLACK

#define PASTE_2(x)  x, x
#define PASTE_4(x)  PASTE_2(x), PASTE_2(x)
#define PASTE_8(x)  PASTE_4(x), PASTE_4(x)
#define PASTE_16(x) PASTE_8(x), PASTE_8(x)

 __extension__ static ___unused___ Rbt_node ____sentinel =
{
    .____left_son   = (Rbt_node *)&____sentinel,
    .____right_son  = (Rbt_node *)&____sentinel,
    .____parent     = (Rbt_node *)&____sentinel,
    .____color      = RBT_BLACK,
    .____data       = {PASTE_16(SENTINEL_VALUE)}
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
___inline___ static Rbt_node *rbt_node_create(const void *data, int size_of, const Rbt_node *parent);

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
___inline___ static Rbt_node* rbt_min_node(const Rbt_node *node);

/*
    Search for node with max key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node *rbt_max_node(const Rbt_node *node);

/*
    Search for node with key equals @data_key ( using cmp )

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node *rbt_node_search(const Rbt *tree, const void *data_key);

/*
    Get successor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Rbt_node *rbt_successor(const Rbt_node *node);

/*
    Get predecessor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/

___inline___ static Rbt_node *rbt_predecessor(const Rbt_node *node);

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

/*
    Do nothing

    Needed by Tree Framework
*/
static int rbt_balance(Rbt *tree);

/*
    Recursive helper for rbt_get_hight

    PARAMS
    @IN node - rbt node

    RETURN
    hight of subtree with root @node
*/
static int __rbt_rek_get_hight(const Rbt_node *node);

/*
    Destroy whole RBT

    PARAMS
    @IN tree - ptr to Rbt
    @IN destroy - call destructor ?

    RETURN
    This is a void function
*/
static void __rbt_destroy(Rbt *tree, bool destroy);

/*
    Delete entry with key equal to @data_key

    PARAMS
    @IN tree - ptr to Rbt
    @IN data_key - fake data with real key
    @IN destroy - call destructor ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __rbt_delete(Rbt *tree, const void *data_key, bool destroy);

___inline___ static Rbt_node *rbt_node_create(const void *data, int size_of, const Rbt_node *parent)
{
    Rbt_node *node;

    TRACE();

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Rbt_node *)malloc(sizeof(Rbt_node) + (size_t)size_of);
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    __ASSIGN__(*(BYTE *)node->____data, *(BYTE *)data, size_of);

    node->____parent = (Rbt_node *)parent;
    node->____left_son = (Rbt_node *)sentinel;
    node->____right_son = (Rbt_node *)sentinel;

    node->____color = RBT_RED; /* always we create red nodes */

    return node;
}

___inline___ static void rbt_node_destroy(Rbt_node *node)
{
    TRACE();

    if (node == NULL)
        return;

    FREE(node);
}

___inline___ static Rbt_node *rbt_successor(const Rbt_node *node)
{
    Rbt_node *parent;

    TRACE();

    assert(node != NULL);
    assert(node != sentinel);

    if (node->____right_son != sentinel)
        return rbt_min_node(node->____right_son);

    parent = node->____parent;
    while (parent != sentinel && node == parent->____right_son)
    {
        node = (Rbt_node *)parent;
        parent = parent->____parent;
    }

    return parent;
}

___inline___ static Rbt_node *rbt_predecessor(const Rbt_node *node)
{
    Rbt_node *parent;

    TRACE();

    assert(node != NULL);
    assert(node != sentinel);

    if (node->____left_son != sentinel)
        return rbt_max_node(node->____left_son);

    parent = node->____parent;
    while (parent != sentinel && node == parent->____left_son)
    {
        node = (Rbt_node *)parent;
        parent = parent->____parent;
    }
    return parent;
}

___inline___ static Rbt_node *rbt_node_search(const Rbt *tree, const void *data_key)
{
    Rbt_node *node;

    TRACE();

    assert(tree != NULL);
    assert(data_key != NULL);

    node = tree->____root;
    while (node != sentinel)
    {
        if (tree->____cmp(node->____data, data_key) == 0)
            return node;
        else if (tree->____cmp(node->____data, data_key) > 0)
            node = node->____left_son;
        else
            node = node->____right_son;
    }

    /* now node is a sentinel */
    return NULL;
}

___inline___ static Rbt_node *rbt_max_node(const Rbt_node *node)
{
    Rbt_node *parent;

    TRACE();

    assert(node != NULL);
    assert(node != sentinel);

    parent = NULL;
    while (node != sentinel)
    {
        parent = (Rbt_node *)node;
        node = node->____right_son;
    }

    return parent;
}

___inline___ static Rbt_node *rbt_min_node(const Rbt_node *node)
{
    Rbt_node *parent;

    TRACE();

    assert(node != NULL);
    assert(node != sentinel);

    parent = NULL;
    while (node != sentinel)
    {
        parent = (Rbt_node *)node;
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
___inline___ static void rbt_rotate_right(Rbt *tree, Rbt_node *node)
{
    Rbt_node *left_son;

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->____left_son != NULL);
    assert(node != sentinel);
    assert(node->____left_son != sentinel);

    left_son = node->____left_son;

    /* update X ptr */
    node->____left_son = left_son->____right_son;

    if (node->____left_son != sentinel)
        node->____left_son->____parent = node;

    left_son->____right_son = node;
    left_son->____parent = node->____parent;
    node->____parent = left_son;

    /* update his child ptr */
    if (left_son->____parent != sentinel)
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
___inline___ static void rbt_rotate_left(Rbt *tree, Rbt_node *node)
{
    Rbt_node *right_son;

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);
    assert(node->____right_son != NULL);
    assert(node != sentinel);
    assert(node->____right_son != sentinel);

    right_son = node->____right_son;

    /* update X ptr */
    node->____right_son = right_son->____left_son;

    if (node->____right_son != sentinel)
        node->____right_son->____parent = node;

    right_son->____left_son = node;
    right_son->____parent = node->____parent;
    node->____parent = right_son;


    /* update his child ptr */
    if (right_son->____parent != sentinel)
    {
        if (right_son->____parent->____left_son == node)
            right_son->____parent->____left_son = right_son;
        else
            right_son->____parent->____right_son = right_son;
    }
    else
        tree->____root = right_son;
}

static int rbt_insert_fixup(Rbt *tree, Rbt_node *node)
{
    Rbt_node *uncle;

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);

    while (node->____parent->____color == RBT_RED)
        if (node->____parent == node->____parent->____parent->____left_son)
        {
            uncle = node->____parent->____parent->____right_son;

            /* recolor */
            if (uncle->____color == RBT_RED)
            {
                node->____parent->____color = RBT_BLACK;
                uncle->____color = RBT_BLACK;
                node = node->____parent->____parent;
                node->____color = RBT_RED;
            }
            else
            {
                if (node == node->____parent->____right_son)
                {
                    node = node->____parent;
                    rbt_rotate_left(tree, node);
                }
                node->____parent->____color = RBT_BLACK;
                node->____parent->____parent->____color = RBT_RED;
                rbt_rotate_right(tree, node->____parent->____parent);
            }
        }
        else
        {
            uncle = node->____parent->____parent->____left_son;

            /* recolor */
            if (uncle->____color == RBT_RED)
            {
                node->____parent->____color = RBT_BLACK;
                uncle->____color = RBT_BLACK;
                node = node->____parent->____parent;
                node->____color = RBT_RED;
            }
            else
            {
                if (node == node->____parent->____left_son)
                {
                    node = node->____parent;
                    rbt_rotate_right(tree, node);
                }

                node->____parent->____color = RBT_BLACK;
                node->____parent->____parent->____color = RBT_RED;
                rbt_rotate_left(tree, node->____parent->____parent);
            }
        }

    tree->____root->____color = RBT_BLACK;

    return 0;
}

static int rbt_delete_fixup(Rbt *tree, Rbt_node *node)
{
    Rbt_node *ptr;

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);

    while (node != tree->____root && node->____color == RBT_BLACK)
        if (node == node->____parent->____left_son)
        {
            ptr = node->____parent->____right_son;
            if (ptr->____color == RBT_RED)
            {
                ptr->____color = RBT_BLACK;
                node->____parent->____color = RBT_RED;
                rbt_rotate_left(tree, node->____parent);
                ptr = node->____parent->____right_son;
            }

            if (ptr->____left_son->____color == RBT_BLACK && ptr->____right_son->____color == RBT_BLACK)
            {
                ptr->____color = RBT_RED;
                node = node->____parent;
            }
            else
            {
                if (ptr->____right_son->____color == RBT_BLACK)
                {
                    ptr->____left_son->____color = RBT_BLACK;
                    ptr->____color = RBT_RED;
                    rbt_rotate_right(tree, ptr);
                    ptr = node->____parent->____right_son;
                }

                ptr->____color = node->____parent->____color;

                node->____parent->____color = RBT_BLACK;
                ptr->____right_son->____color = RBT_BLACK;
                rbt_rotate_left(tree, node->____parent);

                /* end fixup */
                node = tree->____root;
            }
        }
        else
        {
            ptr = node->____parent->____left_son;

            if (ptr->____color == RBT_RED)
            {
                ptr->____color = RBT_BLACK;
                node->____parent->____color = RBT_RED;

                rbt_rotate_right(tree, node->____parent);
                ptr = node->____parent->____left_son;
            }

            if (ptr->____right_son->____color == RBT_BLACK && ptr->____left_son->____color == RBT_BLACK)
            {
                ptr->____color = RBT_RED;
                node = node->____parent;
            }
            else
            {
                if (ptr->____left_son->____color == RBT_BLACK)
                {
                    ptr->____right_son->____color = RBT_BLACK;
                    ptr->____color = RBT_RED;
                    rbt_rotate_left(tree, ptr);
                    ptr = node->____parent->____left_son;
                }

                ptr->____color = node->____parent->____color;
                node->____parent->____color = RBT_BLACK;
                ptr->____left_son->____color = RBT_BLACK;
                rbt_rotate_right(tree, node->____parent);

                /* end fixup */
                node = tree->____root;
            }
        }

    node->____color = RBT_BLACK;

    return 0;
}

static int __rbt_rek_get_hight(const Rbt_node *node)
{
    int left;
    int right;

    if (node == sentinel)
        return 0;

    left = __rbt_rek_get_hight(node->____left_son);
    right = __rbt_rek_get_hight(node->____right_son);

    return MAX(left, right) + 1;
}

static void __rbt_destroy(Rbt *tree, bool destroy)
{
    Rbt_node *node;
    Rbt_node *temp;

    TRACE();

    if (tree == NULL)
        return;

    if (tree->____root == NULL || tree->____root == sentinel)
    {
        FREE(tree);
        return;
    }

    /* destroy tree using inorder */
    node = rbt_min_node(tree->____root);
    while (node != sentinel && node != NULL)
    {
        temp = node;
        node = rbt_successor(node);

        if (destroy && tree->____destroy)
            tree->____destroy((void *)temp->____data);
        
        rbt_node_destroy(temp);
    }

    FREE(tree);
}

static int __rbt_delete(Rbt *tree, const void *data_key, bool destroy)
{
    Rbt_node *node;
    Rbt_node *temp;
    Rbt_node *successor;

    rbt_color_t color;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (tree->____root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_node_search(tree, data_key);
    if (node == NULL)
        ERROR("key doesn't exist in tree, nothing to delete\n", 1);

    temp = sentinel;
    color = node->____color;

    /* case 1 node is a "leaf" node */
    if (node->____left_son == sentinel && node->____right_son == sentinel)
    {
        /* save ptr to parent ( need to fixup ) */
        temp->____parent = node->____parent;

        if (node->____parent != sentinel)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = sentinel;
            else
                node->____parent->____right_son = sentinel;
        }
        else
            tree->____root = sentinel;
    }
    /* case 2 node has only lef subtree */
    else if (node->____right_son == sentinel)
    {
        node->____left_son->____parent = node->____parent;

        /* we need it for fixup */
        temp = node->____left_son;

        if (node->____parent != sentinel)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = node->____left_son;
            else
                node->____parent->____right_son = node->____left_son;
        }
        else
            tree->____root = node->____left_son;
    }
    /* case 3 node has only right subtree */
    else if (node->____left_son == sentinel)
    {
        node->____right_son->____parent = node->____parent;

        /* we need it for fixup */
        temp = node->____right_son;
        if (node->____parent != sentinel)
        {
            if (node->____parent->____left_son == node)
                node->____parent->____left_son = node->____right_son;
            else
                node->____parent->____right_son = node->____right_son;
        }
        else
            tree->____root = node->____right_son;
    }
    /* case 4 node has both children */
    else
    {
        successor = rbt_successor(node);

        /* we need it for fixup */
        temp = successor->____right_son;
        color = successor->____color;
        successor->____color = node->____color;

        /* delete successor */
        if (successor->____parent == node)
            temp->____parent = successor;
        else
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
        if (successor->____right_son != successor)
            successor->____right_son->____parent = successor;

        /* successor is in node place, so update parent ptr */
        successor->____parent = node->____parent;

        if (node->____parent != sentinel)
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

    /* if deleted node was black tree need fixup */
    if (color == RBT_BLACK)
        if (rbt_delete_fixup(tree,temp))
            ERROR("rbt_delete_fixup error\n", 1);

    if (destroy && tree->____destroy != NULL)
        tree->____destroy((void *)node->____data);
        
    rbt_node_destroy(node);

    return 0;
}

Rbt* rbt_create(int size_of, int (*cmp)(const void *a, const void *b), void (*destroy)(void *entry))
{
    Rbt *tree;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    tree = (Rbt *)malloc(sizeof(Rbt));
    if (tree == NULL)
        ERROR("malloc error\n", NULL);

    tree->____root = sentinel;
    tree->____size_of = (size_t)size_of;
    tree->____cmp = cmp;
    tree->____destroy = destroy;

    tree->____nodes = 0;

    return tree;
}

void rbt_destroy(Rbt *tree)
{
    TRACE();

    __rbt_destroy(tree, false);
}

void rbt_destroy_with_entries(Rbt *tree)
{
    TRACE();

    __rbt_destroy(tree, true);
}

int rbt_insert(Rbt *tree, const void *data)
{
    Rbt_node *node;
    Rbt_node *parent;
    Rbt_node *new_node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    /* special case - empty tree */
    if (tree->____root == sentinel)
    {
        node = rbt_node_create(data, (int)tree->____size_of, sentinel);
		if (node == NULL)
			ERROR("rbt_node_create\n", 1);

        /* root is always black */
        node->____color = RBT_BLACK;
        tree->____root = node;
    }
    else
    {
         parent = sentinel;
         node = tree->____root;

        /* find correct place to insert */
        while (node != sentinel)
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

        new_node = rbt_node_create(data, (int)tree->____size_of, parent);
		if (new_node == NULL)
			ERROR("rbt_node_create\n", 1);

        /* new node is the right son */
        if (tree->____cmp(new_node->____data, parent->____data) > 0)
            parent->____right_son = new_node;
        else /* new_node is the left_node */
            parent->____left_son = new_node;

        if (rbt_insert_fixup(tree, new_node))
            ERROR("rbt_insert_fixup error\n", 1);
    }

    ++tree->____nodes;
    return 0;
}

int rbt_delete(Rbt *tree, const void *data_key)
{
    TRACE();

    return __rbt_delete(tree, data_key, false);
}

int rbt_delete_with_entry(Rbt *tree, const void *data_key)
{
    TRACE();

    return __rbt_delete(tree, data_key, true);
}

int rbt_min(const Rbt *tree, void *data)
{
    Rbt_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (tree->____root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_min_node(tree->____root);
    if(node == NULL)
        ERROR("rbt_min_node error\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->____data, tree->____size_of);

    return 0;
}

int rbt_max(const Rbt *tree, void *data)
{
    Rbt_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (tree->____root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_max_node(tree->____root);
    if(node == NULL)
        ERROR("rbt_max_node error\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->____data, tree->____size_of);

    return 0;
}

int rbt_search(const Rbt *tree, const void *data_key, void *data_out)
{
    Rbt_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (data_out == NULL)
        ERROR("data_out == NULL\n", 1);

    if (tree->____root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_node_search(tree, data_key);
    if (node == NULL)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->____data, tree->____size_of);

    return 0;
}

bool rbt_key_exist(const Rbt *tree, const void *data_key)
{
   TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", false);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", false);

    if (tree->____root == sentinel)
        ERROR("Empty Tree\n", false);

   return rbt_node_search(tree, data_key) != NULL;
}

int rbt_to_array(const Rbt *tree, void *array, size_t *size)
{
    void *t;
    BYTE *_t;
    Rbt_node *node;
    size_t offset;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (size == NULL)
        ERROR("size == NULL\n", 1);

    if (tree->____root == sentinel)
        ERROR("Empty Tree\n", 1);

    t = malloc(tree->____size_of * tree->____nodes);
	if (t == NULL)
		ERROR("malloc error\n", 1);

    _t = (BYTE *)t;
    offset = 0;

    node = rbt_min_node(tree->____root);
    if (node == NULL)
        ERROR("rbt_min_node error\n", 1);

    while(node != sentinel && node != NULL)
    {
        __ASSIGN__(_t[offset], *(BYTE *)node->____data, tree->____size_of);
        offset += tree->____size_of;

        node = rbt_successor(node);
    }

    *(void **)array = t;
    *size = tree->____nodes;

    return 0;
}

static int rbt_balance(Rbt *tree)
{
    TRACE();
    LOG("RBT is self balanced, balance no needed\n");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (tree->____root == sentinel)
        ERROR("Tree is empty\n", 1);

    return 0;
}

ssize_t rbt_get_num_entries(const Rbt *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1);

    return (ssize_t)tree->____nodes;
}

int rbt_get_data_size(const Rbt *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    return (int)tree->____size_of;
}

int rbt_get_hight(const Rbt *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (tree->____root == sentinel)
        return 0;

    return __rbt_rek_get_hight(tree->____root);
}

Rbt_iterator *rbt_iterator_create(const Rbt *tree, iti_mode_t mode)
{
    Rbt_iterator *iterator;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_ROOT && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    if (tree->____root == NULL || tree->____root == sentinel)
        return NULL;

    iterator = (Rbt_iterator *)malloc(sizeof(Rbt_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    if (mode == ITI_BEGIN)
        iterator->____node = rbt_min_node(tree->____root);
    else if (mode == ITI_END)
        iterator->____node = rbt_max_node(tree->____root);
    else
        iterator->____node = tree->____root;

    iterator->____size_of = tree->____size_of;

    return iterator;
}

void rbt_iterator_destroy(Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int rbt_iterator_init(const Rbt *tree, Rbt_iterator *iterator, iti_mode_t mode)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (mode != ITI_BEGIN && mode != ITI_ROOT && mode != ITI_END)
        ERROR("Incorrect mode\n", 1);

    if (tree->____root == NULL || tree->____root == sentinel)
        return 1;

    iterator->____size_of = tree->____size_of;

    if (mode == ITI_BEGIN)
        iterator->____node = rbt_min_node(tree->____root);
    else if (mode == ITI_END)
        iterator->____node = rbt_max_node(tree->____root);
    else
        iterator->____node = tree->____root;

    iterator->____size_of = tree->____size_of;

    return 0;
}

int rbt_iterator_next(Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->____node = rbt_successor(iterator->____node);

    return 0;
}

int rbt_iterator_prev(Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->____node = rbt_predecessor(iterator->____node);

    return 0;
}

int rbt_iterator_get_data(const Rbt_iterator *iterator, void *val)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (val == NULL)
        ERROR("val == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->____node->____data, iterator->____size_of);

    return 0;
}

int rbt_iterator_get_node(const Rbt_iterator *iterator, void *node)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (node == NULL)
        ERROR("node == NULL\n", 1);

    *(void **)node = iterator->____node;

    return 0;
}

bool rbt_iterator_end(const Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return iterator->____node == sentinel || iterator->____node == NULL;
}

TREE_WRAPPERS_CREATE(Rbt, rbt)

Tree *tree_rbt_create(int size_of, int (*cmp)(const void *a, const void *b), void (*destroy)(void *entry))
{
    Tree *tree;

    TRACE();

    /* create Tree */
    tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL);

    /* create rbt */
    tree->____tree = (void *)rbt_create(size_of, cmp, destroy);
    if (tree->____tree == NULL)
    {
        FREE(tree);
        ERROR("rbt_create error\n", NULL);
    }

    /* fill hooks */
    TREE_WRAPPERS_ASSIGN(tree);

    return tree;
}