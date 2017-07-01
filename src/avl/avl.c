#include <avl.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define BALANCED        0
#define LEFT_BIGGER     1
#define RIGHT_BIGGER    2

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
___inline___ static Avl_node *avl_node_create(void *data, int size_of, Avl_node *parent);

/*
    Deallocate Avl node

    PARAMS
    @IN node - pointer to bst node

    RETURN:
    This is a void function
*/
___inline___ static void avl_node_destroy(Avl_node *node);


/*
    Search for node with min key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Avl_node* avl_min_node(Avl_node *node);

/*
    Search for node with max key

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Avl_node *avl_max_node(Avl_node *node);

/*
    Search for node with key equals @data_key ( using cmp )

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Avl_node *avl_node_search(Avl *tree, void *data_key);

/*
    Get successor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/
___inline___ static Avl_node *avl_successor(Avl_node *node);

/*
    Get predecessor of node

    PARAMS
    @IN node - pointer to node

    RETURN:
    NULL iff failure
    Pointer to node iff success
*/

___inline___ static Avl_node *avl_predecessor(Avl_node *node);

/*
    Double Left Roation, start from @node

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node

    RETURN:
    This is a void function
*/
___inline___ static void avl_rotate_ll(Avl *tree, Avl_node *node);

/*
    Double Right Roation, start from @node

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node

    RETURN:
    This is a void function
*/
___inline___ static void avl_rotate_rr(Avl *tree, Avl_node *node);


/*
     Left AND Right Rotation, start from @node

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node

    RETURN:
    This is a void function
*/
___inline___ static void avl_rotate_lr(Avl *tree, Avl_node *node);


/*
    Rght AND Left Roation, start from @node

    PARAMS
    @IN tree - pointer to tree
    @IN node - pointer to node

    RETURN:
    This is a void function
*/
___inline___ static void avl_rotate_rl(Avl *tree, Avl_node *node);

/*
    Fix AVL Properties after insert, start from new_node

    PARAMS
    @IN tree - pointer to tree
    @IN new_node - pointer to new_node

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
static int avl_insert_fixup(Avl *tree, Avl_node *new_node);

/*
    Fix AVL Properties after insert, start from node

    PARAMS
    @IN tree - pointer to tree
    @IN parent - parent of deleted node
    @IN node - pointer to node

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
static int avl_delete_fixup(Avl *tree, Avl_node *parent, Avl_node *node);

/*
    Do nothing

    Needed by Tree Framework
*/
static int avl_balance(Avl *tree);

static int avl_balance(Avl *tree)
{
    TRACE("");
    LOG("AVL is self balanced, balance no needed\n", "");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (tree->root == NULL)
        ERROR("Tree is empty\n", 1, "");

    return 0;
}

