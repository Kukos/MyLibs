#include <list.h>
#include <generic.h>
#include <stdlib.h>
#include <log.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

struct List_node
{
    struct List_node    *next;
    size_t              size_of;
    __extension__ BYTE  data[]; /* placeholder for data */
};

struct List
{
    size_t          size_of;    /* size of element */
    size_t          length;     /* length of list */

    List_node       *head;
    List_node       *tail;      /* needed for insert guardian */

    cmp_f cmp;
    destructor_f destroy;
};

struct List_iterator
{
    List_node       *node;
    size_t          size_of;
};

/*
    Create new list node with ptr next and value val

    PARAMS
    @IN next - ptr to next node in list
    @IN data - data sotred in node
    @IN size_of - size of val in bytes

    RETURN:
    NULL iff failure
    Pointer to List_node iff success
*/
___inline___ static List_node *list_node_create(const List_node * ___restrict___ next, const void * ___restrict___ data,
                size_t size_of);


/*
    Destory node

    PARAMS
    @IN node - ptr on list_node

    RETURN:
    This is void function
*/
___inline___ static void list_node_destroy(List_node *node);

/*
    Destroy whole list

    PARAMS
    @IN list - ptr to List
    @IN destroy - call destructor ?

    RETURN:
    This is void function
*/
static void __list_destroy(List *list, bool destroy);

