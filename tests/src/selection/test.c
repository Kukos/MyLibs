#include <selection.h>
#include <test.h>
#include <common.h>
#include <sort.h>
#include <stdlib.h>

CMP(int)
CMP(char)
CMP(double)

typedef struct MyStruct
{
    int key;
    int a;
    char b;
    double c;
    long d;
} MyStruct;

int cmp_MyStruct(const void *a, const void *b)
{
    if ((*(MyStruct *)a).key < (*(MyStruct *)b).key)
        return -1;

    if ((*(MyStruct *)a).key > (*(MyStruct *)b).key)
        return 1;

    return 0;
}

#define CHECK_SELECTION(selection, n, k, type) \
    do { \
        type *t; \
        size_t i; \
        ssize_t index; \
        type var; \
        t = (type *)malloc(sizeof(type) * n); \
        T_ERROR(t == NULL); \
        \
        for (i = 0; i < n; ++i) \
            t[i] = (type){rand()}; \
        index = selection(t, n, sizeof(*t), k, concat(cmp_, type)); \
        T_CHECK(index != -1); \
        var = t[index]; \
        sort(t, n, concat(cmp_, type), sizeof(*t)); \
        T_CHECK(concat(cmp_, type)(&var, &t[k]) == 0); \
        FREE(t); \
    } while (0)


test_f test_selection(size_t n)
{
    CHECK_SELECTION(select_kth, n, 0, int);
    CHECK_SELECTION(select_kth, n, n / 10, int);
    CHECK_SELECTION(select_kth, n, n / 4, int);
    CHECK_SELECTION(select_kth, n, n / 2, int);
    CHECK_SELECTION(select_kth, n, n - 1, int);

    CHECK_SELECTION(select_kth, n, 0, char);
    CHECK_SELECTION(select_kth, n, n / 10, char);
    CHECK_SELECTION(select_kth, n, n / 4, char);
    CHECK_SELECTION(select_kth, n, n / 2, char);
    CHECK_SELECTION(select_kth, n, n - 1, char);

    CHECK_SELECTION(select_kth, n, 0, double);
    CHECK_SELECTION(select_kth, n, n / 10, double);
    CHECK_SELECTION(select_kth, n, n / 4, double);
    CHECK_SELECTION(select_kth, n, n / 2, double);
    CHECK_SELECTION(select_kth, n, n - 1, double);

    CHECK_SELECTION(select_kth, n, 0, MyStruct);
    CHECK_SELECTION(select_kth, n, n / 10, MyStruct);
    CHECK_SELECTION(select_kth, n, n / 4, MyStruct);
    CHECK_SELECTION(select_kth, n, n / 2, MyStruct);
    CHECK_SELECTION(select_kth, n, n - 1, MyStruct);
}

void test(void)
{
    TEST(test_selection(1));
    TEST(test_selection(2));
    TEST(test_selection(100));
    TEST(test_selection(10000));
    TEST(test_selection(50000));
    TEST(test_selection(100000));
}

int main(void)
{
    TEST_INIT("SELECTION");
    test();
    TEST_SUMMARY();
}