___inline___ static Avl_node *avl_node_create(void *data, int size_of, Avl_node *parent)
{
    Avl_node *node;

    TRACE("");

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Avl_node *)malloc(sizeof(Avl_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->data = malloc((size_t)size_of);
    if (node->data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    node->parent = parent;
    node->left_son = NULL;
    node->right_son = NULL;

    node->bf = BALANCED;

    return node;
}

___inline___ static void avl_node_destroy(Avl_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->data);
    FREE(node);
}

___inline___ static Avl_node *avl_successor(Avl_node *node)
{
    Avl_node *parent;

    TRACE("");

    assert(node != NULL);

    if (node->right_son != NULL)
        return avl_min_node(node->right_son);

    parent = node->parent;
    while (parent != NULL && node == parent->right_son)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

___inline___ static Avl_node *avl_predecessor(Avl_node *node)
{
    Avl_node *parent;

    TRACE("");

    assert(node != NULL);

    if (node->left_son != NULL)
        return avl_max_node(node->left_son);

    parent = node->parent;
    while (parent != NULL && node == parent->left_son)
    {
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

___inline___ static Avl_node *avl_node_search(Avl *tree, void *data_key)
{
    Avl_node *node;

    TRACE("");

    assert(tree != NULL);
    assert(data_key != NULL);

    node = tree->root;
    while (node != NULL)
    {
        if (tree->cmp(node->data, data_key) == 0)
            return node;
        else if (tree->cmp(node->data, data_key) > 0)
            node = node->left_son;
        else
            node = node->right_son;
    }

    /* now node is NULL */
    return NULL;
}

___inline___ static Avl_node *avl_max_node(Avl_node *node)
{
    Avl_node *parent;

    TRACE("");

    assert(node != NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = node;
        node = node->right_son;
    }

    return parent;
}

___inline___ static Avl_node *avl_min_node(Avl_node *node)
{
    Avl_node *parent;

    TRACE("");

    assert(node != NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = node;
        node = node->left_son;
    }

    return parent;
}


/*
        parent                              parent
           |                                   |
          node         ---->                   X
        /      \                             /   \
    node_left   X                          node   XR
              /   \                       /     \
             XL    XR                node_left    XL

*/
___inline___ static void avl_rotate_rr(Avl *tree, Avl_node *node)
{
    Avl_node *x;

    TRACE("");

    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(node != NULL);
    assert(node->right_son != NULL);

    x = node->right_son;

    node->right_son = x->left_son;
    if (node->right_son != NULL)
        node->right_son->parent = node;

    x->left_son = node;
    x->parent = node->parent;
    node->parent = x;

    if (x->parent == NULL)
        tree->root = x;
    else
    {
        if (x->parent->left_son == node)
            x->parent->left_son = x;
        else
            x->parent->right_son = x;
    }

    if (x->bf == RIGHT_BIGGER)
    {
        node->bf = BALANCED;
        x->bf = BALANCED;
    }
    else
    {
        node->bf = RIGHT_BIGGER;
        x->bf = LEFT_BIGGER;
    }
}


/*
    parent                                  parent
       |                                       |
     node                                      X
    /    \               ---->                /  \
   X      node_right                         XL   node
  / \                                            /    \
 XL   XR                                        XR      node_right
*/

___inline___ static void avl_rotate_ll(Avl *tree, Avl_node *node)
{
    Avl_node *x;

    TRACE("");

    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(node != NULL);
    assert(node->left_son != NULL);

    x = node->left_son;

    node->left_son = x->right_son;
    if (node->left_son != NULL)
        node->left_son->parent = node;

    x->right_son = node;
    x->parent = node->parent;
    node->parent = x;

    if (x->parent == NULL)
        tree->root = x;
    else
    {
        if (x->parent->left_son == node)
            x->parent->left_son = x;
        else
            x->parent->right_son = x;
    }

    if (x->bf == LEFT_BIGGER)
    {
        node->bf = BALANCED;
        x->bf = BALANCED;
    }
    else
    {
        node->bf = LEFT_BIGGER;
        x->bf = RIGHT_BIGGER;
    }
}

/*
    parent                    parent                              parent
       |                         |                                   |
      node       LL(X)          node         RR(node)                Y
     /     \     ---->         /    \          ---->              /    \
node_left   X             node_left   Y                         node     X
           /  \                      /  \                      /    \   /  \
          Y    XR                   YL    X               node_left YL  YR   XR
         /  \                            /  \
        YL   YR                         YR    XR
*/

___inline___ static void avl_rotate_rl(Avl *tree, Avl_node *node)
{
    Avl_node *x;
    Avl_node *y;

    TRACE("");

    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(node != NULL);
    assert(node->right_son != NULL);
    assert(node->right_son->left_son != NULL);

    x = node->right_son;
    y = x->left_son;

    x->left_son = y->right_son;
    if (y->right_son != NULL)
        y->right_son->parent = x;

    node->right_son = y->left_son;
    if (y->left_son != NULL)
        node->right_son->parent = node;

    y->left_son = node;
    y->parent = node->parent;
    node->parent = y;

    y->right_son = x;
    x->parent = y;

    if (y->parent == NULL)
        tree->root = y;
    else
    {
        if (y->parent->left_son == node)
            y->parent->left_son = y;
        else
            y->parent->right_son = y;
    }

    if (y->bf == RIGHT_BIGGER)
    {
        node->bf = LEFT_BIGGER;
        x->bf = BALANCED;
    }
    else if (y->bf == LEFT_BIGGER)
    {
        node->bf = BALANCED;
        x->bf = RIGHT_BIGGER;
    }
    else
    {
        node->bf = BALANCED;
        x->bf = BALANCED;
    }

    y->bf = BALANCED;
}

/*
    parent                          parent                               parent
       |                               |                                    |
     node           LL(X)             node           RR(node)               Y
    /     \          ---->           /    \           ---->              /     \
   X     node_right                 Y    node_right                     X       node
  /  \                             /  \                                /  \     /   \
 XL   Y                           X    YR                             XL   YL   YR    node_right
     /  \                        /  \
    YL   YR                     XL   YL
*/

___inline___ static void avl_rotate_lr(Avl *tree, Avl_node *node)
{
    Avl_node *x;
    Avl_node *y;

    TRACE("");

    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(node != NULL);
    assert(node->left_son != NULL);
    assert(node->left_son->right_son != NULL);

    x = node->left_son;
    y = x->right_son;

    x->right_son = y->left_son;
    if (x->right_son != NULL)
        x->right_son->parent = x;

    node->left_son = y->right_son;
    if (node->left_son != NULL)
        node->left_son->parent = node;

    y->right_son = node;
    y->parent = node->parent;
    node->parent = y;

    y->left_son = x;
    x->parent = y;

    if (y->parent == NULL)
        tree->root = y;
    else
    {
        if (y->parent->left_son == node)
            y->parent->left_son = y;
        else
            y->parent->right_son = y;
    }

    if (y->bf == RIGHT_BIGGER)
    {
        node->bf = BALANCED;
        x->bf = LEFT_BIGGER;
    }
    else if (y->bf == LEFT_BIGGER)
    {
        node->bf = RIGHT_BIGGER;
        x->bf = BALANCED;
    }
    else
    {
        node->bf = BALANCED;
        x->bf = BALANCED;
    }

    y->bf = BALANCED;
}

static int avl_insert_fixup(Avl *tree, Avl_node *new_node)
{
    Avl_node *parent;
    Avl_node *ptr;

    TRACE("");

    assert(tree != NULL);
    assert(tree->root != NULL);
    assert(new_node != NULL);

    parent = new_node->parent;

    /* if parent has not balanced, now is balanced */
    if (parent->bf != BALANCED)
    {
        parent->bf = BALANCED;
        return 0;
    }

    /* bf parent depened on branch of new_node */
    if (parent->left_son == new_node)
        parent->bf = LEFT_BIGGER;
    else
        parent->bf = RIGHT_BIGGER;

    ptr = parent->parent;
    while (ptr != NULL)
    {
        /* Not balanced + another unbalanced = very unbalanced, do rotate */
        if (ptr->bf != BALANCED)
        {
            if (ptr->bf == LEFT_BIGGER)
            {
                /* add to right, now is balanced */
                if (ptr->right_son == parent)
                    ptr->bf = BALANCED;
                else if (parent->bf == RIGHT_BIGGER)
                    avl_rotate_lr(tree, ptr);
                else
                    avl_rotate_ll(tree, ptr);
            }
            else /* symetric case */
            {
                if (ptr->left_son == parent)
                    ptr->bf = BALANCED;
                else if (parent->bf == LEFT_BIGGER)
                    avl_rotate_rl(tree, ptr);
                else
                    avl_rotate_rr(tree, ptr);
            }

            break;
        }

        /* update bf, depends on side from we come to ptr */
        if (ptr->left_son == parent)
            ptr->bf = LEFT_BIGGER;
        else
            ptr->bf = RIGHT_BIGGER;

        /* go upper */
        parent = ptr;
        ptr = ptr->parent;

    }

    return 0;
}

static int avl_delete_fixup(Avl *tree, Avl_node *parent, Avl_node *node)
{
    Avl_node *ptr;

    TRACE("");

    assert(tree != NULL);

    ptr = NULL;
    while (parent != NULL)
        /* tree was balanced */
        if (parent->bf == BALANCED)
        {
            /* deleted from left, so now right is bigger */
            if (parent->left_son == node)
                parent->bf = RIGHT_BIGGER;
            else /* symetric case */
                parent->bf = LEFT_BIGGER;

            /* tree is balanced, end fixup */
            break;
        }
        else
        {
            /* deleted node from bigger branch, parent is balanced */
            if ((parent->bf == LEFT_BIGGER && parent->left_son == node)
                || (parent->bf == RIGHT_BIGGER && parent->right_son == node))
            {
                parent->bf = BALANCED;

                /* go upper */
                node = parent;
                parent= parent->parent;
            }
            /* deleted node from smaller branch, rotation is needed */
            else
            {
                /* save brother ptr ( need for replay path ) */
                if (parent->left_son == node)
                    ptr = parent->right_son;
                else
                    ptr = parent->left_son;

                /* brother is balanced, need rotation but hight not change */
                if (ptr->bf == BALANCED )
                {
                    if (parent->bf == LEFT_BIGGER)
                        avl_rotate_ll(tree, parent);
                    else
                        avl_rotate_rr(tree, parent);

                    /* hight not change, not need another fix */
                    break;
                }
                else if (parent->bf == ptr->bf)
                {
                    if (parent->bf == LEFT_BIGGER)
                        avl_rotate_ll(tree, parent);
                    else
                        avl_rotate_rr(tree, parent);

                    /* go upper */
                    node = ptr;
                    parent = ptr->parent;
                }
                else
                {
                    if (parent->bf == LEFT_BIGGER)
                        avl_rotate_lr(tree, parent);
                    else
                        avl_rotate_rl(tree, parent);

                    /* go upper */
                    node = parent->parent;
                    parent = node->parent;
                }
            }
        }

    return 0;
}

Avl *avl_create(int size_of, int (*cmp)(void *a, void *b))
{
    Avl *tree;

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    TRACE("");

    tree = (Avl *)malloc(sizeof(Avl));
    if (tree == NULL)
        ERROR("malloc error\n", NULL, "");

    tree->root = NULL;
    tree->size_of = (size_t)size_of;
    tree->cmp = cmp;

    tree->nodes = 0;

    return tree;
}

void avl_destroy(Avl *tree)
{
    Avl_node *node;
    Avl_node *temp;

    TRACE("");

    if (tree == NULL)
        return;

    if (tree->root == NULL)
    {
        FREE(tree);
        return;
    }

    /* destroy tree using inorder */
    node = avl_min_node(tree->root);
    while(node != NULL)
    {
        temp = node;
        node = avl_successor(node);
        avl_node_destroy(temp);
    }

    FREE(tree);
}

void avl_destroy_with_entries(Avl *tree, void (*destructor)(void *data))
{
    Avl_node *node;
    Avl_node *temp;

    TRACE("");

    if (tree == NULL)
        return;

    if (destructor == NULL)
        return;

    if (tree->root == NULL)
    {
        FREE(tree);
        return;
    }

    /* destroy tree using inorder */
    node = avl_min_node(tree->root);
    while(node != NULL)
    {
        temp = node;
        node = avl_successor(node);
        destructor(temp->data);
        avl_node_destroy(temp);
    }

    FREE(tree);
}

int avl_insert(Avl *tree, void *data)
{
    Avl_node *node;
    Avl_node *parent;
    Avl_node *new_node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    /* special case - empty tree */
    if (tree->root == NULL)
    {
        node = avl_node_create(data, (int)tree->size_of, NULL);
		if (node == NULL)
			ERROR("avl_node_create error\n", 1, "");

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
                ERROR("data with this key exist in tree\n", 1, "");
            }
            else if (tree->cmp(node->data, data) > 0)
                node = node->left_son;
            else
                node = node->right_son;
        }

        new_node = avl_node_create(data, (int)tree->size_of, parent);
		if (new_node == NULL)
			ERROR("avl_node_create error\n", 1, "");

        /* new node is the right son */
        if (tree->cmp(new_node->data, parent->data) == 1)
            parent->right_son = new_node;
        else /* new_node is the left_node */
            parent->left_son = new_node;

        if (avl_insert_fixup(tree, new_node))
            ERROR("avl_insert_fixup error\n", 1, "");
    }

    ++tree->nodes;

    return 0;
}

int avl_delete(Avl *tree, void *data_key)
{
    Avl_node *node;
    Avl_node *temp;
    Avl_node *parent;
    Avl_node *successor;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1, "");

    if (tree->root == NULL)
        ERROR("Tree is empty, nothing to delete\n", 1, "");

    node = avl_node_search(tree, data_key);
    if (node == NULL)
        ERROR("data with this key doesn't exist in tree, nothing to delete\n", 1, "");

    temp = NULL;
    parent = node->parent;

    /* case 1 node is leafnode */
    if (node->left_son == NULL && node->right_son == NULL)
    {
        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = NULL;
            else
                node->parent->right_son = NULL;
        }
        else
            tree->root = NULL;

        if (avl_delete_fixup(tree, parent, temp))
            ERROR("avl_delete_fixup error\n", 1, "");
    }
    /* case 2 node has only left subtree */
    else if (node->right_son == NULL)
    {
        node->left_son->parent = node->parent;
        temp = node->left_son;

        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = node->left_son;
            else
                node->parent->right_son = node->left_son;
        }
        else
            tree->root = node->left_son;

        if (avl_delete_fixup(tree, parent, temp))
            ERROR("avl_delete_fixup error\n", 1, "");
    }
    /* case 3 node has only right subtree */
    else if (node->left_son == NULL)
    {
        node->right_son->parent = node->parent;
        temp = node->right_son;

        if (node->parent != NULL)
        {
            if (node->parent->left_son == node)
                node->parent->left_son = node->right_son;
            else
                node->parent->right_son = node->right_son;
        }
        else
            tree->root = node->right_son;

        if (avl_delete_fixup(tree, parent, temp) )
            ERROR("avl_delete_fixup error\n", 1, "");
    }
    /* case 4 node has both children */
    else
    {
        successor = avl_successor(node);

        temp = successor->right_son;
        parent = successor->parent;

        /* delete successor */
        if (successor->right_son != NULL)
            successor->right_son->parent = successor->parent;

        if (successor->parent->left_son == successor)
            successor->parent->left_son = successor->right_son;
        else
            successor->parent->right_son = successor->right_son;

       if (avl_delete_fixup(tree, parent, temp))
            ERROR("avl_delete_fixup error\n", 1, "");

        successor->bf = node->bf;

        successor->left_son = node->left_son;
        successor->right_son = node->right_son;

        if (successor->left_son != NULL)
            successor->left_son->parent = successor;
        if (successor->right_son != NULL)
            successor->right_son->parent = successor;

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

    avl_node_destroy(node);

    return 0;
}

