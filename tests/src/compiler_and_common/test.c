#include <common.h>
#include <compiler.h>
#include <test.h>
#include <string.h>
#include <generic.h>
#include <stdbool.h>
#include <stdint.h>

test_t test_to_string(void)
{
    int ret = 0;

#define KUKOS
#define C_MASTER

    char str1[] = {"KUKOS"};
    char str2[] = {"C_MASTER"};

    ret += T_EXPECT(strcmp(tostring(KUKOS), str1), 0);
    ret += T_EXPECT(strcmp(tostring(C_MASTER), str2), 0);

    return ret ? FAILED : PASSED;

#undef KUKOS
#undef MASTER_C
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

test_t test_container_of(void)
{
    int ret = 0;

    MyStruct s1;
    MyStruct2 *s2 = &(s1.st2);
    MyStruct *ss1 = container_of(s2, MyStruct, st2);

    ret += T_ASSERT(&s1, ss1);

    return ret ? FAILED : PASSED;
}

test_t test_init_variable(void)
{
    int ret = 0;

    /* random values, "the same" behaviour as "random" val in RAM */
    int a = 5;
    double b = 5.0;
    char c = 5;
    int *d = (int *)0x12345;
    void *e = (void *)0x12345;

    int aa = a;
    double bb = b;
    char cc = c;
    int *dd = d;
    void *ee = e;

    init_var(a);
    init_var(b);
    init_var(c);
    init_var(d);
    init_var(e);

    ret += T_ASSERT(a, aa);
    ret += T_ASSERT(b, bb);
    ret += T_ASSERT(c, cc);
    ret += T_ASSERT(d, dd);
    ret += T_ASSERT(e, ee);

    return ret ? FAILED : PASSED;
}

test_t test_least_one(void)
{
    int ret = 0;

    int a = 0x123450 | 1 << 2;
    long b = 0x12340 | 1 << 3;
    long long c = 0x1234567890 | 1 << 4;

    ret += T_EXPECT(least_1_int(a), 2);
    ret += T_EXPECT(least_1_long(b), 3);
    ret += T_EXPECT(least_1_longlong(c), 4);

    a |= 1 << 0;
    b |= 1 << 0;
    c |= 1 << 0;

    ret += T_EXPECT(least_1_int(a), 0);
    ret += T_EXPECT(least_1_long(b), 0);
    ret += T_EXPECT(least_1_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    ret += T_EXPECT(least_1_int(a), -1);
    ret += T_EXPECT(least_1_long(b), -1);
    ret += T_EXPECT(least_1_longlong(c), -1);

    return ret ? FAILED : PASSED;
}

test_t test_leading_zeros(void)
{
    int ret = 0;

    int a = 0x0000F234;
#ifdef __ARCH_64__
    long b = 0x00000000F2345678;
#elif defined(__ARCH_32__)
    long b = 0x00000F234;
#endif
    long long c = 0x0000000000F23456;

    ret += T_EXPECT(leading_0_int(a), 4 * 4);
#ifdef __ARCH_64__
    ret += T_EXPECT(leading_0_long(b), 8 * 4);
#elif defined(__ARCH_32__)
    ret += T_EXPECT(leading_0_long(b), 5 * 4);
#endif
    ret += T_EXPECT(leading_0_longlong(c), 10 * 4);

    a = 0xF0000000;
#ifdef __ARCH_64__
    b = 0xF000000000000000;
#elif defined(__ARCH_32__)
    b = 0xF0000000;
#endif
    c = 0xF000000000000000;

    ret += T_EXPECT(leading_0_int(a), 0);
    ret += T_EXPECT(leading_0_long(b), 0);
    ret += T_EXPECT(leading_0_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    ret += T_EXPECT(leading_0_int(a), sizeof(typeof(a)) << 3);
    ret += T_EXPECT(leading_0_long(b), sizeof(typeof(b)) << 3);
    ret += T_EXPECT(leading_0_longlong(c), sizeof(typeof(c)) << 3);

    return ret ? FAILED : PASSED;
}

test_t test_trailing_zeros(void)
{
    int ret = 0;

    int a = 0x1234F000;
#ifdef __ARCH_64__
    long b = 0x1234567F00000000;
#elif defined(__ARCH_32__)
    long b = 0x123F0000;
#endif
    long long c = 0x12345678F000000;

    ret += T_EXPECT(trailing_0_int(a), 3 * 4);
#ifdef __ARCH_64__
    ret += T_EXPECT(trailing_0_long(b), 8 * 4);
#elif defined(__ARCH_32__)
    ret += T_EXPECT(trailing_0_long(b), 4 * 4);
#endif
    ret += T_EXPECT(trailing_0_longlong(c), 6 * 4);

    a = 0xF;
    b = 0xF;
    c = 0xF;

    ret += T_EXPECT(trailing_0_int(a), 0);
    ret += T_EXPECT(trailing_0_long(b), 0);
    ret += T_EXPECT(trailing_0_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    ret += T_EXPECT(trailing_0_int(a), sizeof(typeof(a)) << 3);
    ret += T_EXPECT(trailing_0_long(b), sizeof(typeof(b)) << 3);
    ret += T_EXPECT(trailing_0_longlong(c), sizeof(typeof(c)) << 3);

    return ret ? FAILED : PASSED;
}

test_t test_number_ones(void)
{
    int ret = 0;

    int a = 0x10204080;
#ifdef __ARCH_64__
    long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    long b = 0x01020408;
#endif
    long long c = 0x0102040801020408;

    ret += T_EXPECT(number_1_int(a), 4);
#ifdef __ARCH_64__
    ret += T_EXPECT(number_1_long(b), 8);
#elif defined(__ARCH_32__)
    ret += T_EXPECT(number_1_long(b), 4);
#endif
    ret += T_EXPECT(number_1_longlong(c), 8);

    a = 0;
    b = 0;
    c = 0;

    ret += T_EXPECT(number_1_int(a), 0);
    ret += T_EXPECT(number_1_long(b), 0);
    ret += T_EXPECT(number_1_longlong(c), 0);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    ret += T_EXPECT(number_1_int(a), sizeof(typeof(a)) << 3);
    ret += T_EXPECT(number_1_long(b), sizeof(typeof(b)) << 3);
    ret += T_EXPECT(number_1_longlong(c), sizeof(typeof(c)) << 3);

    return ret ? FAILED : PASSED;
}

test_t test_ones_parity(void)
{
    int ret = 0;

    int a = 0x10204080;
#ifdef __ARCH_64__
    long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    long b = 0x01020408;
#endif
    long long c = 0x0102040801020408;

    ret += T_EXPECT(parity_1_int(a), true);
    ret += T_EXPECT(parity_1_long(b), true);
    ret += T_EXPECT(parity_1_longlong(c), true);

    a = 0x10204081;
#ifdef __ARCH_64__
    b = 0x1020408010204081;
#elif defined(__ARCH_32__)
    b = 0x01020418;
#endif
    c = 0x0102040801020418;

    ret += T_EXPECT(parity_1_int(a), false);
    ret += T_EXPECT(parity_1_long(b), false);
    ret += T_EXPECT(parity_1_longlong(c), false);

    a = 0;
    b = 0;
    c = 0;

    ret += T_EXPECT(parity_1_int(a), true);
    ret += T_EXPECT(parity_1_long(b), true);
    ret += T_EXPECT(parity_1_longlong(c), true);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    ret += T_EXPECT(parity_1_int(a), true);
    ret += T_EXPECT(parity_1_long(b), true);
    ret += T_EXPECT(parity_1_longlong(c), true);

    a = 0xFFFFFFFE;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFE;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFE;
#endif
    c = 0xFFFFFFFFFFFFFFFE;

    ret += T_EXPECT(parity_1_int(a), false);
    ret += T_EXPECT(parity_1_long(b), false);
    ret += T_EXPECT(parity_1_longlong(c), false);

    return ret ? FAILED : PASSED;
}

test_t test_swap_bytes(void)
{
    int ret = 0;

    int16_t a = 0x1A2B;
    int32_t b = 0x1234ABCD;
    int64_t c = 0x12345678ABCDEF10;

    ret += T_EXPECT(swap_bytes16(a), 0x2B1A);
    ret += T_EXPECT(swap_bytes32(b), 0xCDAB3412);
    ret += T_EXPECT(swap_bytes64(c), 0x10EFCDAB78563412);

    return ret ? FAILED : PASSED;
}

test_t test_bits_operations(void)
{
    int ret = 0;

    int a;
    long long b;

    a = BIT(5);
    b = BIT(50);

    ret += T_ASSERT(a, 1 << 5);
    ret += T_ASSERT(b, 1ull << 50);

    SET_BIT(a, 3);
    SET_BIT(a, 3);

    SET_BIT(b, 54);
    SET_BIT(b, 54);

    ret += T_ASSERT(a, 1 << 5 | 1 << 3);
    ret += T_ASSERT(b, (long long)(1ull << 50 | 1ull << 54));

    ret += T_EXPECT(GET_BIT(a, 3), 1);
    ret += T_EXPECT(GET_BIT(a, 4), 0);

    ret += T_EXPECT(GET_BIT(b, 54), 1);
    ret += T_EXPECT(GET_BIT(b, 60), 0);

    CLEAR_BIT(a, 3);
    CLEAR_BIT(a, 10);

    CLEAR_BIT(b, 50);
    CLEAR_BIT(b, 11);

    ret += T_ASSERT(a, 1 << 5);
    ret += T_ASSERT(b, 1ull << 54);

    return ret ? FAILED : PASSED;
}

test_t test_macro_swap(void)
{
    int ret = 0;

    int a = 5;
    int b = 10;

    long c = 12;
    long d = 21;

    void *e = (void *)0x12345;
    void *f = (void *)0x54321;

    int t[100] = {1,2,3,4};

    SWAP(a, b);
    SWAP(c, d);
    SWAP(e, f);
    SWAP(t[0], t[1]);
    SWAP(*(t + 2), *(t + 3));

    ret += T_ASSERT(a, 10);
    ret += T_ASSERT(b, 5);
    ret += T_ASSERT(c, 21);
    ret += T_ASSERT(d, 12);
    ret += T_ASSERT((long)e, (long)0x54321);
    ret += T_ASSERT((long)f, (long)0x12345);
    ret += T_ASSERT(t[0], 2);
    ret += T_ASSERT(t[1], 1);
    ret += T_ASSERT(t[2], 4);
    ret += T_ASSERT(t[3], 3);

    return ret ? FAILED : PASSED;
}

test_t test_min(void)
{
    int ret = 0;

    int a = 5;
    int b = 10;

    long c = 12;
    long d = 21;

    void *e = (void *)0x12345;
    void *f = (void *)0x54321;

    double g = 1.234;
    double h = 5.678;

    int t[100] = {1,2,3,4};

    ret += T_EXPECT(MIN(a, b), 5);
    ret += T_EXPECT(MIN(c, d), 12);
    ret += T_EXPECT(MIN((long)a, c),5);
    ret += T_EXPECT(MIN(++a, b), 6);
    ret += T_EXPECT(MIN(a++, b), 6);
    ret += T_EXPECT(MIN((long)e, (long)f), (long)0x12345);
    ret += T_EXPECT(MIN(g, h), (double)1.234);
    ret += T_EXPECT(MIN(t[0], t[1]), 1);
    ret += T_EXPECT(MIN(*(t + 2), *(t + 3)), 3);

    return ret ? FAILED : PASSED;
}

test_t test_max(void)
{
    int ret = 0;

    int a = 5;
    int b = 10;

    long c = 12;
    long d = 21;

    void *e = (void *)0x12345;
    void *f = (void *)0x54321;

    double g = 1.234;
    double h = 5.678;

    int t[100] = {1,2,3,4};

    ret += T_EXPECT(MAX(a, b), 10);
    ret += T_EXPECT(MAX(c, d), 21);
    ret += T_EXPECT(MAX((long)a, c),12);
    ret += T_EXPECT(MAX(a, ++b), 11);
    ret += T_EXPECT(MAX(a, b++), 11);
    ret += T_EXPECT(MAX((long)e, (long)f), (long)0x54321);
    ret += T_EXPECT(MAX(g, h), (double)5.678);
    ret += T_EXPECT(MAX(t[0], t[1]), 2);
    ret += T_EXPECT(MAX(*(t + 2), *(t + 3)), 4);

    return ret ? FAILED : PASSED;
}

test_t test_abs(void)
{
    int ret = 0;

    

    return ret ? FAILED : PASSED;
}

int test(void)
{
    TEST(test_to_string());
    TEST(test_container_of());
    TEST(test_init_variable());
    TEST(test_least_one());
    TEST(test_leading_zeros());
    TEST(test_trailing_zeros());
    TEST(test_number_ones());
    TEST(test_ones_parity());
    TEST(test_swap_bytes());

    TEST(test_bits_operations());
    TEST(test_macro_swap());
    TEST(test_min());
    TEST(test_max());
    TEST(test_abs());

    return 0;
}

int main(void)
{
    TEST_INIT
    test();
    TEST_SUMMARY;

    return 0;
}
