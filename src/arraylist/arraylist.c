#include <arraylist.h>
#include <log.h>
#include <generic.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

struct Arraylist_node
{
    struct Arraylist_node *next;
    struct Arraylist_node *prev;
    size_t                size_of;
    __extension__ BYTE    data[]; /* placeholder for data */

};

struct Arraylist
{
    size_t              size_of;    /* size of element */
    size_t              length;     /* length of list */

    Arraylist_node      *head;
    Arraylist_node      *tail;
    destructor_f        destroy;
};

struct Arraylist_iterator
{
    Arraylist_node  *node;
    size_t          size_of;

};

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
___inline___ static Arraylist_node *arraylist_node_create( const Arraylist_node  * ___restrict___ prev,
                                                           const Arraylist_node  * ___restrict___ next,
                                                           const void            * ___restrict___ data,
                                                           size_t                size_of);
/*
    Destory node

    PARAMS
    @IN node - ptr on list_node

    RETURN:
    This is void function
*/
___inline___ static void arraylist_node_destroy(Arraylist_node *node);

/*
    Delete first entry from arraylist

    PARAMS
    @IN alist - pointer to arraylist
    @IN destroy - destroy entry during deleting ?

    RETURN
    0 iff success
    Non-zero value iff vailure
*/
static int __arraylist_delete_first(Arraylist *alist, bool destroy);

/*
    Delete last entry from arraylist

    PARAMS
    @IN alist - pointer to arraylist
    @IN destroy - destroy entry during deleting ?

    RETURN
    0 iff success
    Non-zero value iff vailure
*/
static int __arraylist_delete_last(Arraylist *alist, bool destroy);

/*
    Delete first entry from arraylist

    PARAMS
    @IN alist - pointer to arraylist
    @IN pos - entry pos in alist
    @IN destroy - destroy entry during deleting ?

    RETURN
    0 iff success
    Non-zero value iff vailure
*/
static int __arraylist_delete_pos(Arraylist *alist, size_t pos, bool destroy);

/*
    Destroy whole ArrayList

    PARAMS
    @IN alist - pointer to arraylist
    @IN destroy - call destructor ?

    RETURN
    This is a void function
*/
static void __arraylist_destroy_with_entries(Arraylist *alist, bool destroy);

___inline___ static Arraylist_node *arraylist_node_create(  const Arraylist_node  * ___restrict___ prev,
                                                            const Arraylist_node  * ___restrict___ next,
                                                            const void            * ___restrict___ data,
                                                            size_t size_of)
{
    Arraylist_node *node;

    TRACE();

    assert(data != NULL);
    assert(size_of >= 1);

    node = (Arraylist_node *)malloc(sizeof(Arraylist_node) + size_of);
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    node->next = (Arraylist_node *)next;
    node->prev = (Arraylist_node *)prev;
    node->size_of = size_of;
    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data,size_of);

    return node;
}

___inline___ static void arraylist_node_destroy(Arraylist_node *node)
{
    TRACE();

    if (node == NULL)
        return;

    FREE(node);
}

static int __arraylist_delete_first(Arraylist *alist, bool destroy)
{
    Arraylist_node *node;

    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", 1);

    if (alist->head == NULL)
        ERROR("List is empty, nothing to delete\n", 1);

    if (alist->length == 1)
    {
        if (destroy && alist->destroy)
            alist->destroy((void *)alist->head->data);

        arraylist_node_destroy(alist->head);
        alist->head = NULL;
        alist->tail = NULL;
    }
    else
    {
        node = alist->head;

        alist->head = alist->head->next;
        alist->head->prev = NULL;

        if (destroy && alist->destroy)
            alist->destroy((void *)node->data);

        arraylist_node_destroy(node);
    }

    --alist->length;

    return 0;
}

static int __arraylist_delete_last(Arraylist *alist, bool destroy)
{
    Arraylist_node *node;

    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", 1);

    if (alist->head == NULL)
        ERROR("List is empty, nothing to delete\n", 1);

    if (alist->length == 1)
    {
        if (destroy && alist->destroy != NULL)
            alist->destroy((void *)alist->head->data);

        arraylist_node_destroy(alist->head);
        alist->head = NULL;
        alist->tail = NULL;
    }
    else
    {
        node = alist->tail;

        alist->tail = alist->tail->prev;
        alist->tail->next = NULL;

        if (destroy && alist->destroy != NULL)
            alist->destroy((void *)node->data);

        arraylist_node_destroy(node);
    }

    --alist->length;

    return 0;
}


