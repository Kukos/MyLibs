#include <heap.h>
#include <test.h>
#include <common.h>
#include <stdlib.h>
#include <sort.h>

CMP(int)
CMP(double)
CMP(char)

ARRAY_EQUAL(int)

typedef struct MyStruct
{
    int key;
    int a;
    int b;
    int c;
    int d;
}MyStruct;

MyStruct *my_struct_create(int key)
{
    MyStruct *s;

    s = (MyStruct *)malloc(sizeof(MyStruct));
    s->key = key;

    return s;
}

void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
}

int cmp_my_struct(const void *a, const void *b)
{
    MyStruct *s1 = *(MyStruct **)a;
    MyStruct *s2 = *(MyStruct **)b;

    if (s1->key < s2->key)
        return -1;

    if (s1->key > s2->key)
        return 1;

    return 0;
}

int cmp_heap_entry(const void *a, const void *b)
{
    Heap_entry *e1 = *(Heap_entry **)a;
    Heap_entry *e2 = *(Heap_entry **)b;

    return cmp_int(heap_entry_get_data(e1), heap_entry_get_data(e2));
}

test_f test_create(heap_type type, int ary)
{
    Heap *heap;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    heap_destroy(heap);

    heap = heap_create(type, sizeof(char), ary, cmp_char, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(char));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    heap_destroy(heap);

    heap = heap_create(type, sizeof(double), ary, cmp_double, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(double));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    heap_destroy(heap);

    heap = heap_create(type, sizeof(MyStruct *), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(MyStruct *));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    heap_destroy(heap);
}

test_f test_build(heap_type type, int ary)
{
    Heap *heap;
    Heap_entry **array;
    Heap_entry *top;

    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    array = (Heap_entry **)malloc(sizeof(Heap_entry *) * size);
    T_ERROR(array == NULL);
    for (i = 0; i < size; ++i)
        array[i] = heap_entry_create((void *)&t[i], sizeof(int));

    T_EXPECT(heap_build(heap, array, size), 0);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), size);
    T_EXPECT(heap_is_empty(heap), false);

    sort((void *)array, size, cmp_heap_entry, sizeof(Heap_entry *));

    i = 0;
    while (!heap_is_empty(heap))
    {
        top = heap_get_top(heap);
        if (type == HEAP_MIN)
        {
            T_ASSERT(top, array[i]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[i])), 0);
        }
        else
        {
            T_ASSERT(top, array[size - i - 1]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[size - i - 1])), 0);
        }

        top = heap_extract_top(heap);
        if (type == HEAP_MIN)
        {
            T_ASSERT(top, array[i]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[i])), 0);
        }
        else
        {
            T_ASSERT(top, array[size - i - 1]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[size - i - 1])), 0);
        }

        ++i;
        heap_entry_destroy(top);
    }

    FREE(array);
    FREE(t);
    heap_destroy(heap);
}

test_f test_insert(heap_type type, int ary)
{
    Heap *heap;
    Heap_entry **array;
    Heap_entry *top;

    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    array = (Heap_entry **)malloc(sizeof(Heap_entry *) * size);
    T_ERROR(array == NULL);
    for (i = 0; i < size; ++i)
    {
        array[i] = heap_entry_create((void *)&t[i], sizeof(int));
        T_EXPECT(heap_insert(heap, array[i]), 0);
    }

    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), size);
    T_EXPECT(heap_is_empty(heap), false);

    sort((void *)array, size, cmp_heap_entry, sizeof(Heap_entry *));

    i = 0;
    while (!heap_is_empty(heap))
    {
        top = heap_get_top(heap);
        if (type == HEAP_MIN)
        {
            T_ASSERT(top, array[i]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[i])), 0);
        }
        else
        {
            T_ASSERT(top, array[size - i - 1]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[size - i - 1])), 0);
        }

        top = heap_extract_top(heap);
        if (type == HEAP_MIN)
        {
            T_ASSERT(top, array[i]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[i])), 0);
        }
        else
        {
            T_ASSERT(top, array[size - i - 1]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[size - i - 1])), 0);
        }

        ++i;
        heap_entry_destroy(top);
    }

    FREE(array);
    FREE(t);
    heap_destroy(heap);
}

test_f test_destroy_with_entries(heap_type type, int ary)
{
    Heap *heap;
    MyStruct *s;
    Heap_entry **array;

    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    array = (Heap_entry **)malloc(sizeof(Heap_entry *) * size);
    T_ERROR(array == NULL);
    for (i = 0; i < size; ++i)
        array[i] = heap_entry_create((void *)&t[i], sizeof(int));

    T_EXPECT(heap_build(heap, array, size), 0);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), size);
    T_EXPECT(heap_is_empty(heap), false);

    FREE(array);
    FREE(t);
    heap_destroy(heap);

    heap = heap_create(type, sizeof(MyStruct *), ary, cmp_my_struct, my_struct_destroy);
    T_EXPECT(heap_get_data_size(heap), sizeof(MyStruct *));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    array = (Heap_entry **)malloc(sizeof(Heap_entry *) * size);
    T_ERROR(array == NULL);
    for (i = 0; i < size; ++i)
    {
        s = my_struct_create(t[i]);
        array[i] = heap_entry_create((void *)&s, sizeof(MyStruct *));
    }

    T_EXPECT(heap_build(heap, array, size), 0);
    T_EXPECT(heap_get_data_size(heap), sizeof(MyStruct *));
    T_EXPECT(heap_get_num_entries(heap), size);
    T_EXPECT(heap_is_empty(heap), false);

    FREE(array);
    FREE(t);
    heap_destroy_with_entries(heap);
}

