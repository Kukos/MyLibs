#include <common.h>
#include <compiler.h>
#include <test.h>
#include <stdbool.h>
#include <generic.h>

test_f test_bits_operations(void)
{
    int a;
    long long b;

    a = BIT(5);
    b = BIT(50);

    T_ASSERT(a, 1 << 5);
    T_ASSERT(b, 1ull << 50);

    SET_BIT(a, 3);
    SET_BIT(a, 3);

    SET_BIT(b, 54);
    SET_BIT(b, 54);

    T_ASSERT(a, 1 << 5 | 1 << 3);
    T_ASSERT(b, (long long)(1ull << 50 | 1ull << 54));

    T_EXPECT(GET_BIT(a, 3), 1);
    T_EXPECT(GET_BIT(a, 4), 0);

    T_EXPECT(GET_BIT(b, 54), 1);
    T_EXPECT(GET_BIT(b, 60), 0);

    CLEAR_BIT(a, 3);
    CLEAR_BIT(a, 10);

    CLEAR_BIT(b, 50);
    CLEAR_BIT(b, 11);

    T_ASSERT(a, 1 << 5);
    T_ASSERT(b, 1ull << 54);

    a = 0x1010;
    SET_BIT_VALUE(a, 2, 1);
    T_ASSERT(a, 0x1014);

    SET_BIT_VALUE(a, 4, 0);
    T_ASSERT(a, 0x1004);

    a = 0x1010;
    FLIP_BIT(a, 0);
    FLIP_BIT(a, 4);

    T_ASSERT(a, 0x1001);
}

test_f test_macro_swap(void)
{
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

    T_ASSERT(a, 10);
    T_ASSERT(b, 5);
    T_ASSERT(c, 21);
    T_ASSERT(d, 12);
    T_ASSERT((long)e, (long)0x54321);
    T_ASSERT((long)f, (long)0x12345);
    T_ASSERT(t[0], 2);
    T_ASSERT(t[1], 1);
    T_ASSERT(t[2], 4);
    T_ASSERT(t[3], 3);
}

test_f test_min(void)
{
    int a = 5;
    int b = 10;

    long c = 12;
    long d = 21;

    void *e = (void *)0x12345;
    void *f = (void *)0x54321;

    double g = 1.234;
    double h = 5.678;

    int t[100] = {1,2,3,4};

    T_EXPECT(MIN(a, b), 5);
    T_EXPECT(MIN(c, d), 12);
    T_EXPECT(MIN((long)a, c),5);
    T_EXPECT(MIN(++a, b), 6);
    T_EXPECT(MIN(a++, b), 6);
    T_EXPECT(MIN((long)e, (long)f), (long)0x12345);
    T_EXPECT(MIN(g, h), (double)1.234);
    T_EXPECT(MIN(t[0], t[1]), 1);
    T_EXPECT(MIN(*(t + 2), *(t + 3)), 3);
}

test_f test_max(void)
{
    int a = 5;
    int b = 10;

    long c = 12;
    long d = 21;

    void *e = (void *)0x12345;
    void *f = (void *)0x54321;

    double g = 1.234;
    double h = 5.678;

    int t[100] = {1,2,3,4};

    T_EXPECT(MAX(a, b), 10);
    T_EXPECT(MAX(c, d), 21);
    T_EXPECT(MAX((long)a, c),12);
    T_EXPECT(MAX(a, ++b), 11);
    T_EXPECT(MAX(a, b++), 11);
    T_EXPECT(MAX((long)e, (long)f), (long)0x54321);
    T_EXPECT(MAX(g, h), (double)5.678);
    T_EXPECT(MAX(t[0], t[1]), 2);
    T_EXPECT(MAX(*(t + 2), *(t + 3)), 4);
}

test_f test_abs(void)
{
    int a = 4;
    int b = -5;

    long c = 10;
    long d = -10;

    double e = 1.2;
    double f = -3.4;

    T_EXPECT(ABS(a), 4);
    T_EXPECT(ABS(++a), 5);
    T_EXPECT(ABS(a++), 5);
    T_EXPECT(ABS(b), 5);
    T_EXPECT(ABS(c), 10);
    T_EXPECT(ABS(d), 10);
    T_EXPECT(ABS(e), (double)1.2);
    T_EXPECT(ABS(f), (double)3.4);

    a = -1;

    T_EXPECT(ABS(a++), 1);
    T_EXPECT(ABS(a), 0);
    T_EXPECT(ABS(--a), 1);

    a = -1;
    T_EXPECT(ABS(++a), 0);
}

