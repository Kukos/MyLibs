#include <test.h>
#include <partition.h>
#include <common.h>
#include <search.h>
#include <generic.h>

CMP(int)
CMP(char)
CMP(double)

typedef int (*partition)(void        *t,
                      size_t      offset_left,
                      size_t      offset_right,
                      int         (*cmp)(const void *a, const void *b),
                      int         size_of,
                      size_t      *offset_left_index,
                      size_t      *offset_right_index);

typedef size_t (*get_pivot)(const void *t, size_t len, int size_of, int (*cmp)(const void *a, const void *b));

typedef struct MyStruct
{
    int key;
    int a;
    char b;
    double c;
    long d;
} MyStruct;

static bool array_is_sorted(void *array, size_t n, int size_of, int (*cmp)(const void *a, const void *b))
{
    size_t i;
    size_t size;
    BYTE *t;

    t = (BYTE *)array;

    size = n * size_of - size_of;
    for (i = 0; i < size; i += size_of)
        if (cmp(t + i, t + i + size_of) > 0)
            return false;

    return true;
}

static int cmp_MyStruct(const void *a, const void *b)
{
    if ((*(MyStruct *)a).key < (*(MyStruct *)b).key)
        return -1;

    if ((*(MyStruct *)a).key > (*(MyStruct *)b).key)
        return 1;

    return 0;
}

static void _quicksort_for_test(void      *t,
                                size_t    offset_left,
                                size_t    offset_right,
                                int       (*cmp)(const void *a, const void *b),
                                int       size_of,
                                partition part,
                                get_pivot gpivot)
{
    BYTE *_t;
    size_t p_index;

    size_t offset_left_index;
    size_t offset_right_index;
    size_t range;

    size_t _size_of = (size_t)size_of;

    _t = (BYTE *)t;

    if (offset_left >= offset_right)
        return;

    range = (offset_right - offset_left) / _size_of + 1;
    p_index = gpivot(_t + offset_left, range, size_of, cmp) *_size_of + offset_left;
    __SWAP__(_t[p_index], _t[offset_left], _size_of);

    part(_t, offset_left / _size_of, offset_right / _size_of, cmp, size_of, &offset_left_index, &offset_right_index);

    offset_left_index *= _size_of;
    offset_right_index *= _size_of;

    _quicksort_for_test(_t, offset_right_index, offset_right, cmp, size_of, part, gpivot);
    _quicksort_for_test(_t, offset_left, offset_left_index, cmp, size_of, part, gpivot);
}

static void quicksort_for_test(void *t, size_t num_elements, int(*cmp)(const void *a, const void *b), int size_of, partition part, get_pivot gpivot)
{
    _quicksort_for_test(t, 0, (num_elements - 1) * (size_t)size_of, cmp, size_of, part, gpivot);
}

#define CHECK_PIVOT_1_ROUND(get_pivot) \
    do { \
        int t[] = {1, 2, 3, 4}; \
        char t2[] = "abcdefgh"; \
        double t3[] = {0.1, 0.2, 0.3, 0.4, 0.5}; \
        MyStruct t4[11] = {{0}}; \
        \
        size_t pivot; \
        \
        pivot = get_pivot(t, ARRAY_SIZE(t), sizeof(*t), cmp_int); \
        T_CHECK(PIVOT_IS_CORRECT(pivot, ARRAY_SIZE(t))); \
        \
        pivot = get_pivot(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_char); \
        T_CHECK(PIVOT_IS_CORRECT(pivot, ARRAY_SIZE(t2))); \
        \
        pivot = get_pivot(t3, ARRAY_SIZE(t3), sizeof(*t3), cmp_double); \
        T_CHECK(PIVOT_IS_CORRECT(pivot, ARRAY_SIZE(t3))); \
        \
        pivot = get_pivot(t4, ARRAY_SIZE(t4), sizeof(*t4), cmp_MyStruct); \
        T_CHECK(PIVOT_IS_CORRECT(pivot, ARRAY_SIZE(t4))); \
    } while (0)

test_f test_pivot_trivial(void)
{
#define PIVOT_IS_CORRECT(index, size) (index == 0 || index == size / 2 || index == size - 1)

    /* trivial pivot is rand, so do 100 rounds */
    int i;
    for (i = 0; i < 100; ++i)
        CHECK_PIVOT_1_ROUND(partition_get_pivot_trivial);

#undef PIVOT_IS_CORRECT
}

