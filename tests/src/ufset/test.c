#include <ufset.h>
#include <test.h>
#include <common.h>
#include <stdlib.h>

typedef struct MyStruct
{
    int a;
    int b;
    int c;
    int d;
} MyStruct;

MyStruct *my_struct_create(void)
{
    MyStruct *s;

    s = (MyStruct *)malloc(sizeof(MyStruct));
    return s;
}

void my_struct_destroy(void *data)
{
    MyStruct *s = *(MyStruct **)data;
    FREE(s);
}

test_f test_create(void)
{
    UFSMaster *master;
    UFset *set;
    UFSentry *entry;

    int val = 0;

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    entry = ufs_entry_create((void *)&val, sizeof(int));
    T_ERROR(entry == NULL);

    set = ufset_create(entry, master);
    T_ERROR(set == NULL);

    T_EXPECT(ufset_get_hight(set), 1);
    T_EXPECT(ufset_get_num_entries(set), 1);

    T_EXPECT(ufs_master_get_hight(master), 1);
    T_EXPECT(ufs_master_get_num_entries(master), 1);

    ufs_master_destroy(master);
}

test_f test_create_set(void)
{
    UFSMaster *master;
    UFset *set;
    UFSentry *entry;

    int val = 0;
    int i;
    int size = BIT(10);

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    for (i = 0; i < size; ++i)
    {
        entry = ufs_entry_create((void *)&val, sizeof(int));
        T_ERROR(entry == NULL);

        set = ufset_create(entry, master);
        T_ERROR(set == NULL);

        T_EXPECT(ufset_get_hight(set), 1);
        T_EXPECT(ufset_get_num_entries(set), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);
    }

    ufs_master_destroy(master);
}

test_f test_destroy_with_entries(void)
{
    UFSMaster *master;
    UFset *set;
    UFSentry *entry;

    MyStruct *val;
    int i;
    int size = BIT(10);

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    for (i = 0; i < size; ++i)
    {
        val = my_struct_create();
        entry = ufs_entry_create((void *)&val, sizeof(MyStruct *));
        T_ERROR(entry == NULL);

        set = ufset_create(entry, master);
        T_ERROR(set == NULL);
        T_EXPECT(ufset_get_hight(set), 1);
        T_EXPECT(ufset_get_num_entries(set), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);
    }

    ufs_master_destroy_with_entries(master, my_struct_destroy);
}

test_f test_union(void)
{
#define N 7
    UFSMaster *master;
    UFset *set[N];
    UFSentry *entry[N];

    int val = 0;
    int i;

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    for (i = 0; i < N; ++i)
    {
        entry[i] = ufs_entry_create((void *)&val, sizeof(int));
        T_ERROR(entry[i] == NULL);

        set[i] = ufset_create(entry[i], master);
        T_ERROR(set[i] == NULL);

        T_EXPECT(ufset_get_hight(set[i]), 1);
        T_EXPECT(ufset_get_num_entries(set[i]), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);
    }

    T_EXPECT(ufset_union(set[0], set[3]), 0);
    T_EXPECT(ufset_union(set[1], set[4]), 0);
    T_EXPECT(ufset_union(set[2], set[5]), 0);
    T_EXPECT(ufset_union(set[2], set[6]), 0);
    T_EXPECT(ufset_union(set[4], set[0]), 0);
    T_EXPECT(ufset_union(set[1], set[6]), 0);

    for (i = 0; i < N; ++i)
    {
        T_EXPECT(ufset_get_hight(set[i]), 3);
        T_EXPECT(ufset_get_num_entries(set[i]), N);
    }

    T_EXPECT(ufs_master_get_hight(master), 3);
    T_EXPECT(ufs_master_get_num_entries(master), N);

    ufs_master_destroy(master);
#undef N
}

test_f test_find(void)
{
#define N 7
    UFSMaster *master;
    UFset *set[N];
    UFSentry *entry[N];

    int val = 0;
    int i;

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    for (i = 0; i < N; ++i)
    {
        entry[i] = ufs_entry_create((void *)&val, sizeof(int));
        T_ERROR(entry[i] == NULL);

        set[i] = ufset_create(entry[i], master);
        T_ERROR(set[i] == NULL);
        T_EXPECT(ufset_find(set[i]), set[i]);

        T_EXPECT(ufset_get_hight(set[i]), 1);
        T_EXPECT(ufset_get_num_entries(set[i]), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);
    }

    T_EXPECT(ufset_union(set[0], set[3]), 0);
    T_EXPECT(ufset_find(set[0]), set[3]);
    T_EXPECT(ufset_find(set[3]), set[3]);

    T_EXPECT(ufset_union(set[1], set[4]), 0);
    T_EXPECT(ufset_find(set[1]), set[4]);
    T_EXPECT(ufset_find(set[4]), set[4]);

    T_EXPECT(ufset_union(set[2], set[5]), 0);
    T_EXPECT(ufset_find(set[2]), set[5]);
    T_EXPECT(ufset_find(set[5]), set[5]);

    T_EXPECT(ufset_union(set[2], set[6]), 0);
    T_EXPECT(ufset_find(set[2]), set[5]);
    T_EXPECT(ufset_find(set[5]), set[5]);
    T_EXPECT(ufset_find(set[6]), set[5]);

    T_EXPECT(ufset_union(set[4], set[0]), 0);
    T_EXPECT(ufset_find(set[0]), set[3]);
    T_EXPECT(ufset_find(set[1]), set[3]);
    T_EXPECT(ufset_find(set[3]), set[3]);
    T_EXPECT(ufset_find(set[4]), set[3]);

    T_EXPECT(ufset_union(set[1], set[6]), 0);
    T_EXPECT(ufset_find(set[0]), set[3]);
    T_EXPECT(ufset_find(set[1]), set[3]);
    T_EXPECT(ufset_find(set[2]), set[3]);
    T_EXPECT(ufset_find(set[3]), set[3]);
    T_EXPECT(ufset_find(set[4]), set[3]);
    T_EXPECT(ufset_find(set[5]), set[3]);
    T_EXPECT(ufset_find(set[6]), set[3]);

    for (i = 0; i < N; ++i)
    {
        T_EXPECT(ufset_get_hight(set[i]), 3);
        T_EXPECT(ufset_get_num_entries(set[i]), N);
    }

    T_EXPECT(ufs_master_get_hight(master), 3);
    T_EXPECT(ufs_master_get_num_entries(master), N);

    ufs_master_destroy(master);
#undef N
}

void test(void)
{
    TEST(test_create());
    TEST(test_create_set());
    TEST(test_destroy_with_entries());
    TEST(test_union());
    TEST(test_find());
}

int main(void)
{
    TEST_INIT("UFSET")
    test();
    TEST_SUMMARY;
}
