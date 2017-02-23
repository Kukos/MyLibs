#include <list.h>
#include <generic.h>
#include <stdlib.h>
#include <log.h>

#define FREE(PTR) \
    do { \
        free(PTR); \
        PTR = NULL; \
    } while (0)

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
static List_node *list_node_create(List_node *next, void* data, int size_of);


/*
    Destory node

    PARAMS
    @IN node -ptr on list_node

    RETURN:
    This is void function
*/
static void list_node_destroy(List_node *node);


static List_node *list_node_create(List_node *next, void *data, int size_of)
{
    List_node *node;

    TRACE("");

    if (data == NULL)
        ERROR("data == NULL\n", NULL, "");

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL, "");

    node = (List_node *)malloc(sizeof(List_node));
    if (node == NULL)
        ERROR("malloc error\n", NULL, "");

    node->data = malloc(size_of);
    if (node->data == NULL)
	{
		FREE(node);
        ERROR("malloc error\n", NULL, "");
	}

    node->next = next;
    __ASSIGN__(*(BYTE *)node->data, *(BYTE *)data,size_of);

    return node;
}

static void list_node_destroy(List_node *node)
{
    TRACE("");

    if (node == NULL)
    {
        LOG("node == NULL\n", "");
        return;
    }

    FREE(node->data);
    FREE(node);
}

List_iterator *list_iterator_create(List *list, ITI_MODE mode)
{
    List_iterator *iterator;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    iterator = (List_iterator *)malloc(sizeof(List_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL, "");

    iterator->node = list->head;
    iterator->size_of = list->size_of;

    (void)mode;

    return iterator;
}

void list_iterator_destroy(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
    {
        LOG("iterator == NULL\n", "");
        return;
    }

    FREE(iterator);
}

int list_iterator_init(List *list, List_iterator *iterator, ITI_MODE mode)
{
    TRACE("");

    if (list == NULL || iterator == NULL)
        ERROR("list || iterator == NULL\n", 1, "");

    iterator->node = list->head;
    iterator->size_of = list->size_of;

    (void)mode;

    return 0;
}

int list_iterator_next(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1, "");

    iterator->node = iterator->node->next;

    return 0;
}

int list_iterator_get_data(List_iterator *iterator, void *val)
{
    TRACE("");

    if (iterator == NULL || val == NULL)
        ERROR("iterator || val  == NULL\n", 1, "");

    __ASSIGN__(*(BYTE *)val, *(BYTE *)iterator->node->data, iterator->size_of);

    return 0;
}

BOOL list_iterator_end(List_iterator *iterator)
{
    TRACE("");

    if (iterator == NULL)
        ERROR("iterator ==NULL\n", TRUE, "");

    return iterator->node == NULL;
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

    list->cmp = cmp;
    list->size_of = size_of;

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    return list;
}

void list_destroy(List *list)
{
    List_node *ptr;
    List_node *next;

    TRACE("");

    if (list == NULL)
    {
        LOG("list == NULL\n", "");
        return;
    }

    ptr = list->head;

    while (ptr != NULL)
    {
        next = ptr->next;
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

    if (list->head == NULL)
    {
        node = list_node_create(NULL, entry, list->size_of);

        if (node == NULL)
            ERROR("list_node_create error\n", 1, "");

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
            ERROR("list_node_create error\n", 1, "");

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

int list_delete(List *list, void *entry)
{
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

	if (list->head == NULL )
		ERROR("Nothing to delete\n", 0, "");

    ptr = list->head;
    prev = NULL;

    /* we add guardian */
    guard = list_node_create(NULL, entry, list->size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", 1, "");

    list->tail->next = guard;

    /* skip all entries with key < than entry */
    while (list->cmp(ptr->data,entry) < 0)
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

    return 0;
}

int list_delete_all(List *list, void *entry)
{
    List_node *ptr;
    List_node *prev;
    List_node *guard;

    int deleted;

    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", -1, "");

    ptr = list->head;
    prev= NULL;

    /* we add guardian */
    guard = list_node_create(NULL, entry, list->size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", -1, "");

    list->tail->next = guard;

    /* skip all entries with key < than entry */
    while (list->cmp(ptr->data,entry) < 0)
    {
        prev = ptr;
        ptr = ptr->next;
    }

    deleted = 0;

    /* we find entry to delete */
    if (ptr != guard && list->cmp(ptr->data,entry) == 0)
    {
        /* we delete head node */
        if (prev ==  NULL)
        {
            while (list->cmp(list->head->data, entry) == 0 && list->head != guard)
            {
                ptr = list->head->next;
                list_node_destroy(list->head);
                list->head = ptr;

                ++deleted;
            }

            if (ptr == guard)
                list->head = NULL;
        }
        else
        {
            while (list->cmp(ptr, entry) == 0 && ptr != guard)
            {
                prev->next = ptr->next;

                if (ptr == list->tail)
                    list->tail = prev;

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

    return deleted;
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

    if (list1->size_of != list2->size_of)
        ERROR("size_of are diffrent\n", NULL, "");

    list3 = list_create(list1->size_of, list1->cmp);
    if(list3 == NULL)
        ERROR("list_create error\n", NULL, "");

    ptr1 = list1->head;
    ptr2 = list2->head;

    if (ptr1 != NULL && ptr2 != NULL)
    {
        if (list3->cmp(ptr1->data,ptr2->data) < 0)
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
    else
    {
        node = list_node_create(NULL, ptr2->data, list3->size_of);
        list3->head = node;
        ptr2 = ptr2->next;
    }

    list3->tail = list3->head;
    list3->tail->next = NULL;

    while (ptr1 != NULL && ptr2 != NULL)
    {
        if (list3->cmp(ptr1->data,ptr2->data) < 0)
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

int list_search(List *list, void *val, void *entry)
{
    List_node *ptr;
    List_node *guard;

    TRACE("");

    if (list == NULL || val == NULL || entry == NULL)
        ERROR("list == NULL || val == NULL || entry  == NULL\n", 1, "");

    ptr = list->head;

    /* we add guardian */
    guard = list_node_create(NULL, val, list->size_of);
    if (guard == NULL)
        ERROR("list_node_create error\n", 1, "");

    list->tail->next = guard;

    /* skip all entries with key < than entry */
    while (list->cmp(ptr->data, val) < 0)
        ptr = ptr->next;

    /* we find entry */
    if (ptr != guard)
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

int list_to_array(List *list, void *array, int *size)
{
    List_node *ptr;
    BYTE *t;

    int offset;

    TRACE("");

    if (list == NULL || array == NULL || size == NULL)
        ERROR("list == NULL || array == NULL || size  == NULL\n", 1, "");

    t = (BYTE *)malloc(list->length * list->size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

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
