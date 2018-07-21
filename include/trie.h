#ifndef TRIE_H
#define TRIE_H

/*
    Implementation of popular TRIE (prefix tree on defined ASCII)

    Author:Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL3
*/

#include <iterators.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>
#include <common.h>

typedef struct Trie_node Trie_node;
typedef struct Trie Trie;
typedef struct Trie_iterator Trie_iterator;

IT_FUNC(Trie, trie)

/*
    Create new Trie

    PARAMS
    NO PARAMS

    RETURN:
    %NULL iff failure
    %Pointer iff success
*/
Trie *trie_create(void);

/*
    Destroy TRIE

    PARAMS
    @IN trie - pointer to Trie

    RETURN:
    This is a void function
*/
void trie_destroy(Trie *trie);


/*
    Insert word to trie

    PARAMS
    @IN trie - pointer to trie
    @IN word - word

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int trie_insert(Trie * ___restrict___ trie, const char * ___restrict___ word);

/*
    Search word in trie

    PARAMS
    @IN trie - pointer to trie
    @IN word - word

    RETURN:
    %FALSE iff word doesn't exist in trie
    %TRUE iff word exists in trie
*/
bool trie_find(const Trie * ___restrict___ trie, const char * ___restrict___ word);

/*
    Delete word from trie

    PARAMS
    @IN trie - pointer to trie
    @IN word - word

    RETURN:
    %0 iff success
    %Non-zero value iff failure
*/
int trie_delete(Trie * ___restrict___ trie, const char * ___restrict___ word);

/*
    Get words from TRIE to array

    PARAMS
    @IN trie - pointer to trie
    @OUT size - size of array

    RETURN:
    %NULL iff failure
    %Pointer to array iff success
*/
char **trie_to_array(const Trie * trie, size_t *size);

/*
    Get Num entries in TRIE

    PARAMS
    @IN trie - pointer to TRIE

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t trie_get_num_entries(const Trie *trie);

/*
    Get hight of TRIE

    PARAMS
    @IN trie - pointer to TRIE

    RETURN
    -1 iff failure
    Trie Hight iff success
*/
int trie_get_hight(const Trie *trie);

#endif
