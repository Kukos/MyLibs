#include <common.h>
#include <compiler.h>
#include <test.h>
#include <string.h>
#include <generic.h>
#include <stdbool.h>
#include <stdint.h>

test_f test_to_string(void)
{
#define KUKOS
#define C_MASTER

    char str1[] = {"KUKOS"};
    char str2[] = {"C_MASTER"};

    T_EXPECT(strcmp(tostring(KUKOS), str1), 0);
    T_EXPECT(strcmp(tostring(C_MASTER), str2), 0);

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

test_f test_container_of(void)
{
    MyStruct s1;
    MyStruct2 *s2 = &(s1.st2);
    MyStruct *ss1 = container_of(s2, MyStruct, st2);

    T_ASSERT(&s1, ss1);
}

test_f test_init_variable(void)
{
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

    T_ASSERT(a, aa);
    T_ASSERT(b, bb);
    T_ASSERT(c, cc);
    T_ASSERT(d, dd);
    T_ASSERT(e, ee);
}

test_f test_least_one(void)
{
    int a = 0x123450 | 1 << 2;
    long b = 0x12340 | 1 << 3;
    long long c = 0x1234567890 | 1 << 4;

    T_EXPECT(least_1_int(a), 2);
    T_EXPECT(least_1_long(b), 3);
    T_EXPECT(least_1_longlong(c), 4);

    a |= 1 << 0;
    b |= 1 << 0;
    c |= 1 << 0;

    T_EXPECT(least_1_int(a), 0);
    T_EXPECT(least_1_long(b), 0);
    T_EXPECT(least_1_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(least_1_int(a), -1);
    T_EXPECT(least_1_long(b), -1);
    T_EXPECT(least_1_longlong(c), -1);
}

test_f test_leading_zeros(void)
{
    int a = 0x0000F234;
#ifdef __ARCH_64__
    long b = 0x00000000F2345678;
#elif defined(__ARCH_32__)
    long b = 0x00000F234;
#endif
    long long c = 0x0000000000F23456;

    T_EXPECT(leading_0_int(a), 4 * 4);
#ifdef __ARCH_64__
    T_EXPECT(leading_0_long(b), 8 * 4);
#elif defined(__ARCH_32__)
    T_EXPECT(leading_0_long(b), 5 * 4);
#endif
    T_EXPECT(leading_0_longlong(c), 10 * 4);

    a = 0xF0000000;
#ifdef __ARCH_64__
    b = 0xF000000000000000;
#elif defined(__ARCH_32__)
    b = 0xF0000000;
#endif
    c = 0xF000000000000000;

    T_EXPECT(leading_0_int(a), 0);
    T_EXPECT(leading_0_long(b), 0);
    T_EXPECT(leading_0_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(leading_0_int(a), sizeof(typeof(a)) << 3);
    T_EXPECT(leading_0_long(b), sizeof(typeof(b)) << 3);
    T_EXPECT(leading_0_longlong(c), sizeof(typeof(c)) << 3);
}

test_f test_frailing_zeros(void)
{
    int a = 0x1234F000;
#ifdef __ARCH_64__
    long b = 0x1234567F00000000;
#elif defined(__ARCH_32__)
    long b = 0x123F0000;
#endif
    long long c = 0x12345678F000000;

    T_EXPECT(trailing_0_int(a), 3 * 4);
#ifdef __ARCH_64__
    T_EXPECT(trailing_0_long(b), 8 * 4);
#elif defined(__ARCH_32__)
    T_EXPECT(trailing_0_long(b), 4 * 4);
#endif
    T_EXPECT(trailing_0_longlong(c), 6 * 4);

    a = 0xF;
    b = 0xF;
    c = 0xF;

    T_EXPECT(trailing_0_int(a), 0);
    T_EXPECT(trailing_0_long(b), 0);
    T_EXPECT(trailing_0_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(trailing_0_int(a), sizeof(typeof(a)) << 3);
    T_EXPECT(trailing_0_long(b), sizeof(typeof(b)) << 3);
    T_EXPECT(trailing_0_longlong(c), sizeof(typeof(c)) << 3);
}

test_f test_number_ones(void)
{
    int a = 0x10204080;
#ifdef __ARCH_64__
    long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    long b = 0x01020408;
#endif
    long long c = 0x0102040801020408;

    T_EXPECT(number_1_int(a), 4);
#ifdef __ARCH_64__
    T_EXPECT(number_1_long(b), 8);
#elif defined(__ARCH_32__)
    T_EXPECT(number_1_long(b), 4);
#endif
    T_EXPECT(number_1_longlong(c), 8);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(number_1_int(a), 0);
    T_EXPECT(number_1_long(b), 0);
    T_EXPECT(number_1_longlong(c), 0);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    T_EXPECT(number_1_int(a), sizeof(typeof(a)) << 3);
    T_EXPECT(number_1_long(b), sizeof(typeof(b)) << 3);
    T_EXPECT(number_1_longlong(c), sizeof(typeof(c)) << 3);
}

test_f test_ones_parity(void)
{
    int a = 0x10204080;
#ifdef __ARCH_64__
    long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    long b = 0x01020408;
#endif
    long long c = 0x0102040801020408;

    T_EXPECT(parity_1_int(a), true);
    T_EXPECT(parity_1_long(b), true);
    T_EXPECT(parity_1_longlong(c), true);

    a = 0x10204081;
#ifdef __ARCH_64__
    b = 0x1020408010204081;
#elif defined(__ARCH_32__)
    b = 0x01020418;
#endif
    c = 0x0102040801020418;

    T_EXPECT(parity_1_int(a), false);
    T_EXPECT(parity_1_long(b), false);
    T_EXPECT(parity_1_longlong(c), false);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(parity_1_int(a), true);
    T_EXPECT(parity_1_long(b), true);
    T_EXPECT(parity_1_longlong(c), true);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    T_EXPECT(parity_1_int(a), true);
    T_EXPECT(parity_1_long(b), true);
    T_EXPECT(parity_1_longlong(c), true);

    a = 0xFFFFFFFE;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFE;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFE;
#endif
    c = 0xFFFFFFFFFFFFFFFE;

    T_EXPECT(parity_1_int(a), false);
    T_EXPECT(parity_1_long(b), false);
    T_EXPECT(parity_1_longlong(c), false);
}

test_f test_swap_bytes(void)
{
    int16_t a = 0x1A2B;
    int32_t b = 0x1234ABCD;
    int64_t c = 0x12345678ABCDEF10;

    T_EXPECT(swap_bytes16(a), 0x2B1A);
    T_EXPECT(swap_bytes32(b), 0xCDAB3412);
    T_EXPECT(swap_bytes64(c), 0x10EFCDAB78563412);
}

int test(void)
{
    TEST(test_to_string());
    TEST(test_container_of());
    TEST(test_init_variable());
    TEST(test_least_one());
    TEST(test_leading_zeros());
    TEST(test_frailing_zeros());
    TEST(test_number_ones());
    TEST(test_ones_parity());
    TEST(test_swap_bytes());

    return 0;
}

int main(void)
{
    TEST_INIT("COMPILER");
    test();
    TEST_SUMMARY;

    return 0;
}
