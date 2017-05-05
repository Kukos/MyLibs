#include <stdlib.h>
#include <string.h>
#include <fifo.h>
#include <generic.h>
#include <log.h>
#include <common.h>
#include <compiler.h>
#include <assert.h>

#define INIT_SIZE 1024

Fifo *fifo_create(int size_of)
{
    Fifo *fifo;

    TRACE("");

    assert(size_of < 0);

    fifo = (Fifo *)malloc(sizeof(Fifo));
    if (fifo == NULL)
        ERROR("malloc error\n", NULL, "");

    fifo->____array = malloc(INIT_SIZE * size_of);
    if (fifo->____array == NULL)
    {
        FREE(fifo);

        ERROR("malloc error\n", NULL, "");
    }

    fifo->____head = 0;
    fifo->____tail = 0;
    fifo->____size_of = size_of;
    fifo->____size = INIT_SIZE;

    return fifo;
}

void fifo_destroy(Fifo *fifo)
{
    TRACE("");

    if (fifo == NULL)
        return;

    FREE(fifo->____array);
    FREE(fifo);
}

bool fifo_is_empty(Fifo *fifo)
{
    TRACE("");

    return fifo == NULL || fifo->____array == NULL || fifo->____head == fifo->____tail;
}

int fifo_enqueue(Fifo *fifo, void *val)
{
    BYTE *_t;
    BYTE *_val;

    size_t entries_head;
    size_t entries_tail;
    size_t to_move;

    TRACE("");

    assert(fifo == NULL || fifo->____array == NULL || val == NULL);

    _t = (BYTE *)fifo->____array;
    _val = (BYTE *)val;

    __ASSIGN__(_t[fifo->____tail * fifo->____size_of], *_val, fifo->____size_of);

    ++fifo->____tail;
    if (fifo->____tail >= fifo->____size)
        fifo->____tail -=  fifo->____size;

    /* fifo is full now, resize array */
    if (fifo->____tail == fifo->____head)
    {
        /* resize array */
        fifo->____size <<= 1;
        if ((fifo->____array = realloc(fifo->____array, fifo->____size * fifo->____size_of)) == NULL)
            ERROR("realloc error\n", 1, "");

        /* if head is not at begining of fifo we need move entries */
        if (fifo->____head)
        {
            /* from head to end */
            entries_head = (fifo->____size >> 1) - fifo->____head;

            /* from begining to tail */
            entries_tail = fifo->____tail;

            /* move last entries at the end of new array */
            to_move = entries_tail * fifo->____size_of;
            if (memcpy((void *)(_t + (fifo->____size * fifo->____size_of) - to_move), _t, to_move) == NULL)
                ERROR("memcpy error\n", 1, "");

            /* move head entries at beginig of new array */
            if (memmove(_t, (void *)(_t + (fifo->____head * fifo->____size_of)), entries_head * fifo->____size_of) == NULL)
                ERROR("memmove error\n", 1, "");

            /* move entries from end array to end of fifo */
            if (memcpy(     (void *)(_t + entries_head * fifo->____size_of),
                            (void *)(_t + (fifo->____size * fifo->____size_of) - to_move),
                            to_move) == NULL)
                ERROR("memcpy error\n", 1, "");

            fifo->____head = 0;
        }

        fifo->____tail = fifo->____size >> 1;
    }

    return 0;
}

int fifo_get_head(Fifo *fifo, void *val)
{
    BYTE *_t;

    TRACE("");

    assert(fifo == NULL || fifo->____array == NULL || val == NULL);

    if (fifo_is_empty(fifo))
        ERROR("fifo is empty\n", 1, "");

    _t = (BYTE *)fifo->____array;

    __ASSIGN__(*(BYTE *)val, _t[fifo->____head * fifo->____size_of], fifo->____size_of);

    return 0;
}

