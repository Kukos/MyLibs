#include <list.h>
#include <test.h>
#include <compiler.h>
#include <common.h>
#include <stdlib.h>
#include <time.h>

typedef struct MyStruct
{
    int a;
    int b;
    int c;
    int d;
}MyStruct;

void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
}

int cmp_my_struct(void *a, void *b)
{
    MyStruct *s1 = *(MyStruct **)a;
    MyStruct *s2 = *(MyStruct **)b;

    if (s1->a < s2->a)
        return -1;

    if (s1->a > s2->a)
        return 1;

    return 0;
}

CMP(char)
CMP(int)
CMP(double)

ARRAY_EQUAL(int)

test_f test_create(void)
{
    List *list;

    list = list_create(sizeof(char), cmp_char);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(char));
    T_EXPECT(list_get_num_entries(list), 0);
    list_destroy(list);

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);
    list_destroy(list);

    list = list_create(sizeof(double), cmp_double);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(double));
    T_EXPECT(list_get_num_entries(list), 0);
    list_destroy(list);
}

test_f test_insert(void)
{
    List *list;

    int t[] = {9, 8, 4, 3, 1, 5, 6, 7, 2};
    int t_exp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    size_t size = ARRAY_SIZE(t);
    size_t rsize;
    int i;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp, rt, size), true);

    FREE(rt);
    list_destroy(list);
}

test_f test_delete(void)
{
    List *list;

    int t[] = {9, 8, 4, 1, 1, 5, 6, 7, 2};
    int t_exp[] = {1, 1, 2, 4, 5, 6, 7, 8, 9};
    int del[] = {2, 1, 5, 9, 4, 1, 7, 6, 8};
    int t_exp_del[ARRAY_SIZE(t)][ARRAY_SIZE(t)] = { {1, 1, 4, 5, 6, 7, 8, 9},
                                                    {1, 4, 5, 6, 7, 8, 9},
                                                    {1, 4, 6, 7, 8, 9},
                                                    {1, 4, 6, 7, 8},
                                                    {1, 6, 7, 8},
                                                    {6, 7, 8},
                                                    {6, 8},
                                                    {8},
                                                    {-1}
                                                  };
    int *rt;

    size_t size = ARRAY_SIZE(t);
    size_t rsize;
    int i;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp, rt, size), true);
    FREE(rt);

    for (i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete(list, (void *)&del[i]), 0);
        T_EXPECT(list_get_num_entries(list), size - i - 1);
        T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, size - i - 1);
        T_EXPECT(array_equal_int(t_exp_del[i], rt, size - i - 1), true);
        FREE(rt);
    }

    T_EXPECT(list_delete(list, (void *)&del[i]), 0);
    T_EXPECT(list_get_num_entries(list), size - i - 1);

    list_destroy(list);
}

test_f test_delete_all(void)
{
    List *list;

    int t[] = {9, 8, 4, 1, 1, 5, 6, 7, 2};
    int t_exp[] = {1, 1, 2, 4, 5, 6, 7, 8, 9};
    int del[] = {2, 1, 5, 9, 4, 7, 6, 8};
    int sizes[] = {8, 6, 5, 4, 3, 2, 1, 0};
    int ret[] = {1, 2, 1, 1, 1, 1, 1, 1, 1};
    int t_exp_del[ARRAY_SIZE(t)][ARRAY_SIZE(t)] = { {1, 1, 4, 5, 6, 7, 8, 9},
                                                    {4, 5, 6, 7, 8, 9},
                                                    {4, 6, 7, 8, 9},
                                                    {4, 6, 7, 8},
                                                    {6, 7, 8},
                                                    {6, 8},
                                                    {8},
                                                    {-1}
                                                  };
    int *rt;

    size_t size = ARRAY_SIZE(t);
    size_t rsize;
    int i;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp, rt, size), true);
    FREE(rt);

    for (i = 0; i < size - 2; ++i)
    {
        T_EXPECT(list_delete_all(list, (void *)&del[i]), ret[i]);
        T_EXPECT(list_get_num_entries(list), sizes[i]);
        T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, sizes[i]);
        T_EXPECT(array_equal_int(t_exp_del[i], rt, sizes[i]), true);
        FREE(rt);
    }

    T_EXPECT(list_delete_all(list, (void *)&del[i]), ret[i]);
    T_EXPECT(list_get_num_entries(list), sizes[i]);

    list_destroy(list);
}

