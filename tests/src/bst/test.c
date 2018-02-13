#include <test.h>
#include <bst.h>
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

bool correct_hight(int hight, size_t n)
{
    return hight < n + 1;
}

bool correct_balanced_hight(int hight, size_t n)
{
    return hight < LOG2((unsigned long)n) + 2;
}

test_f test_create(void)
{
    Bst *tree;

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);
    T_EXPECT(bst_get_hight(tree), 0);
    bst_destroy(tree);

    tree = bst_create(sizeof(char), cmp_char, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(char));
    T_EXPECT(bst_get_num_entries(tree), 0);
    T_EXPECT(bst_get_hight(tree), 0);
    bst_destroy(tree);

    tree = bst_create(sizeof(double), cmp_double, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(double));
    T_EXPECT(bst_get_num_entries(tree), 0);
    T_EXPECT(bst_get_hight(tree), 0);
    bst_destroy(tree);

    tree = bst_create(sizeof(MyStruct *), cmp_my_struct, my_struct_destroy);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), 0);
    T_EXPECT(bst_get_hight(tree), 0);
    bst_destroy(tree);
}

test_f test_insert(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(t);
    FREE(rt);
    bst_destroy(tree);
}

test_f test_destroy_with_entries(void)
{
    Bst *tree;
    MyStruct *s;
    size_t size = BIT(10);
    size_t i;

    int *t;
    size_t r;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(MyStruct *), cmp_my_struct, my_struct_destroy);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
    {
        s = my_struct_create(t[i]);
        T_EXPECT(bst_insert(tree, (void *)&s), 0);
    }

    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(t);
    bst_destroy_with_entries(tree);
}

test_f test_insert_the_same(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    for (i = 0; i < size; ++i)
        T_CHECK(bst_insert(tree, (void *)&t[i]) != 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(t);
    FREE(rt);
    bst_destroy(tree);
}

test_f test_min_max(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    int min = -(size << 1);
    int max = -min;
    int val;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    t[0] = min;
    t[1] = max;

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    T_EXPECT(bst_min(tree, (void *)&val), 0);
    T_ASSERT(val, min);

    T_EXPECT(bst_max(tree, (void *)&val), 0);
    T_ASSERT(val, max);

    FREE(t);
    FREE(rt);
    bst_destroy(tree);
}

test_f test_search(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    int val;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    for (i = 0; i < size; ++i)
    {
        T_EXPECT(bst_search(tree, (void *)&t[i], (void *)&val), 0);
        T_ASSERT(val, t[i]);
    }

    for (i = 0; i < size; ++i)
    {
        val = t[i] + size;
        T_CHECK(bst_search(tree, (void *)&val, (void *)&val) != 0);
    }

    FREE(t);
    FREE(rt);
    bst_destroy(tree);
}

test_f test_key_exist(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    int val;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_key_exist(tree, (void *)&t[i]), true);

    for (i = 0; i < size; ++i)
    {
        val = t[i] + size;
        T_EXPECT(bst_key_exist(tree, (void *)&val), false);
    }

    FREE(t);
    FREE(rt);
    bst_destroy(tree);
}

test_f test_delete(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);

    for (i = 0; i < size >> 2; ++i)
    {
        T_EXPECT(bst_delete(tree, (void *)&t[i]), 0);
        T_EXPECT(bst_delete(tree, (void *)&t[i + (size >> 1) + (size >> 2)]), 0);
    }

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size >> 1);

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size >> 1, rsize);
    T_EXPECT(array_equal_int(t + (size >> 2), rt, size >> 1), true);

    FREE(rt);
    FREE(t);
    bst_destroy(tree);
}

