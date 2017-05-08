#include <arraylist.h>
#include <log.h>
#include <generic.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

/*
    Create new alist node with ptr next and value val

    PARAMS
    @IN prev - ptr to prev node
    @IN next - ptr to next node in list
    @IN data - data sotred in node
    @IN size_of - size of val in bytes

    RETURN:
    NULL if failure
    Pointer if success
*/
___inline___ static Arraylist_node *arraylist_node_create( Arraylist_node  *prev,
                                                         Arraylist_node  *next,
                                                         void            *data,
                                                         int             size_of);
/*
    Destory node

    PARAMS
    @IN node -ptr on list_node

    RETURN:
    This is void function
*/
___inline___ static void arraylist_node_destroy(Arraylist_node *node);

___inline___ static Arraylist_node *arraylist_node_create( Arraylist_node  *prev,
                                                         Arraylist_node  *next,
                                                         void            *data,
                                                         int size_of)
{
    Arraylist_node *node;

    TRACE("");

    assert(data == NULL);
    assert(size_of < 1);

    node = (Arraylist_node *)malloc(sizeof(Arraylist_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->data = malloc(size_of);
    if (node->data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    node->next = next;
    node->prev = prev;
    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data,size_of);

    return node;
}

___inline___ static void arraylist_node_destroy(Arraylist_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->data);
    FREE(node);
}

Arraylist_iterator *arraylist_iterator_create(Arraylist *alist, ITI_MODE mode)
{
    Arraylist_iterator *iterator;

    TRACE("");

    assert(alist == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator = (Arraylist_iterator *)malloc(sizeof(Arraylist_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    if (mode == ITI_BEGIN)
        iterator->node = alist->head;
    else
        iterator->node = alist->tail;

    iterator->size_of = alist->size_of;

    return iterator;
}

void arraylist_iterator_destroy(Arraylist_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int arraylist_iterator_init(Arraylist *alist, Arraylist_iterator *iterator,
        ITI_MODE mode)
{
    TRACE("");

    assert(alist == NULL || iterator == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    if(mode == ITI_BEGIN)
        iterator->node = alist->head;
    else
        iterator->node = alist->tail;

    iterator->size_of = alist->size_of;

    return 0;
}

int arraylist_iterator_next(Arraylist_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    iterator->node = iterator->node->next;

    return 0;
}

int arraylist_iterator_prev(Arraylist_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    iterator->node = iterator->node->prev;

    return 0;
}

int arraylist_iterator_get_data(Arraylist_iterator *iterator, void *val)
{
    TRACE("");

    assert(iterator == NULL || val == NULL);

    __ASSIGN__(*(BYTE *)val,*(BYTE *)iterator->node->data,iterator->size_of);

    return 0;
}

bool arraylist_iterator_end(Arraylist_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    return iterator->node == NULL;
}

Arraylist *arraylist_create(int size_of)
{
    Arraylist *alist;

    TRACE("");

    assert(size_of < 1);

    alist = (Arraylist *)malloc(sizeof(Arraylist));
    if (alist == NULL)
        ERROR("malloc error\n", NULL, "");

    alist->size_of = size_of;

    alist->length = 0;
    alist->head = NULL;
    alist->tail = NULL;

    return alist;
}

void arraylist_destroy(Arraylist *alist)
{
    Arraylist_node *ptr;
    Arraylist_node *next;

    TRACE("");

    if (alist == NULL)
        return;

    ptr = alist->head;

    while (ptr != NULL)
    {
        next = ptr->next;
        arraylist_node_destroy(ptr);

        ptr = next;
    }

    FREE(alist);

    return;
}

void arraylist_destroy_with_entries(Arraylist *alist,
        void (*destructor)(void *data))
{
    Arraylist_node *ptr;
    Arraylist_node *next;

    TRACE("");

    if (alist == NULL)
        return;

    ptr = alist->head;

    while (ptr != NULL)
    {
        next = ptr->next;
        destructor(ptr->data);
        arraylist_node_destroy(ptr);

        ptr = next;
    }

    FREE(alist);

    return;
}

int arraylist_insert_first(Arraylist *alist, void *data)
{
    Arraylist_node *node;

    TRACE("");

    assert(alist == NULL || data == NULL);

    /* create node and insert at begining */
    node = arraylist_node_create(NULL, alist->head, data, alist->size_of);
    if (node == NULL)
        ERROR("arraylist_node_create error\n", 1, "");

    /* list was empty  */
    if (alist->head == NULL)
    {
        alist->head = node;
        alist->tail = node;
    }
    else
    {
        alist->head->prev = node;
        alist->head = node;
    }

    ++alist->length;

    return 0;
}

int arraylist_insert_last(Arraylist *alist, void *data)
{
    Arraylist_node *node;

    TRACE("");

    assert(alist == NULL || data == NULL);

    /* create node and insert at the end */
    node = arraylist_node_create(alist->tail, NULL, data, alist->size_of);
    if(node == NULL)
        ERROR("arraylist_node_create error\n", 1, "");

    if(alist->head == NULL)
    {
        alist->head = node;
        alist->tail = node;
    }
    else
    {
        alist->tail->next = node;
        alist->tail = node;
    }

    ++alist->length;

    return 0;
}

int arraylist_insert_pos(Arraylist *alist, size_t pos, void *data)
{
    Arraylist_node *node;
    Arraylist_node *ptr;

    size_t i = 0;

    TRACE("");

    assert(alist == NULL || data == NULL);
    assert(pos < 0 || pos > alist->length);

    /* at the begining */
    if (pos == 0)
        return arraylist_insert_first(alist, data);

    /* at the end */
    if (pos == alist->length)
        return arraylist_insert_last(alist, data);

    /* go from begin */
    if (pos < (alist->length >> 1))
    {
        ptr = alist->head;
        for (i = 0; i < pos; ++i)
            ptr = ptr->next;

    }
    /* go from end */
    else
    {
        pos = alist->length - pos - 1;
        ptr = alist->tail;
        for (i = 0; i < pos; ++i)
            ptr = ptr->prev;
    }

    node = arraylist_node_create(ptr->prev, ptr, data, alist->size_of);
        if (node == NULL)
            ERROR("arraylist_node_create error\n", 1, "");

    ptr->prev->next = node;
    ptr->prev = node;

    ++alist->length;

    return 0;
}

int arraylist_delete_first(Arraylist *alist)
{
    Arraylist_node *node;

    TRACE("");

    assert(alist == NULL);
    assert(alist->head == NULL);

    node = alist->head;

    alist->head = alist->head->next;
    alist->head->prev = NULL;
    arraylist_node_destroy(node);

    --alist->length;

    return 0;
}

int arraylist_delete_last(Arraylist *alist)
{
    Arraylist_node *node;

    TRACE("");

    assert(alist == NULL);
    assert(alist->head == NULL);

    node = alist->tail;

    alist->tail = alist->tail->prev;
    alist->tail->next = NULL;

    arraylist_node_destroy(node);

    --alist->length;

    return 0;
}

int arraylist_delete_pos(Arraylist *alist, size_t pos)
{
    Arraylist_node *ptr;
    size_t i;

    TRACE("");

    assert(alist == NULL);
    assert(pos < 0 || pos > alist->length);

    /* at the begining */
    if (pos == 0)
        return arraylist_delete_first(alist);

    /* at the end */
    if (pos == alist->length - 1)
        return arraylist_delete_last(alist);

    /* go from begin */
    if (pos < (alist->length >> 1))
    {
        ptr = alist->head;
        for (i = 0; i < pos; ++i)
            ptr = ptr->next;

    }
    /* go from end */
    else
    {
        pos = alist->length - pos - 1;
        ptr = alist->tail;
        for (i = 0; i < pos; ++i)
            ptr = ptr->prev;
    }

    ptr->prev->next = ptr->next;
    ptr->next->prev = ptr->prev;

    arraylist_node_destroy(ptr);

    --alist->length;

    return 0;
}

int arraylist_get_pos(Arraylist *alist, size_t pos, void *data)
{
    Arraylist_node *ptr;
    size_t i;

    TRACE("");

    assert(alist == NULL);
    assert(pos < 0 || pos > alist->length);

    /* go from begin */
    if (pos < (alist->length >> 1))
    {
        ptr = alist->head;
        for (i = 0; i < pos; ++i)
            ptr = ptr->next;

    }
    /* go from end */
    else
    {
        pos = alist->length - pos - 1;
        ptr = alist->tail;
        for (i = 0; i < pos; ++i)
            ptr = ptr->prev;
    }

    __ASSIGN__(*(BYTE *)data, *(BYTE *)ptr->data, alist->size_of);

    return 0;
}

Arraylist *arraylist_merge(Arraylist *alist1, Arraylist *alist2)
{
    Arraylist_node *ptr1;
    Arraylist_node *ptr2;

    Arraylist *result;

    TRACE("");

    assert(alist1 == NULL || alist2 == NULL);

    result = arraylist_create(alist1->size_of);
    if (result == NULL)
        ERROR("arraylist_create error\n", NULL, "");

    ptr1 = alist1->head;
    while (ptr1 != NULL)
    {
        arraylist_insert_last(result, ptr1->data);
        ptr1 = ptr1->next;
    }

    ptr2 = alist2->head;
    while (ptr2 != NULL)
    {
        arraylist_insert_last(result, ptr2->data);
        ptr2 = ptr2->next;
    }

    return result;
}

int arraylist_to_array(Arraylist *alist, void *array, size_t *size)
{
    Arraylist_node *ptr;
    BYTE *t;

    size_t offset;

    TRACE("");

    assert(alist == NULL || array == NULL || size == NULL);

    t = (BYTE *)malloc(alist->length * alist->size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    ptr = alist->head;

    offset = 0;

    while (ptr != NULL)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->data, alist->size_of);
        offset += alist->size_of;

        ptr = ptr->next;
    }

    *(void **)array = t;

    *size = alist->length;

    return 0;
}
