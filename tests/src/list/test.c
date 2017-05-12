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

    for (i = 0; i < size; ++i)
    {
        T_EXPECT(list_delete(list, (void *)&del[i]), 0);
        T_EXPECT(list_get_num_entries(list), size - i - 1);
        T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, size - i - 1);
        T_EXPECT(array_equal_int(t_exp_del[i], rt, size - i - 1), true);
        FREE(rt);
    }

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

    for (i = 0; i < size - 1; ++i)
    {
        T_EXPECT(list_delete_all(list, (void *)&del[i]), ret[i]);
        T_EXPECT(list_get_num_entries(list), sizes[i]);
        T_EXPECT(list_to_array(list, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, sizes[i]);
        T_EXPECT(array_equal_int(t_exp_del[i], rt, sizes[i]), true);
        FREE(rt);
    }

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

        T_EXPECT(list_insert(list, (void *)&s), 0);
    }

    list_destroy_with_entries(list, my_struct_destroy);
}

void test(void)
{
    TEST(test_create());
    TEST(test_destroy_with_entries());
    TEST(test_insert());
    TEST(test_delete());
    TEST(test_delete_all());
}

int main(void)
{
    TEST_INIT("LIST");
    test();
    TEST_SUMMARY;
}
