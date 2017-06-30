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

int cmp_my_struct(void *a, void *b)
{
    MyStruct *s1 = *(MyStruct **)a;
    MyStruct *s2 = *(MyStruct **)b;

    if (s1->key < s2->key)
        return -1;

    if (s1->key > s2->key)
        return 1;

    return 0;
}

test_f test_create(void)
{
    Bst *tree;

    tree = bst_create(sizeof(int), cmp_int);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(int));
    T_EXPECT(bst_get_num_entries(tree), 0);
    bst_destroy(tree);

    tree = bst_create(sizeof(char), cmp_char);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(char));
    T_EXPECT(bst_get_num_entries(tree), 0);
    bst_destroy(tree);

    tree = bst_create(sizeof(double), cmp_double);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(double));
    T_EXPECT(bst_get_num_entries(tree), 0);
    bst_destroy(tree);

    tree = bst_create(sizeof(MyStruct *), cmp_my_struct);
    T_ERROR(tree == NULL);
    T_EXPECT(bst_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bst_get_num_entries(tree), 0);
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

    tree = bst_create(sizeof(int), cmp_int);
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

    tree = bst_create(sizeof(MyStruct *), cmp_my_struct);
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

    FREE(t);
    bst_destroy_with_entries(tree, my_struct_destroy);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert());
    TEST(test_destroy_with_entries());
}

int main(void)
{
    TEST_INIT("BST");
    test();
    TEST_SUMMARY;
}