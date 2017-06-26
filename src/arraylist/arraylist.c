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

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Arraylist_node *)malloc(sizeof(Arraylist_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->____data = malloc((size_t)size_of);
    if (node->____data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    node->____next = next;
    node->____prev = prev;
    node->____size_of = (size_t)size_of;
    __ASSIGN__(*(BYTE *)node->____data, *(BYTE *)data,size_of);

    return node;
}

___inline___ static void arraylist_node_destroy(Arraylist_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->____data);
    FREE(node);
}

Arraylist *arraylist_create(int size_of)
{
    Arraylist *alist;

    TRACE("");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    alist = (Arraylist *)malloc(sizeof(Arraylist));
    if (alist == NULL)
        ERROR("malloc error\n", NULL, "");

    alist->____size_of = (size_t)size_of;

    alist->____length = 0;
    alist->____head = NULL;
    alist->____tail = NULL;

    return alist;
}

void arraylist_destroy(Arraylist *alist)
{
    Arraylist_node *ptr;
    Arraylist_node *next;

    TRACE("");

    if (alist == NULL)
        return;

    ptr = alist->____head;

    while (ptr != NULL)
    {
        next = ptr->____next;
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

    ptr = alist->____head;

    while (ptr != NULL)
    {
        next = ptr->____next;
        destructor(ptr->____data);
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

    if (alist == NULL || data == NULL)
        ERROR("alist == NULL || data == NULL\n", 1, "");

    /* create node and insert at begining */
    node = arraylist_node_create(NULL, alist->____head, data, (int)alist->____size_of);
    if (node == NULL)
        ERROR("arraylist_node_create error\n", 1, "");

    /* list was empty  */
    if (alist->____head == NULL)
    {
        alist->____head = node;
        alist->____tail = node;
    }
    else
    {
        alist->____head->____prev = node;
        alist->____head = node;
    }

    ++alist->____length;

    return 0;
}

int arraylist_insert_last(Arraylist *alist, void *data)
{
    Arraylist_node *node;

    TRACE("");

    if (alist == NULL || data == NULL)
        ERROR("alist == NULL || data == NULL\n", 1, "");

    /* create node and insert at the end */
    node = arraylist_node_create(alist->____tail, NULL, data, (int)alist->____size_of);
    if(node == NULL)
        ERROR("arraylist_node_create error\n", 1, "");

    if(alist->____head == NULL)
    {
        alist->____head = node;
        alist->____tail = node;
    }
    else
    {
        alist->____tail->____next = node;
        alist->____tail = node;
    }

    ++alist->____length;

    return 0;
}

int arraylist_insert_pos(Arraylist *alist, size_t pos, void *data)
{
    Arraylist_node *node;
    Arraylist_node *ptr;

    size_t i = 0;

    TRACE("");

    if (alist == NULL || data == NULL)
        ERROR("alist == NULL || data == NULL\n", 1, "");

    if (pos > alist->____length)
        ERROR("pos > lenght\n", 1, "");

    /* at the begining */
    if (pos == 0)
        return arraylist_insert_first(alist, data);

    /* at the end */
    if (pos == alist->____length)
        return arraylist_insert_last(alist, data);

    /* go from begin */
    if (pos < (alist->____length >> 1))
    {
        ptr = alist->____head;
        for (i = 0; i < pos; ++i)
            ptr = ptr->____next;

    }
    /* go from end */
    else
    {
        pos = alist->____length - pos - 1;
        ptr = alist->____tail;
        for (i = 0; i < pos; ++i)
            ptr = ptr->____prev;
    }

    node = arraylist_node_create(ptr->____prev, ptr, data, (int)alist->____size_of);
    if (node == NULL)
        ERROR("arraylist_node_create error\n", 1, "");

    ptr->____prev->____next = node;
    ptr->____prev = node;

    ++alist->____length;

    return 0;
}

int arraylist_delete_first(Arraylist *alist)
{
    Arraylist_node *node;

    TRACE("");

    if (alist == NULL)
        ERROR("alist == NULL\n", 1, "");

    if (alist->____head == NULL)
        ERROR("List is empty, nothing to delete\n", 1, "");

    if (alist->____length == 1)
    {
        arraylist_node_destroy(alist->____head);
        alist->____head = NULL;
        alist->____tail = NULL;
    }
    else
    {
        node = alist->____head;

        alist->____head = alist->____head->____next;
        alist->____head->____prev = NULL;
        arraylist_node_destroy(node);
    }

    --alist->____length;

    return 0;
}

int arraylist_delete_last(Arraylist *alist)
{
    Arraylist_node *node;

    TRACE("");

    if (alist == NULL)
        ERROR("alist == NULL\n", 1, "");

    if (alist->____head == NULL)
        ERROR("List is empty, nothing to delete\n", 1, "");

    if (alist->____length == 1)
    {
        arraylist_node_destroy(alist->____head);
        alist->____head = NULL;
        alist->____tail = NULL;
    }
    else
    {
        node = alist->____tail;

        alist->____tail = alist->____tail->____prev;
        alist->____tail->____next = NULL;

        arraylist_node_destroy(node);
    }

    --alist->____length;

    return 0;
}

int arraylist_delete_pos(Arraylist *alist, size_t pos)
{
    Arraylist_node *ptr;
    size_t i;

    TRACE("");

    if (alist == NULL)
        ERROR("alist == NULL\n", 1, "");

    if (pos >= alist->____length)
        ERROR("pos >= len\n", 1, "");

    /* at the begining */
    if (pos == 0)
        return arraylist_delete_first(alist);

    /* at the end */
    if (pos == alist->____length - 1)
        return arraylist_delete_last(alist);

    /* go from begin */
    if (pos < (alist->____length >> 1))
    {
        ptr = alist->____head;
        for (i = 0; i < pos; ++i)
            ptr = ptr->____next;

    }
    /* go from end */
    else
    {
        pos = alist->____length - pos - 1;
        ptr = alist->____tail;
        for (i = 0; i < pos; ++i)
            ptr = ptr->____prev;
    }

    ptr->____prev->____next = ptr->____next;
    ptr->____next->____prev = ptr->____prev;

    arraylist_node_destroy(ptr);

    --alist->____length;

    return 0;
}

int arraylist_get_pos(Arraylist *alist, size_t pos, void *data)
{
    Arraylist_node *ptr;
    size_t i;

    TRACE("");

    if (alist == NULL)
        ERROR("alist == NULL\n", 1, "");

    if (pos >= alist->____length)
        ERROR("pos >= len\n", 1, "");

    /* go from begin */
    if (pos < (alist->____length >> 1))
    {
        ptr = alist->____head;
        for (i = 0; i < pos; ++i)
            ptr = ptr->____next;

    }
    /* go from end */
    else
    {
        pos = alist->____length - pos - 1;
        ptr = alist->____tail;
        for (i = 0; i < pos; ++i)
            ptr = ptr->____prev;
    }

    __ASSIGN__(*(BYTE *)data, *(BYTE *)ptr->____data, alist->____size_of);

    return 0;
}

Arraylist *arraylist_merge(Arraylist *alist1, Arraylist *alist2)
{
    Arraylist_node *ptr1;
    Arraylist_node *ptr2;

    Arraylist *result;

    TRACE("");

    if (alist1 == NULL || alist2 == NULL)
        ERROR("alist1 == NULL || alist2 == NULL\n", NULL, "");

    result = arraylist_create((int)alist1->____size_of);
    if (result == NULL)
        ERROR("arraylist_create error\n", NULL, "");

    ptr1 = alist1->____head;
    while (ptr1 != NULL)
    {
        arraylist_insert_last(result, ptr1->____data);
        ptr1 = ptr1->____next;
    }

    ptr2 = alist2->____head;
    while (ptr2 != NULL)
    {
        arraylist_insert_last(result, ptr2->____data);
        ptr2 = ptr2->____next;
    }

    return result;
}

int arraylist_to_array(Arraylist *alist, void *array, size_t *size)
{
    Arraylist_node *ptr;
    BYTE *t;

    size_t offset;

    TRACE("");

    if (alist == NULL || array == NULL || size == NULL)
        ERROR("alist == NULL || array == NULL || size == NULL\n", 1, "");

    if (alist->____length == 0)
        ERROR("alist is empty\n", 1, "");

    t = (BYTE *)malloc((size_t)(alist->____length * (size_t)alist->____size_of));
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    ptr = alist->____head;
    offset = 0;
    while (ptr != NULL)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->____data, alist->____size_of);
        offset += (size_t)alist->____size_of;

        ptr = ptr->____next;
    }

    *(void **)array = t;

    *size = alist->____length;

    return 0;
}

int arraylist_get_size_of(Arraylist *alist)
{
    if (alist == NULL)
        ERROR("alist == NULL\n", -1, "");

    return (int)alist->____size_of;
}

ssize_t arraylist_get_num_entries(Arraylist *alist)
{
    if (alist == NULL)
        ERROR("alist == NULL\n", -1, "");

    return (ssize_t)alist->____length;
}

int arraylist_node_get_data(Arraylist_node *node, void *data)
{
    if (node == NULL || data == NULL)
        ERROR("node == NULL || data == NULL\n", 1, "");

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->____data, node->____size_of);

    return 0;
}

