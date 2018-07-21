#include <array.h>
#include <test.h>
#include <common.h>
#include <string.h>

typedef struct MyStruct
{
    int key;
    int a;
    char b;
    double c;
    long d;
} MyStruct;

static int cmp_MyStruct(const void *a, const void *b)
{
    if ((*(MyStruct *)a).key < (*(MyStruct *)b).key)
        return -1;

    if ((*(MyStruct *)a).key > (*(MyStruct *)b).key)
        return 1;

    return 0;
}

typedef struct MyStruct2
{
    int key;
    int a;
    char b;
    double c;
    long d;
} MyStruct2;

static int cmp_MyStruct2(const void *a, const void *b)
{
    if ((*(MyStruct **)a)->key < (*(MyStruct **)b)->key)
        return -1;

    if ((*(MyStruct **)a)->key > (*(MyStruct **)b)->key)
        return 1;

    return 0;
}

static void my_struct2_destroy(void *s)
{
    MyStruct2 *ms = *(MyStruct2 **)s;
    FREE(ms);
}

static MyStruct2 *my_struct2_create(int key)
{
    MyStruct2 *ms;

    ms = malloc(sizeof(*ms));
    ms->key = key;

    return ms;
}

static bool check_array_is_sorted(void *array, size_t n, size_t size_of, cmp_f cmp)
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

CMP(int)
CMP(char)
CMP(double)

#define TEST_CREATE(type, n) \
    do { \
        type *t; \
        t = array_create(n, sizeof(*t)); \
        T_ERROR(t == NULL); \
        array_destroy(t); \
    } while (0)

test_f test_create(void)
{
    TEST_CREATE(int, 1000);
    TEST_CREATE(char, 10000);
    TEST_CREATE(double, 100000);
    TEST_CREATE(MyStruct, 1000000);
}

test_f test_equal(void)
{
    int t[] = {1, 2, 3, 4, 5};
    int t2[] = {1, 2, 4, 3, 5};
    int t3[] = {1, 2, 3, 4, 5};

    double t4[] = {0.1, 0.2, 0.3, 0.4, 0.5};
    double t5[] = {0.4, 0.2, 0.3, 0.1, 0.5};
    double t6[] = {0.1, 0.2, 0.3, 0.4, 0.5};

    T_EXPECT(array_equal(t, t2, ARRAY_SIZE(t), sizeof(*t), cmp_int), false);
    T_EXPECT(array_equal(t, t3, ARRAY_SIZE(t), sizeof(*t), cmp_int), true);

    T_EXPECT(array_equal(t4, t5, ARRAY_SIZE(t4), sizeof(*t4), cmp_double), false);
    T_EXPECT(array_equal(t4, t6, ARRAY_SIZE(t4), sizeof(*t4), cmp_double), true);
}

#undef TEST_CREATE

#define TEST_CLONE(type, n) \
    do { \
        type *t; \
        type *t2; \
        size_t i; \
        t = array_create(n, sizeof(*t)); \
        T_ERROR(t == NULL); \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        t2 = array_clone(t, n, sizeof(*t)); \
        T_ERROR(t2 == NULL); \
        for (i = 0; i < n; ++i) \
            T_CHECK(concat(cmp_, type)(&t[i], &t2[i]) == 0); \
        array_destroy(t); \
        array_destroy(t2); \
    } while (0)


test_f test_clone(void)
{
    TEST_CLONE(int, 1000);
    TEST_CLONE(char, 5000);
    TEST_CLONE(double, 10000);
    TEST_CLONE(MyStruct, 50000);
}

#undef TEST_CLONE

#define TEST_COPY(type, n) \
    do { \
        type *t; \
        type *t2; \
        size_t i; \
        t = array_create(n, sizeof(*t)); \
        T_ERROR(t == NULL); \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        t2 = array_create(n, sizeof(*t2)); \
        T_ERROR(t2 == NULL); \
        array_copy(t2, t, n, sizeof(*t)); \
        for (i = 0; i < n; ++i) \
            T_CHECK(concat(cmp_, type)(&t[i], &t2[i]) == 0); \
        array_destroy(t); \
        array_destroy(t2); \
    } while (0)

test_f test_copy(void)
{
    TEST_COPY(int, 1000);
    TEST_COPY(char, 5000);
    TEST_COPY(double, 10000);
    TEST_COPY(MyStruct, 50000);
}

#undef TEST_COPY

#define TEST_MOVE(type, n) \
    do { \
        static_assert(n >= 100); \
        type *t; \
        type *t2; \
        size_t i; \
        t = array_create(n, sizeof(*t)); \
        T_ERROR(t == NULL); \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        t2 = array_create(n, sizeof(*t2)); \
        T_ERROR(t2 == NULL); \
        array_copy(t2, t, n, sizeof(*t)); \
        array_move(t, t + 1, n - 1, sizeof(*t)); \
        for (i = 0; i < n - 1; ++i) \
            T_CHECK(concat(cmp_, type)(&t[i], &t2[i + 1]) == 0); \
        array_move(t, t + 99, n - 100, sizeof(*t)); \
        for (i = 0; i < n - 100; ++i) \
            T_CHECK(concat(cmp_, type)(&t[i], &t2[i + 100]) == 0); \
        array_destroy(t); \
        array_destroy(t2); \
    } while (0)

