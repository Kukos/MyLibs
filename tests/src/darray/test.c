#include <test.h>
#include <darray.h>
#include <common.h>
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sort.h>

CMP(int)
CMP(double)
CMP(char)

ARRAY_EQUAL(int)
ARRAY_EQUAL(double)
ARRAY_EQUAL(char)

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

test_f test_create(void)
{
    Darray *da[12];

    size_t init_size = BIT(5);
    int i;
    int counter = 0;

    /********** INIT_SIZE = init_size **********/
    /***** SORTED *****/
    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** UNSORTED *****/
    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    /********* INIT_SIZE = 0 **********/
    /***** SORTED *****/
    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** UNSORTED *****/
    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);
    ++counter;

    for (i = 0; i < counter; ++i)
        darray_destroy(da[i]);
}

test_f test_insert(void)
{
    Darray *da[12];

    char *t1;
    int *t2;
    double *t3;

    size_t init_size = BIT(4);
    size_t array_size = BIT(12);

    int i;
    int counter = 0;

    srand(time(NULL));

    /********** INIT_SIZE = init_size **********/
    /***** t1 UNSORTED *****/
    t1 = (char *)malloc(sizeof(char) * array_size);
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 UNSORTED *****/
    t2 = (int *)malloc(sizeof(int) * array_size);
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 UNSORTED *****/
    t3 = (double *)malloc(sizeof(double) * array_size);
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /********** INIT_SIZE = 0 **********/
    /***** t1 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /********** INIT_SIZE = init_size **********/
    /***** t1 SORTED*****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT(sort(t1, array_size, cmp_char, sizeof(char)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT(sort(t2, array_size, cmp_int, sizeof(int)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT(sort(t3, array_size, cmp_double, sizeof(double)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /********** INIT_SIZE = 0 **********/
    /***** t1 SORTED*****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT(sort(t1, array_size, cmp_char, sizeof(char)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT(sort(t2, array_size, cmp_int, sizeof(int)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT(sort(t3, array_size, cmp_double, sizeof(double)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    for (i = 0; i < counter; ++i)
        darray_destroy(da[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_insert_pos(void)
{
    Darray *da;

    int t[] = {2, 3, 5, 7, 11, 13, 17};
    int pos[] = {0, 1, 0, 2, 1, 3, 4};
    int t_exp[] = {5, 11, 2, 13, 17, 7, 3};

    size_t array_size = ARRAY_SIZE(t);

    int i;

    da = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* expected error */
    T_CHECK(darray_insert_pos(da, (void *)&t[0], 0) != 0);

    darray_destroy(da);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert_pos(da, (void *)&t[i], (size_t)pos[i]), 0);

    T_EXPECT(darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int((int *)darray_get_array(da), t_exp, array_size), true);

    darray_destroy(da);
}

