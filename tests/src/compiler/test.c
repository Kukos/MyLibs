#include <common.h>
#include <compiler.h>
#include <test.h>
#include <string.h>
#include <generic.h>
#include <stdbool.h>
#include <stdint.h>

test_f test_to_string(void)
{
#define C_MASTER KUKOS

    char str1[] = {"KUKOS"};
    char str2[] = {"C_MASTER"};

    T_EXPECT(strcmp(tostring(C_MASTER), str1), 0);

#undef C_MASTER
    T_EXPECT(strcmp(tostring(KUKOS), str1), 0);
    T_EXPECT(strcmp(tostring(C_MASTER), str2), 0);
}

test_f test_concat(void)
{
    char str1[] = {"KUKOS"};
    char str2[] = {"C_MASTER"};

#define PART1 KU
#define PART2 KOS


#define C_MAS C_
#define TER MASTER
#define PART21 C_MAS
#define PART22 TER

    T_EXPECT(strcmp(tostring(concat(KUK, OS)), str1), 0);
    T_EXPECT(strcmp(tostring(concat(C_, MASTER)), str2), 0);

    T_EXPECT(strcmp(tostring(concat(PART1, PART2)), str1), 0);
    T_EXPECT(strcmp(tostring(concat(PART21, PART22)), str2), 0);

#undef PART1
#undef PART2
#undef PART21
#undef PART22
#undef C_
#undef MASTER
}

test_f test_unique_var_name(void)
{

#define check_unique_var_name(name) \
    int name = 0; \
    unused(name);

    check_unique_var_name(unique_var_name(var));
    check_unique_var_name(unique_var_name(var));
    check_unique_var_name(unique_var_name(var1));
    check_unique_var_name(unique_var_name(var1));
    check_unique_var_name(unique_var_name(var));
    check_unique_var_name(unique_var_name(var1));

#undef check_unique_var_name
}

typedef struct MyStruct2
{
    char a;
    char b;
    char c;
    char d;
}MyStruct2;

typedef struct MyStruct
{
    int a;
    int b;
    MyStruct2 st2;
    int c;
    int d;
}MyStruct;

test_f test_container_of(void)
{
    MyStruct s1;
    MyStruct2 *s2 = &(s1.st2);
    MyStruct *ss1 = container_of(s2, MyStruct, st2);

    T_ASSERT(&s1, ss1);
}

test_f test_init_variable(void)
{
    /* random values on stack */
    int a;
    double b;
    char c;
    int *d;
    void *e;
    MyStruct s;
    MyStruct s2 = {0};

    init_var(a);
    init_var(b);
    init_var(c);
    init_var(d);
    init_var(e);
    init_var(s);

    T_ASSERT(a, (int)0);
    T_ASSERT(b, (double)0);
    T_ASSERT(c, (char)0);
    T_ASSERT(d, (int *)0);
    T_ASSERT(e, (void *)0);
    T_ASSERT(memcmp(&s, &s2, sizeof(MyStruct)), 0);
}

test_f test_likely(void)
{
    int a = 0;
    int volatile c1 = 5;
    int volatile c2 = 10;

    if (likely(c1 < c2))
        a = 5;

    T_ASSERT(a, 5);

    if (unlikely(c1 != c2))
        a = 10;

    T_ASSERT(a, 10);

    if (likely(c1 + 100 == c2))
        a = 100;

    T_ASSERT(a, 10);

    if (unlikely(c1 > c2))
        a = 50;

    T_ASSERT(a, 10);
}

test_f test_static_assert(void)
{
    static_assert(sizeof(char) == 1);
}

