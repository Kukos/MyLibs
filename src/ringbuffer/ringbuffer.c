#include <ringbuffer.h>
#include <stdlib.h>
#include <log.h>
#include <compiler.h>
#include <common.h>
#include <generic.h>

#define RB_NEXT_HEAD_TAIL(RB, field) \
    do { \
        RB->field += RB->____data_size; \
        if (RB->field >= RB->____max_entries * RB->____data_size) \
            RB->field = 0; \
    } while (0)

#define RB_NEXT_HEAD(RB) RB_NEXT_HEAD_TAIL(RB, ____head)
#define RB_NEXT_TAIL(RB) RB_NEXT_HEAD_TAIL(RB, ____tail)

Ring_buffer *ring_buffer_create(size_t data_size, size_t max_entries, void(*destructor)(void *data))
{
    Ring_buffer *rb;

    TRACE();

    if (data_size == 0)
        ERROR("data_size == 0\n", NULL);

    if (max_entries == 0)
        ERROR("max_entries == 0\n", NULL);

    rb = (Ring_buffer *)malloc(sizeof(Ring_buffer));
    if (rb == NULL)
        ERROR("malloc error\n", NULL);

    rb->____buf = malloc(data_size * max_entries);
    if (rb->____buf == NULL)
    {
        FREE(rb);
        ERROR("malloc error\n", NULL);
    }

    rb->____head = 0;
    rb->____tail = 0;
    rb->____num_entries = 0;
    rb->____max_entries = max_entries;
    rb->____data_size = data_size;
    rb->____destructor = destructor;

    return rb;
}

void ring_buffer_destroy(Ring_buffer *rb)
{
    TRACE();

    if (rb == NULL)
        return;

    FREE(rb->____buf);
    FREE(rb);
}

void ring_buffer_destroy_with_entries(Ring_buffer *rb)
{
    BYTE *t;
    size_t i;

    TRACE();

    if (rb == NULL)
        return;

    if (rb->____destructor != NULL)
    {
        t = (BYTE *)rb->____buf;

        if (rb->____head < rb->____tail)
        {
            for (i = rb->____head; i < rb->____tail; i += rb->____data_size)
                rb->____destructor((void *)(t + i));
        }
        else
        {
            for (i = rb->____tail; i < (rb->____max_entries * rb->____data_size); i += rb->____data_size)
                rb->____destructor((void *)(t + i));

            for (i = 0; i < rb->____head; ++i)
                rb->____destructor((void *)(t + i));
        }
    }

    ring_buffer_destroy(rb);
}

int ring_buffer_enqueue(Ring_buffer *rb, void *data)
{
    BYTE *_t;
    BYTE *_val;

    TRACE();

    if (rb == NULL)
        ERROR("rb == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (ring_buffer_is_full(rb))
    {
        if (rb->____destructor)
            rb->____destructor((void *)((char *)rb->____buf + rb->____tail));

        RB_NEXT_HEAD(rb);
    }
    else
        ++rb->____num_entries;

    _t = (BYTE *)rb->____buf;
    _val = (BYTE *)data;
    __ASSIGN__(_t[rb->____tail], *_val, rb->____data_size);

    RB_NEXT_TAIL(rb);

    return 0;
}

int ring_buffer_get_head(Ring_buffer *rb, void *data)
{
    BYTE *_t;

    TRACE();

    if (rb == NULL)
        ERROR("rb == NULL\n", 1);

    if (data == NULL)
        ERROR("data == NULL\n", 1);

    if (ring_buffer_is_empty(rb))
        ERROR("Ring Buffer is empty\n", 1);

    _t = (BYTE *)rb->____buf;

    __ASSIGN__(*(BYTE *)data, _t[rb->____head], rb->____data_size);

    return 0;
}

int ring_buffer_dequeue(Ring_buffer *rb, void *data)
{
    TRACE();

    int ret;

    ret = ring_buffer_get_head(rb, data);
    if (ret)
        ERROR("Get head error\n", 1);

    RB_NEXT_HEAD(rb);
    --rb->____num_entries;

    return 0;
}

int ring_buffer_to_array(Ring_buffer *rb, void *array, size_t *size)
{
    size_t bytes_to_move;
    size_t temp_bytes_to_move;

    void *t;
    BYTE *_t;

    TRACE();

    if (rb == NULL)
        ERROR("rb == NULL\n", 1);

    if (array == NULL)
        ERROR("array == NULL\n", 1);

    if (ring_buffer_is_empty(rb))
        ERROR("Ring Buffer is empty\n", 1);

    if (ring_buffer_is_full(rb))
    {
        bytes_to_move = rb->____num_entries * rb->____data_size;
    }
    else
    {
        if (rb->____tail < rb->____head)
            bytes_to_move = rb->____tail + (rb->____max_entries * rb->____data_size) - rb->____head;
        else
            bytes_to_move = rb->____tail - rb->____head;
    }

    t = malloc(bytes_to_move);
    if (t == NULL)
        ERROR("malloc error\n", 1);

    _t = (BYTE *)rb->____buf;

    if (ring_buffer_is_full(rb))
    {
        if (memcpy(t, (void *)_t, bytes_to_move) == NULL)
            ERROR("memcpy error\n", 1);
    }
    else
    {
        if (rb->____head < rb->____tail)
        {
            if (memcpy(t, (void *)(_t + rb->____head), bytes_to_move) == NULL)
                ERROR("memcpy error\n", 1);
        }
        else
        {
            temp_bytes_to_move = (rb->____max_entries * rb->____data_size) - rb->____head;
            if (memcpy(t, (void *)(_t + rb->____head), temp_bytes_to_move) == NULL
                || memcpy((void *)((BYTE *)t + temp_bytes_to_move), _t, rb->____tail) == NULL )
                ERROR("memcpy error\n", 1);
        }
    }

    if (size != NULL)
        *size = bytes_to_move / rb->____data_size;

    *(void **)array = t;

    return 0;
}

bool ring_buffer_is_full(Ring_buffer *rb)
{
    TRACE();

    return rb == NULL || rb->____num_entries == rb->____max_entries;
}

bool ring_buffer_is_empty(Ring_buffer *rb)
{
    TRACE();

    return rb == NULL || rb->____num_entries == 0;
}

ssize_t ring_buffer_get_num_entries(Ring_buffer *rb)
{
    TRACE();

    if (rb == NULL)
        ERROR("rb == NULL\n", -1);

    return (ssize_t)rb->____num_entries;
}

int ring_buffer_get_data_size(Ring_buffer *rb)
{
    TRACE();

    if (rb == NULL)
        ERROR("rb == NULL\n", -1);


    return (int)rb->____data_size;
}