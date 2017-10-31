#include <ufset.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <common.h>

UFSMaster *ufs_master_create(void)
{
    UFSMaster *master;

    TRACE();

    master = (UFSMaster *)malloc(sizeof(UFSMaster));
    if (master == NULL)
        ERROR("malloc error\n", NULL);

     master->____set = darray_create(DARRAY_UNSORTED, 0, sizeof(UFset *), NULL);
     if (master->____set == NULL)
     {
         FREE(master);
         ERROR("darray_create error\n", NULL);
     }

     master->____hight = 0;

     return master;
}

void ufs_master_destroy(UFSMaster *master)
{
    UFset *node;

    TRACE();

    if (master == NULL)
        return;

    for_each_data(master->____set, Darray, node)
        ufset_destroy(node);

    darray_destroy(master->____set);
    FREE(master);
}

void ufs_master_destroy_with_entries(UFSMaster *master, void (*destructor)(void *data))
{
    UFset *node;

    TRACE();

    if (master == NULL || destructor == NULL)
        return;

    for_each_data(master->____set, Darray, node)
        ufset_destroy_with_entries(node, destructor);

    darray_destroy(master->____set);
    FREE(master);
}

UFSentry *ufs_entry_create(void *data, int size_of)
{
    UFSentry *entry;

    TRACE();

    if (data == NULL || size_of < 1)
        ERROR("data == NULL || size_of < 1\n", NULL);

    entry = (UFSentry *)malloc(sizeof(UFSentry));
    if(entry == NULL)
        ERROR("malloc error\n", NULL);

    entry->____ufs_ptr = NULL;

    entry->____data = malloc((size_t)size_of);
    if (entry->____data == NULL)
    {
        FREE(entry);
        ERROR("malloc error\n", NULL);
    }

    __ASSIGN__(*(BYTE *)entry->____data, *(BYTE *)data, size_of);

    return entry;
}

void ufs_entry_destroy(UFSentry *entry)
{
    TRACE();

    if (entry == NULL)
        return;

    FREE(entry->____data);
    FREE(entry);
}

void ufs_entry_destroy_with_data(UFSentry *entry, void (*destructor)(void *data))
{
    TRACE();

    if (entry == NULL || destructor == NULL)
        return;

    destructor(entry->____data);
    ufs_entry_destroy(entry);
}

UFset *ufset_create(UFSentry *entry, UFSMaster *master)
{
    UFset *set;

    TRACE();

    if (entry == NULL)
        ERROR("entry == NULL\n", NULL);

    if (master == NULL)
        ERROR("master == NULL\n", NULL);

    set = (UFset *)malloc(sizeof(UFset));
    if (set == NULL)
        ERROR("malloc error\n", NULL);

    set->____entry = entry;
    entry->____ufs_ptr = set;

    set->____parent = set;
    set->____rank = 0;
    set->____master = master;

    if (darray_insert(master->____set, (void *)&set))
        ERROR("darray insert error\n", NULL);

    set->____master->____hight = MAX(set->____master->____hight, 1);

    return set;
}

void ufset_destroy(UFset *set)
{
    TRACE();

    if (set == NULL)
        return;

    ufs_entry_destroy(set->____entry);
    FREE(set);
}

void ufset_destroy_with_entries(UFset *set, void (*destructor)(void *data))
{
    TRACE();

    if (set == NULL || destructor == NULL)
        return;

    ufs_entry_destroy_with_data(set->____entry, destructor);
    FREE(set);
}

int ufset_union(UFset *x, UFset *y)
{
    UFset *x_parent;
    UFset *y_parent;

    if (x == NULL)
        ERROR("x == NULL\n", 1);

    if (y == NULL)
        ERROR("y == NULL\n", 1);

    if (x->____master != y->____master)
        ERROR("Masters are not the same\n", 1);

    x_parent = ufset_find(x);
    if (x_parent == NULL)
        ERROR("ufset_find error\n", 1);

    y_parent = ufset_find(y);
    if (y_parent == NULL)
        ERROR("ufset_find error\n", 1);

    if (x_parent->____rank > y_parent->____rank)
        y_parent->____parent = x_parent;
    else
    {
        x_parent->____parent = y_parent;
        if (x_parent->____rank == y_parent->____rank)
        {
            ++y_parent->____rank;
            y_parent->____master->____hight = MAX(ufset_get_hight(y_parent),  y_parent->____master->____hight);
        }
    }

    return 0;
}

UFset *ufset_find(UFset *set)
{
    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", NULL);

    if (set->____parent != set)
        set->____parent = ufset_find(set->____parent);

    return set->____parent;
}

int ufset_get_hight(UFset *set)
{
    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", -1);

    return ufset_find(set)->____rank + 1;
}

ssize_t ufset_get_num_entries(UFset *set)
{
    UFSMaster *master;
    UFset *node;
    UFset *parent;
    size_t counter = 0;

    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", (ssize_t)-1);

    parent = ufset_find(set);
    if (parent == NULL)
        ERROR("ufs_find error\n", (ssize_t)-1);

    master = set->____master;
    for_each_data(master->____set, Darray, node)
        if (parent == ufset_find(node))
            ++counter;

    return (ssize_t)counter;
}

ssize_t ufs_master_get_num_entries(UFSMaster *master)
{
    TRACE();

    if (master == NULL)
        ERROR("master == NULL\n", (ssize_t)-1);

    return darray_get_num_entries(master->____set);
}

int ufs_master_get_hight(UFSMaster *master)
{
    TRACE();

    if (master == NULL)
        ERROR("master == NULL\n", -1);

    return master->____hight;
}