test_f test_pivot_rand(void)
{
#define PIVOT_IS_CORRECT(index, size) (index >= 0 && index < size)

    /* rand pivot is rand, so do 100 rounds */
    int i;
    for (i = 0; i < 100; ++i)
        CHECK_PIVOT_1_ROUND(partition_get_pivot_rand);
#undef PIVOT_IS_CORRECT
}

#undef CHECK_PIVOT_1_ROUND

test_f test_pivot_median(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9, 10, 18, 17, 16, 15, 14, 13, 12, 11};
    char t2[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9, 10, 18, 17, 16, 15, 14, 13, 12, 11};

    size_t pivot;

    /* t1 */
    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, ARRAY_SIZE(t) / 2);

    t[0] = 9;
    t[ARRAY_SIZE(t) / 2] = 1;

    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 0);

    t[ARRAY_SIZE(t) - 1] = 6;

    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, ARRAY_SIZE(t) - 1);

    /* t2 */
    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_char);
    T_ASSERT(pivot, ARRAY_SIZE(t2) / 2);

    t2[0] = 9;
    t2[ARRAY_SIZE(t2) / 2] = 1;

    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_char);
    T_ASSERT(pivot, 0);

    t2[ARRAY_SIZE(t2) - 1] = 6;

    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_char);
    T_ASSERT(pivot, ARRAY_SIZE(t2) - 1);
}

test_f test_pivot_tukey(void)
{
    int t[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8 , 9,
               19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
               20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
               39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
               40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
               59, 58, 57, 56, 55, 54, 53, 52, 51, 50};

    double t2[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8 , 9,
                   19, 18, 17, 16, 15, 14, 13, 12, 11, 10,
                   20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
                   39, 38, 37, 36, 35, 34, 33, 32, 31, 30,
                   40, 41, 42, 43, 44, 45, 46, 47, 48, 49,
                   59, 58, 57, 56, 55, 54, 53, 52, 51, 50};

    size_t pivot;

    /* t1 */
    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 29);

    t[29] = 90;

    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 36);

    t[36] = 107;
    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 59);

    t[0] = 100;
    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 59);

    t[59] = 0;
    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 45);

    t[45] = 0;
    t[0] = 0;
    pivot = partition_get_pivot_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int);
    T_ASSERT(pivot, 7);

    /* t2 */
    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double);
    T_ASSERT(pivot, 29);

    t2[29] = 90;

    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double);
    T_ASSERT(pivot, 36);

    t2[36] = 107;
    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double);
    T_ASSERT(pivot, 59);

    t2[0] = 100;
    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double);
    T_ASSERT(pivot, 59);

    t2[59] = 0;
    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double);
    T_ASSERT(pivot, 45);

    t2[45] = 0;
    t2[0] = 0;
    pivot = partition_get_pivot_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double);
    T_ASSERT(pivot, 7);
}

#define CHECK_PARTITION(part, type, n) \
    do { \
        type *t; \
        size_t left; \
        size_t right; \
        type pivot; \
        size_t i; \
        \
        t = (type *)malloc(sizeof(type) * n); \
        T_ERROR(t == NULL); \
        \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        pivot = t[0]; \
        T_EXPECT(part(t, 0, n - 1, concat(cmp_, type), sizeof(*t), &left, &right), 0); \
        for (i = 0; i < left; ++i) \
            T_CHECK(concat(cmp_, type)(&pivot, &t[i]) >= 0); \
        for (i = right; i < n; ++i) \
             T_CHECK(concat(cmp_, type)(&pivot, &t[i]) <= 0); \
        for (i = left + 1; i < right; ++i) \
            T_CHECK(concat(cmp_, type)(&pivot, &t[i]) == 0); \
        \
        FREE(t); \
    } while (0)

#define CHECK_ON_QS(part, gpivot, type, n) \
    do { \
        type *t; \
        size_t i; \
        \
        t = (type *)malloc(sizeof(type) * n); \
        T_ERROR(t == NULL); \
        \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        quicksort_for_test(t, n, concat(cmp_, type), sizeof(type), part, gpivot); \
        T_EXPECT(array_is_sorted(t, n, sizeof(type), concat(cmp_, type)), true); \
        \
        FREE(t); \
    } while (0)

test_f test_bentley_partition(size_t n)
{
    CHECK_PARTITION(partition_bentley, int, n);
    CHECK_PARTITION(partition_bentley, char, n);
    CHECK_PARTITION(partition_bentley, double, n);
    CHECK_PARTITION(partition_bentley, MyStruct, n);
}

