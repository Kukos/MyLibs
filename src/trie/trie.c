#include <trie.h>
#include <log.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <compiler.h>
#include <common.h>
#include <darray.h>
#include <assert.h>

#define CHAR_TO_INDEX(c) ((c < ALPHABET_START_CODE || c > ALPHABET_END_CODE) ? \
    -1 : c - ALPHABET_START_CODE)

#define ALPHA(N) ((N) + ALPHABET_START_CODE)
#define NULL_CHAR '\0'

/*
    Create new trie node

    PARAMS
    @IN parent - pointer to parent

    RETURN:
    %NULL iff failure
    %Pointer iff success
*/
static ___inline___ Trie_node *trie_node_create(Trie_node *parent);

/*
    Destroy trie node

    PARAMS
    @IN node - pointer to node

    RETURN:
    This is a void function
*/
static ___inline___ void trie_node_destroy(Trie_node *node);

/*
    Search for node with @word

    PARAMS
    @IN root - pointer to root
    @IN word - word

    RETURN:
    %NULL iff failure
    %Pointer to node iff success
*/
static Trie_node *trie_node_search(Trie_node *root, char *word);

/*
    Check node parenthood

    PARAMS
    @IN node - pointer to node

    RETURN:
    %FALSE iff node hasn't any children
    %TRUE iff node has at least one child
*/
static ___inline___ bool trie_node_has_child(Trie_node *node);

/*
    Search node in parent children

    PARAMS
    @IN parent - pointer to parent
    @IN node - pointer to node

    RETURN:
    %-1 iff failure
    %Index of node iff success
*/
static ___inline___ int trie_node_get_child_index(Trie_node *parent, Trie_node *node);

/*
    Get minimum node of subtree with root in node

    PARAMS
    @IN node - trie node ( subtree root )
    @OUT word - pointer to min node word ( concat with word in addr *word)

    RETURN:
    %NULL iff failure
    %Pointer to Min node iff success
*/
static Trie_node *trie_min_node(Trie_node *node, char **word);

/*
    Get maxium node of subtree with root in node

    PARAMS
    @IN node - trie node ( subtree root )
    @IN num_child - num of child wchich we start serching min node
    @OUT word - pointer to max node word ( concat with word in addr *word)

    RETURN:
    %NULL if failure
    %Pointer to Max node if success
*/
static Trie_node *trie_max_node(Trie_node *node, char **word);

/*
    Get successor of node @node

    PARAMS
    @IN node - trie node
    @IN num_child - num of child wchich we start serching max node
    @OUT word - pointer to successor node ( concat with word in addr *word )

    RETURN:
    %NULL if failure
    %Pointer to Max node if success
*/
static Trie_node *trie_node_successor(Trie_node *node, char **word);

/*
    Get predecessor of node @node

    PARAMS
    @IN node - trie node
    @OUT word - pointer to predecessor node ( concat with word in addr *word )

    RETURN:
    %NULL if failure
    %Pointer to Max node if success
*/
static Trie_node *trie_node_predecessor(Trie_node *node, char **word);

/*
    Simpler version of trie_min_node, using by delete function
    Get minimum node of subbtree root in @node

    PARAMS
    @IN node - pointer to root of subtree

    RETURN:
    %NULL if failure
    %Pointer to min node if success
*/
static Trie_node *trie_min_node_d(Trie_node *node);

/*
    Simpler version of trie_node_successor, using by delete funtion
    Get successor of @node

    PARAMS
    @IN node - pointer to node

    RETURN:
    %NULL if failure
    %Pointer to successor if success
*/
static Trie_node *trie_node_successor_d(Trie_node *node);

static ___inline___ Trie_node *trie_node_create(Trie_node *parent)
{
    Trie_node *node;

    TRACE("");

    node = (Trie_node *)malloc(sizeof(Trie_node));
    if (node == NULL)
        ERROR("node == NULL\n", NULL, "");

    node->parent = parent;
    (void)memset(node->children, 0, sizeof(Trie_node*) * ALPHABET_SIZE);
    node->is_leaf = false;

    return node;
}

static ___inline___ void trie_node_destroy(Trie_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node);
}

static ___inline___ bool trie_node_has_child(Trie_node *node)
{
    size_t i;

    TRACE("");

    if (node == NULL)
        ERROR("node == NULL", false, "");

    for (i = 0; i < ALPHABET_SIZE; ++i)
        if (node->children[i] != NULL)
            return true;

    return false;
}

