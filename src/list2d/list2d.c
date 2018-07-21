#include <list2d.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

struct List2D_node
{
    struct List2D_node      *prev;
    struct List2D_node      *next;
    size_t                  size_of;
    __extension__ BYTE      data[]; /* placeholder for data */
};

struct List2D
{
    size_t          size_of;
    size_t          length;

    List2D_node     *head;

    cmp_f cmp;
    diff_f diff;
    destructor_f destroy;
};

struct List2D_iterator
{
    List2D_node     *node;
    List2D_node     *end_node;
    size_t          size_of;
    bool            first_time;
};


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
___inline___ static List2D_node *list2d_node_create(    const List2D_node     * ___restrict___ prev,
                                                        const List2D_node     * ___restrict___ next,
                                                        const void            * ___restrict___ data,
                                                        size_t                size_of);

/*
    Destory node

    PARAMS
    @IN node - ptr on list_node

    RETURN:
    This is void function
*/
___inline___ static void list2d_node_destroy(List2D_node *node);

/*
    Destroy whole list2d

    PARAMS
    @IN list - pointer to List2D
    @IN destroy - call destructor ?

    RETURN
    This is a void function
*/
static void __list2d_destroy(List2D *list, bool destroy);

/*
    Delete first entry with key equal to @entry

    PARAMS
    @IN list - pointer to List2D
    @IN entry - pointer to fake entry with key
    @IN destroy - call destructor ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __list2d_delete(List2D * ___restrict___ list, const void * ___restrict___ entry, bool destroy);

/*
    Delete ALL entries with key equal to @entry

    PARAMS
    @IN list - pointer to List2D
    @IN entry - pointer to fake entry with key
    @IN destroy - call destructor ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static int __list2d_delete_all(List2D * ___restrict___ list, const void * ___restrict___ entry, bool destroy);

___inline___ static List2D_node *list2d_node_create(    const List2D_node     * ___restrict___ prev,
                                                        const List2D_node     * ___restrict___ next,
                                                        const void            * ___restrict___ data,
                                                        size_t                size_of)
{
    List2D_node *node;

    TRACE();

    assert(data != NULL);
    assert(size_of >= 1);

    node = (List2D_node *)malloc(sizeof(List2D_node) + size_of);
    if (node == NULL)
        ERROR("malloc error\n", NULL);

    node->next = (List2D_node *)next;
    node->prev = (List2D_node *)prev;
    node->size_of = size_of;

    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data, size_of);

    return node;
}

___inline___ static void list2d_node_destroy(List2D_node *node)
{
    TRACE();

    if (node == NULL)
        return;

    FREE(node);
}

static void __list2d_destroy(List2D *list, bool destroy)
{
    List2D_node *ptr;
    List2D_node *end;
    List2D_node *next;

    TRACE();

    if (list == NULL)
        return;

    if (list->length == 0)
    {
        FREE(list);
        return;
    }

    ptr = list->head;
    end = list->head->prev;

    while (ptr != end)
    {
        next = ptr->next;

        if (destroy && list->destroy != NULL)
            list->destroy((void *)ptr->data);

        list2d_node_destroy(ptr);
        ptr = next;
    }

    if (destroy && list->destroy != NULL)
        list->destroy((void *)end->data);

    list2d_node_destroy(end);
    FREE(list);
}

static int __list2d_delete(List2D * ___restrict___ list, const void * ___restrict___ entry, bool destroy)
{
    List2D_node *guard;
    List2D_node *ptr;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", 1);

    if (entry == NULL)
        ERROR("entry == NULL\n", 1);

    if (list->head == NULL)
		ERROR("Nothing to delete\n", 1);

    if (list->diff(list->head->data, entry) < list->diff(list->head->prev->data, entry))
    {
        ptr = list->head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error\n", 1);

        list->head->prev->next  = guard;
        list->head->prev            = guard;

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
            ERROR("list2d_node_create error\n", 1);

        list->head->prev->next = guard;
        list->head->prev = guard;

        /* skip all entries > than new entry */
        while (list->cmp(ptr->data, entry) > 0)
            ptr = ptr->prev;

        while (ptr != guard && ptr->next != guard && list->cmp(ptr->next->data, entry) == 0)
            ptr = ptr->prev;
    }

    if (ptr != guard && list->cmp(ptr->data, entry) == 0)
    {
        if (ptr == list->head)
            list->head = ptr->next;

        ptr->prev->next = ptr->next;
        ptr->next->prev = ptr->prev;

        if (destroy && list->destroy != NULL)
            list->destroy((void *)ptr->data);

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
    if (list->length == 0)
        list->head = NULL;
    else
    {
        list->head->prev = guard->prev;
        guard->prev->next = list->head;
    }

    list2d_node_destroy(guard);

    return 0;
}

