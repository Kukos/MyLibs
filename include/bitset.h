#ifndef BITSET_H
#define BITSET_H

/*
    Implementation of array of bits (bitset)

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <stddef.h>
#include <generic.h>
#include <sys/types.h>
#include <iterators.h>
#include <common.h>

typedef struct Bitset Bitset;
typedef struct Bitset_iterator Bitset_iterator;

IT_FUNC(Bitset, bitset)

/*
    Create bitset

    PARAMS
    @IN size - number of bits in bitset

    RETURN
    NULL iff failure
    Pointer to new bitset iff success
*/
Bitset *bitset_create(size_t size);

/*
    Destroy bitset

    PARAMS
    @IN bitset - pointer to bitset

    RETURN
    This is a void function
*/
void bitset_destroy(Bitset *bitset);

/*
    Get bit value (0 or 1)

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - bit to get

    RETURN
    Bit value iff success
    -1 iff bit on @pos doesnt exist

*/
int bitset_get_bit(const Bitset *bitset, size_t pos);

/*
    Set bit to 1

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - bit to set

    RETURN
    This is a void function
*/
void bitset_set_bit(Bitset *bitset, size_t pos);

/*
    Clear bit to 0

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - bit to clear

    RETURN
    This is a void function
*/
void bitset_clear_bit(Bitset *bitset, size_t pos);

/*
    Set bit to value @val (0 or 1)

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - bit to set
    @IN value - value to set

    RETURN
    This is a void function
*/
void bitset_set_bit_value(Bitset *bitset, size_t pos, int value);

/*
    Flip bit (bit = ~bit)

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - bit to flip

    RETURN
    This is a void function
*/
void bitset_flip_bit(Bitset *bitset, size_t pos);

/*
    Reverse WORD in bitset

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - word pos

    RETURN
    This is a void function
*/
void bitset_reverse_word(Bitset *bitset, size_t pos);

/*
    Reverse all bits in set

    PARAMS
    @IN bitset - pointer to bitset

    RETURN
    This is a void function
*/
void bitset_reverse(Bitset *bitset);

/*
    Get DWORD from bitset

    PARAMS
    @IN bitset - pointer to bitset
    @IN pos - word pos

    RETURN
    -1 iff failure
    DWORD value iff success
*/
DWORD bitset_get_word(const Bitset *bitset, size_t pos);

/*
    Get number of bits in bitset

    PARAMS
    @IN bitset - pointer to bitset

    RETURN
    -1 iff failure
    bitset size iff success
*/
ssize_t bitset_get_size(const Bitset *bitset);

/*
    Get bitmap from bitset

    PARAMS
    @IN bitset - pointer to bit set
    @OUT size - size of array

    RETURN
    NULL iff failure
    pointer to bitmap iff success
*/
DWORD *bitset_get_set(const Bitset *bitset, size_t *size);

#endif