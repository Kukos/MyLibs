#include <list2d.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

/*
    Create new list node with ptr next and value val

    PARAMS
    @IN prev - ptr to prev in list
    @IN next - ptr to next node in list
    @IN data - data sotred in node
    @IN size_of - size of val in bytes

    RETURN:
    NULL if failure
    Pointer to List_node if success
*/
___inline___ static List2D_node *list2d_node_create(    List2D_node     *prev,
                                                        List2D_node     *next,
                                                        void            *data,
                                                        int             size_of);

/*
    Destory node

    PARAMS
    @IN node - ptr on list_node

    RETURN:
    This is void function
*/
___inline___ static void list2d_node_destroy(List2D_node *node);

___inline___ static List2D_node *list2d_node_create(    List2D_node     *prev,
                                                        List2D_node     *next,
                                                        void            *data,
                                                        int             size_of)
{
    List2D_node *node;

    TRACE("");

    assert(data != NULL);
    assert(size_of >= 1);

    node = (List2D_node *)malloc(sizeof(List2D_node));
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

    __ASSIGN__(*(BYTE *)node->____data, *(BYTE *)data, size_of);

    return node;
}

___inline___ static void list2d_node_destroy(List2D_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->____data);
    FREE(node);
}

List2D *list2d_create(int size_of, int (*cmp)(void* a, void *b), int (*diff)(void* a, void* b))
{
    List2D *list;

    TRACE("");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL, "");

    if (diff == NULL)
        ERROR("diff == NULL\n", NULL, "");

    list = (List2D *)malloc(sizeof(List2D));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    list->____cmp       = cmp;
    list->____size_of   = (size_t)size_of;
    list->____diff      = diff;
    list->____length    = 0;
    list->____head      = NULL;

    return list;
}

void list2d_destroy(List2D *list)
{
    List2D_node *ptr;
    List2D_node *end;
    List2D_node *next;

    TRACE("");

    if (list == NULL)
        return;

    if (list->____length == 0)
    {
        FREE(list);
        return;
    }

    ptr = list->____head;
    end = list->____head->____prev;

    while (ptr != end)
    {
        next = ptr->____next;
        list2d_node_destroy(ptr);
        ptr = next;
    }

    list2d_node_destroy(end);
    FREE(list);

    return;
}


void list2d_destroy_with_entries(List2D *list, void (*destructor)(void *data))
{
    List2D_node *ptr;
    List2D_node *end;
    List2D_node *next;

    TRACE("");

    if (list == NULL)
        return;

    if (destructor == NULL)
        return;

    ptr = list->____head;
    end = list->____head->____prev;

    while (ptr != end)
    {
        next = ptr->____next;
        destructor(ptr->____data);
        list2d_node_destroy(ptr);
        ptr = next;
    }

    list2d_node_destroy(end);
    FREE(list);

    return;
}