static int __list2d_delete_all(List2D * ___restrict___ list, const void * ___restrict___ entry, bool destroy)
{
    List2D_node     *guard;
    List2D_node     *ptr;
    List2D_node     *temp;

    int             deleted;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", -1);

    if (entry == NULL)
        ERROR("entry == NULL\n", -1);

    if (list->head == NULL)
		ERROR("Nothing to delete\n", -1);

    deleted = 0;

    if (list->diff(list->head->data, entry) < list->diff(list->head->prev->data, entry))
    {
        ptr = list->head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error\n", 1);

        list->head->prev->next = guard;
        list->head->prev = guard;

        /* skip all entries < than new entry */
        while (list->cmp(ptr->data, entry) < 0)
            ptr = ptr->next;

        if (ptr != guard && list->cmp(ptr->data, entry) == 0)
        {
            while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
            {
                if (ptr == list->head)
                    list->head = ptr->next;

                temp = ptr->next;

                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;

                if (destroy && list->destroy != NULL)
                    list->destroy((void *)ptr->data);

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
            ERROR("list2d_node_create error\n", 1);

        list->head->prev->next = guard;
        list->head->prev = guard;

        /* skip all entries > than new entry */
        while (list->cmp(ptr->data,entry) > 0)
            ptr = ptr->prev;

        if (ptr != guard && list->cmp(ptr->data,entry) == 0)
        {
            while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
            {
                if (ptr == list->head)
                    list->head = ptr->next;

                temp = ptr->prev;

                ptr->prev->next = ptr->next;
                ptr->next->prev = ptr->prev;

                if (destroy && list->destroy != NULL)
                    list->destroy((void *)ptr->data);

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

    list->length -= (size_t)deleted;

    if (list->length == 0)
        list->head = NULL;
    else
    {
        list->head->prev = guard->prev;
        guard->prev->next = list->head;
    }

    list2d_node_destroy(guard);

    return deleted;
}

List2D *list2d_create(size_t size_of, cmp_f cmp, diff_f diff, destructor_f destroy)
{
    List2D *list;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    if (cmp == NULL)
        ERROR("cmp == NULL\n", NULL);

    if (diff == NULL)
        ERROR("diff == NULL\n", NULL);

    list = (List2D *)malloc(sizeof(List2D));
    if (list == NULL)
        ERROR("malloc error\n", NULL);

    list->cmp       = cmp;
    list->size_of   = size_of;
    list->diff      = diff;
    list->destroy   = destroy;
    list->length    = 0;
    list->head      = NULL;

    return list;
}

void list2d_destroy(List2D *list)
{
   TRACE();

   __list2d_destroy(list, false);
}

void list2d_destroy_with_entries(List2D *list)
{
   TRACE();

   __list2d_destroy(list, true);
}

int list2d_insert(List2D * ___restrict___ list, const void * ___restrict___ entry)
{
    List2D_node *node;
    List2D_node *ptr;
    List2D_node *guard;
    List2D_node *new_node;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", 1);

    if (entry == NULL)
        ERROR("entry == NULL\n", 1);

    /* special case - empty list */
    if (list->head == NULL)
    {
        node = list2d_node_create(NULL, NULL, entry, list->size_of);
        if (node == NULL)
            ERROR("malloc error\n", 1);

        node->prev = node;
        node->next = node;
        list->head = node;

        ++list->length;
    }
    else
    {
        /* it is better to go from begining */
        if (list->diff(list->head->data, entry) >= list->diff(list->head->prev->data, entry))
        {
            ptr = list->head;

            /* add guardian at the end of list */
            guard = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
            if (guard == NULL)
                ERROR("malloc error\n", 1);

            list->head->prev->next  = guard;
            list->head->prev            = guard;

            /* skip all entries < than new entry */
            while (list->cmp(ptr->data, entry) < 0)
                ptr = ptr->next;

            /* we need stable property so we skip all entries with the same key */
            while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
                ptr = ptr->next;

            new_node = list2d_node_create(ptr->prev, ptr, entry, list->size_of);
            if (new_node == NULL)
                ERROR("malloc error\n", 1);

            ptr->prev->next     = new_node;
            ptr->prev               = new_node;

            if (ptr == list->head)
                list->head = new_node;

            ++list->length;

            list->head->prev    = guard->prev;
            guard->prev->next       = list->head;

            list2d_node_destroy(guard);
        }
        else
        {
            ptr = list->head->prev;

            /* add guardian at the end of list */
            guard = list2d_node_create(ptr, ptr->next, entry, list->size_of);
            if (guard == NULL)
                ERROR("list2d_node create error\n", 1);

            ptr->next           = guard;
            list->head->prev    = guard;

            /* skip all entries > than new entry */
            while (list->cmp(ptr->data, entry) > 0)
                ptr = ptr->prev;

            new_node = list2d_node_create(ptr, ptr->next, entry, list->size_of);
            if (new_node == NULL)
                ERROR("list2d_node_create error\n", 1);

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

int list2d_delete(List2D *list, const void * ___restrict___ entry)
{
    TRACE();

    return __list2d_delete(list, entry, false);
}

int list2d_delete_with_entry(List2D *list, const void * ___restrict___ entry)
{
    TRACE();

    return __list2d_delete(list, entry, true);
}

int list2d_delete_all(List2D *list, const void * ___restrict___ entry)
{
    TRACE();

    return __list2d_delete_all(list, entry, false);
}

int list2d_delete_all_with_entry(List2D *list, const void * ___restrict___ entry)
{
    TRACE();

    return __list2d_delete_all(list, entry, true);
}

int list2d_search(const List2D * ___restrict___ list, const void *val, void *entry)
{
    List2D_node *guard;
    List2D_node *ptr;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", 1);

    if (val == NULL)
        ERROR("val == NULL\n", 1);

    if (entry == NULL)
        ERROR("entry == NULL\n", 1);

    if (list->head == NULL)
        ERROR("List is empty\n", 1);

    if (list->diff(list->head->data, entry) < list->diff(list->head->prev->data, entry))
    {
        ptr = list->head;

        /* add guardian at the end of list */
        guard = list2d_node_create(ptr->prev, ptr, val, list->size_of);
        if (guard == NULL)
            ERROR("list2d_node_create error\n", 1);

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
            ERROR("list2d_node_create error\n", 1);

        list->head->prev = guard;

        /* skip all entries > than new entry */
        while (list->cmp(ptr->data, val) > 0)
            ptr = ptr->prev;

        while (list->cmp(ptr->data, entry) == 0 && ptr != guard)
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

int list2d_to_array(const List2D * ___restrict___ list, void * ___restrict___ array, size_t * ___restrict___ size)
{
    List2D_node     *ptr;
    List2D_node     *end;
    BYTE            *t;
    size_t          offset;

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

List2D *list2d_merge(const List2D * ___restrict___ list1, const List2D * ___restrict___ list2)
{
    List2D          *result;

    List2D_node     *ptr1;
    List2D_node     *ptr2;
    List2D_node     *guard1;
    List2D_node     *guard2;
    List2D_node     *node;

    BYTE            val;

    if (list1 == NULL)
        ERROR("list1 == NULL\n", NULL);

    if (list2 == NULL)
        ERROR("list2 == NULL\n", NULL);

    result = list2d_create(list1->size_of, list1->cmp, list1->diff, list1->destroy);
    if (result == NULL)
        ERROR("list_create error\n", NULL);

    ptr1 = list1->head;
    ptr2 = list2->head;

    if (ptr1 == NULL && ptr2 == NULL)
        return result;

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
            node = list2d_node_create(result->head->prev, result->head, ptr1->data, result->size_of);
            result->head->prev->next = node;
            result->head->prev = node;

            ptr1 = ptr1->next;
        }
        else
        {
            node = list2d_node_create(result->head->prev, result->head, ptr2->data, result->size_of);
            result->head->prev->next = node;
            result->head->prev = node;

            ptr2 = ptr2->next;
        }
    }

    while (ptr1 != guard1)
    {
        node = list2d_node_create(result->head->prev, result->head, ptr1->data, result->size_of);
        result->head->prev->next = node;
        result->head->prev = node;

        ptr1 = ptr1->next;
    }

    while (ptr2 != guard2)
    {
        node = list2d_node_create(result->head->prev, result->head, ptr2->data, result->size_of);
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

ssize_t list2d_get_data_size(const List2D *list)
{
    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", -1);

    return (ssize_t)list->size_of;
}

ssize_t list2d_get_num_entries(const List2D *list)
{
    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", -1);

    return (ssize_t)list->length;
}

int list2d_node_get_data(const List2D_node * ___restrict___ node, void * ___restrict___ data)
{
    TRACE();

    if (node == NULL || data == NULL)
        ERROR("node == NULL || data == NULL\n", 1);

    __ASSIGN__(*(BYTE *)data, *(BYTE *)node->data, node->size_of);

    return 0;
}

List2D_iterator *list2d_iterator_create(const List2D *list, iti_mode_t mode)
{
    List2D_iterator *iterator;

    TRACE();

    if (list == NULL)
        ERROR("list == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Invalid mode\n", NULL);

    if (list->head == NULL)
        return NULL;

    iterator = (List2D_iterator *)malloc(sizeof(List2D_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

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
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int list2d_iterator_next(List2D_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);


    if (iterator->first_time)
        iterator->first_time = false;

    iterator->node = iterator->node->next;

    return 0;
}

int list2d_iterator_prev(List2D_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);


    if (iterator->first_time)
        iterator->first_time = false;

    iterator->node = iterator->node->prev;

    return 0;
}

int list2d_iterator_get_data(const List2D_iterator * ___restrict___ iterator, void * ___restrict___ val)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (val == NULL)
        ERROR("val == NULL\n", 1);

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

int list2d_iterator_get_node(const List2D_iterator * ___restrict___ iterator, void * ___restrict___ node)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (node == NULL)
        ERROR("node == NULL\n", 1);

    *(void **)node = iterator->node;

    return 0;
}

bool list2d_iterator_end(const List2D_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);


    return ((!iterator->first_time) && iterator->node == iterator->end_node);
}

SLIST_WRAPPERS_CREATE(List2D, list2d)

SList *slist_list2d_create(size_t size_of, cmp_f cmp, diff_f diff, destructor_f destroy)
{
    SList *list;

    TRACE();

    /* create SLIST */
    list = (SList *)malloc(sizeof(SList));
    if (list == NULL)
        ERROR("malloc error\n", NULL);

    /* create list */
    list->____list = (void *)list2d_create(size_of, cmp, diff, destroy);
    if (list->____list == NULL)
    {
        FREE(list);
        ERROR("list_create error\n", NULL);
    }

    /* fill hooks */
    SLIST_WRAPPERS_ASSIGN(list);

    return list;
}