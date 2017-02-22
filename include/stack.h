#ifndef STACK_H
#define STACK_H

#include <darray.h>

/*
    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    Stack using dynamic array

    LICENCE GPL3
*/


#ifndef BOOL
    #define BOOL unsigned char
    #define TRUE 1
    #define FALSE 0
#endif

typedef struct Stack
{
    Darray *darray;

}Stack;


/*
    Create stack of type TYPE

    PARAMS
    @OUT PTR - pointer to stack
    @IN TYPE - type
*/
#define STACK_CREATE(PTR, TYPE) \
    do { \
        PTR = stack_create(sizeof(TYPE)); \
    } while (0);

/*
    Create stack

    PARAMS
    @IN size_of - size of element of stack

    RETURN:
    %NULL iff failure
    %Pointer iff success
*/
Stack *stack_create(int size_of);

/*
    Destroy stack

    PARAMS
    @IN stack - pointer to stack

    RETURN
    This is a void function
*/
void stack_destroy(Stack *stack);

/*
    Push value on stack

    PARAMS:
    @IN stack - pointer to stack
    @IN val - value ((void *)&val)

    RETURN:
    %0 iff success
	%Non-zero value iff failure
*/
int stack_push(Stack *stack, void *val);

/*
    pop value from stack

    PARAMS:
    @IN stack - pointer to stack
    @OUT val - value ((void *)&val)

    RETURN:
    %0 iff success
	%Non-zero value iff failure
*/
int stack_pop(Stack *stack, void *val);

/*
    Get top value from stack

    PARAMS:
    @IN stack - pointer to stack
    @OUT val - value ((void *)&val)

    RETURN:
  	%0 iff success
	%Non-zero value iff failure
*/

int stack_get_top(Stack *stack, void *val);

/*
    PARAMS
    @IN stack - pointer to stack

    RETURN:
    %TRUE iff stack is empty
    %FALSE iff is not empty of failure
*/
BOOL stack_is_empty(Stack *stack);

/*
    Convert stack to array

    PARAMS
    @IN stack - pointer to stack
    @OUT array - array ((void *)&array)
    @OUT size - size of array

	RETURN:
	%0 if success
	%Non-zero value iff failure
*/
int stack_to_array(Stack *stack, void *array, int *size);

#endif