test_f test_move(void)
{
    TEST_MOVE(int, 1000);
    TEST_MOVE(char, 5000);
    TEST_MOVE(double, 10000);
    TEST_MOVE(MyStruct, 50000);
}

#undef TEST_MOVE

test_f test_zeros(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double t2[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    size_t i;

    array_zeros(t, ARRAY_SIZE(t), sizeof(*t));
    array_zeros(t2, ARRAY_SIZE(t2), sizeof(*t2));

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], 0);

    for (i = 0; i < ARRAY_SIZE(t2); ++i)
        T_ASSERT(t2[i], 0);
}

test_f test_set_all(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    double t2[] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

    int val1 = 100;
    double val2 = 10;

    size_t i;

    array_set_all(t, ARRAY_SIZE(t), sizeof(*t), &val1);
    array_set_all(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2);

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], val1);

    for (i = 0; i < ARRAY_SIZE(t2); ++i)
        T_ASSERT(t2[i], val2);
}

#define TEST_FOR_EACH(t, tt, cmp) \
    do { \
        int i; \
        size_t counter; \
        typeof(*t) data; \
        typeof(*t) *ptr; \
        \
        /* for_each */ \
        i = 0; \
        counter = 0; \
        for_each_array(t, ARRAY_SIZE(t), sizeof(*t), ptr, data) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            T_ASSERT(t + i, ptr); \
            \
            ++i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        \
        i = 0; \
        counter = 0; \
        for_each_array(tt, ARRAY_SIZE(t), sizeof(*t), ptr, data) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            T_ASSERT((typeof(ptr))(((BYTE *)tt) + (i * sizeof(*t))), ptr); \
            \
            ++i;\
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        /* for_each_data */ \
        i = 0; \
        counter = 0; \
        for_each_data_array(t, ARRAY_SIZE(t), sizeof(*t), data) \
        { \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            ++i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        \
        i = 0; \
        counter = 0; \
        for_each_data_array(tt, ARRAY_SIZE(t), sizeof(*t), data) \
        { \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            ++i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        /* for_each_ptr */ \
        i = 0; \
        counter = 0; \
        for_each_ptr_array(t, ARRAY_SIZE(t), sizeof(*t), ptr) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_ASSERT(t + i, ptr); \
            \
            ++i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        \
        i = 0; \
        counter = 0; \
        for_each_ptr_array(tt, ARRAY_SIZE(t), sizeof(*t), ptr) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_ASSERT((typeof(ptr))(((BYTE *)tt) + (i * sizeof(*t))), ptr); \
            \
            ++i;\
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
    } while (0)

#define TEST_FOR_EACH_PREV(t, tt, cmp) \
    do { \
        int i; \
        size_t counter; \
        typeof(*t) data; \
        typeof(*t) *ptr; \
        \
        /* for_each_prev */ \
        i = ARRAY_SIZE(t) - 1; \
        counter = 0; \
        for_each_prev_array(t, ARRAY_SIZE(t), sizeof(*t), ptr, data) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            T_ASSERT(t + i, ptr); \
            \
            --i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        \
        i = ARRAY_SIZE(t) - 1; \
        counter = 0; \
        for_each_prev_array(tt, ARRAY_SIZE(t), sizeof(*t), ptr, data) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            T_ASSERT((typeof(ptr))(((BYTE *)tt) + (i * sizeof(*t))), ptr); \
            \
            --i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        /* for_each_data_prev */ \
        i = ARRAY_SIZE(t) - 1; \
        counter = 0; \
        for_each_data_prev_array(t, ARRAY_SIZE(t), sizeof(*t), data) \
        { \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            --i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        \
        i = ARRAY_SIZE(t) - 1; \
        counter = 0; \
        for_each_data_prev_array(tt, ARRAY_SIZE(t), sizeof(*t), data) \
        { \
            T_EXPECT(cmp(&data, &t[i]), 0); \
            --i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        /* for_each_ptr_prev */ \
        i = ARRAY_SIZE(t) - 1; \
        counter = 0; \
        for_each_ptr_prev_array(t, ARRAY_SIZE(t), sizeof(*t), ptr) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_ASSERT(t + i, ptr); \
            \
            --i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
        \
        i = ARRAY_SIZE(t) - 1; \
        counter = 0; \
        for_each_ptr_prev_array(tt, ARRAY_SIZE(t), sizeof(*t), ptr) \
        { \
            T_EXPECT(cmp(ptr, &t[i]), 0); \
            T_ASSERT((typeof(ptr))(((BYTE *)tt) + (i * sizeof(*t))), ptr); \
            \
            --i; \
            ++counter; \
        } \
        T_ASSERT(counter, ARRAY_SIZE(t)); \
    } while (0)

test_f test_array_foreach(void)
{
    int t1[] = {1, 2, 3, 4, 5};
    double t2[] = {0.1, 0.2, 0.3, 0.4, 0.5};
    char t3[] = "Kukos";
    MyStruct t4[] = {{.key = 1}, {.key = 2}, {.key = 3}, {.key = 4}};
    MyStruct2 _t5[] = {{.key = 1}, {.key = 2}, {.key = 3}, {.key = 4}};
    MyStruct2 *t5[] = {&_t5[0], &_t5[1], &_t5[2], &_t5[3], &_t5[4]};

    void *tt1;
    void *tt2;
    void *tt3;
    void *tt4;
    void *tt5;

    tt1 = array_create(ARRAY_SIZE(t1), sizeof(*t1));
    T_ERROR(tt1 == NULL);
    array_copy(tt1, t1, ARRAY_SIZE(t1), sizeof(*t1));

    tt2 = array_create(ARRAY_SIZE(t2), sizeof(*t2));
    T_ERROR(tt2 == NULL);
    array_copy(tt2, t2, ARRAY_SIZE(t2), sizeof(*t2));

    tt3 = array_create(ARRAY_SIZE(t3), sizeof(*t3));
    T_ERROR(tt3 == NULL);
    array_copy(tt3, t3, ARRAY_SIZE(t3), sizeof(*t3));

    tt4 = array_create(ARRAY_SIZE(t4), sizeof(*t4));
    T_ERROR(tt4 == NULL);
    array_copy(tt4, t4, ARRAY_SIZE(t4), sizeof(*t4));

    tt5 = array_create(ARRAY_SIZE(t5), sizeof(*t5));
    T_ERROR(tt5 == NULL);
    array_copy(tt5, t5, ARRAY_SIZE(t5), sizeof(*t5));

    /* FOR_EACH */
    TEST_FOR_EACH(t1, tt1, cmp_int);
    TEST_FOR_EACH(t2, tt2, cmp_double);
    TEST_FOR_EACH(t3, tt3, cmp_char);
    TEST_FOR_EACH(t4, tt4, cmp_MyStruct);
    TEST_FOR_EACH(t5, tt5, cmp_MyStruct2);

    /* FOR_EACH_PREV */
    TEST_FOR_EACH_PREV(t1, tt1, cmp_int);
    TEST_FOR_EACH_PREV(t2, tt2, cmp_double);
    TEST_FOR_EACH_PREV(t3, tt3, cmp_char);
    TEST_FOR_EACH_PREV(t4, tt4, cmp_MyStruct);
    TEST_FOR_EACH_PREV(t5, tt5, cmp_MyStruct2);

    array_destroy(tt1);
    array_destroy(tt2);
    array_destroy(tt3);
    array_destroy(tt4);
    array_destroy(tt5);
}

#undef TEST_FOR_EACH
#undef TEST_FOR_EACH_PREV

test_f test_destroy_with_entries(void)
{
    const size_t size = 1000;
    size_t i;

    MyStruct2 **t;

    t = (MyStruct2 **)array_create(size, sizeof(*t));
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = my_struct2_create(rand());

    array_destroy_with_entries(t, size, sizeof(*t), my_struct2_destroy);
}

test_f test_insert(void)
{
    const size_t size = 10;
    int t[size];
    int val;
    size_t i;

    /* insert first */
    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        T_EXPECT(array_unsorted_insert_first(t, size, sizeof(*t), &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], size - i);

    memset(t, 0, size * sizeof(*t));

    /* insert last */
    for (i = 0; i < size; ++i)
    {
        val = size - i;
        T_EXPECT(array_unsorted_insert_last(t, size - i, sizeof(*t), &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], i + 1);

    memset(t, 0, size * sizeof(*t));

    /* insert pos */
    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        T_EXPECT(array_unsorted_insert_pos(t, size, sizeof(*t), i, &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], i + 1);

}

test_f test_insert_with_entry(void)
{
    const size_t size = 10;
    MyStruct2 *t[size];
    MyStruct2 *val;
    size_t i;

    /* insert first */
    for (i = 0; i < size; ++i)
    {
        val = my_struct2_create(i + 1);
        T_EXPECT(array_unsorted_insert_first(t, size, sizeof(*t), &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i]->key, size - i);

    for (i = 0; i < size; ++i)
        FREE(t[i]);

    /* insert last */
    for (i = 0; i < size; ++i)
    {
        val = my_struct2_create(size - i);
        T_EXPECT(array_unsorted_insert_last(t, size - i, sizeof(*t), &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i]->key, i + 1);

    for (i = 0; i < size; ++i)
        FREE(t[i]);

    /* insert pos */
    for (i = 0; i < size; ++i)
    {
        val = my_struct2_create(i + 1);
        T_EXPECT(array_unsorted_insert_pos(t, size, sizeof(*t), i, &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i]->key, i + 1);

    for (i = 0; i < size; ++i)
        FREE(t[i]);
}

test_f test_insert_sorted(void)
{
    const size_t size = 1000;
    size_t i;
    int t[size];
    int vals[size];
    int r;

    /* ascending */
    for (i = 0; i < size; ++i)
        vals[i] = i + 1;

    for (i = 0; i < size; ++i)
        T_EXPECT(array_sorted_insert(t, i + 1, sizeof(*t), cmp_int, &vals[i]), 0);

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], i + 1);

    /* descending */
    for (i = 0; i < size; ++i)
        vals[i] = size  - i;

    for (i = 0; i < size; ++i)
        T_EXPECT(array_sorted_insert(t, i + 1, sizeof(*t), cmp_int, &vals[i]), 0);

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], i + 1);

    /* random */
    for (i = size - 1; i > 0; --i)
    {
        r = rand() % (i + 1);
        SWAP(vals[i], vals[r]);
    }

    for (i = 0; i < size; ++i)
        T_EXPECT(array_sorted_insert(t, i + 1, sizeof(*t), cmp_int, &vals[i]), 0);

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], i + 1);
}

test_f test_insert_sorted_with_entry(void)
{
    const size_t size = 1000;
    size_t i;
    MyStruct2 *t[size];
    MyStruct2 *val;
    int vals[size];
    int r;

    /* ascending */
    for (i = 0; i < size; ++i)
        vals[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        val = my_struct2_create(vals[i]);
        T_EXPECT(array_sorted_insert(t, i + 1, sizeof(*t), cmp_MyStruct2, &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i]->key, i + 1);

    for (i = 0; i < size; ++i)
        FREE(t[i]);

    /* descending */
    for (i = 0; i < size; ++i)
        vals[i] = size  - i;

    for (i = 0; i < size; ++i)
    {
        val = my_struct2_create(vals[i]);
        T_EXPECT(array_sorted_insert(t, i + 1, sizeof(*t), cmp_MyStruct2, &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i]->key, i + 1);

    for (i = 0; i < size; ++i)
        FREE(t[i]);

    /* random */
    for (i = size - 1; i > 0; --i)
    {
        r = rand() % (i + 1);
        SWAP(vals[i], vals[r]);
    }

    for (i = 0; i < size; ++i)
    {
        val = my_struct2_create(vals[i]);
        T_EXPECT(array_sorted_insert(t, i + 1, sizeof(*t), cmp_MyStruct2, &val), 0);
    }

    for (i = 0; i < size; ++i)
        T_ASSERT(t[i]->key, i + 1);

    for (i = 0; i < size; ++i)
        FREE(t[i]);
}

test_f test_delete(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int texp[] = {3, 4, 5, 8, 9, 10, 10, 10, 10, 10};
    size_t i;

    array_delete_first(t, ARRAY_SIZE(t), sizeof(*t));
    /* 2 3 4 5 6 7 8 9 10 10 */
    for (i = 0; i < ARRAY_SIZE(t) - 1; ++i)
        T_ASSERT(t[i], i + 2);

    T_ASSERT(t[ARRAY_SIZE(t) - 1], 10);

    /* 3 4 5 8 9 10 10 10 10 10 */
    array_delete_pos(t, ARRAY_SIZE(t) - 1, sizeof(*t), 4);
    array_delete_pos(t, ARRAY_SIZE(t) - 2, sizeof(*t), 0);
    array_delete_pos(t, ARRAY_SIZE(t) - 3, sizeof(*t), 3);
    array_delete_pos(t, ARRAY_SIZE(t) - 4, sizeof(*t), ARRAY_SIZE(t) - 5);

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], texp[i]);

    array_delete_last(t, ARRAY_SIZE(t) - 5, sizeof(*t));
    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], texp[i]);
}

