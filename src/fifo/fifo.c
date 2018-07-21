#include <stdlib.h>
#include <string.h>
#include <fifo.h>
#include <generic.h>
#include <log.h>
#include <common.h>
#include <compiler.h>

#define INIT_SIZE (size_t)1024

struct Fifo
{
    void        *array;     /* resizing array */

    size_t      head;       /* head postion in array */
    size_t      tail;       /* tail posiion in array */
    size_t      size;       /* size of allocated array */
    size_t      size_of;    /* size of element in array */

   destructor_f destroy; /* data destructor */
};

Fifo *fifo_create(size_t size_of, destructor_f destroy)
{
    Fifo *fifo;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    fifo = (Fifo *)malloc(sizeof(Fifo));
    if (fifo == NULL)
        ERROR("malloc error\n", NULL);

    fifo->array = malloc(INIT_SIZE * size_of);
    if (fifo->array == NULL)
    {
        FREE(fifo);

        ERROR("malloc error\n", NULL);
    }

    fifo->head = 0;
    fifo->tail = 0;
    fifo->size_of = size_of;
    fifo->size = INIT_SIZE;
    fifo->destroy = destroy;

    return fifo;
}

void fifo_destroy(Fifo *fifo)
{
    TRACE();

    if (fifo == NULL)
        return;

    FREE(fifo->array);
    FREE(fifo);
}

void fifo_destroy_with_entries(Fifo *fifo)
{
    TRACE();

    size_t i;
    BYTE *t;

    if (fifo == NULL)
        return;

    if (fifo->destroy == NULL)
    {
        FREE(fifo->array);
        FREE(fifo);

        return;
    }

    t = (BYTE *)fifo->array;

    if (fifo->head < fifo->tail)
    {
        for (i = fifo->head; i < fifo->tail; ++i)
             fifo->destroy((void *)(t + (i * fifo->size_of)));
    }
    else
    {
        for (i = fifo->tail; i < fifo->size; ++i)
             fifo->destroy((void *)(t + (i * fifo->size_of)));

        for (i = 0; i < fifo->head; ++i)
             fifo->destroy((void *)(t + (i * fifo->size_of)));
    }

    FREE(fifo->array);
    FREE(fifo);
}

bool fifo_is_empty(const Fifo *fifo)
{
    TRACE();

    return fifo == NULL || fifo->array == NULL || fifo->head == fifo->tail;
}

int fifo_enqueue(Fifo *fifo, const void *val)
{
    BYTE *_t;
    BYTE *_val;

    size_t entries_head;
    size_t entries_tail;
    size_t to_move;

    TRACE();

    if (fifo == NULL || val == NULL)
        ERROR("fifo == NULL || val == NULL\n", 1);

    _t = (BYTE *)fifo->array;
    _val = (BYTE *)val;

    __ASSIGN__(_t[fifo->tail * fifo->size_of], *_val, fifo->size_of);

    ++fifo->tail;
    if (fifo->tail >= fifo->size)
        fifo->tail -=  fifo->size;

    /* fifo is full now, resize array */
    if (fifo->tail == fifo->head)
    {
        /* resize array */
        fifo->size <<= 1;
        if ((fifo->array = realloc(fifo->array, fifo->size * fifo->size_of)) == NULL)
            ERROR("realloc error\n", 1);

        /* fifo has been reallocated so update pointer */
        _t = (BYTE *)fifo->array;
        /* if head is not at begining of fifo we need move entries */
        if (fifo->head)
        {
            /* from head to end */
            entries_head = (fifo->size >> 1) - fifo->head;

            /* from begining to tail */
            entries_tail = fifo->tail;

            /* move last entries at the end of new array */
            to_move = entries_tail * fifo->size_of;
            if (memcpy((void *)(_t + (fifo->size * fifo->size_of) - to_move), _t, to_move) == NULL)
                ERROR("memcpy error\n", 1);

            /* move head entries at beginig of new array */
            if (memmove(_t, (void *)(_t + (fifo->head * fifo->size_of)), entries_head * fifo->size_of) == NULL)
                ERROR("memmove error\n", 1);

            /* move entries from end array to end of fifo */
            if (memcpy((void *)(_t + entries_head * fifo->size_of), (void *)(_t + (fifo->size * fifo->size_of) - to_move), to_move) == NULL)
                ERROR("memcpy error\n", 1);

            fifo->head = 0;
        }
        fifo->tail = fifo->size >> 1;
    }

    return 0;
}

int fifo_get_head(const Fifo * ___restrict___ fifo, void * ___restrict___ val)
{
    BYTE *_t;

    TRACE();

    if (fifo == NULL || val == NULL)
        ERROR("fifo == NULL || val == NULL\n", 1);

    if (fifo_is_empty(fifo))
        ERROR("fifo is empty\n", 1);

    _t = (BYTE *)fifo->array;

    __ASSIGN__(*(BYTE *)val, _t[fifo->head * fifo->size_of], fifo->size_of);

    return 0;
}