test_f test_destroy_with_entries(void)
{
    MyStruct *s;
    List *list;

    size_t size = BIT(10);
    size_t i;

    list = list_create(sizeof(MyStruct *), cmp_my_struct);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_num_entries(list), 0);
    T_EXPECT(list_get_size_of(list), sizeof(MyStruct *));

    for (i = 0; i < size; ++i)
    {
        s = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(s == NULL);
        s->a = i;

        T_EXPECT(list_insert(list, (void *)&s), 0);
    }

    list_destroy_with_entries(list, my_struct_destroy);
}

test_f test_search(void)
{
    List *list;

    int t[] = {9, 8, 4, 3, 1, 5, 6, 7, 2};
    int noe[] = {11, 12, 13, 14, 15, -1, -2, -3, -4};
    int t_exp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    size_t size = ARRAY_SIZE(t);
    size_t rsize;
    int i;
    int val;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp, rt, size), true);

    for (i = 0; i < size; ++i)
    {
        T_EXPECT(list_search(list, (void *)&t[i], (void *)&val), 0);
        T_ASSERT(val, t[i]);

        T_CHECK(list_search(list, (void *)&noe[i], (void *)&val) != 0);
    }

    FREE(rt);
    list_destroy(list);
}

test_f test_insert_delete(void)
{
    List *list;

    int *t;
    int *rt;

    size_t size = BIT(10);
    size_t rsize;
    int i;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    t = (int *)malloc(sizeof(int) * size);
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = rand();

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);

    FREE(rt);

    for (i = 0; i < size >> 1; ++i)
    {
        T_EXPECT(list_delete(list, (void *)&t[i]), 0);
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);
    }

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);

    FREE(t);
    FREE(rt);
    list_destroy(list);
}

test_f test_merge(void)
{
    List *list1;
    List *list2;
    List *list3;

    int t1[] = {9, 8, 4, 3, 1, 5, 6, 7, 2};
    int t_exp1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    int t2[] = {-1, -2, -9, -8, -5, -4, -3, -1, -1};
    int t_exp2[] = {-9, -8, -5, -4, -3, -2, -1, -1, -1};

    int t_exp3[] = {-9, -8, -5, -4, -3, -2, -1, -1, -1, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    int *rt;

    size_t size = ARRAY_SIZE(t1);
    size_t rsize;
    int i;

    list1 = list_create(sizeof(int), cmp_int);
    T_ERROR(list1 == NULL);
    T_EXPECT(list_get_size_of(list1), sizeof(int));
    T_EXPECT(list_get_num_entries(list1), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list1, (void *)&t1[i]), 0);

    T_EXPECT(list_get_num_entries(list1), size);
    T_EXPECT(list_to_array(list1, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp1, rt, size), true);

    FREE(rt);

    list2 = list_create(sizeof(int), cmp_int);
    T_ERROR(list2 == NULL);
    T_EXPECT(list_get_size_of(list2), sizeof(int));
    T_EXPECT(list_get_num_entries(list2), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list2, (void *)&t2[i]), 0);

    T_EXPECT(list_get_num_entries(list2), size);
    T_EXPECT(list_to_array(list2, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp2, rt, size), true);

    FREE(rt);

    list3 = list_merge(list1, list2);
    T_ERROR(list3 == NULL);

    T_EXPECT(list_get_num_entries(list3), size << 1);
    T_EXPECT(list_to_array(list3, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size << 1);
    T_EXPECT(array_equal_int(t_exp3, rt, size), true);

    FREE(rt);

    list_destroy(list1);
    list_destroy(list2);
    list_destroy(list3);
}