test_f test_delete_on_whole(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int texp[] = {3, 4, 5, 8, 9, 10, 10, 10, 10, 10};
    size_t i;

    array_delete_first(t, ARRAY_SIZE(t), sizeof(*t));
    /* 2 3 4 5 6 7 8 9 10 10 */
    for (i = 0; i < ARRAY_SIZE(t) - 1; ++i)
        T_ASSERT(t[i], i + 2);

    T_ASSERT(t[ARRAY_SIZE(t) - 1], 10);

    /* 3 4 5 8 9 10 10 10 10 10 */
    array_delete_pos(t, ARRAY_SIZE(t), sizeof(*t), 4);
    array_delete_pos(t, ARRAY_SIZE(t), sizeof(*t), 0);
    array_delete_pos(t, ARRAY_SIZE(t), sizeof(*t), 3);
    array_delete_pos(t, ARRAY_SIZE(t), sizeof(*t), ARRAY_SIZE(t) - 1);

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], texp[i]);

    array_delete_last(t, ARRAY_SIZE(t), sizeof(*t));
    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], texp[i]);
}

test_f test_delete_all(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int t2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int t3[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int texp[] = {2, 4, 6, 8, 10, 10, 10, 10, 10, 10};

    size_t i;

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        array_delete_first(t, ARRAY_SIZE(t) - i, sizeof(*t));

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], 10);

    /* this do nothing */
    for (i = 0; i < ARRAY_SIZE(t2); ++i)
        array_delete_last(t2, ARRAY_SIZE(t2) - i, sizeof(*t2));

    for (i = 0; i < ARRAY_SIZE(t2); ++i)
        T_ASSERT(t2[i], i + 1);

    /* 2 4 6 8 10 10 10 10 10 10 */
    for (i = 0; i < ARRAY_SIZE(t3); ++i)
        array_delete_pos(t3, ARRAY_SIZE(t3) - i, sizeof(*t3), i);

    for (i = 0; i < ARRAY_SIZE(t3); ++i)
        T_ASSERT(t3[i], texp[i]);
}

