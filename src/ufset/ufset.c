#include <ufset.h>
#include <generic.h>
#include <log.h>
#include <stdlib.h>
#include <common.h>
#include <darray.h>

struct UFSMaster
{
    Darray  *set;
    int     hight;
};

struct UFSentry
{
    struct UFset        *ufs_ptr;
    size_t              size_of;
    destructor_f        destroy;
    __extension__ BYTE  data[]; /* placeholder for data */

};

struct UFset
{
    struct UFset    *parent;
    UFSentry        *entry;
    UFSMaster       *master;

    int             rank;
};

UFSMaster *ufs_master_create(void)
{
    UFSMaster *master;

    TRACE();

    master = (UFSMaster *)malloc(sizeof(UFSMaster));
    if (master == NULL)
        ERROR("malloc error\n", NULL);

     master->set = darray_create(DARRAY_UNSORTED, 0, sizeof(UFset *), NULL, NULL);
     if (master->set == NULL)
     {
         FREE(master);
         ERROR("darray_create error\n", NULL);
     }

     master->hight = 0;

     return master;
}

void ufs_master_destroy(UFSMaster *master)
{
    UFset *node;

    TRACE();

    if (master == NULL)
        return;

    for_each_data(master->set, Darray, node)
        ufset_destroy(node);

    darray_destroy(master->set);
    FREE(master);
}

void ufs_master_destroy_with_entries(UFSMaster *master)
{
    UFset *node;

    TRACE();

    if (master == NULL)
        return;

    for_each_data(master->set, Darray, node)
        ufset_destroy_with_entries(node);

    darray_destroy(master->set);
    FREE(master);
}

UFSentry *ufs_entry_create(const void *data, size_t size_of, destructor_f destroy)
{
    UFSentry *entry;

    TRACE();

    if (data == NULL || size_of < 1)
        ERROR("data == NULL || size_of < 1\n", NULL);

    entry = (UFSentry *)malloc(sizeof(UFSentry) + size_of);
    if(entry == NULL)
        ERROR("malloc error\n", NULL);

    entry->ufs_ptr = NULL;
    entry->destroy = destroy;
    entry->size_of = size_of;

    __ASSIGN__(*(BYTE *)entry->data, *(BYTE *)data, size_of);

    return entry;
}

void ufs_entry_destroy(UFSentry *entry)
{
    TRACE();

    if (entry == NULL)
        return;

    FREE(entry);
}

void ufs_entry_destroy_with_data(UFSentry *entry)
{
    TRACE();

    if (entry == NULL)
        return;

    if (entry->destroy != NULL)
        entry->destroy((void *)entry->data);

    ufs_entry_destroy(entry);
}

UFset *ufset_entry_get_ufset(const UFSentry *entry)
{
    TRACE();

    if (entry == NULL)
        ERROR("entry == NULL\n", NULL);

    return entry->ufs_ptr;
}

int ufset_entry_get_data(const UFSentry * ___restrict___ entry, void * ___restrict___ data)
{
    TRACE();

    if (entry == NULL)
        ERROR("entry == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    __ASSIGN__(*(BYTE *)data, entry->data[0], entry->size_of);

    return 0;
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

    set->entry = (UFSentry *)entry;
    entry->ufs_ptr = set;

    set->parent = set;
    set->rank = 0;
    set->master = master;

    if (darray_insert(master->set, (void *)&set))
        ERROR("darray insert error\n", NULL);

    set->master->hight = MAX(set->master->hight, 1);

    return set;
}

void ufset_destroy(UFset *set)
{
    TRACE();

    if (set == NULL)
        return;

    ufs_entry_destroy(set->entry);
    FREE(set);
}

void ufset_destroy_with_entries(UFset *set)
{
    TRACE();

    if (set == NULL)
        return;

    ufs_entry_destroy_with_data(set->entry);
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

    if (x->master != y->master)
        ERROR("Masters are not the same\n", 1);

    x_parent = ufset_find(x);
    if (x_parent == NULL)
        ERROR("ufset_find error\n", 1);

    y_parent = ufset_find(y);
    if (y_parent == NULL)
        ERROR("ufset_find error\n", 1);

    if (x_parent->rank > y_parent->rank)
        y_parent->parent = x_parent;
    else
    {
        x_parent->parent = y_parent;
        if (x_parent->rank == y_parent->rank)
        {
            ++y_parent->rank;
            y_parent->master->hight = MAX(ufset_get_hight(y_parent),  y_parent->master->hight);
        }
    }

    return 0;
}

UFset *ufset_find(UFset *set)
{
    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", NULL);

    if (set->parent != set)
        set->parent = ufset_find(set->parent);

    return set->parent;
}

int ufset_get_hight(UFset *set)
{
    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", -1);

    return ufset_find(set)->rank + 1;
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

    master = set->master;
    for_each_data(master->set, Darray, node)
        if (parent == ufset_find(node))
            ++counter;

    return (ssize_t)counter;
}

UFSMaster *ufset_get_master(const UFset *set)
{
    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", NULL);

    return set->master;
}

UFSentry *ufset_get_entry(const UFset *set)
{
    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", NULL);

    return set->entry;
}

UFSentry **ufset_get_entries(UFset *set, size_t *size)
{
    UFSentry **t;
    UFset *root;
    UFSMaster *master;
    UFset *node;

    size_t len;
    size_t i = 0;

    TRACE();

    if (set == NULL)
        ERROR("set == NULL\n", NULL);

    /* find root */
    root = ufset_find(set);
    if (root == NULL)
        ERROR("ufset_find error\n", NULL);

    /* get num entries of this set */
    len = (size_t)ufset_get_num_entries(root);
    t = (UFSentry **)malloc(sizeof(*t) * len);
    if (t == NULL)
        ERROR("malloc error\n", NULL);

    /* add entries from this set (entries have the same root) */
    master = set->master;
    for_each_data(master->set, Darray, node)
        if (root == ufset_find(node))
            t[i++] = ufset_get_entry(node);

    if (size != NULL)
        *size = len;

    return t;
}

ssize_t ufs_master_get_num_entries(const UFSMaster *master)
{
    TRACE();

    if (master == NULL)
        ERROR("master == NULL\n", (ssize_t)-1);

    return darray_get_num_entries(master->set);
}

int ufs_master_get_hight(const UFSMaster *master)
{
    TRACE();

    if (master == NULL)
        ERROR("master == NULL\n", -1);

    return master->hight;
}

UFset **ufs_master_get_sets(const UFSMaster *master, size_t *size)
{
    TRACE();

    if (master == NULL)
        ERROR("master == NULL\n", NULL);

    if (size != NULL)
        *size = (size_t)darray_get_num_entries(master->set);

    return (UFset **)darray_get_array(master->set);
}