test_f test_delete_with_entries_without_destructor(heap_type type, int ary)
{
    Heap *heap;
    Heap_entry **array;

    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    array = (Heap_entry **)malloc(sizeof(Heap_entry *) * size);
    T_ERROR(array == NULL);
    for (i = 0; i < size; ++i)
        array[i] = heap_entry_create((void *)&t[i], sizeof(int));

    T_EXPECT(heap_build(heap, array, size), 0);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), size);
    T_EXPECT(heap_is_empty(heap), false);

    FREE(array);
    FREE(t);
    heap_destroy_with_entries(heap);
}

test_f test_change_key(heap_type type, int ary)
{
    Heap *heap;
    Heap_entry **array;
    Heap_entry *top;

    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int pos[] = {0, 10, 20, 30, 40, 50, 100, 200, 300, 500};
    int factor = size << 1;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    array = (Heap_entry **)malloc(sizeof(Heap_entry *) * size);
    T_ERROR(array == NULL);
    for (i = 0; i < size; ++i)
    {
        array[i] = heap_entry_create((void *)&t[i], sizeof(int));
        T_EXPECT(heap_insert(heap, array[i]), 0);
    }

    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), size);
    T_EXPECT(heap_is_empty(heap), false);

    for (i = 0; i < ARRAY_SIZE(pos); ++i)
        if (type == HEAP_MAX)
        {
            t[pos[i]] += factor;
            T_EXPECT(heap_change_key(heap, heap_entry_get_pos(array[pos[i]]), (void *)&t[pos[i]]), 0);
        }
        else
        {
            t[pos[i]] -= factor;
            T_EXPECT(heap_change_key(heap, heap_entry_get_pos(array[pos[i]]), (void *)&t[pos[i]]), 0);
        }

    sort((void *)array, size, cmp_heap_entry, sizeof(Heap_entry *));

    i = 0;
    while (!heap_is_empty(heap))
    {
        top = heap_get_top(heap);
        if (type == HEAP_MIN)
        {
            T_ASSERT(top, array[i]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[i])), 0);
        }
        else
        {
            T_ASSERT(top, array[size - i - 1]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[size - i - 1])), 0);
        }

        top = heap_extract_top(heap);
        if (type == HEAP_MIN)
        {
            T_ASSERT(top, array[i]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[i])), 0);
        }
        else
        {
            T_ASSERT(top, array[size - i - 1]);
            T_EXPECT(cmp_int(heap_entry_get_data(top), heap_entry_get_data(array[size - i - 1])), 0);
        }

        ++i;
        heap_entry_destroy(top);
    }

    FREE(array);
    FREE(t);
    heap_destroy(heap);
}

test_f test_empty(heap_type type, int ary)
{
    Heap *heap;

    heap = heap_create(type, sizeof(int), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(int));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);
    T_EXPECT(heap_get_top(heap), NULL);
    T_EXPECT(heap_extract_top(heap), NULL);

    heap_destroy(heap);

    heap = heap_create(type, sizeof(char), ary, cmp_char, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(char));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);
    T_EXPECT(heap_get_top(heap), NULL);
    T_EXPECT(heap_extract_top(heap), NULL);

    heap_destroy(heap);

    heap = heap_create(type, sizeof(double), ary, cmp_double, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(double));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);
    T_EXPECT(heap_get_top(heap), NULL);
    T_EXPECT(heap_extract_top(heap), NULL);

    heap_destroy(heap);

    heap = heap_create(type, sizeof(MyStruct *), ary, cmp_int, NULL);
    T_EXPECT(heap_get_data_size(heap), sizeof(MyStruct *));
    T_EXPECT(heap_get_num_entries(heap), 0);
    T_EXPECT(heap_is_empty(heap), true);
    T_EXPECT(heap_get_top(heap), NULL);
    T_EXPECT(heap_extract_top(heap), NULL);

    heap_destroy(heap);
}

void test(void)
{
    TEST(test_create(HEAP_MIN, 2));
    TEST(test_create(HEAP_MIN, 7));
    TEST(test_create(HEAP_MAX, 2));
    TEST(test_create(HEAP_MAX, 7));

    TEST(test_build(HEAP_MIN, 2));
    TEST(test_build(HEAP_MIN, 7));
    TEST(test_build(HEAP_MAX, 2));
    TEST(test_build(HEAP_MAX, 7));

    TEST(test_insert(HEAP_MIN, 2));
    TEST(test_insert(HEAP_MIN, 7));
    TEST(test_insert(HEAP_MAX, 2));
    TEST(test_insert(HEAP_MAX, 7));

    TEST(test_destroy_with_entries(HEAP_MIN, 2));
    TEST(test_destroy_with_entries(HEAP_MIN, 7));
    TEST(test_destroy_with_entries(HEAP_MAX, 2));
    TEST(test_destroy_with_entries(HEAP_MAX, 7));

    TEST(test_delete_with_entries_without_destructor(HEAP_MIN, 2));
    TEST(test_delete_with_entries_without_destructor(HEAP_MIN, 7));
    TEST(test_delete_with_entries_without_destructor(HEAP_MAX, 2));
    TEST(test_delete_with_entries_without_destructor(HEAP_MAX, 7));

    TEST(test_change_key(HEAP_MIN, 2));
    TEST(test_change_key(HEAP_MIN, 7));
    TEST(test_change_key(HEAP_MAX, 2));
    TEST(test_change_key(HEAP_MAX, 7));

    TEST(test_empty(HEAP_MIN, 2));
    TEST(test_empty(HEAP_MIN, 7));
    TEST(test_empty(HEAP_MAX, 2));
    TEST(test_empty(HEAP_MAX, 7));
}

int main(void)
{
    TEST_INIT("HEAP");
    test();
    TEST_SUMMARY();
}