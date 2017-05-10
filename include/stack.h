#ifndef STACK_H
#define STACK_H

#include <darray.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */
#include <sys/types.h>

/*
    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    Stack using dynamic array

    LICENCE: GPL 3.0
*/

typedef struct Stack
{
    Darray *____darray;

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
    Destroy Stack with all entries ( call destructor for each entries )

    destructor by void * pass addr i.e in array we have MyStruct *,
    so your destructor data = (void *)&ms

    PARAMS
    @IN stack - pointer Stack
    @IN desturctor -  your object destructor

    RETURN:
    This is a void function
*/
void stack_destroy_with_entries(Stack *stack, void (*destructor)(void *data));

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
bool stack_is_empty(Stack *stack);

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
int stack_to_array(Stack *stack, void *array, size_t *size);

/*
    Get Array

    PARAMS
    @IN stack - pointer to Stack

    RETURN
    %NULL iff failure
    %Pointer to array iff success
*/
void *stack_get_array(Stack *stack);

/*
    Get number of entries in stack

    PARAMS
    @IN stack - pointer to Stack

    RETURN
    %-1 iff failure
    %Num of entries iff success
*/
ssize_t stack_get_num_entries(Stack *stack);

/*
    Get size of

    PARAMS
    @IN stack - pointer to Stack

    RETURN
    %-1 iff failure
    %sizeof iff success
*/
int stack_get_size_of(Stack *stack);

#endif
