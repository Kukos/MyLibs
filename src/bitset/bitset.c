#include <generic.h>
#include <bitset.h>
#include <log.h>
#include <common.h>
#include <stdlib.h>
#include <string.h>

#define INT_DIV_CEIL(n ,k) (((n) + (k) - 1) / (k))

#define BITSET_SHIFT (sizeof(DWORD) << 3)
#define BITSET_MASK (BITSET_SHIFT - 1)

#define BITSET_SIZE(bits) (INT_DIV_CEIL(bits, sizeof(DWORD) << 3))
#define BITSET_WORD(bit) ((bit) / BITSET_SHIFT)
#define BITSET_REAL_POS(bit) ((bit) & BITSET_MASK) 

Bitset *bitset_create(size_t size)
{
    Bitset *set;

    TRACE();

    if (size == 0)
        ERROR("size == 0\n", NULL);

    set = (Bitset *)malloc(sizeof(Bitset));
    if (set == NULL)
        ERROR("malloc error\n", NULL);

    set->____set = (DWORD *)malloc(sizeof(DWORD) * BITSET_SIZE(size));
    if (set->____set == NULL)
    {
        FREE(set);
        ERROR("malloc errro\n", NULL);
    }
    (void)memset(set->____set, 0, sizeof(DWORD) * BITSET_SIZE(size));

    set->____size = size;

    return set;
}

void bitset_destroy(Bitset *bitset)
{
    TRACE();

    if (bitset == NULL)
        return;

    FREE(bitset->____set);
    FREE(bitset);
}

int bitset_get_bit(const Bitset *bitset, size_t pos)
{
    DWORD word;

    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", -1);

    if (pos > bitset->____size)
        ERROR("pos > bitset size\n", -1);

    word = bitset->____set[BITSET_WORD(pos)];

    return (int)GET_BIT(word, BITSET_REAL_POS(pos));
}

void bitset_set_bit(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > bitset->____size)
       return;

    SET_BIT(bitset->____set[BITSET_WORD(pos)], BITSET_REAL_POS(pos));
}

void bitset_clear_bit(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > bitset->____size)
       return;

    CLEAR_BIT(bitset->____set[BITSET_WORD(pos)], BITSET_REAL_POS(pos));
}

void bitset_set_bit_value(Bitset *bitset, size_t pos, int value)
{
    TRACE();

    if (bitset == NULL || pos > bitset->____size)
       return;

    SET_BIT_VALUE(bitset->____set[BITSET_WORD(pos)], BITSET_REAL_POS(pos), CAST_TO_BOOL(value));
}

void bitset_flip_bit(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > bitset->____size)
       return;

    FLIP_BIT(bitset->____set[BITSET_WORD(pos)], BITSET_REAL_POS(pos));
}

ssize_t bitset_get_size(const Bitset *bitset)
{
    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", -1);

    return (ssize_t)bitset->____size;
}

DWORD *bitset_get_set(const Bitset *bitset, size_t *size)
{
    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", NULL);

    if (size != NULL)
        *size = BITSET_SIZE(bitset->____size);

    return bitset->____set;
}

Bitset_iterator *bitset_iterator_create(const Bitset *bitset, iti_mode_t mode)
{
    Bitset_iterator *iterator;

    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    iterator = (Bitset_iterator *)malloc(sizeof(Bitset_iterator));
    if (iterator == NULL)
        ERROR("malloc error\n", NULL);

    iterator->____set = bitset->____set;
    iterator->____size = bitset->____size;

	if (mode == ITI_BEGIN)
    {
    	iterator->____index = 0;
        iterator->____bit = 0;
    }
	else
    {
		iterator->____index = (ssize_t)bitset->____size - 1;
        iterator->____bit = (sizeof(DWORD) << 3) - 1;
    }

    return iterator;
}

void bitset_iterator_destroy(Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        return;

    FREE(iterator);
}

int bitset_iterator_init(const Bitset *bitset, Bitset_iterator *iterator, iti_mode_t mode)
{
    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", 1);

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", 1);

    iterator->____set = bitset->____set;
    iterator->____size = bitset->____size;

	if (mode == ITI_BEGIN)
    {
    	iterator->____index = 0;
        iterator->____bit = 0;
    }
	else
    {
		iterator->____index = (ssize_t)bitset->____size - 1;
        iterator->____bit = (sizeof(DWORD) << 3) - 1;
    }

    return 0;
}

bool bitset_iterator_end(const Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return (iterator->____index < 0 || (size_t)iterator->____index >= BITSET_SIZE(iterator->____size));
}

int bitset_iterator_get_data(const Bitset_iterator *iterator, void *val)
{
    TRACE();

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1);

    *(BYTE *)val = (BYTE)GET_BIT(iterator->____set[iterator->____index], iterator->____bit);

    return 0;
}

int bitset_iterator_get_node(const Bitset_iterator *iterator, void *node)
{
    TRACE();

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || node == NULL\n", 1);

    *(DWORD *)node = iterator->____set[iterator->____index];

    return 0;
}

int bitset_iterator_next(Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    ++iterator->____bit;
    if (iterator->____bit >= (ssize_t)(sizeof(DWORD) << 3))
    {
        iterator->____bit = 0;
        ++iterator->____index;
    }

    return 0;
}

int bitset_iterator_prev(Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    --iterator->____bit;
    if (iterator->____bit < 0)
    {
        iterator->____bit = (sizeof(DWORD) << 3) - 1;
        --iterator->____index;
    }

    return 0;
}