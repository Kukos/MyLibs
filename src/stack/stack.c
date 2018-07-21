#include <stack.h>
#include <generic.h>
#include <log.h>
#include <darray.h>
#include <stdlib.h>
#include <common.h>

Stack *stack_create(size_t size_of, destructor_f destroy)
{
    TRACE();

    return darray_create(DARRAY_UNSORTED, 0, size_of, NULL, destroy);
}

void stack_destroy(Stack *stack)
{
    TRACE();

    darray_destroy(stack);
}

void stack_destroy_with_entries(Stack *stack)
{
    TRACE();

    darray_destroy_with_entries(stack);
}

int stack_push(Stack *stack, const void *val)
{
    TRACE();

    return darray_insert(stack, val);
}

int stack_pop(Stack *stack, void* val)
{
    int ret;

    TRACE();

    if (stack == NULL || val == NULL)
        ERROR("stack == NULL || val == NULL\n", 1);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1);

    ret = darray_get_data(stack, (size_t)darray_get_num_entries(stack) - 1, val);
    if (ret)
        ERROR("get data error\n", 1);

    return darray_delete(stack);
}

bool stack_is_empty(const Stack *stack)
{
    TRACE();

    return (stack == NULL || darray_get_num_entries(stack) == 0);
}

int stack_get_top(const Stack *stack, void *val)
{
    TRACE();

    if (stack == NULL || val == NULL)
        ERROR("stack == NULL || val == NULL\n", 1);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1);

    return darray_get_data(stack, (size_t)darray_get_num_entries(stack) - 1, val);
}

int stack_to_array(const Stack *stack, void *array, size_t *size)
{
    void *t;

    TRACE();

    if (stack == NULL || array == NULL)
        ERROR("stack == NULL || array == NULL\n", 1);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n",1);

    t = malloc((size_t)darray_get_num_entries(stack) * (size_t)darray_get_data_size(stack));
    if (t == NULL)
        ERROR("malloc error\n", 1);

    if (memcpy(t, darray_get_array(stack), (size_t)darray_get_num_entries(stack) * (size_t)darray_get_data_size(stack)) == NULL)
        ERROR("memcpy error\n", 1);

    if (size != NULL)
        *size = (size_t)darray_get_num_entries(stack);

    *(void **)array = t;

    return 0;
}

void *stack_get_array(const Stack *stack)
{
    TRACE();

    return darray_get_array(stack);
}

ssize_t stack_get_num_entries(const Stack *stack)
{
    TRACE();

    return darray_get_num_entries(stack);
}

ssize_t stack_get_data_size(const Stack *stack)
{
    TRACE();

    return darray_get_data_size(stack);
}
