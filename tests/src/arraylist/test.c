#include <test.h>
#include <arraylist.h>
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

ARRAY_REVERSE(int)

ARRAY_EQUAL(int)
ARRAY_EQUAL(double)
ARRAY_EQUAL(char)

test_f test_create(void)
{
    Arraylist *al;

    al = arraylist_create(sizeof(char));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(char));
    arraylist_destroy(al);

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));
    arraylist_destroy(al);

    al = arraylist_create(sizeof(double));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(double));
    arraylist_destroy(al);
}

test_f test_insert_first(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    int i;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_first(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    array_reverse_int(t, size);
    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);

    arraylist_destroy(al);

    FREE(rt);
}

test_f test_insert_last(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    int i;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);

    arraylist_destroy(al);

    FREE(rt);
}

test_f test_insert_pos(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int pos[] = {0, 0, 1, 3, 2, 5, 3, 4, 6};
    int pt[] = {2, 3, 5, 7, 8, 1, 9, 4, 6};
    int *rt;

    int i;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_pos(al, pos[i], (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(pt, rt, size), true);

    arraylist_destroy(al);

    FREE(rt);
}

test_f test_insert_a_lot_data(void)
{
    Arraylist *al;

    char *t1;
    int *t2;
    double *t3;

    char *rt1;
    int *rt2;
    double *rt3;

    size_t size = BIT(14);
    size_t i;
    size_t rsize;

    srand(time(NULL));

    t1 = (char *)malloc(sizeof(char) * size);
    T_ERROR(t1 == NULL);

    al = arraylist_create(sizeof(char));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(char));

    for (i = 0; i < size; ++i)
        t1[i] = rand();

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t1[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt1, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_char(t1, rt1, size), true);

    arraylist_destroy(al);

    t2 = (int *)malloc(sizeof(int) * size);
    T_ERROR(t2 == NULL);

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        t2[i] = rand();

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t2[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t2, rt2, size), true);

    arraylist_destroy(al);

    t3 = (double *)malloc(sizeof(double) * size);
    T_ERROR(t3 == NULL);

    al = arraylist_create(sizeof(double));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(double));

    for (i = 0; i < size; ++i)
        t3[i] = rand();

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t3[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt3, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_double(t3, rt3, size), true);

    arraylist_destroy(al);

    FREE(t1);
    FREE(t2);
    FREE(t3);

    FREE(rt1);
    FREE(rt2);
    FREE(rt3);
}

test_f test_get_pos_data(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    int i;
    int val;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);

    for (i = 0; i < size; ++i)
    {
        T_EXPECT(arraylist_get_pos(al, i, (void *)&val), 0);
        T_ASSERT(val, t[i]);
    }

    arraylist_destroy(al);

    FREE(rt);
}

