#include <test.h>
#include <fifo.h>
#include <stdlib.h>
#include <time.h>
#include <common.h>

typedef struct MyStruct
{
    int a;
    int b;
    int c;
    int d;
}MyStruct;

void my_struct_destroy(void *s)
{
    MyStruct *ms = *(MyStruct **)s;
    FREE(ms);
}

ARRAY_EQUAL(char)
ARRAY_EQUAL(int)
ARRAY_EQUAL(double)

test_f test_create(void)
{
    Fifo *q;

    q = fifo_create(sizeof(int));
    T_ERROR(q == NULL);
    T_EXPECT(fifo_get_num_entries(q), 0);
    T_EXPECT(fifo_get_size_of(q), sizeof(int));

    fifo_destroy(q);

    q = fifo_create(sizeof(char));
    T_ERROR(q == NULL);
    T_EXPECT(fifo_get_num_entries(q), 0);
    T_EXPECT(fifo_get_size_of(q), sizeof(char));

    fifo_destroy(q);

    q = fifo_create(sizeof(double));
    T_ERROR(q == NULL);
    T_EXPECT(fifo_get_num_entries(q), 0);
    T_EXPECT(fifo_get_size_of(q), sizeof(double));

    fifo_destroy(q);
}

test_f test_insert(void)
{
    Fifo *q[3];

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

    q[counter] = fifo_create(sizeof(char));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(char));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t1[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    ++counter;

    t2 = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t2 == NULL);

    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    q[counter] = fifo_create(sizeof(int));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t2[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    ++counter;

    t3 = (double *)malloc(sizeof(double) * array_size);
    T_ERROR(t3 == NULL);

    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    q[counter] = fifo_create(sizeof(double));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(double));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t3[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    ++counter;

    for (i = 0; i < counter; ++i)
        fifo_destroy(q[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_delete(void)
{
    Fifo *q[3];

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

    q[counter] = fifo_create(sizeof(char));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(char));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t1[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);

    for (i = 0; i < array_size; ++i)
    {
        T_EXPECT(fifo_is_empty(q[counter]), false);
        T_EXPECT(fifo_get_head(q[counter], (void *)&val1), 0);
        T_ASSERT(val1, t1[i]);

        T_EXPECT(fifo_dequeue(q[counter], (void *)&val1), 0);
        T_ASSERT(val1, t1[i]);
    }

    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_is_empty(q[counter]), true);
    T_CHECK(fifo_get_head(q[counter], (void *)&val1) != 0);
    T_CHECK(fifo_dequeue(q[counter], (void *)&val1) != 0);
    ++counter;

    t2 = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t2 == NULL);

    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    q[counter] = fifo_create(sizeof(int));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t2[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    for (i = 0; i < array_size; ++i)
    {
        T_EXPECT(fifo_is_empty(q[counter]), false);
        T_EXPECT(fifo_get_head(q[counter], (void *)&val2), 0);
        T_ASSERT(val2, t2[i]);

        T_EXPECT(fifo_dequeue(q[counter], (void *)&val2), 0);
        T_ASSERT(val2, t2[i]);
    }

    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_is_empty(q[counter]), true);
    T_CHECK(fifo_get_head(q[counter], (void *)&val2) != 0);
    T_CHECK(fifo_dequeue(q[counter], (void *)&val2) != 0);
    ++counter;

    t3 = (double *)malloc(sizeof(double) * array_size);
    T_ERROR(t3 == NULL);

    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    q[counter] = fifo_create(sizeof(double));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(double));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t3[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    for (i = 0; i < array_size; ++i)
    {
        T_EXPECT(fifo_is_empty(q[counter]), false);
        T_EXPECT(fifo_get_head(q[counter], (void *)&val3), 0);
        T_ASSERT(val3, t3[i]);

        T_EXPECT(fifo_dequeue(q[counter], (void *)&val3), 0);
        T_ASSERT(val3, t3[i]);
    }

    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_is_empty(q[counter]), true);
    T_CHECK(fifo_get_head(q[counter], (void *)&val3) != 0);
    T_CHECK(fifo_dequeue(q[counter], (void *)&val3) != 0);
    ++counter;

    for (i = 0; i < counter; ++i)
        fifo_destroy(q[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_insert_delete(void)
{
    Fifo *q;

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

    q = fifo_create(sizeof(int));
    T_ERROR(q == NULL);
    T_EXPECT(fifo_get_num_entries(q), 0);
    T_EXPECT(fifo_get_size_of(q), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q, (void *)&t[i]), 0);

    T_EXPECT(fifo_get_num_entries(q), array_size);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
    {
        T_EXPECT(fifo_is_empty(q), false);
        T_EXPECT(fifo_get_head(q, (void *)&val), 0);
        T_ASSERT(val, t[i]);

        T_EXPECT(fifo_dequeue(q, (void *)&val), 0);
        T_ASSERT(val, t[i]);
    }

    T_EXPECT(fifo_get_num_entries(q), array_size >> 1);
    T_EXPECT(fifo_to_array(q, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, array_size >> 1);
    T_EXPECT(array_equal_int(t + (array_size >> 1), rt, array_size >> 1), true);

    FREE(rt);

    /* insert half */
    for (i = 0; i < array_size >> 1; ++i)
            T_EXPECT(fifo_enqueue(q, (void *)&t[i]), 0);

    T_EXPECT(fifo_get_num_entries(q), array_size);

    T_EXPECT(fifo_to_array(q, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, array_size);

    /* expected false */
    T_EXPECT(array_equal_int(t, rt, array_size), false);

    /* delete all */
    for (i = 0; i < array_size; ++i)
    {
        T_EXPECT(fifo_is_empty(q), false);
        T_EXPECT(fifo_get_head(q, (void *)&val), 0);
        T_ASSERT(val, t[(i + (array_size >> 1)) & (array_size - 1)]);

        T_EXPECT(fifo_dequeue(q, (void *)&val), 0);
        T_ASSERT(val, t[(i + (array_size >> 1)) & (array_size - 1)]);
    }

    T_EXPECT(fifo_get_num_entries(q), 0);
    T_EXPECT(fifo_is_empty(q), true);
    T_CHECK(fifo_get_head(q, (void *)&val) != 0);
    T_CHECK(fifo_dequeue(q, (void *)&val) != 0);

    fifo_destroy(q);

    FREE(t);
    FREE(rt);
}

test_f test_convert_to_array(void)
{
    Fifo *q[3];

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

    q[counter] = fifo_create(sizeof(char));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(char));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t1[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);

    T_EXPECT(fifo_to_array(q[counter], (void *)&rt1, &rs1), 0);
    T_ASSERT(rs1, array_size);
    T_EXPECT(array_equal_char(rt1, t1, array_size), true);
    ++counter;

    t2 = (int *)malloc(sizeof(int) * array_size);
    T_ERROR(t2 == NULL);

    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    q[counter] = fifo_create(sizeof(int));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(int));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t2[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    T_EXPECT(fifo_to_array(q[counter], (void *)&rt2, &rs2), 0);
    T_ASSERT(rs2, array_size);
    T_EXPECT(array_equal_int(rt2, t2, array_size), true);
    ++counter;

    t3 = (double *)malloc(sizeof(double) * array_size);
    T_ERROR(t3 == NULL);

    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    q[counter] = fifo_create(sizeof(double));
    T_ERROR(q[counter] == NULL);
    T_EXPECT(fifo_get_num_entries(q[counter]), 0);
    T_EXPECT(fifo_get_size_of(q[counter]), sizeof(double));

    for (i = 0; i < array_size; ++i)
        T_EXPECT(fifo_enqueue(q[counter], (void *)&t3[i]), 0);

    T_EXPECT(fifo_get_num_entries(q[counter]), array_size);
    T_EXPECT(fifo_to_array(q[counter], (void *)&rt3, &rs3), 0);
    T_ASSERT(rs3, array_size);
    T_EXPECT(array_equal_double(rt3, t3, array_size), true);
    ++counter;

    for (i = 0; i < counter; ++i)
        fifo_destroy(q[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    FREE(rt1);
    FREE(rt2);
    FREE(rt3);
}

test_f test_destroy_with_entries(void)
{
    MyStruct *s;
    Fifo *q;

    int i;
    size_t size = BIT(10);

    q = fifo_create(sizeof(MyStruct *));
    T_ERROR(q == NULL);
    T_EXPECT(fifo_get_num_entries(q), 0);
    T_EXPECT(fifo_get_size_of(q), sizeof(MyStruct *));

    for (i = 0; i < size; ++i)
    {
        s = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(s == NULL);

        T_EXPECT(fifo_enqueue(q, (void *)&s), 0);
    }

    fifo_destroy_with_entries(q, my_struct_destroy);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert());
    TEST(test_delete());
    TEST(test_insert_delete());
    TEST(test_convert_to_array());
    TEST(test_destroy_with_entries());
}

int main(void)
{
    TEST_INIT("FIFO");
    test();
    TEST_SUMMARY;
}
