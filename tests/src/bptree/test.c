#include <test.h>
#include <bptree.h>
#include <common.h>

CMP(int)

static void bptree_print_node(BPTree_node *node)
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

static void bptree_print(BPTree *tree)
{
    bptree_print_node(tree->____root);
}

#define BPTREE_TEST_SET(func) \
    do { \
        TEST(func(2)); \
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

    tree = bptree_create(fanout, sizeof(int), cmp_int);
    T_ERROR(tree == NULL);

    T_EXPECT(bptree_get_data_size(tree), sizeof(int));
    T_EXPECT(bptree_get_hight(tree), 0);
    T_EXPECT(bptree_get_num_entries(tree), 0);

    for (i = 0; i < tree->____fanout << 2; ++i)
    {
        val = i + 1;
        printf("INSERT INTO TREE VAL = %d\n", val);
        bptree_insert(tree, (void *)&val);
    }

    bptree_print(tree);

    bptree_destroy(tree);
}

void test(void)
{
    BPTREE_TEST_SET(test_create);
    BPTREE_TEST_SET(test_insert);
}

int main(void)
{
    TEST_INIT("B+ TREE");
    test();
    TEST_SUMMARY;
}