test_f test_odd_even(void)
{
    int a = 7;
    int b = 12;

    long c = -12;
    long d = -11;

    T_EXPECT(ODD(a), true);
    T_EXPECT(EVEN(a), false);

    T_EXPECT(ODD(b), false);
    T_EXPECT(EVEN(b), true);

    T_EXPECT(ODD(c), false);
    T_EXPECT(EVEN(c), true);

    T_EXPECT(ODD(d), true);
    T_EXPECT(EVEN(d), false);
}

test_f test_hamming_weight(void)
{
    int a = 0x10204080;
#ifdef __ARCH_64__
    long b = 0x1020408010204080;
#elif defined(__ARCH_32__)
    long b = 0x01020408;
#endif
    long long c = 0x0102040801020408;

    T_EXPECT(HAMM_WEIGHT_int(a), 4);
#ifdef __ARCH_64__
    T_EXPECT(HAMM_WEIGHT_long(b), 8);
#elif defined(__ARCH_32__)
    T_EXPECT(HAMM_WEIGHT_long(b), 4);
#endif
    T_EXPECT(HAMM_WEIGHT_longlong(c), 8);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(HAMM_WEIGHT_int(a), 0);
    T_EXPECT(HAMM_WEIGHT_long(b), 0);
    T_EXPECT(HAMM_WEIGHT_longlong(c), 0);

    a = 0xFFFFFFFF;
#ifdef __ARCH_64__
    b = 0xFFFFFFFFFFFFFFFF;
#elif defined(__ARCH_32__)
    b = 0xFFFFFFFF;
#endif
    c = 0xFFFFFFFFFFFFFFFF;

    T_EXPECT(HAMM_WEIGHT_int(a), sizeof(typeof(a)) << 3);
    T_EXPECT(HAMM_WEIGHT_long(b), sizeof(typeof(b)) << 3);
    T_EXPECT(HAMM_WEIGHT_longlong(c), sizeof(typeof(c)) << 3);
}

test_f test_hamming_distance(void)
{
    int a = 0x10204080;
    int b = 0x10044088;
#ifdef __ARCH_64__
    long c = 0x1020408010204080;
    long d = 0x1020440811220080;
#elif defined(__ARCH_32__)
    long c = 0x01020408;
    long d = 0x00102400;
#endif
    long long e = 0x0102040801020408;
    long long f = 0x1002400800401000;

    T_EXPECT(HAMM_DIST_int(a, b), 3);
#ifdef __ARCH_64__
    T_EXPECT(HAMM_DIST_long(c, d), 6);
#elif defined(__ARCH_32__)
    T_EXPECT(HAMM_DIST_long(c, d), 5);
#endif
    T_EXPECT(HAMM_DIST_longlong(e, f), 10);

    a = 123456;
    b = 123456;

    c = 1234;
    d = 1234;

    e = 123456789;
    f = 123456789;

    T_EXPECT(HAMM_DIST_int(a, b), 0);
    T_EXPECT(HAMM_DIST_long(c, d), 0);
    T_EXPECT(HAMM_DIST_longlong(e, f), 0);

    a = 0x1020;
    b = 0;

    c = 0x1248;
    d = 0;

    e = 0x12488421;
    f = 0;

    T_EXPECT(HAMM_DIST_int(a, b), 2);
    T_EXPECT(HAMM_DIST_long(c, d), 4);
    T_EXPECT(HAMM_DIST_longlong(e, f), 8);
}

test_f test_log2(void)
{
    int a = BIT(5);
    long b = BIT(15);
    long long c = BIT(40);

    T_EXPECT(LOG2_int(a), 5);
    T_EXPECT(LOG2_long(b), 15);
    T_EXPECT(LOG2_longlong(c), 40);

    T_EXPECT(LOG2_int(--a), 4);
    T_EXPECT(LOG2_long(--b), 14);
    T_EXPECT(LOG2_longlong(--c), 39);

    a = 1;
    b = 1l;
    c = 1ll;

    T_EXPECT(LOG2_int(a), 0);
    T_EXPECT(LOG2_long(b), 0);
    T_EXPECT(LOG2_longlong(c), 0);

    a = 0;
    b = 0;
    c = 0;

    T_EXPECT(LOG2_int(a), -1);
    T_EXPECT(LOG2_long(b), -1);
    T_EXPECT(LOG2_longlong(c), -1);
}

