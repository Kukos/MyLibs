#include <test.h>
#include <bptree.h>
#include <common.h>
#include <sort.h>

CMP(int)
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

___unused___ static void bptree_print_node(BPTree_node *node)
{
    int i;
    int *t;

    if (node == NULL)
        return;

    printf("NODE %p", (void *)node);
    node->____is_leaf ? (printf(" [LEAF], PARENT = %p\n", (void *)node->____parent)) : (printf(" PARENT = %p\n", (void *)node->____parent));
    t = (int *)node->____keys;
    for (i = 0; i < node->____keys_c; ++i)
        printf("  %d  ", t[i]);

    printf("\n");

    for (i = 0; i <= node->____keys_c; ++i)
        if (node->____ptrs[i] != NULL)
            bptree_print_node(node->____ptrs[i]);
}

___unused___ static void bptree_print(BPTree *tree)
{
    bptree_print_node(tree->____root);
}

#define BPTREE_TEST_SET(func) \
    do { \
        TEST(func(2)); \
        TEST(func(5)); \
        TEST(func(15)); \
        TEST(func(1000)); \
    } while (0)

test_f test_create(int fanout)
{
    BPTree *tree;


    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    bptree_destroy(tree);
}

test_f test_insert(int fanout)
{
    BPTree *tree;

    int val;
    int i;
    int r;

    int *t;
    size_t size = fanout << 2;

    int *rt;
    size_t rsize;

    /* ascending */
    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    t = (int *)malloc(size * sizeof(int));
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        t[i] = val;
        T_EXPECT(bptree_insert(tree, (void *)&val), 0);
    }
    T_EXPECT(bptree_to_array(tree, (void *)&rt, &rsize), 0);

    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    FREE(t);
    FREE(rt);

    bptree_destroy(tree);

    /* descending */
    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    t = (int *)malloc(size * sizeof(int));
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
    {
        val = -i - 1;
        t[i] = val;
        T_EXPECT(bptree_insert(tree, (void *)&val), 0);
    }
    T_EXPECT(bptree_to_array(tree, (void *)&rt, &rsize), 0);

    sort(t, size, cmp_int, sizeof(int));

    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    FREE(t);
    FREE(rt);

    bptree_destroy(tree);

    /* random */
    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    t = (int *)malloc(size * sizeof(int));
    T_ERROR(t == NULL);

    for (i = 0; i < size; ++i)
        t[i] = i + 1;

    for (i = 0; i < size; ++i)
    {
        r = rand() % (size - i);
        SWAP(t[r], t[size - i - 1]);
    }

    for (i = 0; i < size; ++i)
    {
        val = t[i];
        T_EXPECT(bptree_insert(tree, (void *)&val), 0);
    }

    T_EXPECT(bptree_get_num_entries(tree), size);
    T_EXPECT(bptree_to_array(tree, (void *)&rt, &rsize), 0);

    sort(t, size, cmp_int, sizeof(int));

    T_ASSERT(size, rsize);
    T_EXPECT(array_equal_int(t, rt, size), true);

    FREE(t);
    FREE(rt);

    bptree_destroy(tree);
}

test_f test_destroy_with_entries(int fanout)
{
    BPTree *tree;

    MyStruct *val;
    int i;

    size_t size = fanout << 2;

    tree = bptree_create(fanout, sizeof(MyStruct *), cmp_my_struct);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(MyStruct *));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    for (i = 0; i < size; ++i)
    {
        val = my_struct_create(i + 1);
        T_EXPECT(bptree_insert(tree, (void *)&val), 0);
    }

    bptree_destroy_with_entries(tree, my_struct_destroy);
}

void test(void)
{
    BPTREE_TEST_SET(test_create);
    BPTREE_TEST_SET(test_insert);
    BPTREE_TEST_SET(test_destroy_with_entries);
}

int main(void)
{
    TEST_INIT("B+ TREE");
    test();
    TEST_SUMMARY;
}