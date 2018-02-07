#include <test.h>
#include <common.h>
#include <compiler.h>
#include <ringbuffer.h>

#define DESTRUCTOR_SIMPLE_TYPE(type) \
    ___unused___ static void concat(destructor_, type)(void *data) \
    { \
        \
    } \

DESTRUCTOR_SIMPLE_TYPE(int)

ARRAY_EQUAL(int)

typedef struct MyStruct
{
    int a;
    int b;
    int c;
    int d;
} MyStruct;

___unused___ static void destructor_mystruct(void *data)
{
    MyStruct *s = *(MyStruct **)data;
    FREE(s);
}

test_f test_create(void)
{
    Ring_buffer *rb;
    const size_t max_entries = 10;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    ring_buffer_destroy(rb);

    rb = ring_buffer_create(sizeof(MyStruct *), max_entries, destructor_mystruct);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(MyStruct *));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    ring_buffer_destroy(rb);
}

test_f test_empty(void)
{
    Ring_buffer *rb;
    const size_t max_entries = 10;
    int val;
    int *t;
    size_t size;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);
    T_CHECK(ring_buffer_get_head(rb, (void *)&val) != 0);
    T_CHECK(ring_buffer_dequeue(rb, (void *)&val) != 0);
    T_CHECK(ring_buffer_to_array(rb, (void *)&t, &size) != 0);

    ring_buffer_destroy(rb);
}

test_f test_insert(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    size_t i;
    int val;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries; ++i)
    {
        val = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), true);

    ring_buffer_destroy(rb);
}

test_f test_insert_overwriting(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    size_t i;
    int val;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < (max_entries << 1); ++i)
    {
        val = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), true);

    ring_buffer_destroy(rb);
}

test_f test_destroy_with_entries(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    size_t i;
    MyStruct *val;

    rb = ring_buffer_create(sizeof(MyStruct *), max_entries, destructor_mystruct);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(MyStruct *));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries; ++i)
    {
        val = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(val == NULL);
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), true);

    ring_buffer_destroy_with_entries(rb);
}

test_f test_insert_overwriting_with_destructor(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    size_t i;
    MyStruct *val;

    rb = ring_buffer_create(sizeof(MyStruct *), max_entries, destructor_mystruct);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(MyStruct *));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < (max_entries << 1); ++i)
    {
        val = (MyStruct *)malloc(sizeof(MyStruct));
        T_ERROR(val == NULL);
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), true);

    ring_buffer_destroy_with_entries(rb);
}

test_f test_get_and_dequeue_data(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    size_t i;
    int val;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries; ++i)
    {
        val = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), true);

    i = 0;
    while (!ring_buffer_is_empty(rb))
    {
        T_EXPECT(ring_buffer_get_head(rb, (void *)&val), 0);
        T_ASSERT(i , val);
        T_EXPECT(ring_buffer_dequeue(rb, (void *)&val), 0);
        T_ASSERT(i, val);

        ++i;
    }
    T_ASSERT(i, max_entries);
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    ring_buffer_destroy(rb);
}

test_f test_convert_to_array(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    int values[max_entries];
    size_t i;

    int *t;
    size_t size;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries - 1; ++i)
    {
        values[i] = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&values[i]), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries - 1);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), false);

    T_EXPECT(ring_buffer_to_array(rb, (void *)&t, &size), 0);
    T_CHECK(t != NULL);
    T_ASSERT(size, max_entries - 1);

    T_EXPECT(array_equal_int(values, t, size), true);

    FREE(t);
    ring_buffer_destroy(rb);
}

test_f test_convert_to_array_when_full(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    int values[max_entries];
    size_t i;

    int *t;
    size_t size;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries; ++i)
    {
        values[i] = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&values[i]), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), true);

    T_EXPECT(ring_buffer_to_array(rb, (void *)&t, &size), 0);
    T_CHECK(t != NULL);
    T_ASSERT(size, max_entries);

    T_EXPECT(array_equal_int(values, t, size), true);

    FREE(t);
    ring_buffer_destroy(rb);
}

test_f test_convert_to_array_after_overwriting(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    int values[max_entries << 1];
    size_t i;

    int *t;
    size_t size;
    int val;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries << 1; ++i)
    {
        values[i] = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&values[i]), 0);
    }

    for (i = 0; i < max_entries >> 1; ++i)
        T_EXPECT(ring_buffer_dequeue(rb, (void *)&val), 0);

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries >> 1);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), false);

    T_EXPECT(ring_buffer_to_array(rb, (void *)&t, &size), 0);
    T_CHECK(t != NULL);
    T_ASSERT(size, max_entries >> 1);

    T_EXPECT(array_equal_int(values + max_entries + (max_entries >> 1), t, size), true);

    FREE(t);
    ring_buffer_destroy(rb);
}

test_f test_insert_delete(void)
{
    Ring_buffer *rb;
    const size_t max_entries = BIT(10);
    size_t i;
    int val;
    int expt_val;

    rb = ring_buffer_create(sizeof(int), max_entries, NULL);
    T_ERROR(rb == NULL)

    T_EXPECT(ring_buffer_get_data_size(rb), sizeof(int));
    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    expt_val = 0;
    for (i = 0; i < max_entries - 1; ++i)
    {
        val = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), max_entries - 1);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries >> 1; ++i)
    {
        T_EXPECT(ring_buffer_dequeue(rb, (void *)&val), 0);
        T_ASSERT(val, expt_val++);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), (max_entries >> 1) - 1);
    T_EXPECT(ring_buffer_is_empty(rb), false);
    T_EXPECT(ring_buffer_is_full(rb), false);

    for (i = 0; i < max_entries << 1; ++i)
    {
        val = i;
        T_EXPECT(ring_buffer_enqueue(rb, (void *)&val), 0);
    }

    expt_val = max_entries;
    while (!ring_buffer_is_empty(rb))
    {
        T_EXPECT(ring_buffer_dequeue(rb, (void *)&val), 0);
        T_ASSERT(val, expt_val++);
    }

    T_EXPECT(ring_buffer_get_num_entries(rb), 0);
    T_EXPECT(ring_buffer_is_empty(rb), true);
    T_EXPECT(ring_buffer_is_full(rb), false);

    ring_buffer_destroy(rb);
}

void test(void)
{
    TEST(test_create());
    TEST(test_empty());
    TEST(test_insert());
    TEST(test_insert_overwriting());
    TEST(test_destroy_with_entries());
    TEST(test_insert_overwriting_with_destructor());
    TEST(test_get_and_dequeue_data());
    TEST(test_insert_delete());
    TEST(test_convert_to_array());
    TEST(test_convert_to_array_when_full());
    TEST(test_convert_to_array_after_overwriting());
}

int main(void)
{
    TEST_INIT("RING BUFFER");
    test();
    TEST_SUMMARY();
}