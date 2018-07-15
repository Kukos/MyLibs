#include <test.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <common.h>
#include <assert.h>
#include <generic.h>
#include <time.h>
#include <sort.h>

typedef struct MyStruct
{
    char a;
    char b;
    char c;
    char d;
    int key;
}MyStruct;

CMP(int)
CMP(double)
CMP(char)

int cmp_mystruct(const void *a, const void *b)
{
    if ((*(MyStruct **)a)->key < (*(MyStruct **)b)->key)
        return -1;

    if ((*(MyStruct **)a)->key > (*(MyStruct **)b)->key)
        return 1;

    return 0;
}

bool check_array_is_sorted(void *array, size_t n, size_t size_of, int (*cmp)(const void *a, const void *b))
{
    size_t i;
    size_t size;
    BYTE *t;

    assert(array != NULL);
    assert(n >= 1);
    assert(size_of >= 1);
    assert(cmp != NULL);

    t = (BYTE *)array;

    size = n * size_of - size_of;
    for (i = 0; i < size; i += size_of)
        if (cmp(t + i, t + i + size_of) > 0)
            return false;

    return true;
}

test_f test_insort(size_t n)
{
    size_t i;
    int *t1;
    char *t2;
    double *t3;
    MyStruct **t4;

    srand(time(NULL));

    /* t1 */
    t1 = (int *)malloc(sizeof(int) * n);
    T_ERROR(t1 == NULL);

    for (i = 0; i < n; ++i)
        t1[i] = rand();

    T_EXPECT(insort((void *)t1, n, cmp_int, sizeof(int)), 0);
    T_EXPECT(check_array_is_sorted((void *)t1, n, sizeof(int), cmp_int), true);

    /* t2 */
    t2 = (char *)malloc(sizeof(char) * n);
    T_ERROR(t2 == NULL);

    for (i = 0; i < n; ++i)
        t2[i] = rand();

    T_EXPECT(insort((void *)t2, n, cmp_char, sizeof(char)), 0);
    T_EXPECT(check_array_is_sorted((void *)t2, n, sizeof(char), cmp_char), true);

    /* t3 */
    t3 = (double *)malloc(sizeof(double) * n);
    T_ERROR(t3 == NULL);

    for (i = 0; i < n; ++i)
        t3[i] = rand() + (rand() % 1000 * 0.001);

    T_EXPECT(insort((void *)t3, n, cmp_double, sizeof(double)), 0);
    T_EXPECT(check_array_is_sorted((void *)t3, n, sizeof(double), cmp_double), true);

    /* t4 */
    t4 = (MyStruct **)malloc(sizeof(MyStruct *) * n);
    T_ERROR(t4 == NULL);

    for (i = 0; i < n; ++i)
    {
        t4[i] = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(t4[i] == NULL);
        t4[i]->key = rand();
    }

    T_EXPECT(insort((void *)t4, n, cmp_mystruct, sizeof(MyStruct)), 0);
    T_EXPECT(check_array_is_sorted((void *)t4, n, sizeof(MyStruct), cmp_mystruct), true);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    for (i = 0; i < n; ++i)
        FREE(t4[i]);

    FREE(t4);
}

test_f test_binsort(size_t n)
{
    size_t i;
    int *t1;
    char *t2;
    double *t3;
    MyStruct **t4;

    srand(time(NULL));

    /* t1 */
    t1 = (int *)malloc(sizeof(int) * n);
    T_ERROR(t1 == NULL);

    for (i = 0; i < n; ++i)
        t1[i] = rand() % 100;

    T_EXPECT(binsort((void *)t1, n, cmp_int, sizeof(int)), 0);
    T_EXPECT(check_array_is_sorted((void *)t1, n, sizeof(int), cmp_int), true);

    /* t2 */
    t2 = (char *)malloc(sizeof(char) * n);
    T_ERROR(t2 == NULL);

    for (i = 0; i < n; ++i)
        t2[i] = rand();

    T_EXPECT(binsort((void *)t2, n, cmp_char, sizeof(char)), 0);
    T_EXPECT(check_array_is_sorted((void *)t2, n, sizeof(char), cmp_char), true);

    /* t3 */
    t3 = (double *)malloc(sizeof(double) * n);
    T_ERROR(t3 == NULL);

    for (i = 0; i < n; ++i)
        t3[i] = rand() + (rand() % 1000 * 0.001);

    T_EXPECT(binsort((void *)t3, n, cmp_double, sizeof(double)), 0);
    T_EXPECT(check_array_is_sorted((void *)t3, n, sizeof(double), cmp_double), true);

    /* t4 */
    t4 = (MyStruct **)malloc(sizeof(MyStruct *) * n);
    T_ERROR(t4 == NULL);

    for (i = 0; i < n; ++i)
    {
        t4[i] = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(t4[i] == NULL);
        t4[i]->key = rand();
    }

    T_EXPECT(binsort((void *)t4, n, cmp_mystruct, sizeof(MyStruct)), 0);
    T_EXPECT(check_array_is_sorted((void *)t4, n, sizeof(MyStruct), cmp_mystruct), true);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    for (i = 0; i < n; ++i)
        FREE(t4[i]);

    FREE(t4);
}