test_f test_delete_with_entry(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int texp[] = {3, 4, 5, 8, 9, 10, 10, 10, 10, 10};
    size_t i;
    MyStruct2 *arr[ARRAY_SIZE(t)];

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        arr[i] = my_struct2_create(t[i]);

    array_delete_first_with_entry(arr, ARRAY_SIZE(arr), sizeof(*arr), my_struct2_destroy);
    /* 2 3 4 5 6 7 8 9 10 10 */
    for (i = 0; i < ARRAY_SIZE(arr) - 1; ++i)
        T_ASSERT(arr[i]->key, i + 2);

    /* 3 4 5 8 9 10 10 10 10 10 */
    array_delete_pos_with_entry(arr, ARRAY_SIZE(arr) - 1, sizeof(*arr), 4, my_struct2_destroy);
    array_delete_pos_with_entry(arr, ARRAY_SIZE(arr) - 2, sizeof(*arr), 0, my_struct2_destroy);
    array_delete_pos_with_entry(arr, ARRAY_SIZE(arr) - 3, sizeof(*arr), 3, my_struct2_destroy);
    array_delete_pos_with_entry(arr, ARRAY_SIZE(arr) - 4, sizeof(*arr), ARRAY_SIZE(arr) - 5, my_struct2_destroy);

    for (i = 0; i < ARRAY_SIZE(arr) - 5; ++i)
        T_ASSERT(arr[i]->key, texp[i]);

    array_delete_last_with_entry(arr, ARRAY_SIZE(arr) - 5, sizeof(*arr), my_struct2_destroy);
    for (i = 0; i < ARRAY_SIZE(arr) - 6; ++i)
    {
        T_ASSERT(arr[i]->key, texp[i]);
        FREE(arr[i]);
    }
}

