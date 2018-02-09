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

#include <darray.h>

typedef struct UFSMaster
{
    Darray  *____set;
    int     ____hight;
}UFSMaster;

typedef struct UFSentry
{
    void            *____data;
    struct UFset    *____ufs_ptr;
    void            (*____destroy)(void *entry); 

}UFSentry;

typedef struct UFset
{
    struct UFset    *____parent;
    UFSentry        *____entry;
    UFSMaster       *____master;

    int             ____rank;
}UFset;

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
UFSentry *ufs_entry_create(const void *data, int size_of, void (*destroy)(void *entry));

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

#endif
