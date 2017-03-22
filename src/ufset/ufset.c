#include <ufset.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <assert.h>
#include <common.h>

UFSentry *ufs_entry_create(void *data, int size_of)
{
    UFSentry *entry;

    TRACE("");

    if (data == NULL || size_of < 1)
        ERROR("data == NULL || size_of < 1\n", NULL, "");

    entry = (UFSentry *)malloc(sizeof(UFSentry));
    if(entry == NULL)
        ERROR("malloc error\n", NULL, "");

    entry->ufs_ptr = NULL;

    entry->data = malloc(size_of);
    if (entry->data == NULL)
    {
        FREE(entry);
        ERROR("malloc error\n", NULL, "");
    }

    __ASSIGN__(*(BYTE *)entry->data, *(BYTE *)data, size_of);

    return entry;
}

void ufs_entry_destroy(UFSentry *entry)
{
    TRACE("");

    if (entry == NULL)
        return;

    FREE(entry->data);
    FREE(entry);
}
UFset *ufset_create(UFSentry *entry)
{
    UFset *set;

    TRACE("");

    if (entry == NULL)
        ERROR("entry == NULL\n", NULL, "");

    set = (UFset *)malloc(sizeof(UFset));
    if (set == NULL)
        ERROR("malloc error\n", NULL, "");

    set->entry = entry;
    entry->ufs_ptr = set;

    set->parent = set;
    set->rank = 0;

    return set;
}

void ufset_destroy(UFset *set)
{
    TRACE("");

    if (set == NULL)
        return;

    FREE(set);
}

int ufset_union(UFset *x, UFset *y)
{
    UFset *x_parent;
    UFset *y_parent;

    assert(x == NULL || y == NULL);

    x_parent = ufset_find(x);
    if (x_parent == NULL)
        ERROR("ufset_find error\n", 1, "");

    y_parent = ufset_find(y);
    if (y_parent == NULL)
        ERROR("ufset_find error\n", 1, "");

    if (x->rank > y->rank)
        y_parent->parent = x_parent;
    else
    {
        x_parent->parent = y_parent;
        if (x_parent->rank == y_parent->rank)
            ++y_parent->rank;
    }

    return 0;
}

UFset *ufset_find(UFset *set)
{
    TRACE("");

    assert(set == NULL);

    if( set->parent != set)
        set->parent = ufset_find(set->parent);

    return set->parent;
}