int list2d_insert(List2D *list, void *entry)
{
    List2D_node *node;
    List2D_node *ptr;
    List2D_node *guard;
    List2D_node *new_node;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (entry == NULL)
        ERROR("entry == NULL\n", 1, "");

    /* special case - empty list */
    if (list->____head == NULL)
    {
        node = list2d_node_create(NULL, NULL, entry, (int)list->____size_of);
        if (node == NULL)
            ERROR("malloc error\n", 1, "");

        node->____prev = node;
        node->____next = node;
        list->____head = node;

        ++list->____length;
    }
    else
    {
        /* it is better to go from begining */
        if (list->____diff(list->____head->____data, entry) >= list->____diff(list->____head->____prev->____data, entry))
        {
            ptr = list->____head;

            /* add guardian at the end of list */
            guard = list2d_node_create(ptr->____prev, ptr, entry, (int)list->____size_of);
            if (guard == NULL)
                ERROR("malloc error", 1, "");

            list->____head->____prev->____next  = guard;
            list->____head->____prev            = guard;

            /* skip all entries < than new entry */
            while (list->____cmp(ptr->____data, entry) < 0)
                ptr = ptr->____next;

            /* we need stable property so we skip all entries with the same key */
            while (list->____cmp(ptr->____data, entry) == 0 && ptr != guard)
                ptr = ptr->____next;

            new_node = list2d_node_create(ptr->____prev, ptr, entry, (int)list->____size_of);
            if (new_node == NULL)
                ERROR("malloc error", 1, "");

            ptr->____prev->____next     = new_node;
            ptr->____prev               = new_node;

            if (ptr == list->____head)
                list->____head = new_node;

            ++list->____length;

            list->____head->____prev    = guard->____prev;
            guard->____prev->____next       = list->____head;

            list2d_node_destroy(guard);
        }
        else
        {
            ptr = list->____head->____prev;

            /* add guardian at the end of list */
            guard = list2d_node_create(ptr, ptr->____next, entry, (int)list->____size_of);
            if (guard == NULL)
                ERROR("list2d_node create error", 1, "");

            ptr->____next           = guard;
            list->____head->____prev    = guard;

            /* skip all entries > than new entry */
            while (list->____cmp(ptr->____data, entry) > 0)
                ptr = ptr->____prev;

            new_node = list2d_node_create(ptr, ptr->____next, entry, (int)list->____size_of);
            if (new_node == NULL)
                ERROR("list2d_node_create error", 1, "");

            ptr->____next->____prev     = new_node;
            ptr->____next           = new_node;

            if (ptr == guard)
                list->____head = new_node;

            ++list->____length;

            list->____head->____prev    = guard->____prev;
            guard->____prev->____next   = list->____head;

            list2d_node_destroy(guard);
        }
    }
    return 0;
}

int list2d_delete(List2D *list, void *entry)
{
    List2D_node *guard;
    List2D_node *ptr;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (entry == NULL)
        ERROR("entry == NULL\n", 1, "");

    if (list->____head == NULL)
		ERROR("Nothing to delete\n", 1, "");

    if (list->____diff(list->____head->____data, entry) < list->____diff(list->____head->____prev->____data, entry))
    {
        ptr = list->____head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->____prev, ptr, entry, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->____head->____prev->____next  = guard;
        list->____head->____prev            = guard;

        /* skip all entries < than new entry */
        while (list->____cmp(ptr->____data, entry) < 0)
            ptr = ptr->____next;
    }
    else
    {
        ptr = list->____head->____prev;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr, ptr->____next, entry, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->____head->____prev->____next = guard;
        list->____head->____prev = guard;

        /* skip all entries > than new entry */
        while (list->____cmp(ptr->____data, entry) > 0)
            ptr = ptr->____prev;

        while (ptr != guard && ptr->____next != guard && list->____cmp(ptr->____next->____data, entry) == 0)
            ptr = ptr->____prev;
    }

    if (ptr != guard && list->____cmp(ptr->____data, entry) == 0)
    {
        if (ptr == list->____head)
            list->____head = ptr->____next;

        ptr->____prev->____next = ptr->____next;
        ptr->____next->____prev = ptr->____prev;

        list2d_node_destroy(ptr);
    }
    else
    {
        /* delete guardian */
        list->____head->____prev = guard->____prev;
        guard->____prev->____next = list->____head;

        list2d_node_destroy(guard);

        return 1;
    }

    --list->____length;

    list->____head->____prev = guard->____prev;
    guard->____prev->____next = list->____head;

    list2d_node_destroy(guard);

    return 0;
}

