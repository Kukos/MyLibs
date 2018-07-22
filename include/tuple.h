#ifndef TUPLE_H
#define TUPLE_H

/*
    Implementation of generic tuple and pair, like std :: tuple and std :: pair

    You can store int, double, pointer, structures, etc via macro TUPLE_ENTRY
    Arrays on stack like int arr[] = {1, 2, 3}, you can store too, via macro TUPLE_ARRAY_ENTRY

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL3.0
*/

#include <common.h>
#include <stddef.h>

typedef struct Tuple Tuple;
typedef Tuple Pair;

#define TUPLE_ENTRY(x) (size_t)sizeof(x), (void *)(&(x))
#define TUPLE_ARRAY_ENTRY(arr) (size_t)sizeof(arr), (void *)&arr[0]

/* DO NOT use direcly */
Tuple *_tuple_create(size_t args, ...);

/*
    Create tuple
    You have to use TUPLE_ENTRY or TUPLE_ARRAY_ENTRY, so if you want to create tuple(a, b, c)
    DO:
    Tuple *t = tuple_create(TUPLE_ENTRY(a), TUPLE_ENTRY(b), TUPLE_ENTYR(c))
    if type is int [], use TUPLE_ARRAY_ENTRY, if int * as pointer to array on heap, use TUPLE_ENTRY

    PARAMS
    @IN ... - va args of TUPLE_ENTRY

    RETURN
    NULL iff failure
    Pointer to new tuple iff success
*/
#define tuple_create(...) _tuple_create((size_t)VA_ARGS_LEN(__VA_ARGS__), ##__VA_ARGS__)

/*
    Destroy Tuple

    (Tuple stores only copies of values, so you need to free memory by yourself)

    PARAMS
    @IN t - tuple

    RETURN
    This is a void function
*/
void tuple_destroy(Tuple *t);

/*
    Get ith elemnent. Function get address of element, so cast it properly and use,
    because you have address, you can change value of tuple element

    POS is counted from 0

    PARAMS
    @IN t - tuple
    @IN pos - element at @pos to get

    RETURN
    NULL iff failure
    Pointer to element iff success
*/
void *tuple_get_element(const Tuple *t, size_t pos);

/*
    Create Pair
    You have to use TUPLE_ENTRY or TUPLE_ARRAY_ENTRY, so if you want to create tuple(a, b, c)
    DO:
    Pair *p = pair_create(TUPLE_ENTRY(a), TUPLE_ENTRY(b), TUPLE_ENTYR(c))
    if type is int [], use TUPLE_ARRAY_ENTRY, if int * as pointer to array on heap, use TUPLE_ENTRY

    PARAMS
    @IN X - first value
    @IN Y - second value

    RETURN
    NULL iff failure
    Pointer to new Pair iff success
*/
#define pair_create(X, Y) _tuple_create(4, X, Y)

/*
    Destroy Pair

    (Pair stores only copies of values, so you need to free memory by yourself)

    PARAMS
    @IN P - pair

    RETURN
    This is a void function
*/
#define pair_destroy(P) tuple_destroy(P)

/*
    Get first element from pair
    Function get address of element, so cast it properly and use,
    because you have address, you can change value of pair first element

    PARAMS
    @IN P - pair

    RETURN
    NULL iff failure
    Pointer to element iff success
*/
#define pair_get_first(P) tuple_get_element(P, 0)

/*
    Get second element from pair
    Function get address of element, so cast it properly and use,
    because you have address, you can change value of pair second element

    PARAMS
    @IN P - pair

    RETURN
    NULL iff failure
    Pointer to element iff success
*/
#define pair_get_second(P) tuple_get_element(P, 1)

#endif