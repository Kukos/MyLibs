#include <test.h>
#include <stack.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_EQUAL(type) \
    bool concat(array_equal_, type)(type *t1, type *t2, size_t size) \
    { \
        size_t i; \
        for (i = 0; i < size; ++i) \
            if (t1[i] != t2[i]) \
                return false; \
        \
        return true; \
    }

ARRAY_EQUAL(char)
ARRAY_EQUAL(int)
ARRAY_EQUAL(double)

#define ARRAY_REVERSE(type) \
    void concat(array_reverse_, type)(type *t, size_t size) \
    { \
        size_t i; \
        for (i = 0; i < (size >> 1); ++i) \
            SWAP(t[i], t[size - i - 1]); \
    }

ARRAY_REVERSE(char)
ARRAY_REVERSE(int)
ARRAY_REVERSE(double)

test_f test_create(void)
{
    Stack *s;

    s = stack_create(sizeof(int));
    T_ERROR(s == NULL);
    T_EXPECT(stack_get_num_entries(s), 0);
    T_EXPECT(stack_get_size_of(s), sizeof(int));

    stack_destroy(s);

    s = stack_create(sizeof(char));
    T_ERROR(s == NULL);
    T_EXPECT(stack_get_num_entries(s), 0);
    T_EXPECT(stack_get_size_of(s), sizeof(char));

    stack_destroy(s);

    s = stack_create(sizeof(double));
    T_ERROR(s == NULL);
    T_EXPECT(stack_get_num_entries(s), 0);
    T_EXPECT(stack_get_size_of(s), sizeof(double));

    stack_destroy(s);
}

test_f test_insert(void)
{
    Stack *s[3];

    char *t1;
    int *t2;
    double *t3;

    int counter = 0;
    int i;

    size_t array_size = BIT(12);

    srand(time(NULL));

    t1 = (char *)malloc(sizeof(char) * array_size);
    T_ERROR(t1 == NULL);

    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    s[counter] = stack_create(sizeof(char));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(char));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t1[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    ++counter;

    t2 = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t2 == NULL);

    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    s[counter] = stack_create(sizeof(int));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t2[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    ++counter;

    t3 = (double *)malloc(sizeof(double) * array_size);
    T_ERROR(t3 == NULL);

    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    s[counter] = stack_create(sizeof(double));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(double));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t3[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    ++counter;

    for (i = 0; i < counter; ++i)
        stack_destroy(s[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_delete(void)
{
    Stack *s[3];

    char *t1;
    int *t2;
    double *t3;

    char val1;
    int val2;
    double val3;

    int counter = 0;
    int i;

    size_t array_size = BIT(12);

    srand(time(NULL));

    t1 = (char *)malloc(sizeof(char) * array_size);
    T_ERROR(t1 == NULL);

    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    s[counter] = stack_create(sizeof(char));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(char));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t1[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);

    for (i = array_size - 1; i >= 0; --i)
    {
        T_EXPECT(stack_is_empty(s[counter]), false);
        T_EXPECT(stack_get_top(s[counter], (void *)&val1), 0);
        T_ASSERT(val1, t1[i]);

        T_EXPECT(stack_pop(s[counter], (void *)&val1), 0);
        T_ASSERT(val1, t1[i]);
    }

    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_is_empty(s[counter]), true);
    T_CHECK(stack_get_top(s[counter], (void *)&val1) != 0);
    T_CHECK(stack_pop(s[counter], (void *)&val1) != 0);
    ++counter;

    t2 = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t2 == NULL);

    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    s[counter] = stack_create(sizeof(int));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t2[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    for (i = array_size - 1; i >= 0; --i)
    {
        T_EXPECT(stack_is_empty(s[counter]), false);
        T_EXPECT(stack_get_top(s[counter], (void *)&val2), 0);
        T_ASSERT(val2, t2[i]);

        T_EXPECT(stack_pop(s[counter], (void *)&val2), 0);
        T_ASSERT(val2, t2[i]);
    }

    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_is_empty(s[counter]), true);
    T_CHECK(stack_get_top(s[counter], (void *)&val2) != 0);
    T_CHECK(stack_pop(s[counter], (void *)&val2) != 0);
    ++counter;

    t3 = (double *)malloc(sizeof(double) * array_size);
    T_ERROR(t3 == NULL);

    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    s[counter] = stack_create(sizeof(double));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(double));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t3[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    for (i = array_size - 1; i >= 0; --i)
    {
        T_EXPECT(stack_is_empty(s[counter]), false);
        T_EXPECT(stack_get_top(s[counter], (void *)&val3), 0);
        T_ASSERT(val3, t3[i]);

        T_EXPECT(stack_pop(s[counter], (void *)&val3), 0);
        T_ASSERT(val3, t3[i]);
    }

    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_is_empty(s[counter]), true);
    T_CHECK(stack_get_top(s[counter], (void *)&val3) != 0);
    T_CHECK(stack_pop(s[counter], (void *)&val3) != 0);
    ++counter;

    for (i = 0; i < counter; ++i)
        stack_destroy(s[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_convert_to_array(void)
{
    Stack *s[3];

    char *t1;
    int *t2;
    double *t3;

    char *rt1;
    int *rt2;
    double *rt3;

    size_t rs1;
    size_t rs2;
    size_t rs3;

    int counter = 0;
    int i;

    size_t array_size = BIT(12);

    srand(time(NULL));

    t1 = (char *)malloc(sizeof(char) * array_size);
    T_ERROR(t1 == NULL);

    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    s[counter] = stack_create(sizeof(char));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(char));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t1[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    array_reverse_char(t1, array_size);
    T_EXPECT(array_equal_char((char *)stack_get_array(s[counter]), t1, array_size), 0);

    T_EXPECT(stack_to_array(s[counter], (void *)&rt1, &rs1), 0);
    T_ASSERT(rs1, array_size);
    T_EXPECT(array_equal_char(rt1, t1, array_size), 0);
    ++counter;

    t2 = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t2 == NULL);

    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    s[counter] = stack_create(sizeof(int));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t2[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    array_reverse_int(t2, array_size);
    T_EXPECT(array_equal_int((int *)stack_get_array(s[counter]), t2, array_size), 0);

    T_EXPECT(stack_to_array(s[counter], (void *)&rt2, &rs2), 0);
    T_ASSERT(rs2, array_size);
    T_EXPECT(array_equal_int(rt2, t2, array_size), 0);
    ++counter;

    t3 = (double *)malloc(sizeof(double) * array_size);
    T_ERROR(t3 == NULL);

    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    s[counter] = stack_create(sizeof(double));
    T_ERROR(s[counter] == NULL);
    T_EXPECT(stack_get_num_entries(s[counter]), 0);
    T_EXPECT(stack_get_size_of(s[counter]), sizeof(double));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s[counter], (void *)&t3[i]), 0);

    T_EXPECT(stack_get_num_entries(s[counter]), array_size);
    array_reverse_double(t3, array_size);
    T_EXPECT(array_equal_double((double *)stack_get_array(s[counter]), t3, array_size), 0);

    T_EXPECT(stack_to_array(s[counter], (void *)&rt3, &rs3), 0);
    T_ASSERT(rs3, array_size);
    T_EXPECT(array_equal_double(rt3, t3, array_size), 0);
    ++counter;

    for (i = 0; i < counter; ++i)
        stack_destroy(s[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    FREE(rt1);
    FREE(rt2);
    FREE(rt3);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert());
    TEST(test_delete());
    TEST(test_convert_to_array());
}

int main(void)
{
    TEST_INIT("STACK")
    test();
    TEST_SUMMARY;
}