test_f test_delete_with_entry(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t j;
    size_t r;

    MyStruct **rt;
    int *keys;
    size_t rsize;

    MyStruct *ms;
    MyStruct fake;
    MyStruct *fake_ptr = &fake;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(MyStruct *), cmp_my_struct, my_struct_destroy);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
    {
        ms = my_struct_create(t[i]);
        T_EXPECT(bst_insert(tree, (void *)&ms), 0);
    }

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);

    keys = (int *)malloc(sizeof(int) * rsize);
    T_ERROR(keys == NULL);
    for (j = 0; j < rsize; ++j)
        keys[j] = rt[j]->key;

    T_EXPECT(array_equal_int(t, keys, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);
    FREE(keys);

    for (i = 0; i < size >> 2; ++i)
    {
        fake_ptr ->key = t[i];
        T_EXPECT(bst_delete_with_entry(tree, (void *)&fake_ptr), 0);

        fake_ptr->key = t[i + (size >> 1) + (size >> 2)];
        T_EXPECT(bst_delete_with_entry(tree, (void *)&fake_ptr), 0);
    }

    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), size >> 1);

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size >> 1, rsize);

    keys = (int *)malloc(sizeof(int) * rsize);
    T_ERROR(keys == NULL);
    for (j = 0; j < rsize; ++j)
        keys[j] = rt[j]->key;

    T_EXPECT(array_equal_int(t + (size >> 2), keys, size >> 1), true);

    FREE(rt);
    FREE(keys);
    FREE(t);

    bst_destroy_with_entries(tree);
}

test_f test_delete_with_entry_wo_destr(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);

    for (i = 0; i < size >> 2; ++i)
    {
        T_EXPECT(bst_delete_with_entry(tree, (void *)&t[i]), 0);
        T_EXPECT(bst_delete_with_entry(tree, (void *)&t[i + (size >> 1) + (size >> 2)]), 0);
    }

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size >> 1);

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size >> 1, rsize);
    T_EXPECT(array_equal_int(t + (size >> 2), rt, size >> 1), true);

    FREE(rt);
    FREE(t);
    bst_destroy_with_entries(tree);
}

test_f test_delete_the_same(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);

    for (i = 0; i < size >> 2; ++i)
    {
        T_EXPECT(bst_delete(tree, (void *)&t[i]), 0);
        T_EXPECT(bst_delete(tree, (void *)&t[i + (size >> 1) + (size >> 2)]), 0);
    }

    for (i = 0; i < size >> 2; ++i)
    {
        T_CHECK(bst_delete(tree, (void *)&t[i]) != 0);
        T_CHECK(bst_delete(tree, (void *)&t[i + (size >> 1) + (size >> 2)]) != 0);
    }

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size >> 1);

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size >> 1, rsize);
    T_EXPECT(array_equal_int(t + (size >> 2), rt, size >> 1), true);

    FREE(rt);
    FREE(t);
    bst_destroy(tree);
}

test_f test_insert_delete(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);

    for (i = 0; i < size >> 2; ++i)
    {
        T_EXPECT(bst_delete(tree, (void *)&t[i]), 0);
        T_EXPECT(bst_delete(tree, (void *)&t[i + (size >> 1) + (size >> 2)]), 0);
    }

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size >> 1);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size >> 1, rsize);
    T_EXPECT(array_equal_int(t + (size >> 2), rt, size >> 1), true);

    FREE(rt);

    for (i = 0; i < size >> 2; ++i)
    {
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);
        T_EXPECT(bst_insert(tree, (void *)&t[i + (size >> 1) + (size >> 2)]), 0);
    }

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);
    FREE(t);
    bst_destroy(tree);
}

test_f test_balance(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    FREE(rt);

    T_EXPECT(bst_balance(tree), 0);
    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_balanced_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);
    FREE(t);
    bst_destroy(tree);
}

test_f test_balance_line(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;

    int *rt;
    size_t rsize;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    FREE(rt);

    T_EXPECT(bst_balance(tree), 0);
    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_balanced_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    FREE(rt);
    FREE(t);
    bst_destroy(tree);
}

test_f test_empty(void)
{
    Bst *tree;
    int dummy;
    int *t;
    size_t size;

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);
    T_EXPECT(bst_get_hight(tree), 0);
    T_CHECK(bst_min(tree, (void *)&dummy) != 0);
    T_CHECK(bst_max(tree, (void *)&dummy) != 0);
    T_CHECK(bst_delete(tree, (void *)&dummy) != 0);
    T_CHECK(bst_delete_with_entry(tree, (void *)&dummy) != 0);
    T_EXPECT(bst_key_exist(tree, (void *)&dummy), false);
    T_CHECK(bst_search(tree, (void *)&dummy, (void *)&dummy) != 0);
    T_CHECK(bst_to_array(tree, (void *)&t, &size) != 0);
    T_CHECK(bst_balance(tree) != 0);


    bst_destroy(tree);
}

