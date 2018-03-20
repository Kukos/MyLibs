#include <hash.h>
#include <test.h>

typedef struct MyStruct
{
    char c;
    int i;
    double d;
    char *str;
} MyStruct;

test_f test_general(uint32_t (*hash_f)(const void *data, size_t size))
{
    char c1 = 'K';
    char c2 = c1;

    int i1 = 123456;
    int i2 = i1;

    double d1 = 1234.5678;
    double d2 = d1;

    char str1[] = "qwertyuioplkjhgfdsazxcvbnm";
    char str2[] = "qwertyuioplkjhgfdsazxcvbnm";

    MyStruct ms1 = {.c = c1, .i = i1, .d = d1, .str = str1};
    MyStruct ms2 = ms1;

    uint32_t hash1;
    uint32_t hash2;

    hash1 = hash_f((const void *)&c1, sizeof(c1));
    hash2 = hash_f((const void *)&c2, sizeof(c2));
    T_ASSERT(hash1, hash2);
    T_CHECK(hash1 != 0);

    hash1 = hash_f((const void *)&i1, sizeof(i1));
    hash2 = hash_f((const void *)&i2, sizeof(i2));
    T_ASSERT(hash1, hash2);
    T_CHECK(hash1 != 0);

    hash1 = hash_f((const void *)&d1, sizeof(d1));
    hash2 = hash_f((const void *)&d2, sizeof(d2));
    T_ASSERT(hash1, hash2);
    T_CHECK(hash1 != 0);

    hash1 = hash_f((const void *)&str1, sizeof(str1));
    hash2 = hash_f((const void *)&str2, sizeof(str2));
    T_ASSERT(hash1, hash2);
    T_CHECK(hash1 != 0);

    hash1 = hash_f((const void *)&ms1, sizeof(ms1));
    hash2 = hash_f((const void *)&ms2, sizeof(ms2));
    T_ASSERT(hash1, hash2);
    T_CHECK(hash1 != 0);
}

test_f test_hash_jenkins_one_at_time(void)
{
    test_general(hash_jenkins_one_at_time);
}

test_f test_hash_rs(void)
{
    test_general(hash_rs);
}

test_f test_hash_js(void)
{
    test_general(hash_js);
}

test_f test_hash_elf(void)
{
    test_general(hash_elf);
}

test_f test_hash_sdbm(void)
{
    test_general(hash_sdbm);
}

test_f test_hash_djb(void)
{
    test_general(hash_djb);
}

test_f test_hash_dek(void)
{
    test_general(hash_dek);
}

test_f test_hash_fnv(void)
{
    test_general(hash_fnv);
}

test_f test_hash_ap(void)
{
    test_general(hash_ap);
}

test_f test_hash_murmur(void)
{
    test_general(hash_murmur);
}

test_f test_hash_super_fast_hash(void)
{
    test_general(hash_super_fast_hash);
}

test_f test_hash_xxhash(void)
{
    test_general(hash_xxhash);
}

void test(void)
{
    TEST(test_hash_jenkins_one_at_time());
    TEST(test_hash_rs());
    TEST(test_hash_js());
    TEST(test_hash_elf());
    TEST(test_hash_sdbm());
    TEST(test_hash_djb());
    TEST(test_hash_dek());
    TEST(test_hash_fnv());
    TEST(test_hash_ap());
    TEST(test_hash_murmur());
    TEST(test_hash_super_fast_hash());
    TEST(test_hash_xxhash());
}

int main(void)
{
    TEST_INIT("HASH");
    test();
    TEST_SUMMARY();
}