int list2d_delete_all(List2D *list, void *entry)
{
    List2D_node     *guard;
    List2D_node     *ptr;
    List2D_node     *temp;

    int             deleted;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", -1, "");

    if (entry == NULL)
        ERROR("entry == NULL\n", -1, "");

    if (list->____head == NULL)
		ERROR("Nothing to delete\n", -1, "");

    deleted = 0;

    if (list->____diff(list->____head->____data, entry) < list->____diff(list->____head->____prev->____data, entry))
    {
        ptr = list->____head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->____prev, ptr, entry, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->____head->____prev->____next = guard;
        list->____head->____prev = guard;

        /* skip all entries < than new entry */
        while (list->____cmp(ptr->____data, entry) < 0)
            ptr = ptr->____next;

        if (ptr != guard && list->____cmp(ptr->____data, entry) == 0)
        {
            while (list->____cmp(ptr->____data, entry) == 0 && ptr != guard)
            {
                if (ptr == list->____head)
                    list->____head = ptr->____next;

                temp = ptr->____next;

                ptr->____prev->____next = ptr->____next;
                ptr->____next->____prev = ptr->____prev;

                list2d_node_destroy(ptr);

                ptr = temp;

                ++deleted;
            }
        }
        else
        {
            list->____head->____prev = guard->____prev;
            guard->____prev->____next = list->____head;

            list2d_node_destroy(guard);

            return 0;
        }
    }
    else
    {
        ptr = list->____head->____prev;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr, ptr->____next, entry, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->____head->____prev->____next = guard;
        list->____head->____prev = guard;

        /* skip all entries > than new entry */
        while (list->____cmp(ptr->____data,entry) > 0)
            ptr = ptr->____prev;

        if (ptr != guard && list->____cmp(ptr->____data,entry) == 0)
        {
            while (list->____cmp(ptr->____data, entry) == 0 && ptr != guard)
            {
                if (ptr == list->____head)
                    list->____head = ptr->____next;

                temp = ptr->____prev;

                ptr->____prev->____next = ptr->____next;
                ptr->____next->____prev = ptr->____prev;

                list2d_node_destroy(ptr);

                ptr = temp;

                ++deleted;
            }
        }
        else
        {
            list->____head->____prev = guard->____prev;
            guard->____prev->____next = list->____head;

            list2d_node_destroy(guard);

            return 0;
        }
    }

    list->____length -= (size_t)deleted;

    list->____head->____prev = guard->____prev;
    guard->____prev->____next = list->____head;

    list2d_node_destroy(guard);

    return deleted;
}

int list2d_search(List2D *list, void *val, void *entry)
{
    List2D_node *guard;
    List2D_node *ptr;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (val == NULL)
        ERROR("val == NULL\n", 1, "");

    if (entry == NULL)
        ERROR("entry == NULL\n", 1, "");

    if (list->____head == NULL)
        ERROR("List is empty\n", 1, "");

    if (list->____diff(list->____head->____data, entry) < list->____diff(list->____head->____prev->____data, entry))
    {
        ptr = list->____head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->____prev, ptr, val, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->____head->____prev->____next = guard;

        /* skip all entries < than new entry */
        while (list->____cmp(ptr->____data, val) < 0)
            ptr = ptr->____next;
    }
    else
    {
        ptr = list->____head->____prev;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr, ptr->____next, val, (int)list->____size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->____head->____prev = guard;

        /* skip all entries > than new entry */
        while (list->____cmp(ptr->____data, val) > 0)
            ptr = ptr->____prev;

        while (list->____cmp(ptr->____data, entry) == 0 && ptr != guard)
            ptr = ptr->____prev;
    }

    /* we find entry */
    if (ptr != guard && list->____cmp(ptr->____data, val) == 0)
    {
        __ASSIGN__(*(BYTE *)entry, *(BYTE *)ptr->____data, list->____size_of);

        list->____head->____prev = guard->____prev;
        guard->____prev->____next = list->____head;

        list2d_node_destroy(guard);

        return 0;
    }
    else
    {
        list->____head->____prev = guard->____prev;
        guard->____prev->____next = list->____head;

        list2d_node_destroy(guard);

        return 1;
    }

    return 1;
}

int list2d_to_array(List2D *list, void *array, size_t *size)
{
    List2D_node     *ptr;
    List2D_node     *end;
    BYTE            *t;
    size_t          offset;

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
    end = list->____head->____prev;

    offset = 0;

    while (ptr != end)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->____data, list->____size_of);
        offset += list->____size_of;

        ptr = ptr->____next;
    }

    __ASSIGN__(t[offset], *(BYTE *)end->____data, list->____size_of);

    *(void **)array = t;

    *size = list->____length;

    return 0;
}

