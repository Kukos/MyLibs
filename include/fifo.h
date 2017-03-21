#ifndef FIFO_H
#define FIFO_H

/*
    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    Fifo using resize array

    LICENCE: GPL 3.0
*/

#include <stdbool.h>
#include <stddef.h> /* size_t */

typedef struct Fifo
{
    void    *array;     /* resizing array */

    size_t     head;       /* head postion in array */
    size_t     tail;       /* tail posiion in array */
    size_t     size;       /* size of allocated array */
    int     size_of;    /* size of element in array */
}Fifo;

/*
    Create fifo with type TYPE

    PARAMS
    @OUT PTR - pointer to fifo
    @IN TYPE - type of fifo
*/
#define FIFO_CREATE(PTR, TYPE) \
    do { \
        PTR = fifo_create(sizeof(TYPE)); \
    } while (0)

/*
    Create fifo

    PARAMS
    @IN size_of - size of element

    RETURN:
    %NULL iff failure
    %Pointer to fifo iff success
*/
Fifo *fifo_create(int size_of);

/*
    Destroy fifo

    PARAMS
    @IN pointer to fifo

    RETURN:
    This is a void function
*/
void fifo_destroy(Fifo *fifo);

/*
    Insert val into queue

    PARAMS
    @IN fifo - pointer to fifo
    @IN val- value ((void *)&val)

    RETURN:
    %0 iff success
	%Non-zero value iff failure
*/
int fifo_enqueue(Fifo *fifo, void *val);

/*
    Get first element and delete from queue

    PARAMS
    @IN fifo - pointer to fifo
    @OUT val - value ((void *)&val)

	RETURN:
	%0 iff success
	%Non-zero value iff failure
*/
int fifo_dequeue(Fifo *fifo, void *val);

/*
    Convert fifo to array

    PARAMS
    @IN fifo - pointer to fifo
    @OUT array - pointer to array ((void *)&array)
    @OUT size - size of array

	RETURN:
	%0 iff success
	%Non-zero value iff failure
*/
int fifo_to_array(Fifo *fifo, void *array, size_t *size);

/*
    PARAMS
    @IN fifo - pointer to fifo

    RETURN
    %TRUE iff fifo is empty
    %FALSE iff fifo is not empty or failure
*/
bool fifo_is_empty(Fifo *fifo);

/*
    GET element from queue head

    PARAMS
    @IN fifo - pointer to fifo
    @OUT val - value ((void *)&val)

	RETURN:
	%0 iff success
	%Non-zero value iff failure
*/
int fifo_get_head(Fifo *fifo, void *val);

#endif