int avl_min(Avl *tree, void *data)
{
    Avl_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (tree->root == NULL)
        ERROR("Tree is empty, nothing to delete\n", 1, "");

    node = avl_min_node(tree->root);
    if (node == NULL)
        ERROR("avl_min_node error\n", 1, "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

int avl_max(Avl *tree, void *data)
{
    Avl_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data == NULL)
        ERROR("data == NULL\n", 1, "");

    if (tree->root == NULL)
        ERROR("Tree is empty, nothing to delete\n", 1, "");

    node = avl_max_node(tree->root);
    if (node == NULL)
        ERROR("avl_max_node error\n", 1, "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, tree->size_of);

    return 0;
}

 int avl_search(Avl *tree, void *data_key, void *data_out)
{
    Avl_node *node;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", 1, "");

    if (data_out == NULL)
        ERROR("data_out == NULL\n", 1, "");

    if (tree->root == NULL)
        ERROR("Tree is empty, nothing to delete\n", 1, "");

    node = avl_node_search(tree, data_key);
    if (node == NULL)
        ERROR("avl_search_node error\n", 1, "");

    __ASSIGN__(*(BYTE *)data_out, *(BYTE *)node->data, tree->size_of);

    return 0;
}

bool avl_key_exist(Avl *tree, void *data_key)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", false, "");

    if (data_key == NULL)
        ERROR("data_key == NULL\n", false, "");

    if (tree->root == NULL)
        ERROR("Tree is empty, nothing to delete\n", false, "");

    return avl_node_search(tree, data_key) != NULL;
}

int avl_to_array(Avl *tree, void *array, size_t *size)
{
    void *t;
    BYTE *_t;
    Avl_node *node;
    size_t offset;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (array == NULL)
        ERROR("array == NULL\n", 1, "");

    if (size == NULL)
        ERROR("size == NULL\n", 1, "");

    if (tree->root == NULL)
        ERROR("Tree is empty, nothing to delete\n", 1, "");

    t = malloc(tree->size_of * tree->nodes);
	if (t == NULL)
		ERROR("malloc error\n", 1, "");

    offset = 0;
    _t = (BYTE *)t;
    node = avl_min_node(tree->root);

    while (node != NULL)
    {
        __ASSIGN__(_t[offset], *(BYTE *)node->data, tree->size_of);
        offset += tree->size_of;

        node = avl_successor(node);
    }

    *(void **)array = t;
    *size = tree->nodes;

    return 0;
}

ssize_t avl_get_num_entries(Avl *tree)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", (ssize_t)-1, "");

    return (ssize_t)tree->nodes;
}

