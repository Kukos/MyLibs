#include <test.h>
#include <bptree.h>
#include <common.h>

CMP(int)


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

void test(void)
{
    BPTREE_TEST_SET(test_create);
}

int main(void)
{
    TEST_INIT("B+ TREE");
    test();
    TEST_SUMMARY;
}