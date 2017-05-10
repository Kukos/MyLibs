#include <test.h>
#include <stack.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <common.h>

ARRAY_EQUAL(char)
ARRAY_EQUAL(int)
ARRAY_EQUAL(double)

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

test_f test_insert_delete(void)
{
    Stack *s;

    int *t;
    int *rt;

    int i;
    int val;

    size_t array_size = BIT(12);
    size_t rsize;

    srand(time(NULL));

    t = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t == NULL);

    for (i = 0; i < array_size; ++i)
        t[i] = rand();

    s = stack_create(sizeof(int));
    T_ERROR(s == NULL);
    T_EXPECT(stack_get_num_entries(s), 0);
    T_EXPECT(stack_get_size_of(s), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(stack_push(s, (void *)&t[i]), 0);

    T_EXPECT(stack_get_num_entries(s), array_size);

    /* delete half */
    for (i = array_size - 1; i >= array_size >> 1; --i)
    {
        T_EXPECT(stack_is_empty(s), false);
        T_EXPECT(stack_get_top(s, (void *)&val), 0);
        T_ASSERT(val, t[i]);

        T_EXPECT(stack_pop(s, (void *)&val), 0);
        T_ASSERT(val, t[i]);
    }

    T_EXPECT(stack_get_num_entries(s), array_size >> 1);
    T_EXPECT(array_equal_int(stack_get_array(s), t, array_size), true);

    T_EXPECT(stack_to_array(s, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, array_size >> 1);
    T_EXPECT(array_equal_int(t, rt, array_size >> 1), true);

    FREE(rt);
    /* insert half */
    for (i = array_size >> 1; i < array_size; ++i)
            T_EXPECT(stack_push(s, (void *)&t[i]), 0);

    T_EXPECT(stack_get_num_entries(s), array_size);
    T_EXPECT(array_equal_int(stack_get_array(s), t, array_size), true);

    T_EXPECT(stack_to_array(s, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, array_size);
    T_EXPECT(array_equal_int(t, rt, array_size), true);

    /* delete all */
    for (i = array_size - 1; i >= 0; --i)
    {
        T_EXPECT(stack_is_empty(s), false);
        T_EXPECT(stack_get_top(s, (void *)&val), 0);
        T_ASSERT(val, t[i]);

        T_EXPECT(stack_pop(s, (void *)&val), 0);
        T_ASSERT(val, t[i]);
    }

    T_EXPECT(stack_get_num_entries(s), 0);
    T_EXPECT(stack_is_empty(s), true);
    T_CHECK(stack_get_top(s, (void *)&val) != 0);
    T_CHECK(stack_pop(s, (void *)&val) != 0);

    stack_destroy(s);

    FREE(t);
    FREE(rt);
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
    T_EXPECT(array_equal_char((char *)stack_get_array(s[counter]), t1, array_size), true);

    array_reverse_char(t1, array_size);
    T_EXPECT(stack_to_array(s[counter], (void *)&rt1, &rs1), 0);
    T_ASSERT(rs1, array_size);
    T_EXPECT(array_equal_char(rt1, t1, array_size), false);
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
    T_EXPECT(array_equal_int((int *)stack_get_array(s[counter]), t2, array_size), true);

    array_reverse_int(t2, array_size);
    T_EXPECT(stack_to_array(s[counter], (void *)&rt2, &rs2), 0);
    T_ASSERT(rs2, array_size);
    T_EXPECT(array_equal_int(rt2, t2, array_size), false);
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
    T_EXPECT(array_equal_double((double *)stack_get_array(s[counter]), t3, array_size), true);

    array_reverse_double(t3, array_size);
    T_EXPECT(stack_to_array(s[counter], (void *)&rt3, &rs3), 0);
    T_ASSERT(rs3, array_size);
    T_EXPECT(array_equal_double(rt3, t3, array_size), false);
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

test_f test_convert_to_array2(void)
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
    T_EXPECT(array_equal_char((char *)stack_get_array(s[counter]), t1, array_size), true);

    T_EXPECT(stack_to_array(s[counter], (void *)&rt1, &rs1), 0);
    T_ASSERT(rs1, array_size);
    T_EXPECT(array_equal_char(rt1, t1, array_size), true);
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
    T_EXPECT(array_equal_int((int *)stack_get_array(s[counter]), t2, array_size), true);

    T_EXPECT(stack_to_array(s[counter], (void *)&rt2, &rs2), 0);
    T_ASSERT(rs2, array_size);
    T_EXPECT(array_equal_int(rt2, t2, array_size), true);
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
    T_EXPECT(array_equal_double((double *)stack_get_array(s[counter]), t3, array_size), true);

    T_EXPECT(stack_to_array(s[counter], (void *)&rt3, &rs3), 0);
    T_ASSERT(rs3, array_size);
    T_EXPECT(array_equal_double(rt3, t3, array_size), true);
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
    TEST(test_insert_delete());
    TEST(test_convert_to_array());
    TEST(test_convert_to_array2());
}

int main(void)
{
    TEST_INIT("STACK")
    test();
    TEST_SUMMARY;
}