static int __arraylist_delete_pos(Arraylist *alist, size_t pos, bool destroy)
{
    Arraylist_node *ptr;
    size_t i;

    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", 1);

    if (pos >= alist->length)
        ERROR("pos >= len\n", 1);

    /* at the begining */
    if (pos == 0)
        return __arraylist_delete_first(alist, destroy);

    /* at the end */
    if (pos == alist->length - 1)
        return __arraylist_delete_last(alist, destroy);

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

    if (destroy && alist->destroy != NULL)
        alist->destroy(ptr->data);

    arraylist_node_destroy(ptr);

    --alist->length;

    return 0;
}

static void __arraylist_destroy_with_entries(Arraylist *alist, bool destroy)
{
    Arraylist_node *ptr;
    Arraylist_node *next;

    TRACE();

    if (alist == NULL)
        return;

    ptr = alist->head;

    while (ptr != NULL)
    {
        next = ptr->next;
        if (destroy && alist->destroy != NULL)
            alist->destroy((void *)ptr->data);

        arraylist_node_destroy(ptr);

        ptr = next;
    }

    FREE(alist);

    return;
}

Arraylist *arraylist_create(size_t size_of,  destructor_f destroy)
{
    Arraylist *alist;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    alist = (Arraylist *)malloc(sizeof(Arraylist));
    if (alist == NULL)
        ERROR("malloc error\n", NULL);

    alist->size_of = size_of;

    alist->length = 0;
    alist->head = NULL;
    alist->tail = NULL;
    alist->destroy = destroy;

    return alist;
}

void arraylist_destroy(Arraylist *alist)
{
    TRACE();

    __arraylist_destroy_with_entries(alist, false);
}

void arraylist_destroy_with_entries(Arraylist *alist)
{
    TRACE();

    __arraylist_destroy_with_entries(alist, true);
}

