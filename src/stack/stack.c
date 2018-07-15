#include <stack.h>
#include <generic.h>
#include <log.h>
#include <darray.h>
#include <stdlib.h>
#include <common.h>

Stack *stack_create(size_t size_of, void (*destroy)(void *entry))
{
    Stack *s;

    TRACE();

    if (size_of < 1)
        ERROR("size_of < 1\n", NULL);

    s = (Stack *)malloc(sizeof(Stack));
    if (s == NULL)
        ERROR("malloc error\n", NULL);

    /* Stack is a unsorted dynamic array */
    s->____darray = darray_create(DARRAY_UNSORTED, 0, size_of, NULL, destroy);
    if (s->____darray == NULL)
    {
        FREE(s);
        ERROR("darray_create error\n", NULL);
    }

    return s;
}

void stack_destroy(Stack *stack)
{
    TRACE();

    if (stack == NULL)
        return;

    darray_destroy(stack->____darray);
    FREE(stack);
}

void stack_destroy_with_entries(Stack *stack)
{
    TRACE();

    if (stack == NULL)
        return;

    darray_destroy_with_entries(stack->____darray);

    FREE(stack);
}

int stack_push(Stack *stack, const void *val)
{
    TRACE();

    if (stack == NULL || val == NULL)
        ERROR("stack == NULL || val == NULL\n", 1);

    return darray_insert(stack->____darray,val);
}

int stack_pop(Stack *stack, void* val)
{
    BYTE *_t;

    TRACE();

    if (stack == NULL || val == NULL)
        ERROR("stack == NULL || val == NULL\n", 1);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1);

    _t = (BYTE *)darray_get_array(stack->____darray);

    __ASSIGN__(*(BYTE *)val,
        _t[(darray_get_num_entries(stack->____darray) - 1) * darray_get_data_size(stack->____darray)],
        darray_get_data_size(stack->____darray));

    return darray_delete(stack->____darray);
}

bool stack_is_empty(const Stack *stack)
{
    TRACE();

    return (stack == NULL || stack->____darray == NULL
         || darray_get_num_entries(stack->____darray) == 0);
}

int stack_get_top(const Stack *stack, void *val)
{
    BYTE *_t;

    TRACE();

    if (stack == NULL || val == NULL)
        ERROR("stack == NULL || val == NULL\n", 1);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1);

    _t = (BYTE *)darray_get_array(stack->____darray);

    __ASSIGN__(*(BYTE *)val,
        _t[(darray_get_num_entries(stack->____darray) - 1) * darray_get_data_size(stack->____darray)],
        darray_get_data_size(stack->____darray));

    return 0;
}

int stack_to_array(const Stack *stack, void *array, size_t *size)
{
    void *t;

    TRACE();

    if (stack == NULL || array == NULL)
        ERROR("stack == NULL || array == NULL\n", 1);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n",1);

    t = malloc((size_t)darray_get_num_entries(stack->____darray) * (size_t)darray_get_data_size(stack->____darray));
    if (t == NULL)
        ERROR("malloc error\n", 1);

    if (memcpy(t, darray_get_array(stack->____darray),
    (size_t)darray_get_num_entries(stack->____darray) * (size_t)darray_get_data_size(stack->____darray)) == NULL)
        ERROR("memcpy error\n", 1);

    if (size != NULL)
        *size = (size_t)darray_get_num_entries(stack->____darray);

    *(void **)array = t;

    return 0;
}

void *stack_get_array(const Stack *stack)
{
    TRACE();

    if (stack == NULL)
        ERROR("stack == NULL\n", NULL);

    return darray_get_array(stack->____darray);
}

ssize_t stack_get_num_entries(const Stack *stack)
{
    TRACE();

    if (stack == NULL)
        ERROR("stack == NULL\n", -1);

    return darray_get_num_entries(stack->____darray);
}

ssize_t stack_get_data_size(const Stack *stack)
{
    TRACE();

    if (stack == NULL)
        ERROR("stack == NULL\n", -1);

    return darray_get_data_size(stack->____darray);
}
