#include <test.h>
#include <tuple.h>

typedef struct MyStruct
{
    int key;
    int a;
    int b;
    int c;
} MyStruct;

test_f test_tuple_create(void)
{
    int i;
    char c;
    double d;
    MyStruct s;
    MyStruct *ptr;

    Tuple *t;
    t = tuple_create(TUPLE_ENTRY(i), TUPLE_ENTRY(c), TUPLE_ENTRY(d));
    T_ERROR(t == NULL);
    tuple_destroy(t);

    t = tuple_create(TUPLE_ENTRY(i), TUPLE_ENTRY(c), TUPLE_ENTRY(d), TUPLE_ENTRY(s), TUPLE_ENTRY(ptr));
    T_ERROR(t == NULL);
    tuple_destroy(t);
}

test_f test_tuple_get_elem(void)
{
    int i = 1;
    char c = 'a';
    double d = 3.14;
    MyStruct s = {.key = 1, .a = 10, .b = 100, .c = 12345};
    MyStruct *ptr = &s;
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    Tuple *t;
    t = tuple_create(TUPLE_ENTRY(i), TUPLE_ENTRY(c), TUPLE_ENTRY(d));
    T_ERROR(t == NULL);

    T_EXPECT(*(int *)tuple_get_element(t, 0), i);
    T_EXPECT(*(char *)tuple_get_element(t, 1), c);
    T_EXPECT(*(double *)tuple_get_element(t, 2), d);
    T_CHECK(tuple_get_element(t, 3) == NULL);

    tuple_destroy(t);

    t = tuple_create(TUPLE_ENTRY(s), TUPLE_ARRAY_ENTRY(arr));
    T_ERROR(t == NULL);

    T_EXPECT(memcmp(tuple_get_element(t, 0), &s, sizeof(s)), 0);
    T_EXPECT(memcmp(tuple_get_element(t, 1), arr, sizeof(arr)), 0);
    T_CHECK(tuple_get_element(t, 2) == NULL);

    tuple_destroy(t);

    t = tuple_create(TUPLE_ENTRY(i), TUPLE_ENTRY(c), TUPLE_ENTRY(d), TUPLE_ENTRY(s), TUPLE_ENTRY(ptr));
    T_ERROR(t == NULL);

    T_EXPECT(*(int *)tuple_get_element(t, 0), i);
    T_EXPECT(*(char *)tuple_get_element(t, 1), c);
    T_EXPECT(*(double *)tuple_get_element(t, 2), d);
    T_EXPECT(memcmp(tuple_get_element(t, 3), &s, sizeof(s)), 0);
    T_EXPECT(*(MyStruct **)tuple_get_element(t, 4), ptr);
    T_EXPECT(memcmp(*(MyStruct **)tuple_get_element(t, 4), ptr, sizeof(s)), 0);
    T_CHECK(tuple_get_element(t, 5) == NULL);

    tuple_destroy(t);
}

test_f test_pair_create(void)
{
    int i;
    char c;
    MyStruct s;
    MyStruct *ptr;

    Pair *p;
    p = pair_create(TUPLE_ENTRY(i), TUPLE_ENTRY(c));
    T_ERROR(p == NULL);
    pair_destroy(p);

    p = pair_create(TUPLE_ENTRY(s), TUPLE_ENTRY(ptr));
    T_ERROR(p == NULL);
    pair_destroy(p);
}

test_f test_pair_get_elem(void)
{
    int i = 15;
    char c = 'C';
    MyStruct s = {.key = 9, .a = 12, .b = 76, .c = 666};
    MyStruct *ptr = &s;
    int arr[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double d = 5.12;

    Pair *p;
    p = pair_create(TUPLE_ENTRY(i), TUPLE_ENTRY(c));
    T_ERROR(p == NULL);

    T_EXPECT(*(int *)pair_get_first(p), i);
    T_EXPECT(*(char *)pair_get_second(p), c);

    pair_destroy(p);

    p = pair_create(TUPLE_ENTRY(s), TUPLE_ENTRY(ptr));
    T_ERROR(p == NULL);

    T_EXPECT(memcmp(pair_get_first(p), &s, sizeof(s)), 0);
    T_EXPECT(*(MyStruct **)pair_get_second(p), ptr);
    T_EXPECT(memcmp(*(MyStruct **)pair_get_second(p), ptr, sizeof(s)), 0);

    pair_destroy(p);

    p = pair_create(TUPLE_ARRAY_ENTRY(arr), TUPLE_ENTRY(d));
    T_ERROR(p == NULL);

    T_EXPECT(memcmp(pair_get_first(p), arr, sizeof(arr)), 0);
    T_EXPECT(*(double *)pair_get_second(p), d);

    pair_destroy(p);
}

void test(void)
{
    TEST(test_tuple_create());
    TEST(test_tuple_get_elem());
    TEST(test_pair_create());
    TEST(test_pair_get_elem());
}

int main(void)
{
    TEST_INIT("TUPLE");
    test();
    TEST_SUMMARY();
}