int avl_get_data_size(Avl *tree)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", -1, "");

    return (int)tree->size_of;
}

Avl_iterator *avl_iterator_create(Avl *tree, iti_mode_t mode)
{
    Avl_iterator *iterator;

    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_ROOT && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL, "");

    iterator = (Avl_iterator *)malloc(sizeof(Avl_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    if (mode == ITI_BEGIN)
        iterator->node = avl_min_node(tree->root);
    else if (mode == ITI_END)
        iterator->node = avl_max_node(tree->root);
    else
        iterator->node = tree->root;

    iterator->size_of = tree->size_of;

    return iterator;
}

void avl_iterator_destroy(Avl_iterator *iterator)
{
    TRACE("");

    if(iterator == NULL)
        return;

    FREE(iterator);
}

int avl_iterator_init(Avl *tree, Avl_iterator *iterator, iti_mode_t mode)
{
    TRACE("");

    if (tree == NULL)
        ERROR("tree == NULL\n", 1, "");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_ROOT && mode != ITI_END)
        ERROR("Incorrect mode\n", 1, "");

    if (mode == ITI_BEGIN)
        iterator->node = avl_min_node(tree->root);
    else if (mode == ITI_END)
        iterator->node = avl_max_node(tree->root);
    else
        iterator->node = tree->root;

    iterator->size_of = tree->size_of;

    return 0;
}

