
#include <log.h>
#include <common.h>
#include <stdarg.h>
#include <tuple.h>
#include <generic.h>

struct Tuple
{
    size_t n;
    size_t *sizes;
    void   *elements;
};

Tuple *_tuple_create(size_t args, ...)
{
    Tuple *t;
    va_list temp;
    va_list list;

    size_t sum = 0;
    size_t i;
    size_t size;
    size_t offset;

    void *addr;

    BYTE *_t;

    TRACE();

    if (args == 0)
        ERROR("args == 0\n", NULL);

    t = (Tuple *)malloc(sizeof(Tuple));
    if (t == NULL)
        ERROR("malloc error\n", NULL);

    t->sizes = (size_t *)malloc(sizeof(size_t) * (args / 2));
    if (t->sizes == NULL)
    {
        FREE(t);
        ERROR("malloc error\n", NULL);
    }

    va_start(list, args);

    /* copy to temp */
    va_copy(temp, list);

    /* first step, calculate size of elements */
    for (i = 0; i < args / 2; ++i)
    {
        /* first is size */
        size = va_arg(temp, size_t);
        sum += size;
        t->sizes[i] = size;

        /* second is pointer, omit now */
        (void)va_arg(temp, void *);
    }

    if (sum == 0)
    {
        FREE(t->sizes);
        FREE(t);
        ERROR("element sizes == 0\n", NULL);
    }
    va_end(temp);

    /* alloc elements */
    t->elements = malloc(sum);
    _t = (BYTE *)t->elements;

    /* second step go again */
    offset = 0;
    for (i = 0; i < args / 2; ++i)
    {
        /* first size */
        size = va_arg(list, size_t);

        /* check stack correctness (checking va_copy correctness) */
        if (size != t->sizes[i])
        {
            FREE(t->sizes);
            FREE(t->elements);
            FREE(t);

            ERROR("va_copy error\n", NULL);
        }

        /* now addr of elem */
        addr = va_arg(list, void *);

        __ASSIGN__(_t[offset], *(BYTE *)addr, size);
        offset += size;
    }
    va_end(list);

    t->n = args / 2;
    return t;
}

void tuple_destroy(Tuple *t)
{
    TRACE();

    if (t == NULL)
        return;

    FREE(t->sizes);
    FREE(t->elements);
    FREE(t);
}

void *tuple_get_element(const Tuple *t, size_t pos)
{
    size_t i;
    size_t offset;
    BYTE *_t;

    TRACE();

    if (t == NULL)
        ERROR("t == NULL\n", NULL);

    if (pos >= t->n)
        ERROR("Invalid pos\n", NULL);

    /* get offset of element */
    offset = 0;
    for (i = 0; i < pos; ++i)
        offset += t->sizes[i];

    /* return addr of this element */
    _t = t->elements;
    return (void *)&_t[offset];
}