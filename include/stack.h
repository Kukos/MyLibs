#ifndef STACK_H
#define STACK_H

#include <darray.h>
#include <stdbool.h>
#include <stddef.h> /* size_t */
#include <sys/types.h>
#include <common.h>

/*
    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    Stack using dynamic array

    LICENCE: GPL 3.0
*/

typedef Darray Stack;

/*
    Create stack of type TYPE

    PARAMS
    @OUT PTR - pointer to stack
    @IN TYPE - type
    @IN DESTROY - your data destructor or NULL
*/
#define STACK_CREATE(PTR, TYPE, DESTROY) \
    do { \
        PTR = stack_create(sizeof(TYPE), DESTROY); \
    } while (0);

/*
    Create stack

    PARAMS
    @IN size_of - size of element of stack
    @IN destroy - your data destructor or NULL

    destructor by void * pass addr i.e in array we have MyStruct *,
    so your destructor data = (void *)&ms


    RETURN:
    %NULL iff failure
    %Pointer iff success
*/
Stack *stack_create(size_t size_of, destructor_f destroy);

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

    PARAMS
    @IN stack - pointer Stack

    RETURN:
    This is a void function
*/
void stack_destroy_with_entries(Stack *stack);

/*
    Push value on stack

    PARAMS:
    @IN stack - pointer to stack
    @IN val - value ((void *)&val)

    RETURN:
    %0 iff success
	%Non-zero value iff failure
*/
int stack_push(Stack *stack, const void *val);

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

int stack_get_top(const Stack *stack, void *val);

/*
    PARAMS
    @IN stack - pointer to stack

    RETURN:
    %TRUE iff stack is empty
    %FALSE iff is not empty of failure
*/
bool stack_is_empty(const Stack *stack);

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
int stack_to_array(const Stack *stack, void *array, size_t *size);

/*
    Get Array

    PARAMS
    @IN stack - pointer to Stack

    RETURN
    %NULL iff failure
    %Pointer to array iff success
*/
void *stack_get_array(const Stack *stack);

/*
    Get number of entries in stack

    PARAMS
    @IN stack - pointer to Stack

    RETURN
    %-1 iff failure
    %Num of entries iff success
*/
ssize_t stack_get_num_entries(const Stack *stack);

/*
    Get size of

    PARAMS
    @IN stack - pointer to Stack

    RETURN
    %-1 iff failure
    %sizeof iff success
*/
ssize_t stack_get_data_size(const Stack *stack);

#endif