test_f test_mergesort(size_t n)
{
    size_t i;
    int *t1;
    char *t2;
    double *t3;
    MyStruct **t4;

    srand(time(NULL));

    /* t1 */
    t1 = (int *)malloc(sizeof(int) * n);
    T_ERROR(t1 == NULL);

    for (i = 0; i < n; ++i)
        t1[i] = rand();

    T_EXPECT(mergesort((void *)t1, n, cmp_int, sizeof(int)), 0);
    T_EXPECT(check_array_is_sorted((void *)t1, n, sizeof(int), cmp_int), true);

    /* t2 */
    t2 = (char *)malloc(sizeof(char) * n);
    T_ERROR(t2 == NULL);

    for (i = 0; i < n; ++i)
        t2[i] = rand();

    T_EXPECT(mergesort((void *)t2, n, cmp_char, sizeof(char)), 0);
    T_EXPECT(check_array_is_sorted((void *)t2, n, sizeof(char), cmp_char), true);

    /* t3 */
    t3 = (double *)malloc(sizeof(double) * n);
    T_ERROR(t3 == NULL);

    for (i = 0; i < n; ++i)
        t3[i] = rand() + (rand() % 1000 * 0.001);

    T_EXPECT(mergesort((void *)t3, n, cmp_double, sizeof(double)), 0);
    T_EXPECT(check_array_is_sorted((void *)t3, n, sizeof(double), cmp_double), true);

    /* t4 */
    t4 = (MyStruct **)malloc(sizeof(MyStruct *) * n);
    T_ERROR(t4 == NULL);

    for (i = 0; i < n; ++i)
    {
        t4[i] = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(t4[i] == NULL);
        t4[i]->key = rand();
    }

    T_EXPECT(mergesort((void *)t4, n, cmp_mystruct, sizeof(MyStruct)), 0);
    T_EXPECT(check_array_is_sorted((void *)t4, n, sizeof(MyStruct), cmp_mystruct), true);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    for (i = 0; i < n; ++i)
        FREE(t4[i]);

    FREE(t4);
}

test_f test_quicksort(size_t n)
{
    size_t i;
    int *t1;
    char *t2;
    double *t3;
    MyStruct **t4;

    srand(time(NULL));

    /* t1 */
    t1 = (int *)malloc(sizeof(int) * n);
    T_ERROR(t1 == NULL);

    for (i = 0; i < n; ++i)
        t1[i] = rand();

    T_EXPECT(quicksort((void *)t1, n, cmp_int, sizeof(int)), 0);
    T_EXPECT(check_array_is_sorted((void *)t1, n, sizeof(int), cmp_int), true);

    /* t2 */
    t2 = (char *)malloc(sizeof(char) * n);
    T_ERROR(t2 == NULL);

    for (i = 0; i < n; ++i)
        t2[i] = rand();

    T_EXPECT(quicksort((void *)t2, n, cmp_char, sizeof(char)), 0);
    T_EXPECT(check_array_is_sorted((void *)t2, n, sizeof(char), cmp_char), true);

    /* t3 */
    t3 = (double *)malloc(sizeof(double) * n);
    T_ERROR(t3 == NULL);

    for (i = 0; i < n; ++i)
        t3[i] = rand() + (rand() % 1000 * 0.001);

    T_EXPECT(quicksort((void *)t3, n, cmp_double, sizeof(double)), 0);
    T_EXPECT(check_array_is_sorted((void *)t3, n, sizeof(double), cmp_double), true);

    /* t4 */
    t4 = (MyStruct **)malloc(sizeof(MyStruct *) * n);
    T_ERROR(t4 == NULL);

    for (i = 0; i < n; ++i)
    {
        t4[i] = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(t4[i] == NULL);
        t4[i]->key = rand();
    }

    T_EXPECT(quicksort((void *)t4, n, cmp_mystruct, sizeof(MyStruct)), 0);
    T_EXPECT(check_array_is_sorted((void *)t4, n, sizeof(MyStruct), cmp_mystruct), true);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    for (i = 0; i < n; ++i)
        FREE(t4[i]);

    FREE(t4);
}

test_f test_sort_empty(void)
{
    int *t = t;
    size_t size = 0;

    T_CHECK(insort((void *)t, size, cmp_int, sizeof(int)) != 0);
    T_CHECK(binsort((void *)t, size, cmp_int, sizeof(int)) != 0);
    T_CHECK(mergesort((void *)t, size, cmp_int, sizeof(int)) != 0);
    T_CHECK(quicksort((void *)t, size, cmp_int, sizeof(int)) != 0);
    T_CHECK(sort((void *)t, size, cmp_int, sizeof(int)) != 0);
}

void test(void)
{
    TEST(test_insort(1));
    TEST(test_insort(2));
    TEST(test_insort(100));
    TEST(test_insort(10000));

    TEST(test_binsort(1));
    TEST(test_binsort(2));
    TEST(test_binsort(100));
    TEST(test_binsort(10000));

    TEST(test_mergesort(1));
    TEST(test_mergesort(2));
    TEST(test_mergesort(100));
    TEST(test_mergesort(10000));

    TEST(test_quicksort(1));
    TEST(test_quicksort(2));
    TEST(test_quicksort(100));
    TEST(test_quicksort(10000));

    TEST(test_sort_empty());
}

int main(void)
{
    TEST_INIT("SORT");
    test();
    TEST_SUMMARY();
}