int fifo_dequeue(Fifo *fifo, void *val)
{
    BYTE *_t;
    BYTE *buffer;

    size_t entries;
    size_t entries_head;
    size_t entries_tail;
    size_t buffer_entries;

    TRACE("");

    assert(fifo == NULL || fifo->____array == NULL || val == NULL);

    if (fifo_is_empty(fifo))
        ERROR("fifo is empty\n", 1, "");

    _t = (BYTE *)fifo->____array;

    __ASSIGN__(*(BYTE *)val, _t[fifo->____head * fifo->____size_of], fifo->____size_of);

    ++fifo->____head;
    if (fifo->____head >= fifo->____size)
        fifo->____head -= fifo->____size;

    if (fifo->____tail < fifo->____head)
        entries = fifo->____tail + fifo->____size - fifo->____head;
    else
        entries = fifo->____tail - fifo->____head;

    /* we resize array */
    if (entries < fifo->____size * 0.4 && fifo->____size >> 1 >= INIT_SIZE)
    {
        if (fifo->____head < fifo->____tail)
        {
            if (memmove(_t, (void *)(_t + (fifo->____head * fifo->____size_of)),
                            (fifo->____tail - fifo->____head) * fifo->____size_of) == NULL)
                ERROR("memmove error\n", 1, "");

            fifo->____tail -= fifo->____head;
        }
        else
        {
            entries_head = fifo->____size - fifo->____head;
            entries_tail = fifo->____tail;
            buffer_entries = MIN(entries_head, entries_tail);

            buffer = (BYTE *)malloc(buffer_entries * fifo->____size_of);

            if (entries_head < entries_tail)
            {
                /* move entries to correct place using buffer */
                if (memcpy((void *)buffer,(void *)(_t + (fifo->____head * fifo->____size_of)),
                            buffer_entries * fifo->____size_of) == NULL
                || memmove((void *)(_t + (entries_head * fifo->____size_of)), _t,
                            entries_tail * fifo->____size_of) == NULL
                || memcpy(_t, (void *)buffer, buffer_entries * fifo->____size_of) == NULL)
                        ERROR("memcpy error\n", 1, "");
            }
            else
            {
                if (memcpy((void *)buffer, _t, buffer_entries) == NULL
                || memmove(_t, (void *)(_t + (fifo->____head * fifo->____size_of)),
                            entries_head * fifo->____size_of) == NULL
                || memcpy((void *)(_t + (entries_head * fifo->____size_of)),
                          (void *)buffer, buffer_entries * fifo->____size_of) == NULL )
                    ERROR("memcpy error\n", 1, "");
            }

            fifo->____tail = entries_head + entries_tail;
			FREE(buffer);
        }

        fifo->____size >>=1;
        fifo->____head = 0;

        if ((fifo->____array = realloc(fifo->____array, fifo->____size * fifo->____size_of)) == NULL )
            ERROR("realloc error\n", 1, "");
    }

    return 0;
}

int fifo_to_array(Fifo *fifo, void *array, size_t *size)
{
    size_t entries;
    size_t bytes_to_move;

    void *t;
    BYTE *_t;

    TRACE("");

    assert(fifo == NULL || fifo->____array == NULL || array == NULL || size == NULL);

    if (fifo_is_empty(fifo))
        ERROR("fifo is empty\n", 1, "");

    if( fifo->____tail < fifo->____head)
        entries = fifo->____tail + fifo->____size - fifo->____head;
    else
        entries = fifo->____tail - fifo->____head;

    t = malloc(entries * fifo->____size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    _t = (BYTE *)fifo->____array;

    if (fifo->____head < fifo->____tail)
    {
        if (memcpy(t, (void *)(_t + (fifo->____head * fifo->____size_of)),
        entries * fifo->____size_of) == NULL)
            ERROR("memcpy error\n", 1, "");
    }
    else
    {
        bytes_to_move = (fifo->____size - fifo->____head) * fifo->____size_of;
        if (memcpy(t, (void *)(_t + (fifo->____head * fifo->____size_of)), bytes_to_move) == NULL
                || memcpy((void *)((BYTE *)t + bytes_to_move), _t,
                          fifo->____tail * fifo->____size_of) == NULL )
            ERROR("memcpy error\n", 1, "");
    }

    *size = entries;
    *(void **)array = t;

    return 0;
}

int fifo_get_size_of(Fifo *fifo)
{
    assert(fifo == NULL);

    return fifo->____size_of;
}

ssize_t fifo_get_num_entries(Fifo *fifo)
{
    ssize_t entries;

    assert(fifo == NULL);

    if (fifo->____tail < fifo->____head)
        entries = fifo->____tail + fifo->____size - fifo->____head;
    else
        entries = fifo->____tail - fifo->____head;

    return entries;
}
