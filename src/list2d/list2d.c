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
__inline__ static List2D_node *list2d_node_create(  List2D_node     *prev,
                                                    List2D_node     *next,
                                                    void            *data,
                                                    int size_of);

/*
    Destory and set node to NULL

    PARAMS
    @IN node -ptr on list_node

    RETURN:
    This is void function
*/
__inline__ static void list2d_node_destroy(List2D_node *node);

__inline__ static List2D_node *list2d_node_create(  List2D_node     *prev,
                                                    List2D_node     *next,
                                                    void            *data,
                                                    int size_of)
{
    List2D_node *node;

    TRACE("");

    assert(data == NULL || size_of < 1);

    node = (List2D_node *)malloc(sizeof(List2D_node));
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

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    return node;
}

__inline__ static void list2d_node_destroy(List2D_node *node)
{
    TRACE("");

    if (node == NULL)
        return;

    FREE(node->data);
    FREE(node);
}

List2D_iterator *list2d_iterator_create(List2D *list, ITI_MODE mode)
{
    List2D_iterator *iterator;

    TRACE("");

    assert(list == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator = (List2D_iterator *)malloc(sizeof(List2D_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    iterator->first_time = true;

    if (mode == ITI_BEGIN)
    {
        iterator->node = list->head;
        iterator->end_node = list->head;
    }
    else
    {
        iterator->node = list->head->prev;
        iterator->end_node = list->head->prev;
    }

    iterator->size_of = list->size_of;

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

    assert(list == NULL || iterator == NULL);
    assert(mode != ITI_BEGIN && mode != ITI_END);

    iterator->first_time = true;

    if (mode == ITI_BEGIN)
    {
        iterator->node = list->head;
        iterator->end_node = list->head;
    }
    else
    {
        iterator->node = list->head->prev;
        iterator->end_node = list->head->prev;
    }

    iterator->size_of = list->size_of;

    return 0;
}

int list2d_iterator_next(List2D_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    if (iterator->first_time)
        iterator->first_time = false;

    iterator->node = iterator->node->next;

    return 0;
}

int list2d_iterator_prev(List2D_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    if (iterator->first_time)
        iterator->first_time = false;

    iterator->node = iterator->node->prev;

    return 0;
}

int list2d_iterator_get_data(List2D_iterator *iterator, void *val)
{
    TRACE("");

    assert(iterator == NULL || val == NULL);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

bool list2d_iterator_end(List2D_iterator *iterator)
{
    TRACE("");

    assert(iterator == NULL);

    return ((!iterator->first_time) && iterator->node == iterator->end_node);
}

List2D *list2d_create(int size_of, int (*cmp)(void* a, void *b),
    int (*diff)(void* a, void* b))
{
    List2D *list;

    TRACE("");

    assert(size_of < 1 || cmp == NULL);

    list = (List2D *)malloc(sizeof(List2D));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    list->cmp       = cmp;
    list->size_of   = size_of;
    list->diff      = diff;
    list->length    = 0;
    list->head      = NULL;

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

    ptr = list->head;
    end = list->head->prev;

    while (ptr != end)
    {
        next = ptr->next;
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

    ptr = list->head;
    end = list->head->prev;

    while (ptr != end)
    {
        next = ptr->next;
        destructor(ptr->data);
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

    assert(list == NULL || entry == NULL);

    /* special case - empty list */
    if (list->head == NULL)
    {
        node = list2d_node_create(NULL, NULL, entry,list->size_of);
        if (node == NULL)
            ERROR("malloc error\n", 1, "");

        node->prev = node;
        node->next = node;
        list->head = node;

        ++list->length;
    }
    else
    {
        /* it is better to go from begining */
        if (list->diff == NULL || list->diff(list->head->data, entry) >=
        list->diff(list->head->prev->data, entry))
        {
            ptr = list->head;

            /* add guardian at the end of list */
            guard = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
            if (guard == NULL)
                ERROR("malloc error", 1, "");

            list->head->prev->next  = guard;
            list->head->prev        = guard;

            /* skip all entries < than new entry */
            while (list->cmp(ptr->data, entry) < 0)
                ptr = ptr->next;

            /* we need stable property so we skip all entries with the same key */
            while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
                ptr = ptr->next;

            new_node = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
            if (new_node == NULL)
                ERROR("malloc error", 1, "");

            ptr->prev->next     = new_node;
            ptr->prev           = new_node;

            if (ptr == list->head)
                list->head = new_node;

            ++list->length;

            list->head->prev    = guard->prev;
            guard->prev->next   = list->head;

            list2d_node_destroy(guard);
        }
        else
        {
            ptr = list->head->prev;

            /* add guardian at the end of list */
            guard = list2d_node_create(ptr, ptr->next, entry, list->size_of);
            if (guard == NULL)
                ERROR("list2d_node create error", 1, "");

            ptr->next           = guard;
            list->head->prev    = guard;

            /* skip all entries > than new entry */
            while (list->cmp(ptr->data, entry) > 0)
                ptr = ptr->prev;

            /* we need stable property so we skip all entries with the same key */
            while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
                ptr = ptr->prev;

            new_node = list2d_node_create(ptr, ptr->next, entry, list->size_of);
            if (new_node == NULL)
                ERROR("list2d_node_create error", 1, "");

            ptr->next->prev     = new_node;
            ptr->next           = new_node;

            if (ptr == guard)
                list->head = new_node;

            ++list->length;

            list->head->prev    = guard->prev;
            guard->prev->next   = list->head;

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

    assert(list == NULL || entry == NULL);

    if (list->head == NULL)
		ERROR("Nothing to delete\n", 0, "");

    if (list->diff == NULL || list->diff(list->head->data, entry)
    >= list->diff(list->head->prev->data, entry))
    {
        ptr = list->head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->head->prev->next  = guard;
        list->head->prev        = guard;

        /* skip all entries < than new entry */
        while (list->cmp(ptr->data, entry) < 0)
            ptr = ptr->next;
    }
    else
    {
        ptr = list->head->prev;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr, ptr->next, entry, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->head->prev->next = guard;
        list->head->prev = guard;

        /* skip all entries > than new entry */
        while (list->cmp(ptr->data, entry) > 0)
            ptr = ptr->prev;
    }

    if (ptr != guard && list->cmp(ptr->data, entry) == 0)
    {
        if (ptr == list->head)
            list->head = ptr->next;

        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;

        list2d_node_destroy(ptr);
    }
    else
    {
        /* delete guardian */
        list->head->prev = guard->prev;
        guard->prev->next = list->head;

        list2d_node_destroy(guard);

        return 1;
    }

    --list->length;

    list->head->prev = guard->prev;
    guard->prev->next = list->head;

    list2d_node_destroy(guard);

    return 0;
}

int list2d_delete_all(List2D *list, void *entry)
{
    List2D_node     *guard;
    List2D_node     *ptr;
    List2D_node     *temp;

    size_t          deleted;

    TRACE("");

    assert(list == NULL || entry == NULL);

    if (list->head == NULL)
		ERROR("Nothing to delete\n", 0, "");

    deleted = 0;

    if (list->diff == NULL || list->diff(list->head->data, entry)
    >= list->diff(list->head->prev->data, entry))
    {
        ptr = list->head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->head->prev->next = guard;
        list->head->prev = guard;

        /* skip all entries < than new entry */
        while (list->cmp(ptr->data, entry) < 0)
            ptr = ptr->next;

        if (ptr != guard && list->cmp(ptr->data, entry) == 0)
        {
            while (list->cmp(ptr->data,entry) == 0)
            {
                if (ptr == list->head)
                    list->head = ptr->next;

                temp = ptr->next;

                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;

                list2d_node_destroy(ptr);

                ptr = temp;

                ++deleted;
            }
        }
        else
        {
            list->head->prev = guard->prev;
            guard->prev->next = list->head;

            list2d_node_destroy(guard);

            return 0;
        }
    }
    else
    {
        ptr = list->head->prev;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr, ptr->next, entry, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->head->prev->next = guard;
        list->head->prev = guard;

        /* skip all entries > than new entry */
        while (list->cmp(ptr->data,entry) > 0)
            ptr = ptr->prev;

        if (ptr != guard && list->cmp(ptr->data,entry) == 0)
        {
            while (list->cmp(ptr->data, entry) == 0)
            {
                if (ptr == list->head)
                    list->head = ptr->next;

                temp = ptr->prev;

                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;

                list2d_node_destroy(ptr);

                ptr = temp;

                ++deleted;
            }
        }
        else
        {
            list->head->prev = guard->prev;
            guard->prev->next = list->head;

            list2d_node_destroy(guard);

            return 0;
        }
    }

    list->length -= deleted;

    list->head->prev = guard->prev;
    guard->prev->next = list->head;

    list2d_node_destroy(guard);

    return deleted;
}

int list2d_search(List2D *list, void *val, void *entry)
{
    List2D_node *guard;
    List2D_node *ptr;

    TRACE("");

    assert(list == NULL || val == NULL || entry == NULL);

    if (list->head == NULL)
        ERROR("List is empty\n", 1, "");

    if (list->diff == NULL || list->diff(list->head->data, entry)
    >= list->diff(list->head->prev->data, entry))
    {
        ptr = list->head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->prev, ptr, val, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->head->prev->next = guard;

        /* skip all entries < than new entry */
        while (list->cmp(ptr->data, val) < 0)
            ptr = ptr->next;
    }
    else
    {
        ptr = list->head->prev;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr, ptr->next, val, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error", 1, "");

        list->head->prev = guard;

        /* skip all entries > than new entry */
        while (list->cmp(ptr->data, val) > 0)
            ptr = ptr->prev;
    }

    /* we find entry */
    if (ptr != guard && list->cmp(ptr->data, val) == 0)
    {
        __ASSIGN__(*(BYTE *)entry, *(BYTE *)ptr->data, list->size_of);

        list->head->prev = guard->prev;
        guard->prev->next = list->head;

        list2d_node_destroy(guard);

        return 0;
    }
    else
    {
        list->head->prev = guard->prev;
        guard->prev->next = list->head;

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

    assert(list == NULL || array == NULL || size == NULL);

    t = (BYTE *)malloc(list->length * list->size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    ptr = list->head;
    end = list->head->prev;

    offset = 0;

    while (ptr != end)
    {
        __ASSIGN__(t[offset], *(BYTE *)ptr->data, list->size_of);
        offset += list->size_of;

        ptr = ptr->next;
    }

    __ASSIGN__(t[offset], *(BYTE *)end->data, list->size_of);

    *(void **)array = t;

    *size = list->length;

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

    assert(list1 == NULL || list2 == NULL);

    result = list2d_create(list1->size_of, list1->cmp, list1->diff);

    ptr1 = list1->head;
    ptr2 = list2->head;

    val = 0;

    /* add guardians */
    guard1 = list2d_node_create(ptr1->prev, ptr1, (void *)&val, sizeof(BYTE));
    list1->head->prev->next = guard1;

    guard2 = list2d_node_create(ptr2->prev, ptr2, (void *)&val, sizeof(BYTE));
    list2->head->prev->next = guard2;

    /* special case */
    if (ptr1 != NULL && ptr2 != NULL)
    {
        if (result->cmp(ptr1->data, ptr2->data) < 0)
        {
            node = list2d_node_create(NULL, NULL, ptr1->data, result->size_of);
            node->prev = node;
            node->next = node;

            result->head = node;

            ptr1 = ptr1->next;
        }
        else
        {
            node = list2d_node_create(NULL, NULL, ptr2->data, result->size_of);
            node->prev = node;
            node->next = node;

            result->head = node;

            ptr2 = ptr2->next;
        }
    }
    else if (ptr1 != NULL)
    {
        node = list2d_node_create(NULL, NULL, ptr1->data, result->size_of);
        node->prev = node;
        node->next = node;

        result->head = node;

        ptr1 = ptr1->next;
    }
    else if (ptr2 != NULL)
    {
        node = list2d_node_create(NULL, NULL, ptr2->data, result->size_of);
        node->prev = node;
        node->next = node;

        result->head = node;

        ptr2 = ptr2->next;
    }

    /* add to the end */
    while (ptr1 != guard1 && ptr2 != guard2)
    {
        if (result->cmp(ptr1->data, ptr2->data) < 0)
        {
            node = list2d_node_create(result->head->prev, result->head,
                ptr1->data, result->size_of);
            result->head->prev->next = node;
            result->head->prev = node;

            ptr1 = ptr1->next;
        }
        else
        {
            node = list2d_node_create(result->head->prev, result->head,
                ptr2->data, result->size_of);
            result->head->prev->next = node;
            result->head->prev = node;

            ptr2 = ptr2->next;
        }
    }

    while (ptr1 != guard1)
    {
        node = list2d_node_create(result->head->prev, result->head,
            ptr1->data, result->size_of);
        result->head->prev->next = node;
        result->head->prev = node;

        ptr1 = ptr1->next;
    }

    while (ptr2 != guard2)
    {
        node = list2d_node_create(result->head->prev, result->head,
            ptr2->data, result->size_of);
        result->head->prev->next = node;
        result->head->prev = node;

        ptr2 = ptr2->next;
    }

    /* delete guardians */
    list1->head->prev = guard1->prev;
    guard1->prev->next = list1->head;
    list2d_node_destroy(guard1);

    list2->head->prev = guard2->prev;
    guard2->prev->next = list2->head;
    list2d_node_destroy(guard2);

    result->length = list1->length + list2->length;

    return result;
}