List2D *list2d_merge(List2D *list1, List2D *list2)
{
    List2D          *result;

    List2D_node     *ptr1;
    List2D_node     *ptr2;
    List2D_node     *guard1;
    List2D_node     *guard2;
    List2D_node     *node;

    BYTE            val;

    if (list1 == NULL)
        ERROR("list1 == NULL\n", NULL, "");

    if (list2 == NULL)
        ERROR("list2 == NULL\n", NULL, "");

    result = list2d_create((int)list1->____size_of, list1->____cmp, list1->____diff);

    ptr1 = list1->____head;
    ptr2 = list2->____head;

    if (ptr1 == NULL && ptr2 == NULL)
        return result;

    val = 0;

    /* add guardians */
    guard1 = list2d_node_create(ptr1->____prev, ptr1, (void *)&val, sizeof(BYTE));
    list1->____head->____prev->____next = guard1;

    guard2 = list2d_node_create(ptr2->____prev, ptr2, (void *)&val, sizeof(BYTE));
    list2->____head->____prev->____next = guard2;

    /* special case */
    if (ptr1 != NULL && ptr2 != NULL)
    {
        if (result->____cmp(ptr1->____data, ptr2->____data) < 0)
        {
            node = list2d_node_create(NULL, NULL, ptr1->____data, (int)result->____size_of);
            node->____prev = node;
            node->____next = node;

            result->____head = node;

            ptr1 = ptr1->____next;
        }
        else
        {
            node = list2d_node_create(NULL, NULL, ptr2->____data, (int)result->____size_of);
            node->____prev = node;
            node->____next = node;

            result->____head = node;

            ptr2 = ptr2->____next;
        }
    }
    else if (ptr1 != NULL)
    {
        node = list2d_node_create(NULL, NULL, ptr1->____data, (int)result->____size_of);
        node->____prev = node;
        node->____next = node;

        result->____head = node;

        ptr1 = ptr1->____next;
    }
    else if (ptr2 != NULL)
    {
        node = list2d_node_create(NULL, NULL, ptr2->____data, (int)result->____size_of);
        node->____prev = node;
        node->____next = node;

        result->____head = node;

        ptr2 = ptr2->____next;
    }

    /* add to the end */
    while (ptr1 != guard1 && ptr2 != guard2)
    {
        if (result->____cmp(ptr1->____data, ptr2->____data) < 0)
        {
            node = list2d_node_create(result->____head->____prev, result->____head,
                ptr1->____data, (int)result->____size_of);
            result->____head->____prev->____next = node;
            result->____head->____prev = node;

            ptr1 = ptr1->____next;
        }
        else
        {
            node = list2d_node_create(result->____head->____prev, result->____head,
                ptr2->____data, (int)result->____size_of);
            result->____head->____prev->____next = node;
            result->____head->____prev = node;

            ptr2 = ptr2->____next;
        }
    }

    while (ptr1 != guard1)
    {
        node = list2d_node_create(result->____head->____prev, result->____head,
            ptr1->____data, (int)result->____size_of);
        result->____head->____prev->____next = node;
        result->____head->____prev = node;

        ptr1 = ptr1->____next;
    }

    while (ptr2 != guard2)
    {
        node = list2d_node_create(result->____head->____prev, result->____head,
            ptr2->____data, (int)result->____size_of);
        result->____head->____prev->____next = node;
        result->____head->____prev = node;

        ptr2 = ptr2->____next;
    }

    /* delete guardians */
    list1->____head->____prev = guard1->____prev;
    guard1->____prev->____next = list1->____head;
    list2d_node_destroy(guard1);

    list2->____head->____prev = guard2->____prev;
    guard2->____prev->____next = list2->____head;
    list2d_node_destroy(guard2);

    result->____length = list1->____length + list2->____length;

    return result;
}

int list2d_get_size_of(List2D *list)
{
    if (list == NULL)
        return -1;

    return (int)list->____size_of;
}

