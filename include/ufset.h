#ifndef UFSET_H
#define UFSET_H

/*
    Union find set with path compression

    If you want to create Forest of UFS pls create ONE UFSMaster
    and add every UFset to this Master

    After that you can Destroy whole forest by call ufs_master_destroy
    or if UFSentry data need destruction call ufs_master_destroy_with_entries

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

#include <generic.h>
#include <common.h>

typedef struct UFSMaster UFSMaster;
typedef struct UFSentry UFSentry;
typedef struct UFset UFset;

/*
    Create UFS Master

    PARAMS
    NO PARAMS

    RETURN
    NULL iff failure
    Pointer iff success
*/
UFSMaster *ufs_master_create(void);

/*
    Destroy UFS Master and whole UF SET

    PARAMS
    @IN master - pointer to UFSMaster

    RETURN
    This is a void function
*/
void ufs_master_destroy(UFSMaster *master);

/*
    Destroy UFS Master and whole UF SET and entry data

    PARAMS
    @IN master - pointer to UFSMaster

    RETURN
    This is a void function
*/
void ufs_master_destroy_with_entries(UFSMaster *master);

/*
	Create UFS Entry

	PARAMS
	@IN data - addr to data
	@IN size_of - size of data
    @IN destriy - your data destructor or NULL

	RETURN:
	%NULL iff failure
	%Pointer to UFS Entry iff success
*/
UFSentry *ufs_entry_create(const void *data, size_t size_of, destructor_f destroy);

/*
	Deallocate mem

	PARAMS
	@IN entry - pointer to ufs entry

	RETURN:
	This is a void function
*/
void ufs_entry_destroy(UFSentry *entry);

/*
	Deallocate mem of enrty and data

	PARAMS
    @IN entry - pointer to ufs entry

	RETURN:
	This is a void function
*/
void ufs_entry_destroy_with_data(UFSentry *entry);

/*
    Get ufset where this entry is stored

    PARAMS
    @IN entry - ufset entry

    RETURN
    NULL iff failure
    Pointer to ufset iff success
*/
UFset *ufset_entry_get_ufset(const UFSentry *entry);

/*
    Get data from ufset entry

    PARAMS
    @IN entry - ufset entry
    @OUT data - data from entry

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int ufset_entry_get_data(const UFSentry * ___restrict___ entry, void * ___restrict___ data);

/*
    Create new UF Set with UFS Entry

    PARAMS
    @IN entry - pointer to UFS entry
    @IN master - pointer to master

    RETURN:
    %NULL iff failure
    %Pointer to UFset iff success
*/
UFset *ufset_create(UFSentry *entry, UFSMaster *master);

/*
    Deallocate mem

    PARAMS
    @IN set - pointer to Union Find set

    RETURN:
    This is a void function
*/
void ufset_destroy(UFset *set);

/*
    Deallocate mem of set and data

    PARAMS
    @IN set - pointer to set

    RETURN
    This is a void function
*/
void ufset_destroy_with_entries(UFset *set);
/*
    Merge two independent set in one set

    PARAMS
    @IN x - pointer to UF Set
    @IN y - pointer to UF Set

    RETURN:
    0 iff success
    Non-zero value iff failure
*/
int ufset_union(UFset *x, UFset *y);

/*
    Find root for this set
    Compress path using recurence during searching

    PARAMS
    @IN set - pointer to UF Set

    RETURN:
    NULL iff failure
    Pointer to root of @set iff success
*/
UFset *ufset_find(UFset *set);

/*
    Get Hight of this set

    PARAMS
    @IN set - pointer to set

    RETURN
    -1 iff failure
    Hight iff success
*/
int ufset_get_hight(UFset *set);

/*
    Get num entries of this set

    PARAMS
    @IN set - pointer to set

    RETURN
    -1 iff failure
    Num entries iff success
*/
ssize_t ufset_get_num_entries(UFset *set);

/*
    Get master of this set

    PARAMS
    @IN set - pointer to set

    RETURN
    NULL iff failure
    Pointer to master iff success
*/
UFSMaster *ufset_get_master(const UFset *set);

/*
    Get entry from this ufset

    PARAMS
    @IN set - pointer to set

    RETURN
    NULL iff failure
    Pointer to entry iff success
*/
UFSentry *ufset_get_entry(const UFset *set);

/*
    Find root of this set and return all entries stored in whole set
    NOTE: You must free array by yourself

    PARAMS
    @IN set - pointer to set
    @OUT size - number of entries in array

    RETURN
    Pointer to Array of UFSentry * iff success
    NULL iff failure
*/
UFSentry **ufset_get_entries(UFset *set, size_t *size);

/*
    Get number of entries in UFS

    PARAMS
    @IN master - pointer to UFSMsster

    RETURN
    %-1 iff failure
    %Num of entries iff success
*/
ssize_t ufs_master_get_num_entries(const UFSMaster *master);

/*
    Get hight of UFS FOREST

    PARAMS
    @IN master - pointer to UFSMaster

    RETURN
    -1 iff failure
    Hight iff success
*/
int ufs_master_get_hight(const UFSMaster *master);

/*
    Get this master sets
    Only ptr is return, so do not free it by youtself

    PARAMS
    @IN master- pointer to master
    @OUT size - n of sets

    RETURN
    NULL iff failure
    Pointer to array of sets iff success
*/
UFset **ufs_master_get_sets(const UFSMaster *master, size_t *size);

#endif
