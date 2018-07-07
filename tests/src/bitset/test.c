#include <bitset.h>
#include <test.h>
#include <generic.h>

test_f test_create(void)
{
    const size_t bits = 10;
    size_t i;
    Bitset *set = bitset_create(bits);
    T_ERROR(set == NULL);

    T_CHECK(bitset_get_set(set, NULL) != NULL);
    T_EXPECT(bitset_get_size(set), bits);

    for (i = 0; i < bits; ++i)
        T_EXPECT(bitset_get_bit(set, i), 0);

    bitset_destroy(set);
}

test_f test_set_get_bit(void)
{
    const size_t bits = BIT(10);
    const int bit[] = {1, 10, 100, 150, 1000};
    size_t i;
    size_t j;
    int value;
    Bitset *set = bitset_create(bits);
    T_ERROR(set == NULL);

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_set_bit(set, bit[i]);


    for (i = 0; i < bits; ++i)
    {
        value = 0;
        for (j = 0; j < ARRAY_SIZE(bit); ++j)
            if (bit[j] == i)
                value = 1;

        T_EXPECT(bitset_get_bit(set, i), value);
    }

    bitset_destroy(set);
}

test_f test_clear_bit(void)
{
    const size_t bits = BIT(10);
    const int bit[] = {1, 10, 100, 150, 1000};
    size_t i;
    size_t j;
    int value;
    Bitset *set = bitset_create(bits);
    T_ERROR(set == NULL);

    for (i = 0; i < bits; ++i)
        bitset_set_bit(set, i);

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_clear_bit(set, bit[i]);


    for (i = 0; i < bits; ++i)
    {
        value = 1;
        for (j = 0; j < ARRAY_SIZE(bit); ++j)
            if (bit[j] == i)
                value = 0;

        T_EXPECT(bitset_get_bit(set, i), value);
    }

    bitset_destroy(set);
}

test_f test_flip_bit(void)
{
    const size_t bits = BIT(10);
    const int bit[] = {1, 10, 100, 150, 1000};
    size_t i;
    size_t j;
    int value;
    Bitset *set = bitset_create(bits);
    T_ERROR(set == NULL);

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_set_bit(set, bit[i]);

    for (i = 0; i < bits; ++i)
        bitset_flip_bit(set, i);

    for (i = 0; i < bits; ++i)
    {
        value = 1;
        for (j = 0; j < ARRAY_SIZE(bit); ++j)
            if (bit[j] == i)
                value = 0;

        T_EXPECT(bitset_get_bit(set, i), value);
    }

    bitset_destroy(set);
}


test_f test_set_bit_value(void)
{
    const size_t bits = BIT(10);
    const int bit[] = {1, 10, 100, 150, 1000};
    size_t i;
    size_t j;
    int value;
    Bitset *set = bitset_create(bits);
    T_ERROR(set == NULL);

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_set_bit_value(set, bit[i], 1);

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_set_bit_value(set, bit[i], 1);

    for (i = 0; i < bits; ++i)
    {
        value = 0;
        for (j = 0; j < ARRAY_SIZE(bit); ++j)
            if (bit[j] == i)
                value = 1;

        T_EXPECT(bitset_get_bit(set, i), value);
    }

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_set_bit_value(set, bit[i], 0);

    for (i = 0; i < bits; ++i)
        T_EXPECT(bitset_get_bit(set, i), 0);

    bitset_destroy(set);
}

test_f test_foreach(void)
{
    const size_t bits = BIT(10);
    const int bit[] = {1, 10, 100, 150, 1000};
    size_t i;
    size_t j;
    size_t counter;

    BYTE value;
    BYTE value2;
    DWORD word;
    Bitset *set = bitset_create(bits);
    T_ERROR(set == NULL);

    for (i = 0; i < ARRAY_SIZE(bit); ++i)
        bitset_set_bit(set, bit[i]);

    i = 0;
    counter = 0;
    for_each(set, Bitset, word, value)
    {
        value2 = 0;
        for (j = 0; j < ARRAY_SIZE(bit); ++j)
            if (bit[j] == i)
                value2 = 1;

        T_ASSERT(value, value2);
        ++i;
        ++counter;
    }
    T_ASSERT(counter, bits);

    i = bits - 1;
    counter = 0;
    for_each_prev(set, Bitset, word, value)
    {
        value2 = 0;
        for (j = 0; j < ARRAY_SIZE(bit); ++j)
            if (bit[j] == i)
                value2 = 1;

        T_ASSERT(value, value2);
        --i;
        ++counter;
    }
    T_ASSERT(counter, bits);

    bitset_destroy(set);
}


void test(void)
{
    TEST(test_create());
    TEST(test_set_get_bit());
    TEST(test_clear_bit());
    TEST(test_flip_bit());
    TEST(test_set_bit_value());
    TEST(test_foreach());
}

int main(void)
{
    TEST_INIT("BITSET");
    test();
    TEST_SUMMARY();
}