test_f test_delete(void)
{
    Darray *da[12];

    char *t1;
    int *t2;
    double *t3;

    size_t init_size = BIT(4);
    size_t array_size = BIT(12);

    int i;
    int counter = 0;

    srand(time(NULL));

    /********** INIT_SIZE = init_size **********/
    /***** t1 UNSORTED *****/
    t1 = (char *)malloc(sizeof(char) * array_size);
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t2 UNSORTED *****/
    t2 = (int *)malloc(sizeof(int) * array_size);
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t3 UNSORTED *****/
    t3 = (double *)malloc(sizeof(double) * array_size);
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /********** INIT_SIZE = 0 **********/
    /***** t1 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t2 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t3 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /********** INIT_SIZE = init_size **********/
    /***** t1 SORTED*****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT(sort(t1, array_size, cmp_char, sizeof(char)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t2 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT(sort(t2, array_size, cmp_int, sizeof(int)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t3 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT(sort(t3, array_size, cmp_double, sizeof(double)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /********** INIT_SIZE = 0 **********/
    /***** t1 SORTED*****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT(sort(t1, array_size, cmp_char, sizeof(char)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t2 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT(sort(t2, array_size, cmp_int, sizeof(int)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    /***** t3 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT(sort(t3, array_size, cmp_double, sizeof(double)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* delete all */
    for (i = 0; i < (array_size + 1) >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);
    ++counter;

    for (i = 0; i < counter; ++i)
        darray_destroy(da[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_delete_pos(void)
{
    Darray *da;

    int t[] = {2, 3, 5, 7, 11, 13, 17};
    int t2[] = {17, 13, 11, 3, 7, 5, 2};

    int pos[] = {3, 1, 0, 3, 1, 1, 0};
    int t_exp[ARRAY_SIZE(t)][ARRAY_SIZE(t)] = { {2, 3, 5, 11, 13, 17},
                                                {2, 5, 11, 13, 17},
                                                {5, 11, 13, 17},
                                                {5, 11, 13},
                                                {5, 13},
                                                {5},
                                                {-1}
                                              };

    size_t array_size = ARRAY_SIZE(t);

    int i;

    /* unsorted */
    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    for (i = 0; i < array_size; ++i)
    {

        T_EXPECT(darray_delete_pos(da, (size_t)pos[i]), 0);
        T_EXPECT(darray_get_num_entries(da), array_size - i - 1);
        T_EXPECT(array_equal_int((int *)darray_get_array(da), t_exp[i], array_size - i - 1), true);
    }

    darray_destroy(da);

    /* sorted */
    da = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t2[i]), 0);

    for (i = 0; i < array_size; ++i)
    {
        T_EXPECT(darray_delete_pos(da, (size_t)pos[i]), 0);
        T_EXPECT(darray_get_num_entries(da), array_size - i - 1);
        T_EXPECT(array_equal_int((int *)darray_get_array(da), t_exp[i], array_size - i - 1), true);
    }

    darray_destroy(da);
}

test_f test_insert_delete(void)
{
    Darray *da[12];

    char *t1;
    int *t2;
    double *t3;

    size_t init_size = BIT(4);
    size_t array_size = BIT(12);

    int i;
    int counter = 0;

    srand(time(NULL));

    /********** INIT_SIZE = init_size **********/
    /***** t1 UNSORTED *****/
    t1 = (char *)malloc(sizeof(char) * array_size);
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 UNSORTED *****/
    t2 = (int *)malloc(sizeof(int) * array_size);
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 UNSORTED *****/
    t3 = (double *)malloc(sizeof(double) * array_size);
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /********** INIT_SIZE = 0 **********/
    /***** t1 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 UNSORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_UNSORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /********** INIT_SIZE = init_size **********/
    /***** t1 SORTED*****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT(sort(t1, array_size, cmp_char, sizeof(char)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT(sort(t2, array_size, cmp_int, sizeof(int)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, init_size, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT(sort(t3, array_size, cmp_double, sizeof(double)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /********** INIT_SIZE = 0 **********/
    /***** t1 SORTED*****/
    for (i = 0; i < array_size; ++i)
        t1[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(char), cmp_char);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(char));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT(sort(t1, array_size, cmp_char, sizeof(char)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t1[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_char(t1, (char *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t2 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t2[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(int));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT(sort(t2, array_size, cmp_int, sizeof(int)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t2[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    /***** t3 SORTED *****/
    for (i = 0; i < array_size; ++i)
        t3[i] = rand();

    da[counter] = darray_create(DARRAY_SORTED, 0, sizeof(double), cmp_double);
    T_ERROR(da[counter] == NULL);
    T_EXPECT(darray_get_type(da[counter]), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da[counter]), sizeof(double));
    T_EXPECT(darray_get_num_entries(da[counter]), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT(sort(t3, array_size, cmp_double, sizeof(double)), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete half */
    for (i = 0; i < array_size >> 1; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size >> 1);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size >> 1), true);

    /* insert half again */
    for (i = array_size >> 1; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);

    /* delete all */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_delete(da[counter]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), 0);

    /* insert all again */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da[counter], (void *)&t3[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da[counter]), array_size);
    T_EXPECT(array_equal_double(t3, (double *)darray_get_array(da[counter]), array_size), true);
    ++counter;

    for (i = 0; i < counter; ++i)
        darray_destroy(da[i]);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_sort(void)
{
    Darray *da;

    int *t;

    size_t array_size = BIT(10);
    int i;

    srand(time(NULL));

    t = malloc(sizeof(int) * array_size);
    T_ERROR(t == NULL);

    for (i = 0; i < array_size; ++i)
            t[i] = rand();

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);

    T_EXPECT(sort(t, array_size, cmp_int, sizeof(int)), 0);
    T_EXPECT(darray_sort(da), 0);

    T_EXPECT(array_equal_int(t, (int *)darray_get_array(da), array_size), true);

    FREE(t);

    darray_destroy(da);
}

test_f test_search_first(void)
{
    Darray *da;

    int t[] = {2, 2, 4, 3, 2, 6, 4, 1, 9, 2, 5, 3};
    int t2[] = {1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 9};
    int s[] = {2, 4, 0, 6, 9};
    int expt[] = {0, 2, -1, 5, 8};
    int expt2[] = {1, 7, -1, 10, 11};

    int i;
    int val;

    size_t array_size = ARRAY_SIZE(t);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t, (int *)darray_get_array(da), array_size), true);

    /* search */
    for (i = 0; i < ARRAY_SIZE(expt); ++i)
    {
        T_EXPECT(darray_search_first(da, (void *)&s[i], NULL), (ssize_t)expt[i]);
        T_EXPECT(darray_search_first(da, (void *)&s[i], (void *)&val), (ssize_t)expt[i]);
        if (expt[i] != -1)
            T_ASSERT(val, s[i]);
    }

    darray_destroy(da);

    da = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da), array_size), true);

    /* search */
    for (i = 0; i < ARRAY_SIZE(expt2); ++i)
    {
        T_EXPECT(darray_search_first(da, (void *)&s[i], NULL), (ssize_t)expt2[i]);
        T_EXPECT(darray_search_first(da, (void *)&s[i], (void *)&val), (ssize_t)expt2[i]);
        if (expt2[i] != -1)
            T_ASSERT(val, s[i]);
    }

    darray_destroy(da);
}