test_f test_bentley_on_qs(size_t n)
{
    CHECK_ON_QS(partition_bentley, partition_get_pivot_trivial, int, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_rand, int, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_median, int, n);

    CHECK_ON_QS(partition_bentley, partition_get_pivot_trivial, char, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_rand, char, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_median, char, n);

    CHECK_ON_QS(partition_bentley, partition_get_pivot_trivial, double, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_rand, double, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_median, double, n);

    CHECK_ON_QS(partition_bentley, partition_get_pivot_trivial, MyStruct, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_rand, MyStruct, n);
    CHECK_ON_QS(partition_bentley, partition_get_pivot_median, MyStruct, n);
}

test_f test_hoare_partition(size_t n)
{
    CHECK_PARTITION(partition_hoare, int, n);
    CHECK_PARTITION(partition_hoare, char, n);
    CHECK_PARTITION(partition_hoare, double, n);
    CHECK_PARTITION(partition_hoare, MyStruct, n);
}

test_f test_hoare_on_qs(size_t n)
{
    CHECK_ON_QS(partition_hoare, partition_get_pivot_trivial, int, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_rand, int, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_median, int, n);

    CHECK_ON_QS(partition_hoare, partition_get_pivot_trivial, char, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_rand, char, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_median, char, n);

    CHECK_ON_QS(partition_hoare, partition_get_pivot_trivial, double, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_rand, double, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_median, double, n);

    CHECK_ON_QS(partition_hoare, partition_get_pivot_trivial, MyStruct, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_rand, MyStruct, n);
    CHECK_ON_QS(partition_hoare, partition_get_pivot_median, MyStruct, n);
}

test_f test_lomuto_partition(size_t n)
{
    CHECK_PARTITION(partition_lomuto, int, n);
    CHECK_PARTITION(partition_lomuto, char, n);
    CHECK_PARTITION(partition_lomuto, double, n);
    CHECK_PARTITION(partition_lomuto, MyStruct, n);
}

test_f test_lomuto_on_qs(size_t n)
{
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_trivial, int, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_rand, int, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_median, int, n);

    CHECK_ON_QS(partition_lomuto, partition_get_pivot_trivial, char, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_rand, char, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_median, char, n);

    CHECK_ON_QS(partition_lomuto, partition_get_pivot_trivial, double, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_rand, double, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_median, double, n);

    CHECK_ON_QS(partition_lomuto, partition_get_pivot_trivial, MyStruct, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_rand, MyStruct, n);
    CHECK_ON_QS(partition_lomuto, partition_get_pivot_median, MyStruct, n);
}

test_f test_empty_partition(void)
{
    int t[] = {0,0,0};
    size_t left;
    size_t right;

    T_EXPECT(partition_lomuto(t, 0, 0, cmp_int, sizeof(*t), &left, &right), 0);
    T_EXPECT(partition_hoare(t, 0, 0, cmp_int, sizeof(*t), &left, &right), 0);
    T_EXPECT(partition_bentley(t, 0, 0, cmp_int, sizeof(*t), &left, &right), 0);
}

void test(void)
{
    TEST(test_pivot_trivial());
    TEST(test_pivot_rand());
    TEST(test_pivot_median());
    TEST(test_pivot_tukey());

    TEST(test_bentley_partition(1));
    TEST(test_bentley_partition(2));
    TEST(test_bentley_partition(100));
    TEST(test_bentley_partition(10000));

    TEST(test_bentley_on_qs(1));
    TEST(test_bentley_on_qs(2));
    TEST(test_bentley_on_qs(100));
    TEST(test_bentley_on_qs(10000));

    TEST(test_hoare_partition(1));
    TEST(test_hoare_partition(2));
    TEST(test_hoare_partition(100));
    TEST(test_hoare_partition(10000));

    TEST(test_hoare_on_qs(1));
    TEST(test_hoare_on_qs(2));
    TEST(test_hoare_on_qs(100));
    TEST(test_hoare_on_qs(10000));

    TEST(test_lomuto_partition(1));
    TEST(test_lomuto_partition(2));
    TEST(test_lomuto_partition(100));
    TEST(test_lomuto_partition(10000));

    TEST(test_lomuto_on_qs(1));
    TEST(test_lomuto_on_qs(2));
    TEST(test_lomuto_on_qs(100));
    TEST(test_lomuto_on_qs(10000));

    TEST(test_empty_partition());
}

int main(void)
{
    TEST_INIT("PARTITION");
    test();
    TEST_SUMMARY();
}