test_f test_for_each(void)
{
    Bst *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    int val;
    Bst_node *node;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(bst_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(bst_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), size);
    T_EXPECT(correct_hight(bst_get_hight(tree), bst_get_num_entries(tree)), true);

    i = 0;
    for_each(tree, Bst, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i++], val);
    }

    i = 0;
    for_each_data(tree, Bst, val)
        T_ASSERT(t[i++], val);

    for_each_node(tree, Bst, node)
        T_CHECK(node != 0);

    i = size - 1;
    for_each_prev(tree, Bst, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i--], val);
    }

    i = size - 1;
    for_each_data_prev(tree, Bst, val)
        T_ASSERT(t[i--], val);

    for_each_node_prev(tree, Bst, node)
        T_CHECK(node != 0);

    for_each_root(tree, Bst, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root(tree, Bst, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root(tree, Bst, node)
        T_CHECK(node != 0);

    for_each_root_prev(tree, Bst, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root_prev(tree, Bst, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root_prev(tree, Bst, node)
        T_CHECK(node != 0);

    FREE(t);
    FREE(rt);
    bst_destroy(tree);
}

test_f test_empty_for_each(void)
{
    Bst *tree;
    int t[] = {0, 0, 0 };
    size_t size = ARRAY_SIZE(t);
    size_t i;

    int val;
    Bst_node *node;

    tree = bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);

    i = 0;
    for_each(tree, Bst, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i++], val);
    }

    i = 0;
    for_each_data(tree, Bst, val)
        T_ASSERT(t[i++], val);

    for_each_node(tree, Bst, node)
        T_CHECK(node != 0);

    i = size - 1;
    for_each_prev(tree, Bst, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i--], val);
    }

    i = size - 1;
    for_each_data_prev(tree, Bst, val)
        T_ASSERT(t[i--], val);

    for_each_node_prev(tree, Bst, node)
        T_CHECK(node != 0);

    for_each_root(tree, Bst, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root(tree, Bst, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root(tree, Bst, node)
        T_CHECK(node != 0);

    for_each_root_prev(tree, Bst, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root_prev(tree, Bst, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root_prev(tree, Bst, node)
        T_CHECK(node != 0);

    bst_destroy(tree);
}

test_f test_tree_framework(void)
{
    Tree *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    int val;
    int min = -(size << 1);
    int max = size << 1;

    MyStruct *s;
    size_t loop = 10;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    t[0] = min;
    t[1] = max;
    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = tree_bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_num_entries(tree), 0);
    T_EXPECT(tree_get_hight(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(tree_insert(tree, (void *)&t[i]), 0);

    T_EXPECT(tree_balance(tree), 0);
    T_EXPECT(correct_balanced_hight(tree_get_hight(tree), tree_get_num_entries(tree)), true);
    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(tree_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_num_entries(tree), size);

    for (i = 0; i < size; ++i)
    {
        T_EXPECT(tree_key_exist(tree, (void *)&t[i]), true);
        T_EXPECT(tree_search(tree, (void *)&t[i], (void *)&val), 0);
        T_ASSERT(val, t[i]);
    }

    for (i = 0; i < size; ++i)
    {
        val = t[i] + size - 1;
        T_EXPECT(tree_key_exist(tree, (void *)&val), false);
        T_CHECK(tree_search(tree, (void *)&val, (void *)&val) != 0);
    }

    T_EXPECT(tree_min(tree, (void *)&val), 0);
    T_ASSERT(val, min);

    T_EXPECT(tree_max(tree, (void *)&val), 0);
    T_ASSERT(val, max);

    for (i = 0; i < size; ++i)
        T_EXPECT(tree_delete(tree, (void *)&t[i]), 0);

    FREE(t);
    FREE(rt);
    tree_destroy(tree);

    tree = tree_bst_create(sizeof(MyStruct *), cmp_my_struct, my_struct_destroy);
    T_ERROR(tree == NULL);
    T_EXPECT(tree_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(tree_get_num_entries(tree), 0);

    for (i = 0; i < loop; ++i)
    {
        s = my_struct_create(i + 1);
        T_EXPECT(tree_insert(tree, (void *)&s), 0);
    }

    tree_destroy_with_entries(tree);
}

test_f test_tree_empty(void)
{
    Tree *tree;
    int dummy;
    int *t;
    size_t size;

    tree = tree_bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);

    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_num_entries(tree), 0);
    T_EXPECT(tree_get_hight(tree), 0);
    T_CHECK(tree_min(tree, (void *)&dummy) != 0);
    T_CHECK(tree_max(tree, (void *)&dummy) != 0);
    T_CHECK(tree_delete(tree, (void *)&dummy) != 0);
    T_CHECK(tree_delete_with_entry(tree, (void *)&dummy) != 0);
    T_EXPECT(tree_key_exist(tree, (void *)&dummy), false);
    T_CHECK(tree_search(tree, (void *)&dummy, (void *)&dummy) != 0);
    T_CHECK(tree_to_array(tree, (void *)&t, &size) != 0);
    T_CHECK(tree_balance(tree) != 0);

    tree_destroy(tree);
}

test_f test_tree_for_each(void)
{
    Tree *tree;
    int *t;
    size_t size = BIT(10);
    size_t i;
    size_t r;

    int *rt;
    size_t rsize;

    int val;
    Bst_node *node;

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    tree = tree_bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(tree_insert(tree, (void *)&t[i]), 0);

    sort((void *)t, size, cmp_int, sizeof(int));

    T_EXPECT(tree_to_array(tree, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_num_entries(tree), size);
    T_EXPECT(correct_hight(tree_get_hight(tree), tree_get_num_entries(tree)), true);

    i = 0;
    for_each(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i++], val);
    }

    i = 0;
    for_each_data(tree, Tree, val)
        T_ASSERT(t[i++], val);

    for_each_node(tree, Tree, node)
        T_CHECK(node != 0);

    i = size - 1;
    for_each_prev(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i--], val);
    }

    i = size - 1;
    for_each_data_prev(tree, Tree, val)
        T_ASSERT(t[i--], val);

    for_each_node_prev(tree, Tree, node)
        T_CHECK(node != 0);

    for_each_root(tree, Tree, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root(tree, Tree, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root(tree, Tree, node)
        T_CHECK(node != 0);

    for_each_root_prev(tree, Tree, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root_prev(tree, Tree, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root_prev(tree, Tree, node)
        T_CHECK(node != 0);

    FREE(t);
    FREE(rt);
    tree_destroy(tree);
}

test_f test_tree_empty_for_each(void)
{
    Tree *tree;
    int t[] = {0, 0, 0};
    size_t size = ARRAY_SIZE(t);
    size_t i;

    int val;
    Bst_node *node;

    tree = tree_bst_create(sizeof(int), cmp_int, NULL);
    T_ERROR(tree == NULL);
    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_num_entries(tree), 0);

    i = 0;
    for_each(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i++], val);
    }

    i = 0;
    for_each_data(tree, Tree, val)
        T_ASSERT(t[i++], val);

    for_each_node(tree, Tree, node)
        T_CHECK(node != 0);

    i = size - 1;
    for_each_prev(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i--], val);
    }

    i = size - 1;
    for_each_data_prev(tree, Tree, val)
        T_ASSERT(t[i--], val);

    for_each_node_prev(tree, Tree, node)
        T_CHECK(node != 0);

    for_each_root(tree, Tree, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root(tree, Tree, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root(tree, Tree, node)
        T_CHECK(node != 0);

    for_each_root_prev(tree, Tree, node, val)
    {
        T_CHECK(node != 0);
        T_CHECK(val >= t[0] && val <= t[size - 1]);
    }

    for_each_data_root_prev(tree, Tree, val)
        T_CHECK(val >= t[0] && val <= t[size - 1]);

    for_each_node_root_prev(tree, Tree, node)
        T_CHECK(node != 0);

    tree_destroy(tree);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert());
    TEST(test_destroy_with_entries());
    TEST(test_insert_the_same());
    TEST(test_min_max());
    TEST(test_key_exist());
    TEST(test_search());
    TEST(test_delete());
    TEST(test_delete_with_entry());
    TEST(test_delete_with_entry_wo_destr());
    TEST(test_delete_the_same());
    TEST(test_insert_delete());
    TEST(test_balance());
    TEST(test_balance_line());
    TEST(test_empty());
    TEST(test_for_each());
    TEST(test_empty_for_each());
    TEST(test_tree_framework());
    TEST(test_tree_empty());
    TEST(test_tree_for_each());
    TEST(test_tree_empty_for_each());
}

int main(void)
{
    TEST_INIT("BST");
    test();
    TEST_SUMMARY();
}