test_f test_for_each(void)
{
    List *list;

    int t[] = {9, 8, 4, 3, 1, 5, 6, 7, 2};
    int t_exp[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    size_t size = ARRAY_SIZE(t);
    size_t rsize;
    int i;

    int data;
    List_node *node;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(list_insert(list, (void *)&t[i]), 0);

    T_EXPECT(list_get_num_entries(list), size);
    T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t_exp, rt, size), true);

    i = 0;
    for_each(list, List, node, data)
    {
        T_ASSERT(data, rt[i]);
        T_CHECK(node != NULL);

        ++i;
    }

    i = 0;
    for_each_data(list, List, data)
    {
        T_ASSERT(data, rt[i]);
        ++i;
    }

    for_each_node(list, List, node)
        T_CHECK(node != NULL);

    FREE(rt);
    list_destroy(list);
}

test_f test_empty(void)
{
    List *list;
    int in;
    int out;
    int *t;
    size_t size;

    list = list_create(sizeof(int), cmp_int);
    T_ERROR(list == NULL);
    T_EXPECT(list_get_size_of(list), sizeof(int));
    T_EXPECT(list_get_num_entries(list), 0);

    T_CHECK(list_delete(list, (void *)&in) != 0);
    T_CHECK(list_delete_all(list, (void *)&in) < 0);
    T_CHECK(list_search(list, (void *)&in, (void *)&out) != 0);
    T_CHECK(list_to_array(list, (void *)&t, &size) != 0);

    list_destroy(list);
}

test_f test_merge_empty(void)
{
    List *list1;
    List *list2;
    List *list3;

    int in;
    int out;
    int *t;
    size_t size;

    list1 = list_create(sizeof(int), cmp_int);
    T_ERROR(list1 == NULL);
    T_EXPECT(list_get_size_of(list1), sizeof(int));
    T_EXPECT(list_get_num_entries(list1), 0);

    T_CHECK(list_delete(list1, (void *)&in) != 0);
    T_CHECK(list_delete_all(list1, (void *)&in) < 0);
    T_CHECK(list_search(list1, (void *)&in, (void *)&out) != 0);
    T_CHECK(list_to_array(list1, (void *)&t, &size) != 0);

    list2 = list_create(sizeof(int), cmp_int);
    T_ERROR(list2 == NULL);
    T_EXPECT(list_get_size_of(list2), sizeof(int));
    T_EXPECT(list_get_num_entries(list2), 0);

    T_CHECK(list_delete(list2, (void *)&in) != 0);
    T_CHECK(list_delete_all(list2, (void *)&in) < 0);
    T_CHECK(list_search(list2, (void *)&in, (void *)&out) != 0);
    T_CHECK(list_to_array(list2, (void *)&t, &size) != 0);

    list3 = list_merge(list1, list2);
    T_ERROR(list3 == NULL);
    T_EXPECT(list_get_size_of(list3), sizeof(int));
    T_EXPECT(list_get_num_entries(list3), 0);

    T_CHECK(list_delete(list3, (void *)&in) != 0);
    T_CHECK(list_delete_all(list3, (void *)&in) < 0);
    T_CHECK(list_search(list3, (void *)&in, (void *)&out) != 0);
    T_CHECK(list_to_array(list3, (void *)&t, &size) != 0);

    list_destroy(list1);
    list_destroy(list2);
    list_destroy(list3);
}

void test(void)
{
    TEST(test_create());
    TEST(test_destroy_with_entries());
    TEST(test_insert());
    TEST(test_delete());
    TEST(test_delete_all());
    TEST(test_search());
    TEST(test_insert_delete());
    TEST(test_merge());
    TEST(test_for_each());
    TEST(test_empty());
}

int main(void)
{
    TEST_INIT("LIST");
    test();
    TEST_SUMMARY;
}
