#include <list.h>
#include <generic.h>
#include <stdlib.h>
#include <log.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

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
___inline___ static List_node *list_node_create(List_node *next, void* data,
                int size_of);


/*
    Destory node

    PARAMS
    @IN node - ptr on list_node

    RETURN:
    This is void function
*/
___inline___ static void list_node_destroy(List_node *node);


___inline___ static List_node *list_node_create(List_node *next, void *data,
     int size_of)
{
    List_node *node;

    TRACE("");

    assert(data != NULL);
    assert(size_of >= 1);

    node = (List_node *)malloc(sizeof(List_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->____data = malloc((size_t)size_of);
    if (node->____data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    node->____next = next;
    __ASSIGN__(*(BYTE *)node->____data, *(BYTE *)data, size_of);

    return node;
}

static void list_node_destroy(List_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->____data);
    FREE(node);
}



List *list_create(int size_of, int (*cmp)(void* a, void *b))
{
    List *list;

    TRACE("");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    list = (List *)malloc(sizeof(List));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    list->____cmp = cmp;
    list->____size_of = (size_t)size_of;

    list->____length = 0;
    list->____head = NULL;
    list->____tail = NULL;

    return list;
}

void list_destroy(List *list)
{
    List_node *ptr;
    List_node *next;

    TRACE("");

    if (list == NULL)
        return;

    ptr = list->____head;
    while (ptr != NULL)
    {
        next = ptr->____next;
        list_node_destroy(ptr);

        ptr = next;
    }

    FREE(list);

    return;
}

void list_destroy_with_entries(List *list, void (*destructor)(void *data))
{
    List_node *ptr;
    List_node *next;

    TRACE("");

    if (list == NULL || destructor == NULL)
        return;

    ptr = list->____head;
    while (ptr != NULL)
    {
        next = ptr->____next;
        destructor(ptr->____data);
        list_node_destroy(ptr);

        ptr = next;
    }

    FREE(list);

    return;
}

int list_insert(List *list, void *entry)
{
    List_node *node;
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

    if (list->____head == NULL)
    {
        node = list_node_create(NULL, entry, (int)list->____size_of);
        if (node == NULL)
            ERROR("list_node_create error\n", 1, "");

        list->____head = node;
        list->____tail = node;

        ++list->____length;
    }
    else
    {
        ptr = list->____head;
        prev = NULL;

        /* add guardian at the end of list */
        guard = list_node_create(NULL, entry, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list_node_create error\n", 1, "");

        list->____tail->____next = guard;

        /* skip all entries < than new entry */
        while (list->____cmp(ptr->____data, entry) < 0)
        {
            prev = ptr;
            ptr = ptr->____next;
        }

        /* we need stable property so we skip all entries with the same key */
        while (list->____cmp(ptr->____data, entry) == 0 && ptr != guard)
        {
            prev = ptr;
            ptr = ptr->____next;
        }

        /* now we insert new node ( we use our guard ) */
        guard->____next = ptr;

        /* insert in middle or at the end */
        if (prev != NULL)
        {
            prev->____next = guard;

            if (prev == list->____tail)
                list->____tail = guard;
        }
        else
            list->____head = guard;

        ++list->____length;

        list->____tail->____next = NULL;
    }

    return 0;
}

int list_delete(List *list, void *entry)
{
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

	if (list->____head == NULL )
		ERROR("Nothing to delete\n", 1, "");

    ptr = list->____head;
    prev = NULL;

    /* we add guardian */
    guard = list_node_create(NULL, entry, (int)list->____size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", 1, "");

    list->____tail->____next = guard;

    /* skip all entries with key < than entry */
    while (list->____cmp(ptr->____data, entry) < 0)
    {
        prev = ptr;
        ptr = ptr->____next;
    }

    /* we find entry to delete */
    if (ptr != guard && list->____cmp(ptr->____data, entry) == 0)
    {
        /* we delete head node */
        if (prev ==  NULL)
        {
            ptr = list->____head->____next;
            list_node_destroy(list->____head);
            list->____head = ptr;

            if (ptr == guard)
                list->____head = NULL;
        }
        else
        {
            prev->____next = ptr->____next;

            if (ptr == list->____tail)
                list->____tail = prev;

            list_node_destroy(ptr);
        }
    }
    else
	{
		list_node_destroy(guard);
    	list->____tail->____next = NULL;
        return 1;
	}

    list_node_destroy(guard);

    list->____tail->____next = NULL;
    --list->____length;

    return 0;
}

int list_delete_all(List *list, void *entry)
{
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    size_t deleted;

    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", -1, "");

	if (list->____head == NULL )
		ERROR("Nothing to delete\n", -1, "");

    ptr = list->____head;
    prev= NULL;

    /* we add guardian */
    guard = list_node_create(NULL, entry, (int)list->____size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", -1, "");

    list->____tail->____next = guard;

    /* skip all entries with key < than entry */
    while (list->____cmp(ptr->____data, entry) < 0)
    {
        prev = ptr;
        ptr = ptr->____next;
    }

    deleted = 0;

    /* we find entry to delete */
    if (ptr != guard && list->____cmp(ptr->____data, entry) == 0)
    {
        /* we delete head node */
        if (prev ==  NULL)
        {
            while (list->____cmp(list->____head->____data, entry) == 0 && list->____head != guard)
            {
                ptr = list->____head->____next;
                list_node_destroy(list->____head);
                list->____head = ptr;

                ++deleted;
            }

            if (ptr == guard)
                list->____head = NULL;
        }
        else
        {
            while (list->____cmp(ptr->____data, entry) == 0 && ptr != guard)
            {
                prev->____next = ptr->____next;

                if (ptr == list->____tail)
                    list->____tail = prev;

                list_node_destroy(ptr);

                ptr = prev->____next;

                ++deleted;
            }
        }
    }
    else
	{
		list_node_destroy(guard);
    	list->____tail->____next = NULL;

        return 0;
	}

    list_node_destroy(guard);
    list->____tail->____next = NULL;

    list->____length -= deleted;

    return (int)deleted;
}

List *list_merge(List *list1, List *list2)
{
    List *list3;
    List_node *ptr1;
    List_node *ptr2;
    List_node *node;

    TRACE("");

    if (list1 == NULL || list2 == NULL)
        ERROR("list1 == NULL || list2 == NULL\n", NULL, "");

    if (list1->____size_of != list2->____size_of)
        ERROR("lists have diff entries types\n", NULL, "");

    list3 = list_create((int)list1->____size_of, list1->____cmp);
    if(list3 == NULL)
        ERROR("list_create error\n", NULL, "");

    ptr1 = list1->____head;
    ptr2 = list2->____head;

    if (ptr1 == NULL && ptr2 == NULL)
        return list3;

    if (ptr1 != NULL && ptr2 != NULL)
    {
        if (list3->____cmp(ptr1->____data, ptr2->____data) < 0)
        {
            node = list_node_create(NULL, ptr1->____data, (int)list3->____size_of);
            list3->____head = node;
            ptr1 = ptr1->____next;
        }
        else
        {
            node = list_node_create(NULL, ptr2->____data, (int)list3->____size_of);
            list3->____head = node;
            ptr2 = ptr2->____next;
        }
    }
    else if (ptr1 != NULL)
    {
        node = list_node_create(NULL, ptr1->____data, (int)list3->____size_of);
        list3->____head = node;
        ptr1 = ptr1->____next;
    }
    else if (ptr2 != NULL)
    {
        node = list_node_create(NULL, ptr2->____data, (int)list3->____size_of);
        list3->____head = node;
        ptr2 = ptr2->____next;
    }

    list3->____tail = list3->____head;
    list3->____tail->____next = NULL;

    while (ptr1 != NULL && ptr2 != NULL)
    {
        if (list3->____cmp(ptr1->____data, ptr2->____data) < 0)
        {
            node = list_node_create(NULL, ptr1->____data, (int)list3->____size_of);
            list3->____tail->____next = node;
            list3->____tail = node;
            ptr1 = ptr1->____next;
        }
        else
        {
            node = list_node_create(NULL, ptr2->____data, (int)list3->____size_of);
            list3->____tail->____next = node;
            list3->____tail = node;
            ptr2 = ptr2->____next;
        }
    }

    while (ptr1 != NULL)
    {
        node = list_node_create(NULL, ptr1->____data, (int)list3->____size_of);
        list3->____tail->____next = node;
        list3->____tail = node;
        ptr1 = ptr1->____next;
    }

    while (ptr2 != NULL)
    {
        node = list_node_create(NULL, ptr2->____data, (int)list3->____size_of);
        list3->____tail->____next = node;
        list3->____tail = node;
        ptr2 = ptr2->____next;
    }

    list3->____length = list1->____length + list2->____length;

    return list3;
}

int list_search(List *list, void *val, void *entry)
{
    List_node *ptr;
    List_node *guard;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (val == NULL || entry == NULL)
        ERROR("val == NULL || entry == NULL\n", 1, "");

    if (list->____length == 0)
        ERROR("List is empty\n", 1, "");

    ptr = list->____head;

    /* we add guardian */
    guard = list_node_create(NULL, val, (int)list->____size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", 1, "");

    list->____tail->____next = guard;

    /* skip all entries with key < than entry */
    while (list->____cmp(ptr->____data, val) < 0)
        ptr = ptr->____next;

    /* we find entry */
    if (ptr != guard && list->____cmp(ptr->____data, val) == 0)
    {
        __ASSIGN__(*(BYTE *)entry, *(BYTE *)ptr->____data, list->____size_of);

        list_node_destroy(guard);
        list->____tail->____next = NULL;

        return 0;
    }
    else
    {
        list_node_destroy(guard);
        list->____tail->____next = NULL;

        return 1;
    }
}

int list_to_array(List *list, void *array, size_t *size)
{
    List_node *ptr;
    BYTE *t;

    size_t offset;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (array == NULL)
        ERROR("array == NULL\n", 1, "");

    if (size == NULL)
        ERROR("size == NULL\n", 1, "");

    if (list->____length == 0)
        ERROR("list is empty\n", 1, "");

    t = (BYTE *)malloc(list->____length * list->____size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    ptr = list->____head;

    offset = 0;

    while (ptr != NULL)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->____data, list->____size_of);
        offset += list->____size_of;

        ptr = ptr->____next;
    }

    *(void **)array = t;

    *size = list->____length;

    return 0;
}

int list_get_data_size(List *list)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", -1, "");

    return (int)list->____size_of;
}

ssize_t list_get_num_entries(List *list)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", (ssize_t)-1, "");

    return (ssize_t)list->____length;
}

List_iterator *list_iterator_create(List *list, ITI_MODE mode)
{
    List_iterator *iterator;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    if (mode != ITI_BEGIN)
        ERROR("Incorrect mode\n", NULL, "");

    iterator = (List_iterator *)malloc(sizeof(List_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    iterator->____node = list->____head;
    iterator->____size_of = list->____size_of;

    return iterator;
}

void list_iterator_destroy(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int list_iterator_init(List *list, List_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN)
        ERROR("Incorrect mode\n", 1, "");

    iterator->____node = list->____head;
    iterator->____size_of = list->____size_of;

    return 0;
}

int list_iterator_next(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->____node = iterator->____node->____next;

    return 0;
}

int list_iterator_prev(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->____node = NULL;

    return 0;
}

int list_iterator_get_data(List_iterator *iterator, void *val)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (val == NULL)
        ERROR("val == NULL\n", 1, "");

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->____node->____data, iterator->____size_of);

    return 0;
}

int list_iterator_get_node(List_iterator *iterator, void *node)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (node == NULL)
        ERROR("val == NULL\n", 1, "");

    *(void **)node = iterator->____node;

    return 0;
}

bool list_iterator_end(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true, "");

    return iterator->____node == NULL;
}

SLIST_WRAPPERS_CREATE(List, list)

SList *slist_list_create(int size_of, int (*cmp)(void* a, void *b))
{
    SList *list;

    TRACE("");

    /* create SLIST */
    list = (SList *)malloc(sizeof(SList));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    /* create list */
    list->____list = (void *)list_create(size_of, cmp);
    if (list->____list == NULL)
    {
        FREE(list);
        ERROR("list_create error\n", NULL, "");
    }

    /* fill hooks */
    SLIST_WRAPPERS_ASSIGN(list);

    return list;
}