#include <hash.h>
#include <log.h>
#include <generic.h>
#include <common.h>

uint32_t hash_jenkins_one_at_time(const void *data, size_t size)
{
    size_t i;
    uint32_t hash = 0;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for (i = 0; i < size; ++i)
    {
        hash += key[i];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

uint32_t hash_rs(const void *data, size_t size)
{
    /* consts */
    uint32_t const_a = 63689;
    uint32_t const_b = 378551;
    uint32_t hash = 1;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
    {
        hash *= const_a + key[i];
        const_a *= const_b;
    }

    /* sometimes a + key = max uint32 + 1, so then return 1 instead of 0 */
    if (hash == 0)
        hash = 1;

    return hash;
}

uint32_t hash_js(const void *data, size_t size)
{
    uint32_t hash = 1315423911;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
        hash ^= ((hash << 5) + key[i] + (hash >> 2));

    return hash;
}

uint32_t hash_elf(const void *data, size_t size)
{
    uint32_t hash = 0;
    uint32_t temp = 0;
    size_t i;
    const BYTE *key = (const BYTE *)data;
    const uint32_t mask = 0xF0000000UL;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
    {
        hash = (hash << 4) + key[i];
        temp = hash & mask;
        if (temp != 0)
            hash ^= (temp >> 24);
        hash &= ~temp;
    }

    return hash;
}

uint32_t hash_sdbm(const void *data, size_t size)
{
    uint32_t hash = 0;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
        hash = key[i] + (hash << 6) + (hash << 16) - hash;

    return hash;
}

uint32_t hash_djb(const void *data, size_t size)
{
    uint32_t hash = 5381;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
        hash = ((hash << 5) + hash) + key[i];

    return hash;
}

uint32_t hash_dek(const void *data, size_t size)
{
    uint32_t hash = (uint32_t)size;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
        hash = ((hash << 5) ^ (hash >> 27)) ^ key[i];

    return hash;
}

uint32_t hash_fnv(const void *data, size_t size)
{
    const uint32_t fnv_prime = 0x811C9DC5;
    uint32_t hash = 0;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
    {
        hash *= fnv_prime;
        hash ^= key[i];
    }

    return hash;
}

uint32_t hash_ap(const void *data, size_t size)
{
    uint32_t hash = 0xAAAAAAAA;
    size_t i;
    const BYTE *key = (const BYTE *)data;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    for(i = 0; i < size; ++i)
    {
	if (ODD(i))
            hash ^= ~((hash << 11) + (key[i] ^ (hash >> 5)));
	else
            hash ^= (hash <<  7) ^ key[i] * (hash >> 3);
    }

    return hash;
}

uint32_t hash_murmur(const void *data, size_t size)
{
    /* consts */
    const uint32_t const_m = 0x5bd1e995;
    const uint32_t const_r = 24;
    const uint32_t const_a = 0x35111741;

    uint32_t hash = const_a ^ (uint32_t)size;
    size_t i = 0;
    const size_t rem_by_4 = size & 0x3;
    const BYTE *key = (const BYTE *)data;
    uint32_t acc;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    /* 4 bytes at 1 time */
    size >>= 2;
    for (i = 0; i < size; i += 4)
    {
        acc = *(uint32_t *)&key[i];
	acc *= const_m;
        acc ^= acc >> const_r;
	acc *= const_m;

	hash *= const_m;
	hash ^= acc;
    }

    key = &key[i];

    /* the last part */
    switch (rem_by_4)
    {
        case 3:
        {
            hash ^= (uint32_t)key[2] << 16;
        }
	    case 2:
        {
            hash ^= (uint32_t)key[1] << 8;
        }
	    case 1:
        {
            hash ^= key[0];
	    hash *= const_m;
        }
        case 0:
        {
            break;
        }
        default:
        {
            break;
        }
    };

    /* final step */
    hash ^= hash >> 13;
    hash *= const_m;
    hash ^= hash >> 15;

    return hash;
}

uint32_t hash_super_fast_hash(const void *data, size_t size)
{
    const uint32_t mask1 = 0x0000FFFF;
    const uint32_t mask2 = 0xFFFF0000;
    uint32_t hash = (uint32_t)size;
    uint32_t temp;

    size_t i = 0;
    const size_t rem_by_4 = size & 0x3;
    const BYTE *key = (const BYTE *)data;
    uint32_t acc;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    /* 4 bytes at 1 time */
    size >>= 2;
    for (i = 0; i < size; i += 4)
    {
        acc = *(uint32_t *)&key[i];
        hash += acc & mask1;
        temp = ((acc & mask2)<< 11) ^ hash;
        hash = (hash << 16) ^ temp;
        hash += hash >> 11;
    }

    key = &key[i];

    /* the last part */
    switch (rem_by_4)
    {
        case 3:
        {
            hash += *(uint16_t *)key;
            hash ^= hash << 16;
            hash ^= (uint32_t)key[2] << 18;
            hash += hash >> 11;
            break;
        }
        case 2:
        {
            hash += *(uint16_t *)key;
            hash ^= hash << 11;
            hash += hash >> 17;
            break;
        }
        case 1:
        {
            hash += key[0];
            hash ^= hash << 10;
            hash += hash >> 1;
            break;
        }
        case 0:
        {
            break;
        }
        default:
        {
            break;
        }
    }

    /* Final step */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
}


/* private xxhash function */
static uint32_t ___unused___ hash_xxhash_round(uint32_t seed, uint32_t input, uint32_t prime1, uint32_t prime2);
#define rotl(x, r) (((x) << (r)) | ((x) >> (32 - (r))))

static uint32_t hash_xxhash_round(uint32_t seed, uint32_t input, uint32_t prime1, uint32_t prime2)
{
    seed += input * prime2;
    seed  = rotl(seed, 13);
    seed *= prime1;
    return seed;
}

#define hash_xxhash_round(seed, input) hash_xxhash_round(seed, input, prime1, prime2)

uint32_t hash_xxhash(const void *data, size_t size)
{
    const uint32_t prime1 = 2654435761U;
    const uint32_t prime2 = 2246822519U;
    const uint32_t prime3 = 3266489917U;
    const uint32_t prime4 =  668265263U;
    const uint32_t prime5 =  374761393U;

    uint32_t hash = prime5;
    uint32_t temp1;
    uint32_t temp2;
    uint32_t temp3;
    uint32_t temp4;

    const BYTE *key = (const BYTE *)data;
    const uint32_t *acc_array = (const uint32_t *)data;
    size_t max_loop;
    size_t i = 0;
    const size_t rem_by_4 = size & 0x3;

    TRACE();

    if (data == NULL || size == 0)
        return 0;

    /* main hash magic for large blocks */
    if (size >= 16)
    {
        temp1 = prime1 + prime2;
        temp2 = prime2;
        temp3 = 0;
        temp4 = -prime1;

        max_loop = (size - 16) >> 2;
        do {
            temp1 = hash_xxhash_round(temp1, acc_array[i++]);
            temp2 = hash_xxhash_round(temp2, acc_array[i++]);
            temp3 = hash_xxhash_round(temp3, acc_array[i++]);
            temp4 = hash_xxhash_round(temp4, acc_array[i++]);
        } while (i <= max_loop);

        hash = rotl(temp1, 1) + rotl(temp2, 7) + rotl(temp3, 12) + rotl(temp4, 18);
    }

    /* 4byte at 1 time */
    hash += (uint32_t)size;
    max_loop = size >> 2;
    for(; i < max_loop; ++i)
    {
        hash += acc_array[i] * prime3;
        hash = rotl(hash, 17) * prime4 ;
    }

    /* last part */
    key = (const BYTE *)&acc_array[i];
    for (i = 0; i < rem_by_4; ++i)
    {
        hash += key[i] * prime5;
        hash = rotl(hash, 11) * prime1;
    }

    /* final step */
    hash ^= hash >> 15;
    hash *= prime2;
    hash ^= hash >> 13;
    hash *= prime3;
    hash ^= hash >> 16;

    return hash;
}
