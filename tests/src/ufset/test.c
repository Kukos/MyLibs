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
    int data;

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    entry = ufs_entry_create((void *)&val, sizeof(int), NULL);
    T_ERROR(entry == NULL);

    set = ufset_create(entry, master);
    T_ERROR(set == NULL);

    T_EXPECT(ufset_get_hight(set), 1);
    T_EXPECT(ufset_get_num_entries(set), 1);

    T_EXPECT(ufs_master_get_hight(master), 1);
    T_EXPECT(ufs_master_get_num_entries(master), 1);

    T_EXPECT(ufset_entry_get_ufset(entry), set);
    T_EXPECT(ufset_entry_get_data(entry, &data), 0);
    T_EXPECT(ufset_get_entry(set), entry);
    T_EXPECT(ufset_get_master(set), master);
    T_ASSERT(data, val);

    ufs_master_destroy(master);
}

test_f test_create_set(void)
{
    const size_t size = 100;
    UFSMaster *master;
    UFset *set[size];
    UFSentry *entry;
    UFSentry **entries;
    UFset **t;

    int val = 0;
    size_t i;
    int data;
    size_t len;

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        entry = ufs_entry_create((void *)&val, sizeof(int), NULL);
        T_ERROR(entry == NULL);

        set[i] = ufset_create(entry, master);
        T_ERROR(set[i] == NULL);

        T_EXPECT(ufset_get_hight(set[i]), 1);
        T_EXPECT(ufset_get_num_entries(set[i]), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);

        T_EXPECT(ufset_entry_get_ufset(entry), set[i]);
        T_EXPECT(ufset_entry_get_data(entry, &data), 0);

        T_EXPECT(ufset_get_entry(set[i]), entry);
        T_EXPECT(ufset_get_master(set[i]), master);

        entries = ufset_get_entries(set[i], &len);
        T_ERROR(entries == NULL);
        T_ASSERT(len, 1);
        T_ASSERT(entries[0], entry);
        FREE(entries);
    }

    t = ufs_master_get_sets(master, &len);
    T_ERROR(t == NULL);
    T_ASSERT(len, size);
    for (i = 0; i < size; ++i)
        T_ASSERT(t[i], set[i]);

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
        entry = ufs_entry_create((void *)&val, sizeof(MyStruct *), my_struct_destroy);
        T_ERROR(entry == NULL);

        set = ufset_create(entry, master);
        T_ERROR(set == NULL);
        T_EXPECT(ufset_get_hight(set), 1);
        T_EXPECT(ufset_get_num_entries(set), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);
    }

    ufs_master_destroy_with_entries(master);
}

test_f test_delete_with_entries_without_destructor(void)
{
    UFSMaster *master;
    UFset *set;
    UFSentry *entry;

    int val;
    int i;
    int size = BIT(10);

    master = ufs_master_create();
    T_ERROR(master == NULL);
    T_EXPECT(ufs_master_get_hight(master), 0);
    T_EXPECT(ufs_master_get_num_entries(master), 0);

    for (i = 0; i < size; ++i)
    {
        val = i + 1;
        entry = ufs_entry_create((void *)&val, sizeof(int), NULL);
        T_ERROR(entry == NULL);

        set = ufset_create(entry, master);
        T_ERROR(set == NULL);
        T_EXPECT(ufset_get_hight(set), 1);
        T_EXPECT(ufset_get_num_entries(set), 1);

        T_EXPECT(ufs_master_get_hight(master), 1);
        T_EXPECT(ufs_master_get_num_entries(master), i + 1);
    }

    ufs_master_destroy_with_entries(master);
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
        entry[i] = ufs_entry_create((void *)&val, sizeof(int), NULL);
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
        entry[i] = ufs_entry_create((void *)&val, sizeof(int), NULL);
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
    TEST(test_delete_with_entries_without_destructor());
    TEST(test_union());
    TEST(test_find());
}

int main(void)
{
    TEST_INIT("UFSET")
    test();
    TEST_SUMMARY();
}
