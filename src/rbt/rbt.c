#include <rbt.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <compiler.h>
#include <common.h>
#include <assert.h>

typedef char rbt_color_t;

struct Rbt_node
{
    struct Rbt_node     *left_son;
    struct Rbt_node     *right_son;
    struct Rbt_node     *parent;
    rbt_color_t         color;
    size_t              size_of;
    __extension__ BYTE  data[]; /* placeholder for data */
};

struct Rbt_iterator
{
    size_t      size_of;
    Rbt_node    *node;
};

struct Rbt
{
    Rbt_node    *root;
    size_t      nodes;
    size_t      size_of;

    cmp_f cmp;
    destructor_f destroy;
};

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
    .left_son   = (Rbt_node *)&____sentinel,
    .right_son  = (Rbt_node *)&____sentinel,
    .parent     = (Rbt_node *)&____sentinel,
    .color      = RBT_BLACK,
    .data       = {PASTE_16(SENTINEL_VALUE)}
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
___inline___ static Rbt_node *rbt_node_create(const void * ___restrict___ data, size_t size_of, const Rbt_node * ___restrict___ parent);

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
___inline___ static Rbt_node *rbt_node_search(const Rbt * ___restrict___ tree, const void * ___restrict___ data_key);

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
static int __rbt_delete(Rbt * ___restrict___ tree, const void * ___restrict___ data_key, bool destroy);

___inline___ static Rbt_node *rbt_node_create(const void * ___restrict___ data, size_t size_of, const Rbt_node * ___restrict___ parent)
{
    Rbt_node *node;

    TRACE();

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Rbt_node *)malloc(sizeof(Rbt_node) + size_of);
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    node->parent = (Rbt_node *)parent;
    node->left_son = (Rbt_node *)sentinel;
    node->right_son = (Rbt_node *)sentinel;
    node->size_of = size_of;

    node->color = RBT_RED; /* always we create red nodes */

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

    if (node->right_son != sentinel)
        return rbt_min_node(node->right_son);

    parent = node->parent;
    while (parent != sentinel && node == parent->right_son)
    {
        node = (Rbt_node *)parent;
        parent = parent->parent;
    }

    return parent;
}

___inline___ static Rbt_node *rbt_predecessor(const Rbt_node *node)
{
    Rbt_node *parent;

    TRACE();

    assert(node != NULL);
    assert(node != sentinel);

    if (node->left_son != sentinel)
        return rbt_max_node(node->left_son);

    parent = node->parent;
    while (parent != sentinel && node == parent->left_son)
    {
        node = (Rbt_node *)parent;
        parent = parent->parent;
    }
    return parent;
}

___inline___ static Rbt_node *rbt_node_search(const Rbt * ___restrict___ tree, const void * ___restrict___ data_key)
{
    Rbt_node *node;

    TRACE();

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
        node = node->right_son;
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

    TRACE();

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

    TRACE();

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

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);

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

    TRACE();

    assert(tree != NULL);
    assert(node != NULL);

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

static int __rbt_rek_get_hight(const Rbt_node *node)
{
    int left;
    int right;

    if (node == sentinel)
        return 0;

    left = __rbt_rek_get_hight(node->left_son);
    right = __rbt_rek_get_hight(node->right_son);

    return MAX(left, right) + 1;
}

static void __rbt_destroy(Rbt *tree, bool destroy)
{
    Rbt_node *node;
    Rbt_node *temp;
    Rbt_node **nodes;
    size_t i = 0;

    TRACE();

    if (tree == NULL)
        return;

    if (tree->root == NULL || tree->root == sentinel)
    {
        FREE(tree);
        return;
    }

    nodes = malloc(sizeof(Rbt_node *) * tree->nodes);
    if (nodes == NULL)
    {
        LOG("Malloc error\n");
        return;
    }

    /* destroy tree using inorder */
    node = rbt_min_node(tree->root);
    while (node != sentinel && node != NULL)
    {
        temp = node;
        node = rbt_successor(node);

        if (destroy && tree->destroy)
            tree->destroy((void *)temp->data);

        nodes[i++] = temp;
    }

    for (i = 0; i < tree->nodes; ++i)
        rbt_node_destroy(nodes[i]);

    FREE(nodes);
    FREE(tree);
}

static int __rbt_delete(Rbt * ___restrict___ tree, const void * ___restrict___ data_key, bool destroy)
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

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_node_search(tree, data_key);
    if (node == NULL)
        ERROR("key doesn't exist in tree, nothing to delete\n", 1);

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
            ERROR("rbt_delete_fixup error\n", 1);

    if (destroy && tree->destroy != NULL)
        tree->destroy((void *)node->data);

    rbt_node_destroy(node);

    return 0;
}

Rbt* rbt_create(size_t size_of, cmp_f cmp, destructor_f destroy)
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

    tree->root = sentinel;
    tree->size_of = size_of;
    tree->cmp = cmp;
    tree->destroy = destroy;

    tree->nodes = 0;

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