int fifo_dequeue(Fifo * ___restrict___ fifo, void * ___restrict___ val)
{
    BYTE *_t;
    BYTE *buffer;

    size_t entries;
    size_t entries_head;
    size_t entries_tail;
    size_t buffer_entries;

    TRACE();

    if (fifo == NULL || val == NULL)
        ERROR("fifo == NULL || val == NULL\n", 1);

    if (fifo_is_empty(fifo))
        ERROR("fifo is empty\n", 1);

    _t = (BYTE *)fifo->array;

    __ASSIGN__(*(BYTE *)val, _t[fifo->head * fifo->size_of], fifo->size_of);

    ++fifo->head;
    if (fifo->head >= fifo->size)
        fifo->head -= fifo->size;

    if (fifo->tail < fifo->head)
        entries = fifo->tail + fifo->size - fifo->head;
    else
        entries = fifo->tail - fifo->head;

    /* we resize array */
    if ((double)entries < (double)fifo->size * 0.4 && fifo->size >> 1 >= INIT_SIZE)
    {
        if (fifo->head < fifo->tail)
        {
            if (memmove(_t, (void *)(_t + (fifo->head * fifo->size_of)), entries * fifo->size_of) == NULL)
                ERROR("memmove error\n", 1);

            fifo->tail = entries;
        }
        else
        {
            entries_head = fifo->size - fifo->head;
            entries_tail = fifo->tail;
            buffer_entries = MIN(entries_head, entries_tail);

            buffer = (BYTE *)malloc(buffer_entries * fifo->size_of);

            if (entries_head < entries_tail)
            {
                /* move entries to correct place using buffer */
                if (memcpy((void *)buffer,(void *)(_t + (fifo->head * fifo->size_of)), buffer_entries * fifo->size_of) == NULL ||
                    memmove((void *)(_t + (entries_head * fifo->size_of)), _t, entries_tail * fifo->size_of) == NULL ||
                    memcpy(_t, (void *)buffer, buffer_entries * fifo->size_of) == NULL)
                        ERROR("memcpy error\n", 1);
            }
            else
            {
                if (memcpy((void *)buffer, _t, buffer_entries) == NULL ||
                    memmove(_t, (void *)(_t + (fifo->head * fifo->size_of)), entries_head * fifo->size_of) == NULL ||
                    memcpy((void *)(_t + (entries_head * fifo->size_of)), (void *)buffer, buffer_entries * fifo->size_of) == NULL )
                        ERROR("memcpy error\n", 1);
            }

            fifo->tail = entries_head + entries_tail;
			FREE(buffer);
        }

        fifo->size >>=1;
        fifo->head = 0;

        if ((fifo->array = realloc(fifo->array, fifo->size * fifo->size_of)) == NULL )
            ERROR("realloc error\n", 1);
    }

    return 0;
}

int fifo_to_array(const Fifo * ___restrict___ fifo, void * ___restrict___ array, size_t * ___restrict___ size)
{
    size_t entries;
    size_t bytes_to_move;

    void *t;
    BYTE *_t;

    TRACE();

    if (fifo == NULL || array == NULL)
        ERROR("fifo == NULL || array == NULL\n", 1);

    if (fifo_is_empty(fifo))
        ERROR("fifo is empty\n", 1);

    if (fifo->tail < fifo->head)
        entries = fifo->tail + fifo->size - fifo->head;
    else
        entries = fifo->tail - fifo->head;

    t = malloc(entries * fifo->size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1);

    _t = (BYTE *)fifo->array;

    if (fifo->head < fifo->tail)
    {
        if (memcpy(t, (void *)(_t + (fifo->head * fifo->size_of)), entries * fifo->size_of) == NULL)
            ERROR("memcpy error\n", 1);
    }
    else
    {
        bytes_to_move = (fifo->size - fifo->head) * fifo->size_of;
        if (memcpy(t, (void *)(_t + (fifo->head * fifo->size_of)), bytes_to_move) == NULL ||
            memcpy((void *)((BYTE *)t + bytes_to_move), _t, fifo->tail * fifo->size_of) == NULL )
                ERROR("memcpy error\n", 1);
    }

    if (size != NULL)
        *size = entries;

    *(void **)array = t;

    return 0;
}

ssize_t fifo_get_data_size(const Fifo *fifo)
{
    TRACE();

    if (fifo == NULL)
        ERROR("fifo == NULL\n", -1);

    return (ssize_t)fifo->size_of;
}

ssize_t fifo_get_num_entries(const Fifo *fifo)
{
    ssize_t entries;

    TRACE();

    if (fifo == NULL)
        ERROR("fifo == NULL\n", -1);

    if (fifo->tail < fifo->head)
        entries = (ssize_t)fifo->tail + (ssize_t)fifo->size - (ssize_t)fifo->head;
    else
        entries = (ssize_t)fifo->tail - (ssize_t)fifo->head;

    return entries;
}
