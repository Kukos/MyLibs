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
    s->darray = darray_create(UNSORTED, 0, size_of, NULL);
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

    _t = (BYTE *)stack->darray->array;

    __ASSIGN__(*(BYTE *)val,
        _t[(stack->darray->num_entries - 1) * stack->darray->size_of],
        stack->darray->size_of);

    return darray_delete(stack->darray);
}

bool stack_is_empty(Stack *stack)
{
    TRACE("");

    return (stack == NULL || stack->darray == NULL
         || stack->darray->num_entries == 0);
}

int stack_get_top(Stack *stack, void *val)
{
    BYTE *_t;

    TRACE("");

    assert(stack == NULL || stack->darray == NULL || val == NULL);

    if (stack_is_empty(stack))
        ERROR("stack is empty\n", 1, "");

    _t = (BYTE *)stack->darray->array;

    __ASSIGN__(*(BYTE *)val,
        _t[(stack->darray->num_entries - 1) * stack->darray->size_of],
        stack->darray->size_of);

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

    t = malloc(stack->darray->num_entries * stack->darray->size_of);
    if (t == NULL)
        ERROR("malloc error\n", 1, "");

    if (memcpy(t, stack->darray,
    stack->darray->num_entries * stack->darray->size_of) == NULL)
        ERROR("memcpy error\n", 1 ,"");

    *size = stack->darray->num_entries;
    *(void **)array = t;

    return 0;
}