CMP(char)
CMP(int)
CMP(double)

test_f test_cmp_macro(void)
{
    char a;
    char b;

    int c;
    int d;

    double e;
    double f;

    a = 1;
    b = 1;
    T_EXPECT(cmp_char((void *)&a, (void *)&b), 0);

    a = 10;
    b = 20;
    T_EXPECT(cmp_char((void *)&a, (void *)&b), -1);

    a = 100;
    b = 20;
    T_EXPECT(cmp_char((void *)&a, (void *)&b), 1);

    c = 11;
    d = 11;
    T_EXPECT(cmp_int((void *)&c, (void *)&d), 0);

    c = 120;
    d = 10000;
    T_EXPECT(cmp_int((void *)&c, (void *)&d), -1);

    c = 99999;
    d = -10000;
    T_EXPECT(cmp_int((void *)&c, (void *)&d), 1);

    e = -19.09;
    f = -19.09;
    T_EXPECT(cmp_double((void *)&e, (void *)&f), 0);

    e = -1000.212;
    f = 123123.2131;
    T_EXPECT(cmp_double((void *)&e, (void *)&f), -1);

    e = 213123123.2131;
    f = 123.3123;
    T_EXPECT(cmp_double((void *)&e, (void *)&f), 1);
}

ARRAY_EQUAL(char)
ARRAY_EQUAL(int)
ARRAY_EQUAL(double)

test_f test_array_cmp_macro(void)
{
    char a[] = "Kukos";
    char b[] = "Master C";
    char c[] = "Kukos";

    int d[] = {1, 2, 3, 4, 5};
    int e[] = {5, 4, 3, 2, 1};
    int f[] = {1, 2, 3, 4, 5};

    double g[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    double h[] = {1.5, 2.4, 3.3, 4.2, 5.1};
    double i[] = {1.1, 2.2, 3.3, 4.4, 5.5};


    T_EXPECT(array_equal_char(a, b, ARRAY_SIZE(a)), false);
    T_EXPECT(array_equal_char(b, c, ARRAY_SIZE(c)), false);
    T_EXPECT(array_equal_char(a, c, ARRAY_SIZE(a)), true);

    T_EXPECT(array_equal_int(d, e, ARRAY_SIZE(d)), false);
    T_EXPECT(array_equal_int(e, f, ARRAY_SIZE(e)), false);
    T_EXPECT(array_equal_int(d, f, ARRAY_SIZE(d)), true);

    T_EXPECT(array_equal_double(g, h, ARRAY_SIZE(g)), false);
    T_EXPECT(array_equal_double(h, i, ARRAY_SIZE(h)), false);
    T_EXPECT(array_equal_double(g, i, ARRAY_SIZE(g)), true);
}

ARRAY_REVERSE(char)
ARRAY_REVERSE(int)
ARRAY_REVERSE(double)

test_f test_array_reverse_macro(void)
{
    char a[] = {'K', 'u', 'k', 'o', 's'};
    char b[] = "sokuK";

    int c[] = {1, 2, 3, 4, 5};
    int d[] = {5, 4, 3, 2, 1};

    double e[] = {1.1, 2.2, 3.3, 4.4, 5.5};
    double f[] = {5.5, 4.4, 3.3, 2.2, 1.1};

    array_reverse_char(a, ARRAY_SIZE(a));
    T_EXPECT(array_equal_char(a, b, ARRAY_SIZE(a)), true);

    array_reverse_int(c, ARRAY_SIZE(c));
    T_EXPECT(array_equal_int(c, d, ARRAY_SIZE(c)), true);

    array_reverse_double(e, ARRAY_SIZE(e));
    T_EXPECT(array_equal_double(e, f, ARRAY_SIZE(e)), true);
}

void test(void)
{
    TEST(test_bits_operations());
    TEST(test_macro_swap());
    TEST(test_min());
    TEST(test_max());
    TEST(test_abs());
    TEST(test_odd_even());
    TEST(test_hamming_weight());
    TEST(test_hamming_distance());
    TEST(test_log2());
    TEST(test_cmp_macro());
    TEST(test_array_cmp_macro());
    TEST(test_array_reverse_macro());
}

int main(void)
{
    TEST_INIT("COMMON");
    test();
    TEST_SUMMARY;
}
