#ifndef UFSET_H
#define UFSET_H

/*
    Union find set with path compression

    Please note that in Union find set can't traverse from up to bottom
    so can't free SET from root

    Please call ufset_destroy for all UFSET,
    Pointers are stored in entries

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3
*/

typedef struct UFSentry
{
    void            *data;
    struct UFset    *ufs_ptr;

}UFSentry;

typedef struct UFset
{
    struct UFset    *parent;
    UFSentry        *entry;

    int             rank;
}UFset;

/*
	Create UFS Entry

	PARAMS
	@IN data - addr to data
	@IN size_of - size of data

	RETURN:
	%NULL iff failure
	%Pointer to UFS Entry iff success
*/
UFSentry *ufs_entry_create(void *data, int size_of);

/*
	Deallocate mem

	PARAMS
	@IN entry - pointer to ufs entry

	RETURN:
	This is a void function
*/
void ufs_entry_destroy(UFSentry *entry);


/*
    Create new UF Set with UFS Entry

    PARAMS
    @IN entry - pointer to UFS entry

    RETURN:
    %NULL iff failure
    %Pointer to UFset iff success
*/
UFset *ufset_create(UFSentry *entry);

/*
    Deallocate mem

    PARAMS
    @IN set - pointer to Union Find set

    RETURN:
    This is a void function
*/
void ufset_destroy(UFset *set);


/*
    Merge two independent set in one set

    PARAMS
    @IN x - pointer to UF Set
    @IN y - pointer to UF Set

    RETURN:
    0 iff success
    Positive value iff failure
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

#endif