ssize_t list2d_get_num_entries(List2D *list)
{
    if (list == NULL)
        return -1;

    return (ssize_t)list->____length;
}

List2D_iterator *list2d_iterator_create(List2D *list, ITI_MODE mode)
{
    List2D_iterator *iterator;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Invalid mode\n", NULL, "");

    iterator = (List2D_iterator *)malloc(sizeof(List2D_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    iterator->____first_time = true;

    if (mode == ITI_BEGIN)
    {
        iterator->____node = list->____head;
        iterator->____end_node = list->____head;
    }
    else
    {
        iterator->____node = list->____head->____prev;
        iterator->____end_node = list->____head->____prev;
    }

    iterator->____size_of = list->____size_of;

    return iterator;
}

void list2d_iterator_destroy(List2D_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int list2d_iterator_init(List2D *list, List2D_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Invalid mode\n", 1, "");

    iterator->____first_time = true;

    if (mode == ITI_BEGIN)
    {
        iterator->____node = list->____head;
        iterator->____end_node = list->____head;
    }
    else
    {
        iterator->____node = list->____head->____prev;
        iterator->____end_node = list->____head->____prev;
    }

    iterator->____size_of = list->____size_of;

    return 0;
}

int list2d_iterator_next(List2D_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");


    if (iterator->____first_time)
        iterator->____first_time = false;

    iterator->____node = iterator->____node->____next;

    return 0;
}

int list2d_iterator_prev(List2D_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");


    if (iterator->____first_time)
        iterator->____first_time = false;

    iterator->____node = iterator->____node->____prev;

    return 0;
}

int list2d_iterator_get_data(List2D_iterator *iterator, void *val)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (val == NULL)
        ERROR("val == NULL\n", 1, "");

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->____node->____data, iterator->____size_of);

    return 0;
}

int list2d_iterator_get_node(List2D_iterator *iterator, void *node)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    if (node == NULL)
        ERROR("node == NULL\n", 1, "");

    *(void **)node = iterator->____node;

    return 0;
}

bool list2d_iterator_end(List2D_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true, "");


    return ((!iterator->____first_time) && iterator->____node == iterator->____end_node);
}

SLIST_WRAPPERS_CREATE(List2D, list2d)

SList *slist_list2d_create(int size_of, int (*cmp)(void* a,void *b),
    int (*diff)(void* a, void* b))
{
    SList *list;

    TRACE("");

    /* create SLIST */
    list = (SList *)malloc(sizeof(SList));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    /* create list */
    list->____list = (void *)list2d_create(size_of, cmp, diff);
    if (list->____list == NULL)
    {
        FREE(list);
        ERROR("list_create error\n", NULL, "");
    }

    /* fill hooks */
    SLIST_WRAPPERS_ASSIGN(list);

    return list;
}

SLIST_ITERATOR_WRAPPERS_CREATE(List2D_iterator, list2d_iterator)

SList_iterator *slist_list2d_iterator_create(SList *list, ITI_MODE mode)
{
    SList_iterator *it;

    TRACE("");

    it = (SList_iterator *)malloc(sizeof(SList_iterator));
    if (it == NULL)
        ERROR("malloc error\n", NULL, "");

    it->____iterator = (void *)list2d_iterator_create((List2D *)slist_get_list(list), mode);
    if (it->____iterator == NULL)
    {
        FREE(it);
        ERROR("list_iterator_create error\n", NULL, "");
    }

    SLIST_ITERATOR_WRAPPERS_ASSIGN(it);

    return it;
}

int slist_list2d_iterator_init(SList *list, SList_iterator *it, ITI_MODE mode)
{
    TRACE("");

    if (it == NULL)
        ERROR("it == NULL\n", 1, "");

    if (list2d_iterator_init((List2D *)slist_get_list(list),
            (List2D_iterator *)slist_iterator_get_iterator(it), mode))
    {
        ERROR("list_iterator_init error\n", 1, "");
    }

    SLIST_ITERATOR_WRAPPERS_ASSIGN(it);

    return 0;
}