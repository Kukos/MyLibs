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

struct Bitset
{
    DWORD *set;
    size_t size;
};

struct Bitset_iterator
{
    DWORD  *set;
    ssize_t index;
    ssize_t bit;
    size_t  size;
};

Bitset *bitset_create(size_t size)
{
    Bitset *set;

    TRACE();

    if (size == 0)
        ERROR("size == 0\n", NULL);

    set = (Bitset *)malloc(sizeof(Bitset));
    if (set == NULL)
        ERROR("malloc error\n", NULL);

    set->set = (DWORD *)malloc(sizeof(DWORD) * BITSET_SIZE(size));
    if (set->set == NULL)
    {
        FREE(set);
        ERROR("malloc errro\n", NULL);
    }
    (void)memset(set->set, 0, sizeof(DWORD) * BITSET_SIZE(size));

    set->size = size;

    return set;
}

void bitset_destroy(Bitset *bitset)
{
    TRACE();

    if (bitset == NULL)
        return;

    FREE(bitset->set);
    FREE(bitset);
}

int bitset_get_bit(const Bitset *bitset, size_t pos)
{
    DWORD word;

    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", -1);

    if (pos > bitset->size)
        ERROR("pos > bitset size\n", -1);

    word = bitset->set[BITSET_WORD(pos)];

    return (int)GET_BIT(word, BITSET_REAL_POS(pos));
}

void bitset_set_bit(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > bitset->size)
       return;

    SET_BIT(bitset->set[BITSET_WORD(pos)], BITSET_REAL_POS(pos));
}

void bitset_clear_bit(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > bitset->size)
       return;

    CLEAR_BIT(bitset->set[BITSET_WORD(pos)], BITSET_REAL_POS(pos));
}

void bitset_set_bit_value(Bitset *bitset, size_t pos, int value)
{
    TRACE();

    if (bitset == NULL || pos > bitset->size)
       return;

    SET_BIT_VALUE(bitset->set[BITSET_WORD(pos)], BITSET_REAL_POS(pos), CAST_TO_BOOL(value));
}

void bitset_flip_bit(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > bitset->size)
       return;

    FLIP_BIT(bitset->set[BITSET_WORD(pos)], BITSET_REAL_POS(pos));
}

void bitset_reverse_word(Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > BITSET_SIZE(bitset->size))
       return;

    bitset->set[pos] = REVERSE_BITS(bitset->set[pos]);
}

void bitset_reverse(Bitset *bitset)
{
    size_t i;
    size_t size =  BITSET_SIZE(bitset->size);

    TRACE();

    if (bitset == NULL)
        return;

    for (i = 0; i < size / 2; ++i)
    /* new gcc optimize it and guess that this i s pointer overflow, so to make him happy, do max with 0 */
        SWAP(bitset->set[i], bitset->set[MAX((ssize_t)0, (ssize_t)(size - i - 1))]);

    for (i = 0; i < size; ++i)
        bitset->set[i] = REVERSE_BITS(bitset->set[i]);
}

ssize_t bitset_get_size(const Bitset *bitset)
{
    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", -1);

    return (ssize_t)bitset->size;
}

DWORD bitset_get_word(const Bitset *bitset, size_t pos)
{
    TRACE();

    if (bitset == NULL || pos > BITSET_SIZE(bitset->size))
        ERROR("bitset == NULL || pos > size\n", (DWORD)-1);

    return bitset->set[pos];
}

DWORD *bitset_get_set(const Bitset *bitset, size_t *size)
{
    TRACE();

    if (bitset == NULL)
        ERROR("bitset == NULL\n", NULL);

    if (size != NULL)
        *size = BITSET_SIZE(bitset->size);

    return bitset->set;
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

    iterator->set = bitset->set;
    iterator->size = bitset->size;

	if (mode == ITI_BEGIN)
    {
    	iterator->index = 0;
        iterator->bit = 0;
    }
	else
    {
		iterator->index = (ssize_t)(BITSET_SIZE(bitset->size) - 1);
        iterator->bit = (sizeof(DWORD) << 3) - 1;
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

bool bitset_iterator_end(const Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", true);

    return (iterator->index < 0 || (size_t)iterator->index >= BITSET_SIZE(iterator->size));
}

int bitset_iterator_get_data(const Bitset_iterator *iterator, void *val)
{
    TRACE();

    if (iterator == NULL || val == NULL)
        ERROR("iterator == NULL || val == NULL\n", 1);

    *(BYTE *)val = (BYTE)GET_BIT(iterator->set[iterator->index], iterator->bit);

    return 0;
}

int bitset_iterator_get_node(const Bitset_iterator *iterator, void *node)
{
    TRACE();

    if (iterator == NULL || node == NULL)
        ERROR("iterator == NULL || node == NULL\n", 1);

    *(DWORD *)node = iterator->set[iterator->index];

    return 0;
}

int bitset_iterator_next(Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (iterator->bit++ >= (ssize_t)(sizeof(DWORD) << 3) - 1)
    {
        iterator->bit = 0;
        ++iterator->index;
    }

    return 0;
}

int bitset_iterator_prev(Bitset_iterator *iterator)
{
    TRACE();

    if (iterator == NULL)
        ERROR("iterator == NULL\n", 1);

    if (iterator->bit-- < 1)
    {
        iterator->bit = (sizeof(DWORD) << 3) - 1;
        --iterator->index;
    }

    return 0;
}