#define TEST_SORT(type, n) \
    do { \
        type *t; \
        size_t i; \
        t = array_create(n, sizeof(*t)); \
        T_ERROR(t == NULL); \
        \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        T_EXPECT(array_sort(t, n, sizeof(*t), concat(cmp_, type)), 0); \
        T_EXPECT(check_array_is_sorted(t, n, sizeof(*t), concat(cmp_, type)), true); \
        \
        array_destroy(t); \
    } while (0)

test_f test_sort(void)
{
    TEST_SORT(int, 1000);
    TEST_SORT(char, 5000);
    TEST_SORT(double, 10000);
    TEST_SORT(MyStruct, 50000);
}

#undef TEST_SORT

test_f test_check_sorted(void)
{
    int t[] = {1, 2, 3, 4, 5};
    char t2[] = "abcdefghijk";

    T_EXPECT(array_is_sorted(t, ARRAY_SIZE(t), sizeof(*t), cmp_int), true);
    T_EXPECT(array_is_sorted(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), cmp_char), true);

    t[2] = 10;
    t2[4] = 'C';

    T_EXPECT(array_is_sorted(t, ARRAY_SIZE(t), sizeof(*t), cmp_int), false);
    T_EXPECT(array_is_sorted(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), cmp_char), false);
}

test_f test_check_reverse_sorted(void)
{
    int t[] = {5, 4, 3, 2, 1, 0};
    char t2[] = "ZVRDBA";

    T_EXPECT(array_is_reverse_sorted(t, ARRAY_SIZE(t), sizeof(*t), cmp_int), true);
    T_EXPECT(array_is_reverse_sorted(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), cmp_char), true);

    t[2] = 10;
    t2[4] = 'Y';

    T_EXPECT(array_is_reverse_sorted(t, ARRAY_SIZE(t), sizeof(*t), cmp_int), false);
    T_EXPECT(array_is_reverse_sorted(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), cmp_char), false);
}

test_f test_select_kth(void)
{
    int t[] = {1, 6, 3, 9, 8, 2, 4, 5, 7};
    int val1;
    double t2[] = {0.1, 0.6, 0.3, 0.9, 0.8, 0.2, 0.4, 0.5, 0.7};
    double val2;
    ssize_t index;

    /* t1 */
    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 0, cmp_int, &val1);
    T_ASSERT(t[index], 1);
    T_ASSERT(val1, 1);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 1, cmp_int, &val1);
    T_ASSERT(t[index], 2);
    T_ASSERT(val1, 2);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 2, cmp_int, &val1);
    T_ASSERT(t[index], 3);
    T_ASSERT(val1, 3);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 3, cmp_int, &val1);
    T_ASSERT(t[index], 4);
    T_ASSERT(val1, 4);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 4, cmp_int, &val1);
    T_ASSERT(t[index], 5);
    T_ASSERT(val1, 5);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 5, cmp_int, NULL);
    T_ASSERT(t[index], 6);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 6, cmp_int, NULL);
    T_ASSERT(t[index], 7);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 7, cmp_int, NULL);
    T_ASSERT(t[index], 8);

    index = array_select_kth(t, ARRAY_SIZE(t), sizeof(*t), 8, cmp_int, NULL);
    T_ASSERT(t[index], 9);

    /* t2 */
    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 0, cmp_double, &val2);
    T_ASSERT(t2[index], 0.1);
    T_ASSERT(val2, 0.1);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 1, cmp_double, &val2);
    T_ASSERT(t2[index], 0.2);
    T_ASSERT(val2, 0.2);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 2, cmp_double, &val2);
    T_ASSERT(t2[index], 0.3);
    T_ASSERT(val2, 0.3);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 3, cmp_double, &val2);
    T_ASSERT(t2[index], 0.4);
    T_ASSERT(val2, 0.4);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 4, cmp_double, &val2);
    T_ASSERT(t2[index], 0.5);
    T_ASSERT(val2, 0.5);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 5, cmp_double, &val2);
    T_ASSERT(t2[index], 0.6);
    T_ASSERT(val2, 0.6);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 6, cmp_double, &val2);
    T_ASSERT(t2[index], 0.7);
    T_ASSERT(val2, 0.7);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 7, cmp_double, NULL);
    T_ASSERT(t2[index], 0.8);

    index = array_select_kth(t2, ARRAY_SIZE(t2), sizeof(*t2), 8, cmp_double, NULL);
    T_ASSERT(t2[index], 0.9);
}