test_f test_least_one(void)
{
    unsigned int a = 0x123450 | 1 << 2;
    unsigned long b = 0x12340 | 1 << 3;
    unsigned long long c = 0x1234567890 | 1 << 4;

    T_EXPECT(least_1(a), 2);
    T_EXPECT(least_1(b), 3);
    T_EXPECT(least_1(c), 4);

    a |= 1 << 0;
    b |= 1 << 0;
    c |= 1 << 0;

    T_EXPECT(least_1(a), 0);
    T_EXPECT(least_1(b), 0);
    T_EXPECT(least_1(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(least_1(a), -1);
    T_EXPECT(least_1(b), -1);
    T_EXPECT(least_1(c), -1);
}

test_f test_leading_zeros(void)
{
    unsigned int a = 0x0000F234;
#ifdef __ARCH_64__
    unsigned long b = 0x00000000F2345678;
#elif defined(__ARCH_32__)
    unsigned long b = 0x00000F234;
#endif
    unsigned long long c = 0x0000000000F23456;

    T_EXPECT(leading_0(a), 4 * 4);
#ifdef __ARCH_64__
    T_EXPECT(leading_0(b), 8 * 4);
#elif defined(__ARCH_32__)
    T_EXPECT(leading_0(b), 5 * 4);
#endif
    T_EXPECT(leading_0(c), 10 * 4);

    a = 0xF0000000;
#ifdef __ARCH_64__
    b = 0xF000000000000000;
#elif defined(__ARCH_32__)
    b = 0xF0000000;
#endif
    c = 0xF000000000000000;

    T_EXPECT(leading_0(a), 0);
    T_EXPECT(leading_0(b), 0);
    T_EXPECT(leading_0(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(leading_0(a), sizeof(typeof(a)) << 3);
    T_EXPECT(leading_0(b), sizeof(typeof(b)) << 3);
    T_EXPECT(leading_0(c), sizeof(typeof(c)) << 3);
}

test_f test_frailing_zeros(void)
{
    unsigned int a = 0x1234F000;
#ifdef __ARCH_64__
    unsigned long b = 0x1234567F00000000;
#elif defined(__ARCH_32__)
    unsigned long b = 0x123F0000;
#endif
    unsigned long long c = 0x12345678F000000;

    T_EXPECT(trailing_0(a), 3 * 4);
#ifdef __ARCH_64__
    T_EXPECT(trailing_0(b), 8 * 4);
#elif defined(__ARCH_32__)
    T_EXPECT(trailing_0(b), 4 * 4);
#endif
    T_EXPECT(trailing_0(c), 6 * 4);

    a = 0xF;
    b = 0xF;
    c = 0xF;

    T_EXPECT(trailing_0(a), 0);
    T_EXPECT(trailing_0(b), 0);
    T_EXPECT(trailing_0(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(trailing_0(a), sizeof(typeof(a)) << 3);
    T_EXPECT(trailing_0(b), sizeof(typeof(b)) << 3);
    T_EXPECT(trailing_0(c), sizeof(typeof(c)) << 3);
}

test_f test_number_ones(void)
{
    unsigned int a = 0x10204080;
#ifdef __ARCH_64__
    unsigned long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    unsigned long b = 0x01020408;
#endif
    unsigned long long c = 0x0102040801020408;

    T_EXPECT(number_1(a), 4);
#ifdef __ARCH_64__
    T_EXPECT(number_1(b), 8);
#elif defined(__ARCH_32__)
    T_EXPECT(number_1(b), 4);
#endif
    T_EXPECT(number_1(c), 8);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(number_1(a), 0);
    T_EXPECT(number_1(b), 0);
    T_EXPECT(number_1(c), 0);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    T_EXPECT(number_1(a), sizeof(typeof(a)) << 3);
    T_EXPECT(number_1(b), sizeof(typeof(b)) << 3);
    T_EXPECT(number_1(c), sizeof(typeof(c)) << 3);
}

test_f test_ones_parity(void)
{
    unsigned int a = 0x10204080;
#ifdef __ARCH_64__
    unsigned long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    unsigned long b = 0x01020408;
#endif
    unsigned long long c = 0x0102040801020408;

    T_EXPECT(parity_1(a), true);
    T_EXPECT(parity_1(b), true);
    T_EXPECT(parity_1(c), true);

    a = 0x10204081;
#ifdef __ARCH_64__
    b = 0x1020408010204081;
#elif defined(__ARCH_32__)
    b = 0x01020418;
#endif
    c = 0x0102040801020418;

    T_EXPECT(parity_1(a), false);
    T_EXPECT(parity_1(b), false);
    T_EXPECT(parity_1(c), false);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(parity_1(a), true);
    T_EXPECT(parity_1(b), true);
    T_EXPECT(parity_1(c), true);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    T_EXPECT(parity_1(a), true);
    T_EXPECT(parity_1(b), true);
    T_EXPECT(parity_1(c), true);

    a = 0xFFFFFFFE;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFE;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFE;
#endif
    c = 0xFFFFFFFFFFFFFFFE;

    T_EXPECT(parity_1(a), false);
    T_EXPECT(parity_1(b), false);
    T_EXPECT(parity_1(c), false);
}

test_f test_swap_bytes(void)
{
    uint16_t a = 0x1A2B;
    uint32_t b = 0x1234ABCD;
    uint64_t c = 0x12345678ABCDEF10;

    T_EXPECT(swap_bytes16(a), 0x2B1A);
    T_EXPECT(swap_bytes32(b), 0xCDAB3412);
    T_EXPECT(swap_bytes64(c), 0x10EFCDAB78563412);
}

test_f test_barrier(void)
{
    int a;
    int b;

    a = 0;
    T_ASSERT(a, 0);
    sw_mem_barrier();
    b = 1;
    T_ASSERT(a, 0);
    T_ASSERT(b, 1);
    hw_mem_barrier();
    a = 1;
    T_ASSERT(a, 1);
    T_ASSERT(b, 1);
}

test_f test_atomic(void)
{
    int a;

    atomic(a = 1);
    T_ASSERT(a, 1);
}

test_f test_alloc_on_stack(void)
{
    const size_t size = 100;
    char *t = alloc_on_stack(size);
    char *t2 = alloc_on_stack_align(size, 16);
    size_t i;

    for (i = 0; i < size; ++i)
        t[i] = t2[i] = (char)i;

    for (i = 0; i < size; ++i)
    {
        T_ASSERT(t[i], i);
        T_ASSERT(t2[i], i);
    }

}

test_f test_cache(void)
{
    const size_t size = BIT(12); /* 4KB */
    char *t;
    size_t i;

    t = malloc(size);
    T_ERROR(t == NULL);

    /* high prio */
    load_to_cache(t, CACHE_WRITE, CACHE_SAVE_HIGH_PRIO);
    for (i = 0; i < size; ++i)
        t[i] = 'a' + (i % ('z' - 'a' + 1));

    clear_cache(t, t + size);
    hw_mem_barrier();

    load_to_cache(t, CACHE_READ, CACHE_NO_SAVE);
    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], 'a' + (i % ('z' - 'a' + 1)));

    /*  normal prio */
    load_to_cache(t, CACHE_WRITE, CACHE_SAVE_NORMAL_PRIO);
    for (i = 0; i < size; ++i)
        t[i] = 'a' + (i % ('z' - 'a' + 1));

    clear_cache(t, t + size);
    hw_mem_barrier();

    load_to_cache(t, CACHE_READ, CACHE_NO_SAVE);
    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], 'a' + (i % ('z' - 'a' + 1)));

    /* low prio */
    load_to_cache(t, CACHE_WRITE, CACHE_SAVE_LOW_PRIO);
    for (i = 0; i < size; ++i)
        t[i] = 'a' + (i % ('z' - 'a' + 1));

    clear_cache(t, t + size);
    hw_mem_barrier();

    load_to_cache(t, CACHE_READ, CACHE_NO_SAVE);
    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], 'a' + (i % ('z' - 'a' + 1)));

    FREE(t);
}