test_f test_search_last(void)
{
    Darray *da;

    int t[] = {2, 2, 4, 3, 2, 6, 4, 1, 9, 2, 5, 3};
    int t2[] = {1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 9};
    int s[] = {2, 4, 0, 6, 9};
    int expt[] = {9, 6, -1, 5, 8};
    int expt2[] = {4, 8, -1, 10, 11};

    int i;
    int val;

    size_t array_size = ARRAY_SIZE(t);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t, (int *)darray_get_array(da), array_size), true);

    /* search */
    for (i = 0; i < ARRAY_SIZE(expt); ++i)
    {
        T_EXPECT(darray_search_last(da, (void *)&s[i], NULL), (ssize_t)expt[i]);
        T_EXPECT(darray_search_last(da, (void *)&s[i], (void *)&val), (ssize_t)expt[i]);
        if (expt[i] != -1)
            T_ASSERT(val, s[i]);
    }

    darray_destroy(da);

    da = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da), array_size), true);

    /* search */
    for (i = 0; i < ARRAY_SIZE(expt2); ++i)
    {
        T_EXPECT(darray_search_last(da, (void *)&s[i], NULL), (ssize_t)expt2[i]);
        T_EXPECT(darray_search_last(da, (void *)&s[i], (void *)&val), (ssize_t)expt2[i]);
        if (expt2[i] != -1)
            T_ASSERT(val, s[i]);
    }

    darray_destroy(da);
}

test_f test_min(void)
{
    Darray *da;

    int t[] = {2, 2, 4, 3, 2, 6, 4, 1, 9, 2, 5, 3};
    int t2[] = {1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 9};

    int min = 1;
    int min_pos = 7;
    int i;
    int val;

    size_t array_size = ARRAY_SIZE(t);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t, (int *)darray_get_array(da), array_size), true);

    /* get min */
    T_EXPECT(darray_min(da, NULL), min_pos);
    T_EXPECT(darray_min(da, (void *)&val), min_pos);
    T_ASSERT(val, min);

    darray_destroy(da);

    da = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da), array_size), true);

    /* get min */
    T_EXPECT(darray_min(da, NULL), 0);
    T_EXPECT(darray_min(da, (void *)&val), 0);
    T_ASSERT(val, min);

    darray_destroy(da);
}