test_f test_median(void)
{
    int t[] = {1, 6, 3, 9, 8, 2, 4, 5, 7};
    int val1;
    double t2[] = {0.1, 0.6, 0.3, 0.9, 0.8, 0.2, 0.4, 0.5, 0.7};
    double val2;
    ssize_t index;

    index = array_median(t, ARRAY_SIZE(t), sizeof(*t), cmp_int, &val1);
    T_ASSERT(t[index], 5);
    T_ASSERT(val1, 5);

    index = array_median(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double, &val2);
    T_ASSERT(t2[index], 0.5);
    T_ASSERT(val2, 0.5);
}

test_f test_min(void)
{
    int t[] = {1, 6, 3, 9, 8, 2, 4, 5, 7};
    int val1;
    double t2[] = {0.1, 0.6, 0.3, 0.9, 0.8, 0.2, 0.4, 0.5, 0.7};
    double val2;
    ssize_t index;

    index = array_min(t, ARRAY_SIZE(t), sizeof(*t), cmp_int, &val1);
    T_ASSERT(t[index], 1);
    T_ASSERT(val1, 1);

    index = array_min(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double, &val2);
    T_ASSERT(t2[index], 0.1);
    T_ASSERT(val2, 0.1);
}

test_f test_max(void)
{
    int t[] = {1, 6, 3, 9, 8, 2, 4, 5, 7};
    int val1;
    double t2[] = {0.1, 0.6, 0.3, 0.9, 0.8, 0.2, 0.4, 0.5, 0.7};
    double val2;
    ssize_t index;

    index = array_max(t, ARRAY_SIZE(t), sizeof(*t), cmp_int, &val1);
    T_ASSERT(t[index], 9);
    T_ASSERT(val1, 9);

    index = array_max(t2, ARRAY_SIZE(t2), sizeof(*t2), cmp_double, &val2);
    T_ASSERT(t2[index], 0.9);
    T_ASSERT(val2, 0.9);
}

test_f test_reverse(void)
{
    int t[] = {1, 6, 3, 9, 8, 2, 4, 5, 7};
    int rv[] = {7, 5, 4, 2, 8, 9, 3, 6, 1};
    double t2[] = {0.1, 0.6, 0.3, 0.9, 0.8, 0.2, 0.4, 0.5, 0.7, 0.0};
    double rv2[] = {0.0, 0.7, 0.5, 0.4, 0.2, 0.8, 0.9, 0.3, 0.6, 0.1};

    size_t i;

    array_reverse(t, ARRAY_SIZE(t), sizeof(*t));
    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], rv[i]);

    array_reverse(t2, ARRAY_SIZE(t2), sizeof(*t2));
    for (i = 0; i < ARRAY_SIZE(t2); ++i)
        T_ASSERT(t2[i], rv2[i]);
}

test_f test_lower_bound(void)
{
    int t[] = {1, 2, 3, 4, 7, 8, 9, 10};
    double t2[] = {0.1, 0.2, 0.3, 0.4, 0.7, 0.8, 0.9, 1.0};
    int val;
    double val2;

    /* t */
    val = 0;
    T_EXPECT(array_lower_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 0);

    val = 1;
    T_EXPECT(array_lower_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 0);

    val = 4;
    T_EXPECT(array_lower_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 3);

    val = 6;
    T_EXPECT(array_lower_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 4);

    val = 10;
    T_EXPECT(array_lower_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 7);

    val = 11;
    T_EXPECT(array_lower_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 8);

    /* t2 */
    val2 = 0.0;
    T_EXPECT(array_lower_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 0);

    val2 = 0.1;
    T_EXPECT(array_lower_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 0);

    val2 = 0.4;
    T_EXPECT(array_lower_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 3);

    val2 = 0.6;
    T_EXPECT(array_lower_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 4);

    val2 = 1.0;
    T_EXPECT(array_lower_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 7);

    val2 = 1.1;
    T_EXPECT(array_lower_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 8);
}

