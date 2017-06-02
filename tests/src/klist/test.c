#include <test.h>
#include <klist.h>

typedef struct MyStruct
{
    int a;
    char b;
    char c;
    char d;
    char e;
    long f;

    KList list;

}MyStruct;

test_f test_init(void)
{
    KLIST_MASTER(master);
    KList_master *master2;
    MyStruct s;

    (void)s;

    T_ASSERT(klist_get_head(&master), NULL);
    T_ASSERT(klist_get_tail(&master), NULL);
    T_ASSERT(klist_get_num_entries(&master), 0);

    master2 = klist_master_create();
    T_ERROR(master2 == NULL);

    T_ASSERT(klist_get_head(master2), NULL);
    T_ASSERT(klist_get_tail(master2), NULL);
    T_ASSERT(klist_get_num_entries(master2), 0);

    klist_master_destroy(master2);
}

test_f test_insert(void)
{
#define N   100
    KLIST_MASTER(master);
    KLIST_MASTER(master2);

    MyStruct s[N];
    MyStruct s2[N];

    MyStruct *str;
    KList *ptr;
    int i;

    /* INSERT LAST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
    }

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
    }

    /* INSERT FIRST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s2[i].list);
        s2[i].a = i + 1;
        T_EXPECT(klist_insert_first(&master2, &s2[i].list), 0);
    }

    klist_for_each(ptr, &master2, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
    }

    klist_for_each_prev(ptr, &master2, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
    }
#undef N
}

test_f test_insert_pos(void)
{
#define N 10

    int pos[N] = {0, 1, 0, 2, 3, 0, 2, 1, 5, 7};
    int t[N][N] = { {1},
                    {1, 2},
                    {3, 1, 2},
                    {3, 1, 4, 2},
                    {3, 1, 4, 5, 2},
                    {6, 3, 1, 4, 5, 2},
                    {6, 3, 7, 1, 4, 5, 2},
                    {6, 8, 3, 7, 1, 4, 5, 2},
                    {6, 8, 3, 7, 1, 9, 4, 5, 2},
                    {6, 8, 3, 7, 1, 9, 4, 10, 5, 2}
                  };

    KLIST_MASTER(master);
    MyStruct s[N];

    MyStruct *str;
    KList *ptr;
    int i;
    int j;

    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_pos(&master, pos[i], &s[i].list), 0);

        klist_for_each(ptr, &master, j)
        {
            str = klist_entry(ptr, typeof(MyStruct), list);
            T_ASSERT(str->a, t[i][j]);
        }
    }


#undef N
}

test_f test_insert_after_before(void)
{
#define N   15
#define M   10

#define INSERT_AFTER(node, where, val) \
    do { \
        KLIST_INIT(&(node)->list); \
        (node)->a = (val); \
        T_EXPECT(klist_insert_after(where, &(node)->list), 0); \
    } while (0)

#define INSERT_BEFORE(node, where, val) \
    do { \
        KLIST_INIT(&(node)->list); \
        (node)->a = (val); \
        T_EXPECT(klist_insert_before(where, &(node)->list), 0); \
    } while (0)

    int t[N] = {12, 1, 2, 3, 13, 4, 5, 14, 6, 15, 7, 8, 9, 10, 11};

    KLIST_MASTER(master);
    KList *lists[N];

    MyStruct s[N];

    MyStruct *str;
    KList *ptr;
    int i;

    /* INSERT LAST */
    for (i = 0; i < M; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
        lists[i] = &s[i].list;
    }

    INSERT_AFTER(&(s[i]), lists[9], i + 1);
    ++i;

    INSERT_BEFORE(&(s[i]), lists[0], i + 1);
    ++i;

    INSERT_AFTER(&(s[i]), lists[2], i + 1);
    ++i;

    INSERT_AFTER(&(s[i]), lists[4], i + 1);
    ++i;

    INSERT_BEFORE(&(s[i]), lists[6], i + 1);
    ++i;

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, t[i]);
    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, t[N - i - 1]);
    }

#undef N
#undef M
#undef INSERT_BEFORE
#undef INSERT_AFTER
}