int arraylist_insert_first(Arraylist * ___restrict___ alist, const void * ___restrict___ data)
{
    Arraylist_node *node;

    TRACE();

    if (alist == NULL || data == NULL)
        ERROR("alist == NULL || data == NULL\n", 1);

    /* create node and insert at begining */
    node = arraylist_node_create(NULL, alist->head, data, alist->size_of);
    if (node == NULL)
        ERROR("arraylist_node_create error\n", 1);

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

int arraylist_insert_last(Arraylist * ___restrict___ alist, const void * ___restrict___ data)
{
    Arraylist_node *node;

    TRACE();

    if (alist == NULL || data == NULL)
        ERROR("alist == NULL || data == NULL\n", 1);

    /* create node and insert at the end */
    node = arraylist_node_create(alist->tail, NULL, data, alist->size_of);
    if(node == NULL)
        ERROR("arraylist_node_create error\n", 1);

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

int arraylist_insert_pos(Arraylist * ___restrict___ alist, size_t pos, const void * ___restrict___ data)
{
    Arraylist_node *node;
    Arraylist_node *ptr;

    size_t i = 0;

    TRACE();

    if (alist == NULL || data == NULL)
        ERROR("alist == NULL || data == NULL\n", 1);

    if (pos > alist->length)
        ERROR("pos > lenght\n", 1);

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
        ERROR("arraylist_node_create error\n", 1);

    ptr->prev->next = node;
    ptr->prev = node;

    ++alist->length;

    return 0;
}


int arraylist_delete_first(Arraylist *alist)
{
    return __arraylist_delete_first(alist, false);
}

int arraylist_delete_first_with_entry(Arraylist *alist)
{
    return __arraylist_delete_first(alist, true);
}

int arraylist_delete_last(Arraylist *alist)
{
    return __arraylist_delete_last(alist, false);
}

int arraylist_delete_last_with_entry(Arraylist *alist)
{
    return __arraylist_delete_last(alist, true);
}

int arraylist_delete_pos(Arraylist *alist, size_t pos)
{
    return __arraylist_delete_pos(alist, pos, false);
}

int arraylist_delete_pos_with_entry(Arraylist *alist, size_t pos)
{
    return __arraylist_delete_pos(alist, pos, true);
}

int arraylist_get_pos(const Arraylist * ___restrict___ alist, size_t pos, void * ___restrict___ data)
{
    Arraylist_node *ptr;
    size_t i;

    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", 1);

    if (pos >= alist->length)
        ERROR("pos >= len\n", 1);

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

Arraylist *arraylist_merge(const Arraylist  * ___restrict___ alist1, const Arraylist * ___restrict___ alist2)
{
    Arraylist_node *ptr1;
    Arraylist_node *ptr2;

    Arraylist *result;

    TRACE();

    if (alist1 == NULL || alist2 == NULL)
        ERROR("alist1 == NULL || alist2 == NULL\n", NULL);

    result = arraylist_create(alist1->size_of, alist1->destroy);
    if (result == NULL)
        ERROR("arraylist_create error\n", NULL);

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

int arraylist_to_array(const Arraylist * ___restrict___ alist, void * ___restrict___ array, size_t * ___restrict___ size)
{
    Arraylist_node *ptr;
    BYTE *t;

    size_t offset;

    TRACE();

    if (alist == NULL || array == NULL || size == NULL)
        ERROR("alist == NULL || array == NULL || size == NULL\n", 1);

    if (alist->length == 0)
        ERROR("alist is empty\n", 1);

    t = (BYTE *)malloc((size_t)(alist->length * (size_t)alist->size_of));
    if (t == NULL)
        ERROR("malloc error\n", 1);

    ptr = alist->head;
    offset = 0;
    while (ptr != NULL)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->data, alist->size_of);
        offset += (size_t)alist->size_of;

        ptr = ptr->next;
    }

    *(void **)array = t;

    *size = alist->length;

    return 0;
}

ssize_t arraylist_get_data_size(const Arraylist *alist)
{
    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", -1);

    return (ssize_t)alist->size_of;
}

ssize_t arraylist_get_num_entries(const Arraylist *alist)
{
    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", -1);

    return (ssize_t)alist->length;
}

int arraylist_node_get_data(const Arraylist_node * ___restrict___ node, void * ___restrict___ data)
{
    TRACE();

    if (node == NULL || data == NULL)
        ERROR("node == NULL || data == NULL\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, node->size_of);

    return 0;
}

Arraylist_iterator *arraylist_iterator_create(const Arraylist *alist, iti_mode_t mode)
{
    Arraylist_iterator *iterator;

    TRACE();

    if (alist == NULL)
        ERROR("alist == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    if (alist->head == NULL)
        return NULL;

    iterator = (Arraylist_iterator *)malloc(sizeof(Arraylist_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    if (mode == ITI_BEGIN)
        iterator->node = alist->head;
    else
        iterator->node = alist->tail;

    iterator->size_of = alist->size_of;

    return iterator;
}

void arraylist_iterator_destroy(Arraylist_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int arraylist_iterator_next(Arraylist_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = iterator->node->next;

    return 0;
}

int arraylist_iterator_prev(Arraylist_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = iterator->node->prev;

    return 0;
}

int arraylist_iterator_get_data(const Arraylist_iterator * ___restrict___ iterator, void * ___restrict___ val)
{
    TRACE();

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

int arraylist_iterator_get_node(const Arraylist_iterator * ___restrict___ iterator, void * ___restrict___ node)
{
    TRACE();

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1);

    *(void **)node = iterator->node;

    return 0;
}

bool arraylist_iterator_end(const Arraylist_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return iterator->node == NULL;
}

ULIST_WRAPPERS_CREATE(Arraylist, arraylist)

UList *ulist_arraylist_create(size_t size_of, destructor_f destroy)
{
    UList *list;

    TRACE();

    /* create ULIST */
    list = (UList *)malloc(sizeof(UList));
    if (list == NULL)
        ERROR("malloc error\n", NULL);

    /* create arraylist */
    list->____list = (void *)arraylist_create(size_of, destroy);
    if (list->____list == NULL)
    {
        FREE(list);
        ERROR("arraylist_create error\n", NULL);
    }

    /* fill hooks */
    ULIST_WRAPPERS_ASSIGN(list);

    return list;
}