test_f test_max(void)
{
    Darray *da;

    int t[] = {2, 2, 4, 3, 2, 6, 4, 1, 9, 2, 5, 3};
    int t2[] = {1, 2, 2, 2, 2, 3, 3, 4, 4, 5, 6, 9};

    int max = 9;
    int max_pos = 8;
    int i;
    int val;

    size_t array_size = ARRAY_SIZE(t);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t, (int *)darray_get_array(da), array_size), true);

    /* get max */
    T_EXPECT(darray_max(da, NULL), max_pos);
    T_EXPECT(darray_max(da, (void *)&val), max_pos);
    T_ASSERT(val, max);

    darray_destroy(da);

    da = darray_create(DARRAY_SORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_SORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t2, (int *)darray_get_array(da), array_size), true);

    /* get max */
    T_EXPECT(darray_max(da, NULL), array_size - 1);
    T_EXPECT(darray_max(da, (void *)&val), array_size - 1);
    T_ASSERT(val, max);

    darray_destroy(da);
}

test_f test_for_each(void)
{
    Darray *da;

    int t[] = {2, 2, 4, 3, 2, 6, 4, 1, 9, 2, 5, 3};

    int *dptr;
    int i;
    int val;
    int *ptr;

    size_t array_size = ARRAY_SIZE(t);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(int), cmp_int);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(int));
    T_EXPECT(darray_get_num_entries(da), 0);

    /* insert */
    for (i = 0; i < array_size; ++i)
        T_EXPECT(darray_insert(da, (void *)&t[i]), 0);

    T_EXPECT((size_t)darray_get_num_entries(da), array_size);
    T_EXPECT(array_equal_int(t, (int *)darray_get_array(da), array_size), true);

    i = 0;
    dptr = (int *)darray_get_array(da);

    for_each(da, Darray, ptr, val)
    {
        T_ASSERT(val, t[i]);
        T_ASSERT(ptr, dptr);

        ++i;
        ++dptr;
    }

    i = array_size - 1;
    dptr = (int *)darray_get_array(da) + array_size - 1;

    for_each_prev(da, Darray, ptr, val)
    {
        T_ASSERT(val, t[i]);
        T_ASSERT(ptr, dptr);

        --i;
        --dptr;
    }

    i = 0;
    for_each_data(da, Darray, val)
    {
        T_ASSERT(val, t[i]);
        ++i;
    }

    i = array_size - 1;
    for_each_data_prev(da, Darray, val)
    {
        T_ASSERT(val, t[i]);
        --i;
    }

    dptr = (int *)darray_get_array(da);
    for_each_node(da, Darray, ptr)
    {
        T_ASSERT(ptr, dptr);
        ++dptr;
    }

    dptr = (int *)darray_get_array(da) + array_size - 1;
    for_each_node_prev(da, Darray, ptr)
    {
        T_ASSERT(ptr, dptr);
        --dptr;
    }

    darray_destroy(da);

}

test_f test_destroy_with_entries(void)
{
    MyStruct *s;
    Darray *da;

    int i;
    size_t size = BIT(10);

    da = darray_create(DARRAY_UNSORTED, 0, sizeof(MyStruct *), NULL);
    T_ERROR(da == NULL);
    T_EXPECT(darray_get_type(da), DARRAY_UNSORTED);
    T_EXPECT(darray_get_size_of(da), sizeof(MyStruct *));
    T_EXPECT(darray_get_num_entries(da), 0);

    for (i = 0; i < size; ++i)
    {
        s = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(s == NULL);

        T_EXPECT(darray_insert(da, (void *)&s), 0);
    }

    darray_destroy_with_entries(da, my_struct_destroy);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert());
    TEST(test_insert_pos());
    TEST(test_delete());
    TEST(test_delete_pos());
    TEST(test_insert_delete());
    TEST(test_sort());
    TEST(test_search_first());
    TEST(test_search_last());
    TEST(test_min());
    TEST(test_max());
    TEST(test_for_each());
    TEST(test_destroy_with_entries());
}

int main(void)
{
    TEST_INIT("DARRAY");
    test();
    TEST_SUMMARY;
}