static ___inline___ int trie_node_get_child_index(Trie_node *parent, Trie_node *node)
{
    size_t i;

    TRACE("");

    assert(parent == NULL || node == NULL);

    for (i = 0; i < ALPHABET_SIZE; ++i)
        if (parent->children[i] == node)
            return i;

    return -1;
}

static Trie_node *trie_node_search(Trie_node *root,char *word)
{
    size_t length;
    size_t i;
    size_t index;

    Trie_node *ptr;

    TRACE("");

    assert(root == NULL);

    length = strlen(word);
    ptr = root;

    for(i = 0; i < length; ++i)
    {
        index = CHAR_TO_INDEX(word[i]);
        if (ptr->children[index] == NULL)
            return NULL;

        ptr = ptr->children[index];
    }

    if( ptr != NULL && ptr->is_leaf)
        return ptr;
    else
        return NULL;
}

static Trie_node *trie_min_node(Trie_node *node,char **word)
{
    Darray *letters;
    Trie_node *parent;

    char c;
    char null = NULL_CHAR;

    size_t i;
    size_t offset;

    TRACE("");

    assert(node == NULL || word == NULL);

    letters = darray_create(DARRAY_UNSORTED, 0, sizeof(char), NULL);
    if (letters == NULL)
        ERROR("darray error\n", NULL, "");

    parent = NULL;
    if (node->parent != NULL)
    {
        c = ALPHA(trie_node_get_child_index(node->parent, node));
        if (darray_insert(letters, (void*)&c))
        {
            darray_destroy(letters);

            ERROR("darray_insert error\n", NULL, "");
        }
    }

    while (node != NULL)
    {
        parent = node;

        for (i = 0; i < ALPHABET_SIZE; ++i)
            if (node->children[i] != NULL)
            {
                c = ALPHA(i);
                if (darray_insert(letters, (void*)&c))
                {
                    darray_destroy(letters);

                    ERROR("darray insert error\n", NULL, "");
                }

                node = node->children[i];
                break;
            }

        /* node has not any children */
        if (i == ALPHABET_SIZE)
            node = NULL;
    }

    /* end word */
    if (darray_insert(letters, (void*)&null))
    {
        darray_destroy(letters);

        ERROR("darray_insert error\n", NULL, "");
    }

    offset = 0;
    if (*word == NULL)
        *word = (char *)malloc(sizeof(char) * darray_get_num_entries(letters));
    else
    {
        offset = strlen(*word);
        *word = (char *)realloc((void *)*word, (offset + darray_get_num_entries(letters)) * sizeof(char));
    }

    if (*word == NULL)
    {
        darray_destroy(letters);

        ERROR("malloc error\n", NULL, "");
    }

    if(memcpy((void *)(*word + offset), (void *)darray_get_array(letters),
        darray_get_num_entries(letters) * sizeof(char)) == NULL)
    {
        darray_destroy(letters);
        FREE(*word);

        ERROR("memcpy error\n", NULL, "");
    }

    darray_destroy(letters);

    return parent;
}

static Trie_node *trie_max_node(Trie_node *node,char **word)
{
    size_t i;
    size_t offset = 0;

    char c;
    char null = NULL_CHAR;

    Darray *letters;
    Trie_node *parent;

    TRACE("");

    assert(node == NULL || word == NULL);

    letters = darray_create(DARRAY_UNSORTED, 0, sizeof(char), NULL);
    if (letters == NULL)
        ERROR("darray_create error\n", NULL, "");

    parent = NULL;
    if (node->parent != NULL)
    {
        c = ALPHA(trie_node_get_child_index(node->parent, node));
        if (darray_insert(letters, (void*)&c))
        {
            darray_destroy(letters);

            ERROR("darray_insert error\n", NULL, "");
        }
    }

    while (node != NULL)
    {
        parent = node;

        for (i = ALPHABET_SIZE - 1; i >= 0; --i)
            if (node->children[i] != NULL)
            {
                c = ALPHA(i);
                if (darray_insert(letters, (void*)&c))
                {
                    darray_destroy(letters);

                    ERROR("darray insert error\n", NULL, "");
                }

                node = node->children[i];
                break;
            }

        /* node has not any children */
        if (i == -1)
            node = NULL;
    }

    /* end word */
    if (darray_insert(letters, (void*)&null))
    {
        darray_destroy(letters);

        ERROR("darray_insert error\n", NULL, "");
    }

    if (*word == NULL)
        *word = (char *)malloc(sizeof(char) * darray_get_num_entries(letters));
    else
    {
        offset = strlen(*word);
        *word = (char *)realloc((void *)*word,
            (offset + darray_get_num_entries(letters)) * sizeof(char));
    }

    if (*word == NULL)
    {
        darray_destroy(letters);

        ERROR("malloc error\n", NULL, "");
    }

    if (memcpy((void *)(*word + offset), (void *)darray_get_array(letters),
        darray_get_num_entries(letters) * sizeof(char)) == NULL)
    {
        darray_destroy(letters);
        FREE(*word);

        ERROR("memcpy error\n", NULL, "");
    }

    darray_destroy(letters);

    return parent;
}