Arraylist_iterator *arraylist_iterator_create(Arraylist *alist, ITI_MODE mode)
{
    Arraylist_iterator *iterator;

    TRACE("");

    if (alist == NULL)
        ERROR("alist == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL, "");

    iterator = (Arraylist_iterator *)malloc(sizeof(Arraylist_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    if (mode == ITI_BEGIN)
        iterator->____node = alist->____head;
    else
        iterator->____node = alist->____tail;

    iterator->____size_of = alist->____size_of;

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

    if (alist == NULL || iterator == NULL)
        ERROR("alist == NULL || iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", 1, "");

    if(mode == ITI_BEGIN)
        iterator->____node = alist->____head;
    else
        iterator->____node = alist->____tail;

    iterator->____size_of = alist->____size_of;

    return 0;
}

int arraylist_iterator_next(Arraylist_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->____node = iterator->____node->____next;

    return 0;
}

int arraylist_iterator_prev(Arraylist_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->____node = iterator->____node->____prev;

    return 0;
}

int arraylist_iterator_get_data(Arraylist_iterator *iterator, void *val)
{
    TRACE("");

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1, "");

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->____node->____data, iterator->____size_of);

    return 0;
}

int arraylist_iterator_get_node(Arraylist_iterator *iterator, void *node)
{
    TRACE("");

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1, "");

    *(void **)node = iterator->____node;

    return 0;
}

bool arraylist_iterator_end(Arraylist_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    return iterator->____node == NULL;
}

ULIST_WRAPPERS_CREATE(Arraylist, arraylist)

UList *ulist_arraylist_create(int size_of)
{
    UList *list;

    TRACE("");

    /* create ULIST */
    list = (UList *)malloc(sizeof(UList));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    /* create arraylist */
    list->____list = (void *)arraylist_create(size_of);
    if (list->____list == NULL)
    {
        FREE(list);
        ERROR("arraylist_create error\n", NULL, "");
    }

    /* fill hooks */
    ULIST_WRAPPERS_ASSIGN(list);

    return list;
}