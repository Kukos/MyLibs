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

int cmp_my_struct(const void *a, const void *b)
{
    const MyStruct *s1 = *(MyStruct **)a;
    const MyStruct *s2 = *(MyStruct **)b;

    if (s1->key < s2->key)
        return -1;

    if (s1->key > s2->key)
        return 1;

    return 0;
}

/*
    To remove after manual testing
*/
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
        TEST(func(16)); \
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

test_f test_min_max(int fanout)
{
    BPTree *tree;

    int val;
    int i;
    int r;

    int *t;
    size_t size = fanout << 2;

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
    sort(t, size, cmp_int, sizeof(int));

    T_EXPECT(bptree_min(tree, (void *)&val), 0);
    T_ASSERT(val, t[0]);

    T_EXPECT(bptree_max(tree, (void *)&val), 0);
    T_ASSERT(val, t[size - 1]);

    FREE(t);

    bptree_destroy(tree);
}

test_f test_key_exist(int fanout)
{
    BPTree *tree;

    int val;
    int i;
    int r;

    int *t;
    size_t size = fanout << 2;

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
    sort(t, size, cmp_int, sizeof(int));

    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        T_EXPECT(bptree_key_exist(tree, (void *)&val), true);

        val = -i;
        T_EXPECT(bptree_key_exist(tree, (void *)&val), false);

        val = i + 1 + size;
        T_EXPECT(bptree_key_exist(tree, (void *)&val), false);
    }

    FREE(t);

    bptree_destroy(tree);
}

test_f test_search(int fanout)
{
    BPTree *tree;

    int val;
    int out;
    int i;
    int r;

    int *t;
    size_t size = fanout << 2;

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
    sort(t, size, cmp_int, sizeof(int));

    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        T_EXPECT(bptree_search(tree, (void *)&val, (void *)&out), 0);
        T_ASSERT(val, out);

        val = -i;
        T_CHECK(bptree_search(tree, (void *)&val, (void *)&out) != 0);

        val = i + 1 + size;
        T_CHECK(bptree_search(tree, (void *)&val, (void *)&out) != 0);
    }
    FREE(t);

    bptree_destroy(tree);
}

test_f test_empty(int fanout)
{
    BPTree *tree;
    int val;

    size_t size;
    int *t;

    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_CHECK(bptree_min(tree, (void *)&val) != 0);
    T_CHECK(bptree_max(tree, (void *)&val) != 0);
    T_CHECK(bptree_search(tree, (void *)&val, (void *)&val) != 0);
    T_CHECK(bptree_delete(tree, (void *)&val) != 0);
    T_CHECK(bptree_to_array(tree, (void *)&t, &size) != 0);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);
    T_EXPECT(bptree_key_exist(tree, (void *)&val), false);

    bptree_destroy(tree);
}

test_f test_for_each(int fanout)
{
    BPTree *tree;
    BPTree_node *node;

    int val;
    int i;
    int r;

    int *t;
    size_t size = fanout << 2;

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
    sort(t, size, cmp_int, sizeof(int));

    i = 0;
    for_each(tree, BPTree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_prev(tree, BPTree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    i = 0;
    for_each_data(tree, BPTree, val)
    {
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_data_prev(tree, BPTree, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    for_each_node(tree, BPTree, node)
        T_CHECK(node != NULL);

    for_each_node_prev(tree, BPTree, node)
        T_CHECK(node != NULL);

    FREE(t);
    bptree_destroy(tree);
}

test_f test_empty_for_each(int fanout)
{
    BPTree *tree;
    BPTree_node *node;

    int val;
    int i;

    int t[] = {0, 0, 0};
    size_t size = ARRAY_SIZE(t);

    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    i = 0;
    for_each(tree, BPTree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_prev(tree, BPTree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    i = 0;
    for_each_data(tree, BPTree, val)
    {
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_data_prev(tree, BPTree, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    for_each_node(tree, BPTree, node)
        T_CHECK(node != NULL);

    for_each_node_prev(tree, BPTree, node)
        T_CHECK(node != NULL);

    bptree_destroy(tree);
}

test_f test_tree_empty(int fanout)
{
    Tree *tree;
    int val;

    size_t size;
    int *t;

    tree = tree_bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_CHECK(tree_min(tree, (void *)&val) != 0);
    T_CHECK(tree_max(tree, (void *)&val) != 0);
    T_CHECK(tree_search(tree, (void *)&val, (void *)&val) != 0);
    T_CHECK(tree_delete(tree, (void *)&val) != 0);
    T_CHECK(tree_to_array(tree, (void *)&t, &size) != 0);

    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_hight(tree), 0);
    T_EXPECT(tree_get_num_entries(tree), 0);
    T_EXPECT(tree_key_exist(tree, (void *)&val), false);

    tree_destroy(tree);
}

test_f test_tree_for_each(int fanout)
{
    Tree *tree;
    BPTree_node *node;

    int val;
    int i;
    int r;

    int *t;
    size_t size = fanout << 2;

    tree = tree_bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_hight(tree), 0);
    T_EXPECT(tree_get_num_entries(tree), 0);

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
        T_EXPECT(tree_insert(tree, (void *)&val), 0);
    }

    T_EXPECT(tree_get_num_entries(tree), size);
    sort(t, size, cmp_int, sizeof(int));

    i = 0;
    for_each(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_prev(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    i = 0;
    for_each_data(tree, Tree, val)
    {
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_data_prev(tree, Tree, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    for_each_node(tree, Tree, node)
        T_CHECK(node != NULL);

    for_each_node_prev(tree, Tree, node)
        T_CHECK(node != NULL);

    FREE(t);
    tree_destroy(tree);
}

test_f test_tree_empty_for_each(int fanout)
{
    Tree *tree;
    BPTree_node *node;

    int val;
    int i;

    int t[] = {0, 0, 0};
    size_t size = ARRAY_SIZE(t);

    tree = tree_bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(tree_get_data_size(tree), sizeof(int));
    T_EXPECT(tree_get_hight(tree), 0);
    T_EXPECT(tree_get_num_entries(tree), 0);

    i = 0;
    for_each(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_prev(tree, Tree, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    i = 0;
    for_each_data(tree, Tree, val)
    {
        T_ASSERT(t[i] , val);
        ++i;
    }

    i = size - 1;
    for_each_data_prev(tree, Tree, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(t[i] , val);
        --i;
    }

    for_each_node(tree, Tree, node)
        T_CHECK(node != NULL);

    for_each_node_prev(tree, Tree, node)
        T_CHECK(node != NULL);

    tree_destroy(tree);
}

void test(void)
{
    BPTREE_TEST_SET(test_create);
    BPTREE_TEST_SET(test_insert);
    BPTREE_TEST_SET(test_destroy_with_entries);
    BPTREE_TEST_SET(test_min_max);
    BPTREE_TEST_SET(test_key_exist);
    BPTREE_TEST_SET(test_search);
    BPTREE_TEST_SET(test_empty);
    BPTREE_TEST_SET(test_for_each);
    BPTREE_TEST_SET(test_empty_for_each);
    BPTREE_TEST_SET(test_tree_empty);
    BPTREE_TEST_SET(test_tree_for_each);
    BPTREE_TEST_SET(test_tree_empty_for_each);
}

int main(void)
{
    TEST_INIT("B+ TREE");
    test();
    TEST_SUMMARY;
}