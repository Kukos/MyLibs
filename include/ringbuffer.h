#ifndef RING_BUFFER_H
#define RING_BUFFER_H

/*
    Simple implementation of Ring buffer(FIFO) with fixed size (passed to constructor)
    with overwriting if buffer is full (also with auto free memory for overwriting
    data by destructor function (can be NULL or just empty function for data without
    dynamically memory allocation)

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0

*/

#include <stddef.h>
#include <stdbool.h>
#include <sys/types.h> /* ssize_t */
#include <common.h>

typedef struct Ring_buffer Ring_buffer;

/*
    Create ring buffer

    PARAMS
    @IN data_size - sizeof( single data )
    @IN max_entries - maximum number of entries in ring buffer
    @IN destructor - your data destructor (or NULL)

    RETURN
    NULL iff failure
    Pointer to Ring Buffer iff success
*/
Ring_buffer *ring_buffer_create(size_t data_size, size_t max_entries, destructor_f destructor);

/*
    Create ring buffer for TYPE

    PARAMS
    @OUT PTR - pointer to stack
    @IN TYPE - type
    @IN ENTRIES - max entries
    @IN DESTRUCTOR - destructor
*/
#define RING_BUFFER_CREATE(PTR, TYPE, ENTRIES, DESTRUCTOR) \
    do { \
        PTR = ring_buffer_create(sizeof(TYPE), ENTRIES, DESTRUCTOR); \
    } while (0);

/*
    Destroy ring buffer (without entries)

    PARAMS
    @IN rb - pointer to Ring Buffer

    RETURN
    This is a void function
*/
void ring_buffer_destroy(Ring_buffer *rb);

/*
    Destroy Ring Buffer WITH entries

    PARAMS
    @IN rb - pointer to Ring Buffer

    RETURN
    This is a void function
*/
void ring_buffer_destroy_with_entries(Ring_buffer *rb);

/*
    Insert data to head Ring buffer FIFO

    PARAMS
    @IN rb - pointer to Ring Buffer
    @IN data - data to insert (void *)&var

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int ring_buffer_enqueue(Ring_buffer * ___restrict___ rb, const void * ___restrict___ data);


/*
    Get head data from Ring buffer FIFO

    PARAMS
    @IN rb - pointer to Ring Buffer
    @OUT data - data from RingBuffer head (void *)&var

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int ring_buffer_get_head(const Ring_buffer * ___restrict___ rb, void * ___restrict___ data);

/*
    Get and Delete data from head Ring buffer FIFO

    PARAMS
    @IN rb - pointer to Ring Buffer
    @OUT data - data from RingBuffer (void *)&var

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int ring_buffer_dequeue(Ring_buffer * ___restrict___ rb, void * ___restrict___ data);

/*
    Check ring buffer entries and full info

    PARAMS
    @IN rb - pointer to ring buffer

    RETURN
    true iff ring buffer is full
    false iff ring buffer is not full

*/
bool ring_buffer_is_full(const Ring_buffer *rb);

/*
    Check ring buffer entries and empty info

    PARAMS
    @IN rb - pointer to ring buffer

    RETURN
    true iff ring buffer is empty
    false iff ring buffer is not empty

*/
bool ring_buffer_is_empty(const Ring_buffer *rb);

/*
    Convert Ring Buffer to array

    PARAMS
    @IN rb - pointer to Ring Buffer
    @OUT array - pointer to array ((void *)&array)
    @OUT size - size of array

	RETURN:
	0 iff success
	Non-zero value iff failure
*/
int ring_buffer_to_array(const Ring_buffer * ___restrict___ rb, void * ___restrict___ array, size_t * ___restrict___ size);

/*
    Get number of entries in Ring Buffer

    PARAMS
    @IN rb - pointer to Ring Buffer

    RETURN
    -1 iff failure
    Num of entries iff success
*/
ssize_t ring_buffer_get_num_entries(const Ring_buffer *rb);

/*
    Get size of

    PARAMS
    @IN rb - pointer to Ring Buffer

    RETURN
    -1 iff failure
    sizeof iff success
*/
ssize_t ring_buffer_get_data_size(const Ring_buffer *rb);

#endif