/*
    Delete first entry equal to entry key

    PARAMS
    @IN list - ptr to List
    @IN entry - fake entry with key
    @IN destroy - call destructor ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __list_delete(List *list, const void *entry, bool destroy);

/*
    Delete ALL entries equal to entry key

    PARAMS
    @IN list - ptr to List
    @IN entry - fake entry with key
    @IN destroy - call destructor ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __list_delete_all(List * ___restrict___ list, const void * ___restrict___ entry, bool destroy);

___inline___ static List_node *list_node_create(const List_node * ___restrict___ next, const void * ___restrict___ data,
     size_t size_of)
{
    List_node *node;

    TRACE();

    assert(data != NULL);
    assert(size_of >= 1);

    node = (List_node *)malloc(sizeof(List_node) + size_of);
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    node->next = (List_node *)next;
    node->size_of = size_of;
    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    return node;
}

static void list_node_destroy(List_node *node)
{
    TRACE();

    if (node == NULL)
        return;

    FREE(node);
}

static void __list_destroy(List *list, bool destroy)
{
    List_node *ptr;
    List_node *next;

    TRACE();

    if (list == NULL)
        return;

    ptr = list->head;
    while (ptr != NULL)
    {
        next = ptr->next;
        if (destroy && list->destroy != NULL)
            list->destroy((void *)ptr->data);

        list_node_destroy(ptr);

        ptr = next;
    }

    FREE(list);

    return;
}


static int __list_delete(List * ___restrict___ list, const void * ___restrict___ entry, bool destroy)
{
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    TRACE();

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1);

	if (list->head == NULL )
		ERROR("Nothing to delete\n", 1);

    ptr = list->head;
    prev = NULL;

    /* we add guardian */
    guard = list_node_create(NULL, entry, list->size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", 1);

    list->tail->next = guard;

    /* skip all entries with key < than entry */
    while (list->cmp(ptr->data, entry) < 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    /* we find entry to delete */
    if (ptr != guard && list->cmp(ptr->data, entry) == 0)
    {
        /* we delete head node */
        if (prev ==  NULL)
        {
            ptr = list->head->next;

            if (destroy && list->destroy != NULL)
                list->destroy((void *)list->head->data);

            list_node_destroy(list->head);
            list->head = ptr;

            if (ptr == guard)
                list->head = NULL;
        }
        else
        {
            prev->next = ptr->next;

            if (ptr == list->tail)
                list->tail = prev;

            if (destroy && list->destroy != NULL)
                list->destroy((void *)ptr->data);

            list_node_destroy(ptr);
        }
    }
    else
	{
		list_node_destroy(guard);
    	list->tail->next = NULL;
        return 1;
	}

    list_node_destroy(guard);

    list->tail->next = NULL;
    --list->length;

    if (list->length == 0)
    {
        list->tail = NULL;
        list->head = NULL;
    }

    return 0;
}

static int __list_delete_all(List * ___restrict___ list, const void * ___restrict___ entry, bool destroy)
{
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    size_t deleted;

    TRACE();

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", -1);

	if (list->head == NULL )
		ERROR("Nothing to delete\n", -1);

    ptr = list->head;
    prev= NULL;

    /* we add guardian */
    guard = list_node_create(NULL, entry, list->size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", -1);

    list->tail->next = guard;

    /* skip all entries with key < than entry */
    while (list->cmp(ptr->data, entry) < 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    deleted = 0;

    /* we find entry to delete */
    if (ptr != guard && list->cmp(ptr->data, entry) == 0)
    {
        /* we delete head node */
        if (prev ==  NULL)
        {
            while (list->cmp(list->head->data, entry) == 0 && list->head != guard)
            {
                ptr = list->head->next;

                if (destroy && list->destroy != NULL)
                    list->destroy((void *)list->head->data);

                list_node_destroy(list->head);
                list->head = ptr;

                ++deleted;
            }

            if (ptr == guard)
                list->head = NULL;
        }
        else
        {
            while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
            {
                prev->next = ptr->next;

                if (ptr == list->tail)
                    list->tail = prev;

                if (destroy && list->destroy != NULL)
                    list->destroy((void *)ptr->data);

                list_node_destroy(ptr);

                ptr = prev->next;

                ++deleted;
            }
        }
    }
    else
	{
		list_node_destroy(guard);
    	list->tail->next = NULL;

        return 0;
	}

    list_node_destroy(guard);
    list->tail->next = NULL;

    list->length -= deleted;

    if (list->length == 0)
    {
        list->tail = NULL;
        list->head = NULL;
    }


    return (int)deleted;
}


List *list_create(size_t size_of, cmp_f cmp, destructor_f destroy)
{
    List *list;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    list = (List *)malloc(sizeof(List));
    if (list == NULL)
        ERROR("malloc error\n", NULL);

    list->cmp = cmp;
    list->destroy = destroy;
    list->size_of = size_of;

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void list_destroy(List *list)
{
    TRACE();

    __list_destroy(list, false);
}

void list_destroy_with_entries(List *list)
{
    TRACE();

    __list_destroy(list, true);
}

int list_insert(List * ___restrict___ list, const void * ___restrict___ entry)
{
    List_node *node;
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    TRACE();

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1);

    if (list->head == NULL)
    {
        node = list_node_create(NULL, entry, list->size_of);
        if (node == NULL)
            ERROR("list_node_create error\n", 1);

        list->head = node;
        list->tail = node;

        ++list->length;
    }
    else
    {
        ptr = list->head;
        prev = NULL;

        /* add guardian at the end of list */
        guard = list_node_create(NULL, entry, list->size_of);
        if (guard == NULL)
            ERROR("list_node_create error\n", 1);

        list->tail->next = guard;

        /* skip all entries < than new entry */
        while (list->cmp(ptr->data, entry) < 0)
        {
            prev = ptr;
            ptr = ptr->next;
        }

        /* we need stable property so we skip all entries with the same key */
        while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
        {
            prev = ptr;
            ptr = ptr->next;
        }

        /* now we insert new node ( we use our guard ) */
        guard->next = ptr;

        /* insert in middle or at the end */
        if (prev != NULL)
        {
            prev->next = guard;

            if (prev == list->tail)
                list->tail = guard;
        }
        else
            list->head = guard;

        ++list->length;

        list->tail->next = NULL;
    }

    return 0;
}

int list_delete(List * ___restrict___ list, const void * ___restrict___ entry)
{
    TRACE();

    return __list_delete(list, entry, false);
}

int list_delete_with_entry(List * ___restrict___ list, const void * ___restrict___ entry)
{
    TRACE();

    return __list_delete(list, entry, true);
}

int list_delete_all(List * ___restrict___ list, const void * ___restrict___ entry)
{
    TRACE();

    return __list_delete_all(list, entry, false);
}

int list_delete_all_with_entry(List * ___restrict___ list, const void * ___restrict___ entry)
{
    TRACE();

    return __list_delete_all(list, entry, true);
}

List *list_merge(const List * ___restrict___ list1, const List * ___restrict___ list2)
{
    List *list3;
    List_node *ptr1;
    List_node *ptr2;
    List_node *node;

    TRACE();

    if (list1 == NULL || list2 == NULL)
        ERROR("list1 == NULL || list2 == NULL\n", NULL);

    if (list1->size_of != list2->size_of)
        ERROR("lists have diff entries types\n", NULL);

    list3 = list_create(list1->size_of, list1->cmp, list1->destroy);
    if(list3 == NULL)
        ERROR("list_create error\n", NULL);

    ptr1 = list1->head;
    ptr2 = list2->head;

    if (ptr1 == NULL && ptr2 == NULL)
        return list3;

    if (ptr1 != NULL && ptr2 != NULL)
    {
        if (list3->cmp(ptr1->data, ptr2->data) < 0)
        {
            node = list_node_create(NULL, ptr1->data, list3->size_of);
            list3->head = node;
            ptr1 = ptr1->next;
        }
        else
        {
            node = list_node_create(NULL, ptr2->data, list3->size_of);
            list3->head = node;
            ptr2 = ptr2->next;
        }
    }
    else if (ptr1 != NULL)
    {
        node = list_node_create(NULL, ptr1->data, list3->size_of);
        list3->head = node;
        ptr1 = ptr1->next;
    }
    else if (ptr2 != NULL)
    {
        node = list_node_create(NULL, ptr2->data, list3->size_of);
        list3->head = node;
        ptr2 = ptr2->next;
    }

    list3->tail = list3->head;
    list3->tail->next = NULL;

    while (ptr1 != NULL && ptr2 != NULL)
    {
        if (list3->cmp(ptr1->data, ptr2->data) < 0)
        {
            node = list_node_create(NULL, ptr1->data, list3->size_of);
            list3->tail->next = node;
            list3->tail = node;
            ptr1 = ptr1->next;
        }
        else
        {
            node = list_node_create(NULL, ptr2->data, list3->size_of);
            list3->tail->next = node;
            list3->tail = node;
            ptr2 = ptr2->next;
        }
    }

    while (ptr1 != NULL)
    {
        node = list_node_create(NULL, ptr1->data, list3->size_of);
        list3->tail->next = node;
        list3->tail = node;
        ptr1 = ptr1->next;
    }

    while (ptr2 != NULL)
    {
        node = list_node_create(NULL, ptr2->data, list3->size_of);
        list3->tail->next = node;
        list3->tail = node;
        ptr2 = ptr2->next;
    }

    list3->length = list1->length + list2->length;

    return list3;
}

int list_search(const List * ___restrict___ list, const void *val, void *entry)
{
    List_node *ptr;
    List_node *guard;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", 1);

    if (val == NULL || entry == NULL)
        ERROR("val == NULL || entry == NULL\n", 1);

    if (list->length == 0)
        ERROR("List is empty\n", 1);

    ptr = list->head;

    /* we add guardian */
    guard = list_node_create(NULL, val, list->size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", 1);

    list->tail->next = guard;

    /* skip all entries with key < than entry */
    while (list->cmp(ptr->data, val) < 0)
        ptr = ptr->next;

    /* we find entry */
    if (ptr != guard && list->cmp(ptr->data, val) == 0)
    {
        __ASSIGN__(*(BYTE *)entry, *(BYTE *)ptr->data, list->size_of);

        list_node_destroy(guard);
        list->tail->next = NULL;

        return 0;
    }
    else
    {
        list_node_destroy(guard);
        list->tail->next = NULL;

        return 1;
    }
}

int list_to_array(const List * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size)
{
    List_node *ptr;
    BYTE *t;

    size_t offset;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", 1);

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (size == NULL)
        ERROR("size == NULL\n", 1);

    if (list->length == 0)
        ERROR("list is empty\n", 1);

    t = (BYTE *)malloc(list->length * list->size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1);

    ptr = list->head;

    offset = 0;

    while (ptr != NULL)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->data, list->size_of);
        offset += list->size_of;

        ptr = ptr->next;
    }

    *(void **)array = t;

    *size = list->length;

    return 0;
}

ssize_t list_get_data_size(const List *list)
{
    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", -1);

    return (ssize_t)list->size_of;
}

ssize_t list_get_num_entries(const List *list)
{
    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", (ssize_t)-1);

    return (ssize_t)list->length;
}

int list_node_get_data(const List_node * ___restrict___ node, void * ___restrict___ data)
{
    TRACE();

    if (node == NULL || data == NULL)
        ERROR("node == NULL || data == NULL\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, node->size_of);

    return 0;
}

List_iterator *list_iterator_create(const List *list, iti_mode_t mode)
{
    List_iterator *iterator;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", NULL);

    if (mode != ITI_BEGIN)
        ERROR("Incorrect mode\n", NULL);

    if (list->head == NULL)
        return NULL;

    iterator = (List_iterator *)malloc(sizeof(List_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    iterator->node = list->head;
    iterator->size_of = list->size_of;

    return iterator;
}

void list_iterator_destroy(List_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int list_iterator_next(List_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = iterator->node->next;

    return 0;
}

int list_iterator_prev(List_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    iterator->node = NULL;

    return 0;
}

int list_iterator_get_data(const List_iterator * ___restrict___ iterator, void * ___restrict___ val)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (val == NULL)
        ERROR("val == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

int list_iterator_get_node(const List_iterator * ___restrict___ iterator, void * ___restrict___ node)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (node == NULL)
        ERROR("val == NULL\n", 1);

    *(void **)node = iterator->node;

    return 0;
}

bool list_iterator_end(const List_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return iterator->node == NULL;
}

SLIST_WRAPPERS_CREATE(List, list)

SList *slist_list_create(size_t size_of, cmp_f cmp, destructor_f destroy)
{
    SList *list;

    TRACE();

    /* create SLIST */
    list = (SList *)malloc(sizeof(SList));
    if (list == NULL)
        ERROR("malloc error\n", NULL);

    /* create list */
    list->____list = (void *)list_create(size_of, cmp, destroy);
    if (list->____list == NULL)
    {
        FREE(list);
        ERROR("list_create error\n", NULL);
    }

    /* fill hooks */
    SLIST_WRAPPERS_ASSIGN(list);

    return list;
}