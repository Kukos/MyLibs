#include <search.h>
#include <test.h>
#include <common.h>

CMP(int)
CMP(char)

test_f test_find_first_unsorted(void)
{
    int t1[] = {1, 9, 8, 5, 3, 5, 2, 1, 4, 7, 7, 3};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {0, 6, 4, 8, 3, -1, 9, 2, 1};

    char t2[] = "aBzCDDfguC";
    char f2[] = "abcdefABCDEF";
    ssize_t pos2[] = {0, -1, -1, -1, -1, 6, -1, 1, 3, 4, -1, -1};

    size_t i;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
        T_EXPECT(find_first_unsorted((void *)(f1 + i), t1, ARRAY_SIZE(t1), cmp_int, sizeof(int)), pos1[i]);

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
        T_EXPECT(find_first_unsorted((void *)(f2 + i), t2, ARRAY_SIZE(t2), cmp_char, sizeof(char)), pos2[i]);
}

test_f test_find_last_unsorted(void)
{
    int t1[] = {1, 9, 8, 5, 3, 5, 2, 1, 4, 7, 7, 3};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {7, 6, 11, 8, 5, -1, 10, 2, 1};

    char t2[] = "aBzCDDfguC";
    char f2[] = "abcdefABCDEF";
    ssize_t pos2[] = {0, -1, -1, -1, -1, 6, -1, 1, 9, 5, -1, -1};

    size_t i;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
        T_EXPECT(find_last_unsorted((void *)(f1 + i), t1, ARRAY_SIZE(t1), cmp_int, sizeof(int)), pos1[i]);

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
        T_EXPECT(find_last_unsorted((void *)(f2 + i), t2, ARRAY_SIZE(t2), cmp_char, sizeof(char)), pos2[i]);
}

test_f test_find_first_sorted(void)
{
    int t1[] = {1, 1, 1, 2, 2, 2, 3, 4, 5, 7, 7, 7, 9, 9, 9, 9, 9};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {0, 3, 6, 7, 8, -1, 9, -1, 12};

    char t2[] = "aaabbbbcfffgggghijklmmmmmnnnn";
    char f2[] = "abcdefghijklmn";
    ssize_t pos2[] = {0, 3, 7, -1, -1, 8, 11, 15, 16, 17, 18, 19, 20, 25};

    size_t i;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
        T_EXPECT(find_first_sorted((void *)(f1 + i), t1, ARRAY_SIZE(t1), cmp_int, sizeof(int)), pos1[i]);

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
        T_EXPECT(find_first_sorted((void *)(f2 + i), t2, ARRAY_SIZE(t2) - 1, cmp_char, sizeof(char)), pos2[i]);
}

test_f test_find_last_sorted(void)
{
    int t1[] = {1, 1, 1, 2, 2, 2, 3, 4, 5, 7, 7, 7, 9, 9, 9, 9, 9};
    int f1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    ssize_t pos1[] = {2, 5, 6, 7, 8, -1, 11, -1, 16};

    char t2[] = "aaabbbbcfffgggghijklmmmmmnnnn";
    char f2[] = "abcdefghijklmn";
    ssize_t pos2[] = {2, 6, 7, -1, -1, 10, 14, 15, 16, 17, 18, 19, 24, 28};

    size_t i;

    for (i = 0; i < ARRAY_SIZE(f1); ++i)
        T_EXPECT(find_last_sorted((void *)(f1 + i), t1, ARRAY_SIZE(t1), cmp_int, sizeof(int)), pos1[i]);

    for (i = 0; i < ARRAY_SIZE(f2) - 1; ++i)
        T_EXPECT(find_last_sorted((void *)(f2 + i), t2, ARRAY_SIZE(t2) - 1, cmp_char, sizeof(char)), pos2[i]);
}


test_f test_find_empty_array(void)
{
    int t[] = {0};
    int key = 5;

    T_EXPECT(find_first_unsorted((void *)&key, t, 0, cmp_int, sizeof(int)), -1);
    T_EXPECT(find_last_unsorted((void *)&key, t, 0, cmp_int, sizeof(int)), -1);
    T_EXPECT(find_first_sorted((void *)&key, t, 0, cmp_int, sizeof(int)), -1);
    T_EXPECT(find_last_sorted((void *)&key, t, 0, cmp_int, sizeof(int)), -1);
}

void test(void)
{
    TEST(test_find_first_unsorted());
    TEST(test_find_last_unsorted());
    TEST(test_find_first_sorted());
    TEST(test_find_last_sorted());
    TEST(test_find_empty_array());
}

int main(void)
{
    TEST_INIT("SEARCH");
    test();
    TEST_SUMMARY;
}