test_f test_write_once(void)
{
    int i;
    char c;
    double d;
    uint8_t t[10];

    int ii = 10;
    char cc = 'a';
    double dd = 3.14;
    uint8_t tt[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    WRITE_ONCE(i, ii);
    WRITE_ONCE(c, cc);
    WRITE_ONCE(d, dd);
    WRITE_ONCE(t, tt);

    T_ASSERT(i, i);
    T_ASSERT(c, cc);
    T_ASSERT(d, dd);
    T_ASSERT(memcmp(t, tt, sizeof(t)), 0);
}

test_f test_read_once(void)
{
    int i;
    char c;
    double d;
    uint8_t t[10];

    int ii = 10;
    char cc = 'a';
    double dd = 3.14;
    uint8_t tt[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    READ_ONCE(i, ii);
    READ_ONCE(c, cc);
    READ_ONCE(d, dd);
    READ_ONCE(t, tt);

    T_ASSERT(i, i);
    T_ASSERT(c, cc);
    T_ASSERT(d, dd);
    T_ASSERT(memcmp(t, tt, sizeof(t)), 0);
}

test_f test_write_once_sync(void)
{
    int i;
    char c;
    double d;
    uint8_t t[10];

    int ii = 10;
    char cc = 'a';
    double dd = 3.14;
    uint8_t tt[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    WRITE_ONCE_SYNC(i, ii);
    WRITE_ONCE_SYNC(c, cc);
    WRITE_ONCE_SYNC(d, dd);
    WRITE_ONCE_SYNC(t, tt);

    T_ASSERT(i, i);
    T_ASSERT(c, cc);
    T_ASSERT(d, dd);
    T_ASSERT(memcmp(t, tt, sizeof(t)), 0);
}

test_f test_read_once_sync(void)
{
    int i;
    char c;
    double d;
    uint8_t t[10];

    int ii = 10;
    char cc = 'a';
    double dd = 3.14;
    uint8_t tt[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    READ_ONCE_SYNC(i, ii);
    READ_ONCE_SYNC(c, cc);
    READ_ONCE_SYNC(d, dd);
    READ_ONCE_SYNC(t, tt);

    T_ASSERT(i, i);
    T_ASSERT(c, cc);
    T_ASSERT(d, dd);
    T_ASSERT(memcmp(t, tt, sizeof(t)), 0);
}

test_f test_write_once_val(void)
{
    int i;
    char c;
    double d;

    WRITE_ONCE_VAL(i, 10);
    WRITE_ONCE_VAL(c, (char)'a');
    WRITE_ONCE_VAL(d, 3.14);

    T_ASSERT(i, 10);
    T_ASSERT(c, 'a');
    T_ASSERT(d, 3.14);
}

test_f test_read_once_val(void)
{
    int i;
    char c;
    double d;

    READ_ONCE_VAL(i, 10);
    READ_ONCE_VAL(c, (char)'a');
    READ_ONCE_VAL(d, 3.14);

    T_ASSERT(i, 10);
    T_ASSERT(c, 'a');
    T_ASSERT(d, 3.14);
}

test_f test_write_once_val_sync(void)
{
    int i;
    char c;
    double d;

    WRITE_ONCE_VAL_SYNC(i, 10);
    WRITE_ONCE_VAL_SYNC(c, (char)'a');
    WRITE_ONCE_VAL_SYNC(d, 3.14);

    T_ASSERT(i, 10);
    T_ASSERT(c, 'a');
    T_ASSERT(d, 3.14);
}

test_f test_read_once_val_sync(void)
{
    int i;
    char c;
    double d;

    READ_ONCE_VAL_SYNC(i, 10);
    READ_ONCE_VAL_SYNC(c, (char)'a');
    READ_ONCE_VAL_SYNC(d, 3.14);

    T_ASSERT(i, 10);
    T_ASSERT(c, 'a');
    T_ASSERT(d, 3.14);
}

void test(void)
{
    TEST(test_to_string());
    TEST(test_concat());
    TEST(test_unique_var_name());
    TEST(test_likely());
    TEST(test_static_assert());
    TEST(test_container_of());
    TEST(test_init_variable());
    TEST(test_least_one());
    TEST(test_leading_zeros());
    TEST(test_frailing_zeros());
    TEST(test_number_ones());
    TEST(test_ones_parity());
    TEST(test_swap_bytes());
    TEST(test_barrier());
    TEST(test_atomic());
    TEST(test_alloc_on_stack());
    TEST(test_cache());
    TEST(test_write_once());
    TEST(test_read_once());
    TEST(test_write_once_sync());
    TEST(test_read_once_sync());
    TEST(test_write_once_val());
    TEST(test_read_once_val());
    TEST(test_write_once_val_sync());
    TEST(test_read_once_val_sync());
}

int main(void)
{
    TEST_INIT("COMPILER");
    test();
    TEST_SUMMARY();
}