test_f test_upper_bound(void)
{
    int t[] = {1, 2, 3, 4, 7, 8, 9, 10};
    double t2[] = {0.1, 0.2, 0.3, 0.4, 0.7, 0.8, 0.9, 1.0};
    int val;
    double val2;

    /* t */
    val = 0;
    T_EXPECT(array_upper_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 0);

    val = 1;
    T_EXPECT(array_upper_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 1);

    val = 4;
    T_EXPECT(array_upper_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 4);

    val = 6;
    T_EXPECT(array_upper_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 4);

    val = 10;
    T_EXPECT(array_upper_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 8);

    val = 11;
    T_EXPECT(array_upper_bound(t, ARRAY_SIZE(t), sizeof(*t), &val, cmp_int), 8);

    /* t2 */
    val2 = 0.0;
    T_EXPECT(array_upper_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 0);

    val2 = 0.1;
    T_EXPECT(array_upper_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 1);

    val2 = 0.4;
    T_EXPECT(array_upper_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 4);

    val2 = 0.6;
    T_EXPECT(array_upper_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 4);

    val2 = 1.0;
    T_EXPECT(array_upper_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 8);

    val2 = 1.1;
    T_EXPECT(array_upper_bound(t2, ARRAY_SIZE(t2), sizeof(*t2), &val2, cmp_double), 8);
}

test_f test_find_first_unsorted(void)
{
    int t1[] = {1, 9, 8, 5, 3, 5, 2, 1, 4, 7, 7, 3};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {0, 6, 4, 8, 3, -1, 9, 2, 1};

    char t2[] = "aBzCDDfguC";
    char f2[] = "abcdefABCDEF";
    ssize_t pos2[] = {0, -1, -1, -1, -1, 6, -1, 1, 3, 4, -1, -1};

    size_t i;

    int val1;
    char val2;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
    {
        T_EXPECT(array_unsorted_find_first(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, &val1), pos1[i]);
        if (pos1[i] != -1)
            T_ASSERT(val1, t1[pos1[i]]);
        T_EXPECT(array_unsorted_find_first(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, NULL), pos1[i]);
    }

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
    {
        T_EXPECT(array_unsorted_find_first(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, &val2), pos2[i]);
        if (pos2[i] != -1)
            T_ASSERT(val2, t2[pos2[i]]);
        T_EXPECT(array_unsorted_find_first(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, NULL), pos2[i]);
    }
}

test_f test_find_last_unsorted(void)
{
    int t1[] = {1, 9, 8, 5, 3, 5, 2, 1, 4, 7, 7, 3};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {7, 6, 11, 8, 5, -1, 10, 2, 1};

    char t2[] = "aBzCDDfguC";
    char f2[] = "abcdefABCDEF";
    ssize_t pos2[] = {0, -1, -1, -1, -1, 6, -1, 1, 9, 5, -1, -1};

    size_t i;

    int val1;
    char val2;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
    {
        T_EXPECT(array_unsorted_find_last(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, &val1), pos1[i]);
        if (pos1[i] != -1)
            T_ASSERT(val1, t1[pos1[i]]);
        T_EXPECT(array_unsorted_find_last(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, NULL), pos1[i]);
    }

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
    {
        T_EXPECT(array_unsorted_find_last(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, &val2), pos2[i]);
        if (pos2[i] != -1)
            T_ASSERT(val2, t2[pos2[i]]);
        T_EXPECT(array_unsorted_find_last(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, NULL), pos2[i]);
    }
}

test_f test_find_first_sorted(void)
{
    int t1[] = {1, 1, 1, 2, 2, 2, 3, 4, 5, 7, 7, 7, 9, 9, 9, 9, 9};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {0, 3, 6, 7, 8, -1, 9, -1, 12};

    char t2[] = "aaabbbbcfffgggghijklmmmmmnnnn";
    char f2[] = "abcdefghijklmn";
    ssize_t pos2[] = {0, 3, 7, -1, -1, 8, 11, 15, 16, 17, 18, 19, 20, 25};

    size_t i;

    int val1;
    char val2;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
    {
        T_EXPECT(array_sorted_find_first(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, &val1), pos1[i]);
        if (pos1[i] != -1)
            T_ASSERT(val1, t1[pos1[i]]);
        T_EXPECT(array_sorted_find_first(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, NULL), pos1[i]);
    }

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
    {
        T_EXPECT(array_sorted_find_first(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, &val2), pos2[i]);
        if (pos2[i] != -1)
            T_ASSERT(val2, t2[pos2[i]]);
        T_EXPECT(array_sorted_find_first(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, NULL), pos2[i]);
    }
}

test_f test_find_last_sorted(void)
{
    int t1[] = {1, 1, 1, 2, 2, 2, 3, 4, 5, 7, 7, 7, 9, 9, 9, 9, 9};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {2, 5, 6, 7, 8, -1, 11, -1, 16};

    char t2[] = "aaabbbbcfffgggghijklmmmmmnnnn";
    char f2[] = "abcdefghijklmn";
    ssize_t pos2[] = {2, 6, 7, -1, -1, 10, 14, 15, 16, 17, 18, 19, 24, 28};

    size_t i;

    int val1;
    char val2;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
    {
        T_EXPECT(array_sorted_find_last(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, &val1), pos1[i]);
        if (pos1[i] != -1)
            T_ASSERT(val1, t1[pos1[i]]);
        T_EXPECT(array_sorted_find_last(t1, ARRAY_SIZE(t1), sizeof(*t1), &f1[i], cmp_int, NULL), pos1[i]);
    }

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
    {
        T_EXPECT(array_sorted_find_last(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, &val2), pos2[i]);
        if (pos2[i] != -1)
            T_ASSERT(val2, t2[pos2[i]]);
        T_EXPECT(array_sorted_find_last(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), &f2[i], cmp_char, NULL), pos2[i]);
    }
}

