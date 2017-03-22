#ifndef TRIE_H
#define TRIE_H

/*
    Implementation of popular TRIE (prefix tree on defined ASCII)

    Author:Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL3
*/

#define ALPHABET_START_CODE 'a'
#define ALPHABET_END_CODE   'z'
#define ALPHABET_SIZE       (ALPHABET_END_CODE - ALPHABET_START_CODE + 1)

#include <iterators.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct Trie_node
{
    struct Trie_node    *parent;
    struct Trie_node    *children[ALPHABET_SIZE];
    bool                is_leaf; /* if TRUE then word from root to this node exist in trie */

}Trie_node;

typedef struct Trie
{
    Trie_node   *root;
    size_t      entries;

}Trie;

typedef struct Trie_iterator
{
    char        *word;
    Trie_node   *node;

}Trie_iterator;

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
    %Positive value iff failure
*/
int trie_insert(Trie *trie, char *word);

/*
    Search word in trie

    PARAMS
    @IN trie - pointer to trie
    @IN word - word

    RETURN:
    %FALSE iff word doesn't exist in trie
    %TRUE iff word exists in trie
*/
bool trie_find(Trie *trie, char *word);

/*
    Delete word from trie

    PARAMS
    @IN trie - pointer to trie
    @IN word - word

    RETURN:
    %0 iff success
    %Positive value iff failure
*/
int trie_delete(Trie *trie, char *word);

/*
    Get words from TRIE to array

    PARAMS
    @IN trie - pointer to trie
    @OUT size - size of array

    RETURN:
    %NULL iff failure
    %Pointer to array iff success
*/
char **trie_to_array(Trie *trie, size_t *size);

#endif