int rbt_insert(Rbt * ___restrict___ tree, const void * ___restrict___ data)
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
    if (tree->root == sentinel)
    {
        node = rbt_node_create(data, tree->size_of, sentinel);
		if (node == NULL)
			ERROR("rbt_node_create\n", 1);

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
                return 1;
            }
            else if (tree->cmp(node->data, data) > 0)
                node = node->left_son;
            else
                node = node->right_son;
        }

        new_node = rbt_node_create(data, tree->size_of, parent);
		if (new_node == NULL)
			ERROR("rbt_node_create\n", 1);

        /* new node is the right son */
        if (tree->cmp(new_node->data, parent->data) > 0)
            parent->right_son = new_node;
        else /* new_node is the left_node */
            parent->left_son = new_node;

        if (rbt_insert_fixup(tree, new_node))
            ERROR("rbt_insert_fixup error\n", 1);
    }

    ++tree->nodes;
    return 0;
}

int rbt_delete(Rbt * ___restrict___ tree, const void * ___restrict___ data_key)
{
    TRACE();

    return __rbt_delete(tree, data_key, false);
}

int rbt_delete_with_entry(Rbt * ___restrict___ tree, const void * ___restrict___ data_key)
{
    TRACE();

    return __rbt_delete(tree, data_key, true);
}

int rbt_min(const Rbt * ___restrict___ tree, void * ___restrict___ data)
{
    Rbt_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_min_node(tree->root);
    if(node == NULL)
        ERROR("rbt_min_node error\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

int rbt_max(const Rbt * ___restrict___ tree, void * ___restrict___ data)
{
    Rbt_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_max_node(tree->root);
    if(node == NULL)
        ERROR("rbt_max_node error\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

int rbt_search(const Rbt * ___restrict___ tree, const void *data_key, void *data_out)
{
    Rbt_node *node;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1);

    if (data_out == NULL)
        ERROR("data_out == NULL\n", 1);

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", 1);

    node = rbt_node_search(tree, data_key);
    if (node == NULL)
        return 1;

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->data, tree->size_of);

    return 0;
}

bool rbt_key_exist(const Rbt * ___restrict___ tree, const void * ___restrict___ data_key)
{
   TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", false);

    if (data_key == NULL)
        ERROR("data_key == NULL\n", false);

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", false);

   return rbt_node_search(tree, data_key) != NULL;
}

int rbt_to_array(const Rbt * ___restrict___ tree, void * ___restrict___ array, size_t * ___restrict___ size)
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

    if (tree->root == sentinel)
        ERROR("Empty Tree\n", 1);

    t = malloc(tree->size_of * tree->nodes);
	if (t == NULL)
		ERROR("malloc error\n", 1);

    _t = (BYTE *)t;
    offset = 0;

    node = rbt_min_node(tree->root);
    if (node == NULL)
        ERROR("rbt_min_node error\n", 1);

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

static int rbt_balance(Rbt *tree)
{
    TRACE();
    LOG("RBT is self balanced, balance no needed\n");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1);

    if (tree->root == sentinel)
        ERROR("Tree is empty\n", 1);

    return 0;
}

ssize_t rbt_get_num_entries(const Rbt *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1);

    return (ssize_t)tree->nodes;
}

ssize_t rbt_get_data_size(const Rbt *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    return (ssize_t)tree->size_of;
}

int rbt_get_hight(const Rbt *tree)
{
    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", -1);

    if (tree->root == sentinel)
        return 0;

    return __rbt_rek_get_hight(tree->root);
}

int rbt_node_get_data(const Rbt_node * ___restrict___ node, void * ___restrict___ data)
{
    TRACE();

    if (node == NULL || data == NULL)
        ERROR("node == NULL || data == NULL\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, node->size_of);

    return 0;
}

Rbt_iterator *rbt_iterator_create(const Rbt *tree, iti_mode_t mode)
{
    Rbt_iterator *iterator;

    TRACE();

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_ROOT && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    if (tree->root == NULL || tree->root == sentinel)
        return NULL;

    iterator = (Rbt_iterator *)malloc(sizeof(Rbt_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

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
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int rbt_iterator_next(Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = rbt_successor(iterator->node);

    return 0;
}

int rbt_iterator_prev(Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = rbt_predecessor(iterator->node);

    return 0;
}

int rbt_iterator_get_data(const Rbt_iterator * ___restrict___ iterator, void * ___restrict___ val)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (val == NULL)
        ERROR("val == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

int rbt_iterator_get_node(const Rbt_iterator * ___restrict___ iterator, void * ___restrict___ node)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (node == NULL)
        ERROR("node == NULL\n", 1);

    *(void **)node = iterator->node;

    return 0;
}

bool rbt_iterator_end(const Rbt_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return iterator->node == sentinel || iterator->node == NULL;
}

TREE_WRAPPERS_CREATE(Rbt, rbt)

Tree *tree_rbt_create(size_t size_of, cmp_f cmp, destructor_f destroy)
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