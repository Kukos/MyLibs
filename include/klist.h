#ifndef KLIST_H
#define KLIST_H

/*
    New Implementation for cache friendly list inspired by Kernel List

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h>
#include <compiler.h>

struct KList;

typedef struct KList_master
{
    struct KList *____head;
    struct KList *____tail;
    size_t        ____length;
}KList_master;

typedef struct KList
{
    struct KList    *____prev;
    struct KList    *____next;
    KList_master    *____parent;

}KList;

#define KLIST_INIT(name) { &(name), &(name), (NULL) }

#define KLIST (name) \
	KList name = KLIST_INIT(name)

#define KLIST_MASTER_INIT(name) { (NULL), (NULL), (0) }

#define KLIST_MASTER(name) \
    KList_master name = KLIST_MASTER_INIT(name)

/*
    get the struct for this entry

    PARAMS
    @ptr - KList pointer.
    @type - type of parent of KList
    @member - the name of KList struct field in parent struct
*/
#define klist_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define klist_for_each(ptr, List) \
	for ( size_t ______i = 0, ptr = klist_get_head(List); \
          ______i < klist_get_num_entries(List); \
          ++______i, ptr = ptr->____next)

#define klist_for_each_prev(ptr, List) \
	for ( size_t ______i = 0, ptr = klist_get_tail(List); \
          ______i < klist_get_num_entries(List); \
          ++______i, ptr = ptr->____prev)

/*
    Create KList_master

    PARAMS
    NO PARAMS

    RETURN
    Pointer to KList_master if success
    NULL iff failure
*/
KList_master *klist_master_create(void);

/*
    Destroy KList_master

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    This is a void function
*/
void klist_master_destroy(KList_master *list);

/*
    Insert new entry as first entry

    PARAMS
    @IN list - pointer to KList_master
    @IN entry - entry to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_insert_first(KList_master *list, KList *entry);

/*
    Insert new entry as last entry

    PARAMS
    @IN list - pointer to KList_master
    @IN entry - entry to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_insert_last(KList_master *list, KList *entry);

/*
    Insert new entry as position @pos

    PARAMS
    @IN list - pointer to KList_master
    @IN pos = position where we insert entry
    @IN entry - entry to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_insert_pos(KList_master *list, size_t pos, KList *entry);

/*
    Insert new entry before @node

    PARAMS
    @IN node - Klist node before new entry
    @IN entry - entry to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_insert_before(KList *node, KList *entry);

/*
    Insert new entry after @node

    PARAMS
    @IN node - Klist node after new entry
    @IN entry - entry to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_insert_after(KList *node, KList *entry);

/*
    Delete first entry from list

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_delete_first(KList_master *list);

/*
    Delete last entry from list

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_delete_last(KList_master *list);

/*
    Delete entry from list at position @pos

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_delete_pos(KList_master *list, size_t pos);

/*
    Delete node from list

    PARAMS
    @IN node - pointer to node to delete

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_delete_node(KList *node);

/*
    Get entry from list at position @pos

    PARAMS
    @IN list - pointer to KList_master
    @IN pos - entry position
    @OUT entry - entry from list

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int klist_get_pos(KList_master *list, size_t pos, KList **entry);

/*
    Get list length

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    -1 iff failure
    List length iff success
*/
ssize_t klist_get_num_entries(KList_master *list);

/*
    Get KList HEAD

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    NULL iff failure
    Pointer to head iff success
*/
KList *klist_get_head(KList_master *list);

/*
    Get KList TAIL

    PARAMS
    @IN list - pointer to KList_master

    RETURN
    NULL iff failure
    Pointer to tail iff success
*/
KList *klist_get_tail(KList_master *list);

#endif