test_f test_merge(void)
{
    int t1[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int t2[] = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    int overlay[] = {1, 2, 3, 4, 5, 6, 7, 8 ,9,
                     9, 8, 7, 6, 5, 4, 3, 2, 1};
    int *rt;
    int *rt2;

    int i;
    size_t size1 = ARRAY_SIZE(t1);
    size_t size2 = ARRAY_SIZE(t2);
    size_t overlay_size = ARRAY_SIZE(overlay);
    size_t rsize;

    Arraylist *al1;
    Arraylist *al2;
    Arraylist *al3;

    al1 = arraylist_create(sizeof(int));
    T_ERROR(al1 == NULL);
    T_EXPECT(arraylist_get_num_entries(al1), 0);
    T_EXPECT(arraylist_get_size_of(al1), sizeof(int));

    al2 = arraylist_create(sizeof(int));
    T_ERROR(al2 == NULL);
    T_EXPECT(arraylist_get_num_entries(al2), 0);
    T_EXPECT(arraylist_get_size_of(al2), sizeof(int));


    al3 = arraylist_merge(al1, al2);
    T_ERROR(al3 == NULL);
    T_EXPECT(arraylist_get_num_entries(al3), 0);
    T_EXPECT(arraylist_get_size_of(al3), sizeof(int));

    arraylist_destroy(al3);

    for (i = 0; i < size1; ++i)
        T_EXPECT(arraylist_insert_last(al1, (void *)&t1[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al1), size1);

    T_EXPECT(arraylist_to_array(al1, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size1);
    T_EXPECT(array_equal_int(t1, rt, size1), true);
    FREE(rt);

    al3 = arraylist_merge(al1, al2);
    T_ERROR(al3 == NULL);
    T_EXPECT(arraylist_get_num_entries(al3), size1);
    T_EXPECT(arraylist_get_size_of(al3), sizeof(int));

    T_EXPECT(arraylist_to_array(al3, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size1);
    T_EXPECT(array_equal_int(t1, rt2, size1), true);
    FREE(rt2);

    arraylist_destroy(al3);

    for (i = 0; i < size2; ++i)
        T_EXPECT(arraylist_insert_last(al2, (void *)&t2[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al2), size2);

    T_EXPECT(arraylist_to_array(al2, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size2);
    T_EXPECT(array_equal_int(t2, rt, size2), true);
    FREE(rt);

    al3 = arraylist_merge(al1, al2);
    T_ERROR(al3 == NULL);
    T_EXPECT(arraylist_get_num_entries(al3), overlay_size);
    T_EXPECT(arraylist_get_size_of(al3), sizeof(int));

    T_EXPECT(arraylist_to_array(al3, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, overlay_size);
    T_EXPECT(array_equal_int(overlay, rt2, overlay_size), true);
    FREE(rt2);

    arraylist_destroy(al1);
    arraylist_destroy(al2);
    arraylist_destroy(al3);
}

test_f test_delete_first(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    int i;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);
    FREE(rt);

    for (i = 0; i < size - 1; ++i)
    {
        T_EXPECT(arraylist_delete_first(al), 0);
        T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, size - i - 1);
        T_EXPECT(array_equal_int(t + i + 1, rt, rsize), true);
        FREE(rt);
    }

    T_EXPECT(arraylist_delete_first(al), 0);
    T_EXPECT(arraylist_get_num_entries(al), 0);

    arraylist_destroy(al);
}

test_f test_delete_last(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    int i;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);
    FREE(rt);

    for (i = 0; i < size - 1; ++i)
    {
        T_EXPECT(arraylist_delete_last(al), 0);
        T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, size - i - 1);
        T_EXPECT(array_equal_int(t, rt, rsize), true);
        FREE(rt);
    }

    T_EXPECT(arraylist_delete_last(al), 0);
    T_EXPECT(arraylist_get_num_entries(al), 0);

    arraylist_destroy(al);
}

test_f test_delete_pos(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int pos[] = {5, 7, 3, 1, 0, 3, 2, 1, 0};
    int t_exp[ARRAY_SIZE(t)][ARRAY_SIZE(t)] = { {1, 2, 3, 4, 5, 7, 8, 9},
                                                {1, 2, 3, 4, 5, 7, 8},
                                                {1, 2, 3, 5, 7, 8},
                                                {1, 3, 5, 7, 8},
                                                {3, 5, 7, 8},
                                                {3, 5, 7},
                                                {3, 5},
                                                {3},
                                                {-1}
                                              };
    int *rt;

    int i;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);
    FREE(rt);

    for (i = 0; i < size - 1; ++i)
    {
        T_EXPECT(arraylist_delete_pos(al, pos[i]), 0);
        T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
        T_ASSERT(rsize, size - i - 1);
        T_EXPECT(array_equal_int(t_exp[i], rt, rsize), true);
        FREE(rt);
    }

    T_EXPECT(arraylist_delete_pos(al, pos[i]), 0);
    T_EXPECT(arraylist_get_num_entries(al), 0);

    arraylist_destroy(al);
}

test_f test_insert_delete(void)
{
    Arraylist *al;

    char *t1;
    int *t2;
    double *t3;

    char *rt1;
    int *rt2;
    double *rt3;

    size_t size = BIT(14);
    ssize_t i;
    size_t rsize;

    srand(time(NULL));

    t1 = (char *)malloc(sizeof(char) * size);
    T_ERROR(t1 == NULL);

    al = arraylist_create(sizeof(char));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(char));

    for (i = 0; i < size; ++i)
        t1[i] = rand();

    /* insert all */
    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t1[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt1, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_char(t1, rt1, size), true);
    FREE(rt1);

    /* delete half last entries */
    for (i = 0; i < size >> 1; ++i)
        T_EXPECT(arraylist_delete_last(al), 0);

    T_EXPECT(arraylist_get_num_entries(al), size >> 1);

    T_EXPECT(arraylist_to_array(al, (void *)&rt1, &rsize), 0);
    T_ASSERT(rsize, size >> 1);
    T_EXPECT(array_equal_char(t1, rt1, size >> 1), true);
    FREE(rt1)

    /* insert again */
    for (i = size >> 1; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t1[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt1, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_char(t1, rt1, size), true);
    FREE(rt1);

    /* delete half first entries */
    for (i = 0; i < size >> 1; ++i)
        T_EXPECT(arraylist_delete_first(al), 0);

    T_EXPECT(arraylist_get_num_entries(al), size >> 1);

    T_EXPECT(arraylist_to_array(al, (void *)&rt1, &rsize), 0);
    T_ASSERT(rsize, size >> 1);
    T_EXPECT(array_equal_char(t1 + (size >> 1), rt1, size >> 1), true);
    FREE(rt1);

    /* insert again */
    for (i = (size >> 1) - 1; i >= 0; --i)
        T_EXPECT(arraylist_insert_first(al, (void *)&t1[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt1, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_char(t1, rt1, size), true);
    FREE(rt1);

    arraylist_destroy(al);

    t2 = (int *)malloc(sizeof(int) * size);
    T_ERROR(t2 == NULL);

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        t2[i] = rand();

    /* insert all */
    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t2[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t2, rt2, size), true);
    FREE(rt2);

    /* delete half last entries */
    for (i = 0; i < size >> 1; ++i)
        T_EXPECT(arraylist_delete_last(al), 0);

    T_EXPECT(arraylist_get_num_entries(al), size >> 1);

    T_EXPECT(arraylist_to_array(al, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size >> 1);
    T_EXPECT(array_equal_int(t2, rt2, size >> 1), true);
    FREE(rt2)

    /* insert again */
    for (i = size >> 1; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t2[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t2, rt2, size), true);
    FREE(rt2);

    /* delete half first entries */
    for (i = 0; i < size >> 1; ++i)
        T_EXPECT(arraylist_delete_first(al), 0);

    T_EXPECT(arraylist_get_num_entries(al), size >> 1);

    T_EXPECT(arraylist_to_array(al, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size >> 1);
    T_EXPECT(array_equal_int(t2 + (size >> 1), rt2, size >> 1), true);
    FREE(rt2);

    /* insert again */
    for (i = (size >> 1) - 1; i >= 0; --i)
        T_EXPECT(arraylist_insert_first(al, (void *)&t2[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt2, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t2, rt2, size), true);
    FREE(rt2);

    arraylist_destroy(al);

    t3 = (double *)malloc(sizeof(double) * size);
    T_ERROR(t3 == NULL);

    al = arraylist_create(sizeof(double));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(double));

    for (i = 0; i < size; ++i)
        t3[i] = rand();

    /* insert all */
    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t3[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt3, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_double(t3, rt3, size), true);
    FREE(rt3);

    /* delete half last entries */
    for (i = 0; i < size >> 1; ++i)
        T_EXPECT(arraylist_delete_last(al), 0);

    T_EXPECT(arraylist_get_num_entries(al), size >> 1);

    T_EXPECT(arraylist_to_array(al, (void *)&rt3, &rsize), 0);
    T_ASSERT(rsize, size >> 1);
    T_EXPECT(array_equal_double(t3, rt3, size >> 1), true);
    FREE(rt3)

    /* insert again */
    for (i = size >> 1; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t3[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt3, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_double(t3, rt3, size), true);
    FREE(rt3);

    /* delete half first entries */
    for (i = 0; i < size >> 1; ++i)
        T_EXPECT(arraylist_delete_first(al), 0);

    T_EXPECT(arraylist_get_num_entries(al), size >> 1);

    T_EXPECT(arraylist_to_array(al, (void *)&rt3, &rsize), 0);
    T_ASSERT(rsize, size >> 1);
    T_EXPECT(array_equal_double(t3 + (size >> 1), rt3, size >> 1), true);
    FREE(rt3);

    /* insert again */
    for (i = (size >> 1) - 1; i >= 0; --i)
        T_EXPECT(arraylist_insert_first(al, (void *)&t3[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt3, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_double(t3, rt3, size), true);
    FREE(rt3);

    arraylist_destroy(al);

    FREE(t1);
    FREE(t2);
    FREE(t3);
}

test_f test_for_each(void)
{
    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *rt;

    int i;
    int val;
    size_t size = ARRAY_SIZE(t);
    size_t rsize;

    Arraylist *al;
    Arraylist_node *node;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(int));

    for (i = 0; i < size; ++i)
        T_EXPECT(arraylist_insert_last(al, (void *)&t[i]), 0);

    T_EXPECT(arraylist_get_num_entries(al), size);

    T_EXPECT(arraylist_to_array(al, (void *)&rt, &rsize), 0);
    T_ASSERT(rsize, size);
    T_EXPECT(array_equal_int(t, rt, size), true);
    FREE(rt);

    i = 0;
    for_each(al, Arraylist, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(val, t[i]);
        T_EXPECT(arraylist_node_get_data(node, (void *)&val), 0);
        T_ASSERT(val, t[i]);
        ++i;
    }

    i = size - 1;
    for_each_prev(al, Arraylist, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(val, t[i]);
        T_EXPECT(arraylist_node_get_data(node, (void *)&val), 0);
        T_ASSERT(val, t[i]);
        --i;
    }

    i = 0;
    for_each_node(al, Arraylist, node)
    {
        T_CHECK(node != NULL);
        T_EXPECT(arraylist_node_get_data(node, (void *)&val), 0);
        T_ASSERT(val, t[i]);
        ++i;
    }

    i = size - 1;
    for_each_node_prev(al, Arraylist, node)
    {
        T_CHECK(node != NULL);
        T_EXPECT(arraylist_node_get_data(node, (void *)&val), 0);
        T_ASSERT(val, t[i]);
        --i;
    }

    i = 0;
    for_each_data(al, Arraylist, val)
    {
        T_ASSERT(val, t[i]);
        ++i;
    }

    i = size - 1;
    for_each_data_prev(al, Arraylist, val)
    {
        T_ASSERT(val, t[i]);
        --i;
    }

    arraylist_destroy(al);
}

test_f test_destroy_with_entries(void)
{
    MyStruct *s;
    Arraylist *al;

    size_t size = BIT(10);
    size_t i;

    al = arraylist_create(sizeof(MyStruct *));
    T_ERROR(al == NULL);
    T_EXPECT(arraylist_get_num_entries(al), 0);
    T_EXPECT(arraylist_get_size_of(al), sizeof(MyStruct *));

    for (i = 0; i < size; ++i)
    {
        s = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(s == NULL);

        T_EXPECT( arraylist_insert_last(al, (void *)&s), 0);
    }

    arraylist_destroy_with_entries(al, my_struct_destroy);
}

test_f test_list_empty(void)
{
    Arraylist *al;
    int val;
    int *t;
    size_t size;

    al = arraylist_create(sizeof(int));
    T_ERROR(al == NULL);

    T_CHECK(arraylist_delete_first(al) != 0);
    T_CHECK(arraylist_delete_last(al) != 0);
    T_CHECK(arraylist_delete_pos(al, 3) != 0);
    T_CHECK(arraylist_get_pos(al, 3, (void *)&val) != 0);
    T_CHECK(arraylist_to_array(al, (void *)&t, &size) != 0);

    T_EXPECT(arraylist_get_size_of(al), sizeof(int));
    T_EXPECT(arraylist_get_num_entries(al), 0);

    arraylist_destroy(al);
}

test_f test_merge_empty(void)
{
    Arraylist *al1;
    Arraylist *al2;
    Arraylist *al3;

    int val;
    int *t;
    size_t size;

    al1 = arraylist_create(sizeof(int));
    T_ERROR(al1 == NULL);

    T_CHECK(arraylist_delete_first(al1) != 0);
    T_CHECK(arraylist_delete_last(al1) != 0);
    T_CHECK(arraylist_delete_pos(al1, 3) != 0);
    T_CHECK(arraylist_get_pos(al1, 3, (void *)&val) != 0);
    T_CHECK(arraylist_to_array(al1, (void *)&t, &size) != 0);

    T_EXPECT(arraylist_get_size_of(al1), sizeof(int));
    T_EXPECT(arraylist_get_num_entries(al1), 0);

    al2 = arraylist_create(sizeof(int));
    T_ERROR(al2 == NULL);

    T_CHECK(arraylist_delete_first(al2) != 0);
    T_CHECK(arraylist_delete_last(al2) != 0);
    T_CHECK(arraylist_delete_pos(al2, 3) != 0);
    T_CHECK(arraylist_get_pos(al2, 3, (void *)&val) != 0);
    T_CHECK(arraylist_to_array(al2, (void *)&t, &size) != 0);

    T_EXPECT(arraylist_get_size_of(al2), sizeof(int));
    T_EXPECT(arraylist_get_num_entries(al2), 0);

    al3 = arraylist_merge(al1, al2);
    T_ERROR(al3 == NULL);

    T_CHECK(arraylist_delete_first(al3) != 0);
    T_CHECK(arraylist_delete_last(al3) != 0);
    T_CHECK(arraylist_delete_pos(al3, 3) != 0);
    T_CHECK(arraylist_get_pos(al3, 3, (void *)&val) != 0);
    T_CHECK(arraylist_to_array(al3, (void *)&t, &size) != 0);

    T_EXPECT(arraylist_get_size_of(al3), sizeof(int));
    T_EXPECT(arraylist_get_num_entries(al3), 0);

    arraylist_destroy(al1);
    arraylist_destroy(al2);
    arraylist_destroy(al3);
}

test_f test_ulist_framework(void)
{
    /* I have tested whole arraylist so here I test only call correct funcion in easiest way */

    UList *ulist1;
    UList *ulist2;
    UList *ulist3;

    int val = 0x12345;
    int *rt;
    int t[] = {val, val, val};
    size_t size = 0;
    size_t rsize;

    ulist1 = ulist_arraylist_create(sizeof(int));
    T_ERROR(ulist1 == NULL);
    T_EXPECT(ulist_get_size_of(ulist1), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist1), size);

    T_EXPECT(ulist_insert_first(ulist1, (void *)&val), 0);
    T_EXPECT(ulist_get_num_entries(ulist1), ++size);

    T_EXPECT(ulist_insert_last(ulist1, (void *)&val), 0);
    T_EXPECT(ulist_get_num_entries(ulist1), ++size);

    T_EXPECT(ulist_insert_pos(ulist1, 1, (void *)&val), 0);
    T_EXPECT(ulist_get_num_entries(ulist1), ++size);

    T_EXPECT(ulist_to_array(ulist1, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);

    T_EXPECT(array_equal_int(rt, t, size), true);
    FREE(rt);

    size = 0;
    ulist2 = ulist_arraylist_create(sizeof(int));
    T_ERROR(ulist2 == NULL);
    T_EXPECT(ulist_get_size_of(ulist2), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist2), size);

    T_EXPECT(ulist_insert_first(ulist2, (void *)&val), 0);
    T_EXPECT(ulist_get_num_entries(ulist2), ++size);

    T_EXPECT(ulist_insert_last(ulist2, (void *)&val), 0);
    T_EXPECT(ulist_get_num_entries(ulist2), ++size);

    T_EXPECT(ulist_insert_pos(ulist2, 1, (void *)&val), 0);
    T_EXPECT(ulist_get_num_entries(ulist2), ++size);

    T_EXPECT(ulist_to_array(ulist2, (void *)&rt, &rsize), 0);
    T_ASSERT(size, rsize);

    T_EXPECT(array_equal_int(rt, t, size), true);
    FREE(rt);

    ulist3 = ulist_merge(ulist1, ulist2);
    size = 6;
    T_ERROR(ulist3 == NULL);
    T_EXPECT(ulist_get_size_of(ulist3), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist3), size);

    T_EXPECT(ulist_delete_pos(ulist3, 1), 0);
    T_EXPECT(ulist_get_num_entries(ulist3), --size);

    T_EXPECT(ulist_delete_first(ulist3), 0);
    T_EXPECT(ulist_get_num_entries(ulist3), --size);

    T_EXPECT(ulist_delete_last(ulist3), 0);
    T_EXPECT(ulist_get_num_entries(ulist3), --size);

    ulist_destroy(ulist1);
    ulist_destroy(ulist2);
    ulist_destroy(ulist3);
}

test_f test_ulist_for_each(void)
{
    UList *ulist;

    int t[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    size_t size = ARRAY_SIZE(t);
    int i;
    void *node;
    int val;

    ulist = ulist_arraylist_create(sizeof(int));
    T_ERROR(ulist == NULL);
    T_EXPECT(ulist_get_size_of(ulist), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist), 0);

    for (i = 0; i < size; ++i)
        T_EXPECT(ulist_insert_last(ulist, (void *)&t[i]), 0);

    T_EXPECT(ulist_get_num_entries(ulist), size);

    i = 0;
    for_each_container(ulist, UList, Arraylist, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(val, t[i]);
        ++i;
    }

    i = size - 1;
    for_each_prev_container(ulist, UList, Arraylist, node, val)
    {
        T_CHECK(node != NULL);
        T_ASSERT(val, t[i]);
        --i;
    }

    for_each_node_container(ulist, UList, Arraylist, node)
    {
        T_CHECK(node != NULL);
    }

    for_each_node_prev_container(ulist, UList, Arraylist, node)
    {
        T_CHECK(node != NULL);
    }

    i = 0;
    for_each_data_container(ulist, UList, Arraylist, val)
    {
        T_ASSERT(val, t[i]);
        ++i;
    }

    i = size - 1;
    for_each_data_prev_container(ulist, UList, Arraylist, val)
    {
        T_ASSERT(val, t[i]);
        --i;
    }

    ulist_destroy(ulist);
}

test_f test_ulist_empty(void)
{
    UList *ulist;
    int val;
    int *t;
    size_t size;

    ulist = ulist_arraylist_create(sizeof(int));
    T_ERROR(ulist == NULL);

    T_CHECK(ulist_delete_first(ulist) != 0);
    T_CHECK(ulist_delete_last(ulist) != 0);
    T_CHECK(ulist_delete_pos(ulist, 3) != 0);
    T_CHECK(ulist_get_pos(ulist, 3, (void *)&val) != 0);
    T_CHECK(ulist_to_array(ulist, (void *)&t, &size) != 0);

    T_EXPECT(ulist_get_size_of(ulist), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist), 0);

    ulist_destroy(ulist);
}

test_f test_ulist_merge_empty(void)
{
    UList *ulist1;
    UList *ulist2;
    UList *ulist3;
    int val;
    int *t;
    size_t size;

    ulist1 = ulist_arraylist_create(sizeof(int));
    T_ERROR(ulist1 == NULL);

    T_CHECK(ulist_delete_first(ulist1) != 0);
    T_CHECK(ulist_delete_last(ulist1) != 0);
    T_CHECK(ulist_delete_pos(ulist1, 3) != 0);
    T_CHECK(ulist_get_pos(ulist1, 3, (void *)&val) != 0);
    T_CHECK(ulist_to_array(ulist1, (void *)&t, &size) != 0);

    T_EXPECT(ulist_get_size_of(ulist1), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist1), 0);

    ulist2 = ulist_arraylist_create(sizeof(int));
    T_ERROR(ulist2 == NULL);

    T_CHECK(ulist_delete_first(ulist2) != 0);
    T_CHECK(ulist_delete_last(ulist2) != 0);
    T_CHECK(ulist_delete_pos(ulist2, 3) != 0);
    T_CHECK(ulist_get_pos(ulist2, 3, (void *)&val) != 0);
    T_CHECK(ulist_to_array(ulist2, (void *)&t, &size) != 0);

    T_EXPECT(ulist_get_size_of(ulist2), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist2), 0);

    ulist3 = ulist_merge(ulist1, ulist2);
    T_ERROR(ulist3 == NULL);

    T_CHECK(ulist_delete_first(ulist3) != 0);
    T_CHECK(ulist_delete_last(ulist3) != 0);
    T_CHECK(ulist_delete_pos(ulist3, 3) != 0);
    T_CHECK(ulist_get_pos(ulist3, 3, (void *)&val) != 0);
    T_CHECK(ulist_to_array(ulist3, (void *)&t, &size) != 0);

    T_EXPECT(ulist_get_size_of(ulist3), sizeof(int));
    T_EXPECT(ulist_get_num_entries(ulist3), 0);

    ulist_destroy(ulist1);
    ulist_destroy(ulist2);
    ulist_destroy(ulist3);
}

void test(void)
{
    TEST(test_create());
    TEST(test_insert_first());
    TEST(test_insert_last());
    TEST(test_insert_pos());
    TEST(test_insert_a_lot_data());
    TEST(test_get_pos_data());
    TEST(test_merge());
    TEST(test_delete_first());
    TEST(test_delete_last());
    TEST(test_delete_pos());
    TEST(test_insert_delete());
    TEST(test_for_each());
    TEST(test_destroy_with_entries());
    TEST(test_list_empty());
    TEST(test_merge_empty());
    TEST(test_ulist_framework());
    TEST(test_ulist_for_each());
    TEST(test_ulist_empty());
    TEST(test_ulist_merge_empty());
}

int main(void)
{
    TEST_INIT("ARRAYLIST");
    test();
    TEST_SUMMARY;
}