test_f test_get_pos(void)
{
#define N   100
    KLIST_MASTER(master);

    MyStruct s[N];

    MyStruct *str;
    KList *ptr;
    int i;

    /* INSERT LAST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
    }

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
        T_EXPECT(klist_get_pos(&master, i), ptr);

    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
        T_EXPECT(klist_get_pos(&master, N - i - 1), ptr);
    }

#undef N
}

test_f test_delete(void)
{
#define N   100
    KLIST_MASTER(master);
    MyStruct s[N];

    MyStruct *str;
    KList *ptr;
    int i;
    int j;
    int cnt;

    /* INSERT LAST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
    }

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
    }

    /* DELETE LAST */
    for (j = 0; j < N; ++j)
    {
        T_EXPECT(klist_delete_last(&master), 0);
        for (i = 0; i < N - j; ++i)
        {
            klist_for_each(ptr, &master, cnt)
            {
                str = klist_entry(ptr, typeof(MyStruct), list);
                T_ASSERT(str->a, cnt + 1);
            }

            klist_for_each_prev(ptr, &master, cnt)
            {
                str = klist_entry(ptr, typeof(MyStruct), list);
                T_ASSERT(str->a, N - cnt - j - 1);
            }
        }
    }

    T_EXPECT(klist_get_head(&master), NULL);
    T_EXPECT(klist_get_tail(&master), NULL);
    T_EXPECT(klist_get_num_entries(&master), 0);

    /* INSERT LAST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
    }

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
    }

    /* DELETE FIRST */
    for (j = 0; j < N; ++j)
    {
        T_EXPECT(klist_delete_first(&master), 0);
        for (i = 0; i < N - j; ++i)
        {
            klist_for_each(ptr, &master, cnt)
            {
                str = klist_entry(ptr, typeof(MyStruct), list);
                T_ASSERT(str->a, cnt + 2 + j);
            }

            klist_for_each_prev(ptr, &master, cnt)
            {
                str = klist_entry(ptr, typeof(MyStruct), list);
                T_ASSERT(str->a, N - cnt);
            }
        }
    }

    T_EXPECT(klist_get_head(&master), NULL);
    T_EXPECT(klist_get_tail(&master), NULL);
    T_EXPECT(klist_get_num_entries(&master), 0);

#undef N
}

test_f test_delete_pos(void)
{
#define N 9

    int pos[N] = {0, 1, 6, 3, 2, 0, 1, 1, 0};
    int t[N][N] = { {2, 3, 4, 5, 6, 7, 8, 9},
                    {2, 4, 5, 6, 7, 8, 9},
                    {2, 4, 5, 6, 7, 8},
                    {2, 4, 5, 7, 8},
                    {2, 4, 7, 8},
                    {4, 7, 8},
                    {4, 8},
                    {4},
                    {-1}
                  };

    KLIST_MASTER(master);
    MyStruct s[N];

    MyStruct *str;
    KList *ptr;
    int i;
    int j;

    /* INSERT LAST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
    }

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
    }

    for (i = 0; i < N; ++i)
    {
        T_EXPECT(klist_delete_pos(&master, pos[i]), 0);

        klist_for_each(ptr, &master, j)
        {
            str = klist_entry(ptr, typeof(MyStruct), list);
            T_ASSERT(str->a, t[i][j]);
        }

        klist_for_each_prev(ptr, &master, j)
        {
            str = klist_entry(ptr, typeof(MyStruct), list);
            T_ASSERT(str->a, t[i][N - j - 2 - i]);
        }
    }

    T_EXPECT(klist_get_head(&master), NULL);
    T_EXPECT(klist_get_tail(&master), NULL);
    T_EXPECT(klist_get_num_entries(&master), 0);

#undef N
}

test_f test_delete_node(void)
{
#define N 9

    int pos[N] = {0, 8, 5, 4, 2, 7, 1, 3, 6};
    int t[N][N] = { {2, 3, 4, 5, 6, 7, 8, 9},
                    {2, 3, 4, 5, 6, 7, 8},
                    {2, 3, 4, 5, 7, 8},
                    {2, 3, 4, 7, 8},
                    {2, 4, 7, 8},
                    {2, 4, 7},
                    {4, 7},
                    {7},
                    {-1}
                  };

    KLIST_MASTER(master);
    MyStruct s[N];
    KList *lists[N];

    MyStruct *str;
    KList *ptr;
    int i;
    int j;

    /* INSERT LAST */
    for (i = 0; i < N; ++i)
    {
        KLIST_INIT(&s[i].list);
        s[i].a = i + 1;
        T_EXPECT(klist_insert_last(&master, &s[i].list), 0);
        lists[i] = &s[i].list;
    }

    klist_for_each(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, i + 1);
    }

    klist_for_each_prev(ptr, &master, i)
    {
        str = klist_entry(ptr, typeof(MyStruct), list);
        T_ASSERT(str->a, N - i);
    }

    for (i = 0; i < N; ++i)
    {
        T_EXPECT(klist_delete_node(lists[pos[i]]), 0);

        klist_for_each(ptr, &master, j)
        {
            str = klist_entry(ptr, typeof(MyStruct), list);
            T_ASSERT(str->a, t[i][j]);
        }

        klist_for_each_prev(ptr, &master, j)
        {
            str = klist_entry(ptr, typeof(MyStruct), list);
            T_ASSERT(str->a, t[i][N - j - 2 - i]);
        }
    }

    T_EXPECT(klist_get_head(&master), NULL);
    T_EXPECT(klist_get_tail(&master), NULL);
    T_EXPECT(klist_get_num_entries(&master), 0);

#undef N
}

void test(void)
{
    TEST(test_init());
    TEST(test_insert());
    TEST(test_insert_pos());
    TEST(test_insert_after_before());
    TEST(test_get_pos());
    TEST(test_delete());
    TEST(test_delete_pos());
    TEST(test_delete_node());
}

int main(void)
{
    TEST_INIT("KLIST");
    test();
    TEST_SUMMARY;
}