int avl_iterator_next(Avl_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");


    iterator->node = avl_successor(iterator->node);

    return 0;
}

int avl_iterator_prev(Avl_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");


    iterator->node = avl_predecessor(iterator->node);

    return 0;
}

int avl_iterator_get_data(Avl_iterator *iterator, void *val)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (val == NULL)
        ERROR("val == NULL\n", 1, "");


    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

int avl_iterator_get_node(Avl_iterator *iterator, void *node)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (node == NULL)
        ERROR("node == NULL\n", 1, "");


    *(void **)node = iterator->node;

    return 0;
}

bool avl_iterator_end(Avl_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true, "");

    return iterator->node == NULL;
}

TREE_WRAPPERS_CREATE(Avl, avl)

Tree *tree_avl_create(int size_of, int (*cmp)(void* a,void *b))
{
    Tree *tree;

    TRACE("");

    /* create Tree */
    tree = (Tree *)malloc(sizeof(Tree));
    if (tree == NULL)
        ERROR("malloc error\n", NULL, "");

    /* create avl */
    tree->____tree = (void *)avl_create(size_of, cmp);
    if (tree->____tree == NULL)
    {
        FREE(tree);
        ERROR("avl_create error\n", NULL, "");
    }

    /* fill hooks */
    TREE_WRAPPERS_ASSIGN(tree);

    return tree;
}