test_f test_shuffle(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char t2[] = "abcdefghij";

    size_t i;

    array_shuffle(t, ARRAY_SIZE(t), sizeof(*t));
    array_shuffle(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2));

    T_EXPECT(check_array_is_sorted(t, ARRAY_SIZE(t), sizeof(*t), cmp_int), false);
    T_EXPECT(check_array_is_sorted(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), cmp_char), false);

    T_EXPECT(array_sort(t, ARRAY_SIZE(t), sizeof(*t), cmp_int), 0);
    T_EXPECT(array_sort(t2, ARRAY_SIZE(t2) - 1, sizeof(*t2), cmp_char), 0);

    for (i = 0; i < ARRAY_SIZE(t); ++i)
        T_ASSERT(t[i], i + 1);

    for (i = 0; i < ARRAY_SIZE(t2) - 1; ++i)
        T_ASSERT(t2[i], 'a' + i);
}

test_f test_empty(void)
{
    void *t = NULL;
    void *t2 = NULL;

    int t3[] = {0};
    int key = 5;

    t = array_create(0, 10);
    T_CHECK(t == NULL);

    t = array_create(10, 0);
    T_CHECK(t == NULL);

    array_move(t, t2, 0, 1);
    array_zeros(t, 0, 0);
    array_set_all(t, 0, 0, &key);
    array_copy(t, t2, 0, 1);
    array_shuffle(t, 0, 1);
    t = array_clone(t2, 0, 1);
    T_CHECK(t == NULL);

    T_EXPECT(array_unsorted_find_first(t3, 0, sizeof(*t3), &key, cmp_int, NULL), -1);
    T_EXPECT(array_unsorted_find_last(t3, 0, sizeof(*t3), &key, cmp_int, NULL), -1);
    T_EXPECT(array_sorted_find_first(t3, 0, sizeof(*t3), &key, cmp_int, NULL), -1);
    T_EXPECT(array_sorted_find_last(t3, 0, sizeof(*t3), &key, cmp_int, NULL), -1);

    T_CHECK(array_sort(t3, 0, sizeof(*t3), cmp_int) != 0);
    T_EXPECT(array_is_sorted(t3, 0, sizeof(*t3), cmp_int), false);
    T_EXPECT(array_is_reverse_sorted(t3, 0, sizeof(*t3), cmp_int), false);

    T_EXPECT(array_select_kth(t3, 0, sizeof(*t3), 0, cmp_int, &key), -1);
    T_EXPECT(array_median(t3, 0, sizeof(*t3), cmp_int, &key), -1);
    T_EXPECT(array_min(t3, 0, sizeof(*t3), cmp_int, &key), -1);
    T_EXPECT(array_max(t3, 0, sizeof(*t3), cmp_int, &key), -1);

    T_EXPECT(array_upper_bound(t3, 0, sizeof(*t3), &key, cmp_int), -1);
    T_EXPECT(array_lower_bound(t3, 0, sizeof(*t3), &key, cmp_int), -1);

    T_CHECK(array_delete_first(t3, 0, sizeof(*t3)) != 0);
    T_CHECK(array_delete_last(t3, 0, sizeof(*t3)) != 0);
    T_CHECK(array_delete_pos(t3, 0, sizeof(*t3), 0) != 0);

    T_CHECK(array_delete_first_with_entry(t, 0, sizeof(MyStruct2 *), my_struct2_destroy) != 0);
    T_CHECK(array_delete_last_with_entry(t, 0, sizeof(MyStruct2 *), my_struct2_destroy) != 0);
    T_CHECK(array_delete_pos_with_entry(t, 0, sizeof(MyStruct2 *), 0, my_struct2_destroy) != 0);

    T_EXPECT(array_equal(t, t2, 0, 1, cmp_int), false);

    /* this line will crash iff code is broken */
    array_reverse(t, 0, sizeof(int));
}

void test(void)
{
    TEST(test_create());
    TEST(test_equal());
    TEST(test_clone());
    TEST(test_copy());
    TEST(test_move());
    TEST(test_zeros());
    TEST(test_set_all());
    TEST(test_array_foreach());
    TEST(test_destroy_with_entries());
    TEST(test_insert());
    TEST(test_insert_with_entry());
    TEST(test_insert_sorted());
    TEST(test_insert_sorted_with_entry());
    TEST(test_delete());
    TEST(test_delete_on_whole());
    TEST(test_delete_all());
    TEST(test_delete_with_entry());
    TEST(test_sort());
    TEST(test_check_sorted());
    TEST(test_check_reverse_sorted());
    TEST(test_select_kth());
    TEST(test_median());
    TEST(test_min());
    TEST(test_max());
    TEST(test_reverse());
    TEST(test_lower_bound());
    TEST(test_upper_bound());
    TEST(test_find_first_sorted());
    TEST(test_find_last_sorted());
    TEST(test_find_first_unsorted());
    TEST(test_find_last_unsorted());
    TEST(test_shuffle());
    TEST(test_empty());
}

int main(void)
{
    TEST_INIT("ARRAY");
    test();
    TEST_SUMMARY();
}