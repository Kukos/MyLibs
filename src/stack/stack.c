#include <stack.h>
#include <generic.h>
#include <log.h>
#include <darray.h>
#include <stdlib.h>
#include <assert.h>
#include <common.h>

Stack *stack_create(int size_of)
{
    Stack *s;

    TRACE("");

    assert(size_of < 0);

    s = (Stack *)malloc(sizeof(Stack));
    if (s == NULL)
        ERROR("malloc error\n", NULL, "");

    /* Stack is a unsorted dynamic array */
    s->darray = darray_create(DARRAY_UNSORTED, 0, size_of, NULL);
    if (s->darray == NULL)
    {
        FREE(s);
        ERROR("darray_create error\n", NULL, "");
    }

    return s;
}

void stack_destroy(Stack *stack)
{
    TRACE("");

    if (stack == NULL)
        return;

    darray_destroy(stack->darray);
    FREE(stack);
}

int stack_push(Stack *stack, void *val)
{
    TRACE("");

    assert(stack == NULL || stack->darray == NULL || val == NULL);

    return darray_insert(stack->darray,val);
}

int stack_pop(Stack *stack, void* val)
{
    BYTE *_t;

    TRACE("");

    assert(stack == NULL || stack->darray == NULL || val == NULL);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1, "");

    _t = (BYTE *)darray_get_array(stack->darray);

    __ASSIGN__(*(BYTE *)val,
        _t[(darray_get_num_entries(stack->darray) - 1) * darray_get_size_of(stack->darray)],
        darray_get_size_of(stack->darray));

    return darray_delete(stack->darray);
}

bool stack_is_empty(Stack *stack)
{
    TRACE("");

    return (stack == NULL || stack->darray == NULL
         || darray_get_num_entries(stack->darray) == 0);
}

int stack_get_top(Stack *stack, void *val)
{
    BYTE *_t;

    TRACE("");

    assert(stack == NULL || stack->darray == NULL || val == NULL);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1, "");

    _t = (BYTE *)darray_get_array(stack->darray);

    __ASSIGN__(*(BYTE *)val,
        _t[(darray_get_num_entries(stack->darray) - 1) * darray_get_size_of(stack->darray)],
        darray_get_size_of(stack->darray));

    return 0;
}

int stack_to_array(Stack *stack, void *array, size_t *size)
{
    void *t;

    TRACE("");

    assert(stack == NULL || stack->darray == NULL
         || array == NULL || size == NULL);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n",1 ,"");

    t = malloc(darray_get_num_entries(stack->darray) * darray_get_size_of(stack->darray));
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    if (memcpy(t, stack->darray,
    darray_get_num_entries(stack->darray) * darray_get_size_of(stack->darray)) == NULL)
        ERROR("memcpy error\n", 1 ,"");

    *size = darray_get_num_entries(stack->darray);
    *(void **)array = t;

    return 0;
}