static Trie_node *trie_node_successor(Trie_node *node, char **word)
{
    size_t length;
    size_t i;

    Trie_node *parent;

    TRACE("");

    assert(node == NULL || word == NULL);

    parent = node->parent;

    length = strlen(*word);
    while (parent != NULL )
    {
        /* we go to parent so delete last letter */
        (*word)[--length] = NULL_CHAR;

        for (i = trie_node_get_child_index(parent,node) + 1; i < ALPHABET_SIZE; ++i)
            if (parent->children[i] != NULL)
                break;

        /* parent has other child */
        if (i < ALPHABET_SIZE)
            return trie_min_node(parent->children[i], word);
        else if(parent->is_leaf) /* parent has own word, so is successor */
            return parent;

        /* parent has not any children we go upper */
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

static Trie_node *trie_node_predecessor(Trie_node *node, char **word)
{
    size_t length;
    size_t i;

    Trie_node *parent;

    TRACE("");

    assert(node == NULL || word == NULL);

    parent = node->parent;

    length = strlen(*word);
    while (parent != NULL )
    {
        /* we go to parent so delete last letter */
        (*word)[--length] = NULL_CHAR;

        for (i = trie_node_get_child_index(parent,node) - 1; i >= 0; --i)
            if (parent->children[i] != NULL)
                break;

        /* parent has other child */
        if (i >= 0)
            return trie_max_node(parent->children[i],word);
        else if(parent->is_leaf )  /* parent has own word, so is predecessor */
            return parent;

        /* parent has not any children we go upper */
        node = parent;
        parent = parent->parent;
    }

    return parent;
}

static Trie_node *trie_min_node_d(Trie_node *node)
{
    size_t i;
    Trie_node *parent;

    TRACE("");

    assert(node == NULL);

    parent = NULL;
    while (node != NULL)
    {
        parent = node;

        for (i = 0; i < ALPHABET_SIZE; ++i)
            if (node->children[i] != NULL)
            {
                node = node->children[i];
                break;
            }

        /* node has not any children */
        if (i == ALPHABET_SIZE)
            node = NULL;
    }

    return parent;
}

static Trie_node *trie_node_successor_d(Trie_node *node)
{
    size_t i;
    Trie_node *parent;

    TRACE("");

    assert(node == NULL);
    parent = node->parent;

    if (parent == NULL)
        return NULL;

    for (i = trie_node_get_child_index(parent,node) + 1; i < ALPHABET_SIZE; ++i)
        if (parent->children[i] != NULL)
            break;

    /* parent has other child */
    if (i < ALPHABET_SIZE)
        return trie_min_node_d(parent->children[i]);
    else if(parent->is_leaf) /* parent has own word, so is successor */
        return parent;

    return parent;
}

Trie *trie_create(void)
{
    Trie *trie;

    TRACE("");

    trie = (Trie *)malloc(sizeof(Trie));
    if (trie == NULL)
        ERROR("malloc error\n", NULL, "");

    trie->root = trie_node_create(NULL);
    if (trie->root == NULL)
    {
        FREE(trie);
        ERROR("malloc error\n", NULL, "");
    }

    trie->entries = 0;

    return trie;
}

void trie_destroy(Trie *trie)
{
    Trie_node *node;
    Trie_node *temp;

    TRACE("");

    if (trie == NULL)
        return;

    node = trie_min_node_d(trie->root);
    temp = NULL;

    while (node != NULL)
    {
        temp = node;
        node = trie_node_successor_d(node);
        trie_node_destroy(temp);
    }

    FREE(trie);
}

int trie_insert(Trie *trie, char *word)
{
    size_t i;
    size_t length;
    size_t index;
    char *substr;

    Trie_node *ptr;
    Trie_node *parent;

    TRACE("");

    if (trie == NULL || word == NULL)
        ERROR("trie == NULL || word == NULL\n", 1, "");

    length = strlen(word);

    ptr = trie->root;
    for(i = 0; i < length; ++i)
    {
        parent = ptr;
        index = CHAR_TO_INDEX(word[i]);
        if (index == -1)
        {
            substr = (char *)malloc(sizeof(char) * (i + 1));
            if (substr == NULL)
                ERROR("Wrong character!\n", 1, "");

            if (memcpy((void *)substr, (void *)word, sizeof(char) * i) == NULL)
                ERROR("Wrong character!\n", 1, "");

            *(substr + i) = NULL_CHAR;

            trie_delete(trie, substr);
            FREE(substr);

            ERROR("Wrong character!\n", 1, "");
        }

        /* if has not child, create child with new letter */
        if (ptr->children[index] == NULL)
            ptr->children[index] = trie_node_create(parent);

        ptr = ptr->children[index];
    }

    ptr->is_leaf = true;
    ++trie->entries;

    return 0;
}

bool trie_find(Trie *trie, char *word)
{
    TRACE("");

    if(trie == NULL)
        return false;

    return  trie_node_search(trie->root, word) != NULL;
}

int trie_delete(Trie *trie, char *word)
{
    bool end;

    Trie_node *node;
    Trie_node *parent;

    TRACE("");

    assert(trie == NULL);

    if (trie->entries == 0)
        ERROR("Nothing to delete\n", 1, "");

    node = trie_node_search(trie->root, word);
    if (node == NULL)
        ERROR("Word %s doesn't exist in tree\n", 1, word);

    --trie->entries;
    node->is_leaf = false; /* mark as deleted */

    end = false;
    while (node != trie->root && !end)
    {
        parent = node->parent;
        if (!node->is_leaf && !trie_node_has_child(node)) /* is not sufix or prefix so destroy it */
        {
            node->parent->children[trie_node_get_child_index(node->parent,node)] = NULL;
            trie_node_destroy(node);
        }
        else
            end = true;

        node = parent;
    }

    return 1;
}

char **trie_to_array(Trie *trie, size_t *size)
{
    char **words;
    Trie_iterator it;

    size_t l;
    size_t i;

    TRACE("");

    assert(trie == NULL || size == NULL);

    words = (char **)malloc(sizeof(char*) * trie->entries);
    if (words == NULL)
        ERROR("malloc error\n", NULL, "");

    for (trie_iterator_init(trie, &it, ITI_BEGIN), i = 0;!trie_iterator_end(&it);
         trie_iterator_next(&it), ++i)
    {
        l = strlen(it.word) + 1;
        words[i] = (char *)malloc(sizeof(char) * l);
        if (words[i] == NULL)
        {
            --i;
            while(i)
                FREE(words[i--]);

            FREE(words);
            FREE(it.word);

            ERROR("malloc error\n", NULL, "");
        }
        if (memcpy((void *)words[i], (void *)it.word, l) == NULL)
        {
            while(i)
                FREE(words[i--]);

            FREE(words);
            FREE(it.word);

            ERROR("malloc error\n", NULL, "");
        }
    }

    FREE(it.word);

    *size = trie->entries;
    return words;
}

Trie_iterator *trie_iterator_create(Trie *trie,ITI_MODE mode)
{
    Trie_iterator *iterator;

    TRACE("");

    assert(trie == NULL)
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator = (Trie_iterator *)malloc(sizeof(Trie_iterator));
    if(iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    iterator->word = NULL;

    if(mode == ITI_BEGIN)
        iterator->node = trie_min_node(trie->root, &iterator->word);
    else if ( mode == ITI_END)
        iterator->node = trie_max_node(trie->root, &iterator->word);

    return iterator;
}

void trie_iterator_destroy(Trie_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator->word);
    FREE(iterator);
}

int trie_iterator_init(Trie *trie, Trie_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    assert(trie == NULL);
    assert(iterator == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator->word = NULL;

    if(mode == ITI_BEGIN)
        iterator->node = trie_min_node(trie->root, &iterator->word);
    else if ( mode == ITI_END)
        iterator->node = trie_max_node(trie->root, &iterator->word);

    return 0;
}

int trie_iterator_next(Trie_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    iterator->node = trie_node_successor(iterator->node, &iterator->word);

    return 0;
}

int trie_iterator_prev(Trie_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    iterator->node = trie_node_predecessor(iterator->node, &iterator->word);

    return 0;
}

bool trie_iterator_end(Trie_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    return iterator->node == NULL;
}
