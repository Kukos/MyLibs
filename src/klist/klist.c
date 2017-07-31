#include <klist.h>
#include <log.h>
#include <stdlib.h>
#include <common.h>

/*
    is KList empty ?

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    true iff klist is empty
    false iff klist is not empty
*/
static ___inline___ bool klist_is_empty(KList_master *list);

/*
    Insert new node between prev and next nodes

    PARAMS
    @IN list - pointer to KList_master
    @IN _new - new node
    @IN prev - prev of new node
    @IN next - next of new node
    @IN change_head - Can we changed_head ?

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static ___inline___ int klist_insert(KList_master *list, KList *_new, KList *prev, KList *next, bool change_head);

/*
    Get Klist from @list on position @pos

    PARAMS
    @IN list - pointer to KList_master
    @IN pos - pos

    RETURN
    NULL iff failure
    Pointer to KList iff success
*/
static ___inline___ KList *klist_pos(KList_master *list, size_t pos);

static ___inline___ bool klist_is_empty(KList_master *list)
{
	return !CAST_TO_BOOL(list->____length);
}

static ___inline___ int klist_insert(KList_master *list, KList *_new, KList *prev, KList *next, bool change_head)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (_new == NULL || prev == NULL || next == NULL)
        ERROR("new == NULL || prev == NULL || next == NULL\n", 1, "");

    if (_new->____parent != NULL)
        ERROR("Node has parent, please delete node from list\n", 1, "");

    if (klist_is_empty(list))
        list->____head = _new;
    else
    {
        if (change_head && list->____head == next)
            list->____head = _new;
    }

	next->____prev = _new;
	_new->____next = next;
	_new->____prev = prev;
	prev->____next = _new;

    _new->____parent = list;

    ++list->____length;

    return 0;
}

static ___inline___ int klist_delete(KList *entry)
{
    TRACE("");

    if (entry == NULL)
        ERROR("entry == NULL\n", 1, "");

    entry->____next->____prev = entry->____prev;
    entry->____prev->____next = entry->____next;

    --entry->____parent->____length;

    if (klist_is_empty(entry->____parent))
        entry->____parent->____head = NULL;
    else
    {
        if (entry->____parent->____head == entry)
            entry->____parent->____head = entry->____next;
    }

    entry->____parent = NULL;

    return 0;
}

static ___inline___ KList *klist_pos(KList_master *list, size_t pos)
{
    KList *ptr;
    size_t i;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    if (klist_is_empty(list))
        return NULL;

    if (list->____length < pos)
        ERROR("list is shorten than %zu\n", NULL, pos);

    for (i = 0, ptr = list->____head; i < pos; ++i, ptr = ptr->____next)
        ;

    return ptr;
}

int klist_insert_first(KList_master *list, KList *entry)
{
    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

   if (!klist_is_empty(list))
        return klist_insert(list, entry, list->____head->____prev, list->____head, true);
    else
        return klist_insert(list, entry, entry, entry, true);
}

int klist_insert_last(KList_master *list, KList *entry)
{
    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

    if (!klist_is_empty(list))
        return klist_insert(list, entry, list->____head->____prev, list->____head, false);
    else
        return klist_insert(list, entry, entry, entry, true);
}

int klist_insert_before(KList *node, KList *entry)
{
    TRACE("");

    if (node == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

    if (node->____parent == NULL)
        ERROR("Node hasn't parent\n", 1, "");

    return klist_insert(node->____parent, entry, node->____prev, node, true);
}

int klist_insert_after(KList *node, KList *entry)
{
    TRACE("");

    if (node == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

    if (node->____parent == NULL)
        ERROR("Node hasn't parent\n", 1, "");

    return klist_insert(node->____parent, entry, node, node->____next, false);
}

int klist_insert_pos(KList_master *list, size_t pos, KList *entry)
{
    KList *ptr;

    TRACE("");

    if (list == NULL || entry == NULL)
        ERROR("list == NULL || entry == NULL\n", 1, "");

    if (klist_is_empty(list))
    {
        if (pos == 0)
            return klist_insert(list, entry, entry, entry, true);
        else
            ERROR("pos is invalif\n", 1, "");
    }

    ptr = klist_pos(list, pos);
    if (ptr == NULL)
        ERROR("pos is invalid\n", 1, "");

    return klist_insert(list, entry, ptr->____prev, ptr, pos == 0);
}

int klist_delete_first(KList_master *list)
{
    KList *ptr;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (klist_is_empty(list))
        ERROR("list is empty\n", 1, "");

    ptr = klist_pos(list, 0);
    return klist_delete(ptr);
}

int klist_delete_last(KList_master *list)
{
    KList *ptr;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (klist_is_empty(list))
        ERROR("list is empty\n", 1, "");

    ptr = klist_pos(list, 0);
    return klist_delete(ptr->____prev);
}

int klist_delete_node(KList *node)
{
    TRACE("");

    if (node == NULL)
        ERROR("node == NULL\n", 1, "");

    if (node->____parent == NULL)
        ERROR("Node hasn't parent\n", 1, "");

    return klist_delete(node);
}

int klist_delete_pos(KList_master *list, size_t pos)
{
    KList *ptr;

    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", 1, "");

    if (klist_is_empty(list))
        ERROR("list is empty\n", 1, "");

    ptr = klist_pos(list, pos);
    if (ptr == NULL)
        ERROR("Invalid pos\n", 1, "");

    return klist_delete(ptr);
}

KList *klist_get_pos(KList_master *list, size_t pos)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    return klist_pos(list, pos);
}

ssize_t klist_get_num_entries(KList_master *list)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", -1, "");

    return (ssize_t)list->____length;
}

KList *klist_get_head(KList_master *list)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    if (klist_is_empty(list))
        return NULL;
    else
        return list->____head;
}

KList *klist_get_tail(KList_master *list)
{
    TRACE("");

    if (list == NULL)
        ERROR("list == NULL\n", NULL, "");

    if (klist_is_empty(list))
        return NULL;
    else
        return list->____head->____prev;
}

KList_master *klist_master_create(void)
{
    KList_master *list;

    TRACE("");

    list = (KList_master *)malloc(sizeof(KList_master));
    if (list == NULL)
        ERROR("malloc error\n", NULL, "");

    list->____head = NULL;
    list->____length = 0;

    return list;
}

void klist_master_destroy(KList_master *list)
{
    TRACE("");

    if (list == NULL)
        return;

    FREE(list);
}