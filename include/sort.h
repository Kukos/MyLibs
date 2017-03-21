#ifndef SORT_H
#define SORT_H

/*
    Author Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0

    This is sort header file, every sort are "template sort"
    I use cmp function for compare elements

    Example of cmp function:

    int cmp(void *a, void *b)
    {
        if (*(int *)a == *(int *)b)
            return 0;
        else if (*(int *)a < *(int *)b)
            return -1;
        else
            return 1;
    }

    for sort you can use macros or functions

    e.i
    int *t -- array of int
    int size - number of elements in array t

    QUICKSORT(t, size, cmp, int);

    or

    quciksort(t, size, cmp, sizeof(int));

*/

#include <stddef.h> /* size_t */

/*
    Define for insort, please check insort funciton description
*/
#define INSORT(ARRAY, NUM, CMP, TYPE) (insort(ARRAY, NUM, CMP, sizeof(TYPE)))

/*
    Classic inserion sort

    PARAMS
    @IN t - array
    @IN num_element - number of element in array t
    @IN cmp - function to compare 2 element in array t(see example above)
    @IN size_of - size of element in array t

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
int insort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of);

/*
    Define for binsort, please check binsort funciton description
*/
#define BINSORT(ARRAY, NUM, CMP, TYPE) (binsort(ARRAY, NUM, CMP, sizeof(TYPE)))

/*
    Two directional binary insort

    PARAMS
    @INt - array
    @IN num_element - number of element in array t
    @IN cmp - function to compare 2 element in array t(see example above)
    @IN size_of - size of element in array t

    RETURN:
   	%0 if success
    %Non-zero value if failure
*/
int binsort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of);


/*
    Define for mergesort, please check mergesort funciton description
*/
#define MERGESORT(ARRAY, NUM, CMP, TYPE) (mergesort(ARRAY, NUM, CMP, sizeof(TYPE)))

/*
    iterative mergesort with cut off to insort

    PARAMS
    @IN t - array
    @IN num_element - number of element in array t
    @IN cmp - function to compare 2 element in array t(see example above)
    @IN size_of - size of element in array t

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
int mergesort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of);


/*
    Define for quicksort, please check quicksort funciton description
*/
#define QUICKSORT(ARRAY, NUM, CMP, TYPE) (quicksort(ARRAY, NUM, CMP, sizeof(TYPE)))

/*
    optimal quikcsort ( cut off to insort, tukey, bentley partition )

    PARAMS
    @IN t - array
    @IN num_element - number of element in array t
    @IN cmp - function to compare 2 element in array t(see example above)
    @IN size_of - size of element in array t

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
int quicksort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of);

/*
    Define for sort, please check sort funciton description
*/
#define SORT(ARRAY, NUM, CMP, TYPE) (sort(ARRAY, NUM, CMP, sizeof(TYPE)))

/*
    optimal sort (depeneds on number of entries to sort )

    PARAMS
    @IN t - array
    @IN num_element - number of element in array t
    @IN cmp - function to compare 2 element in array t ( see example of cmp function )
    @IN size_of - size of element in array t

    RETURN:
    %0 if success
    %Non-zero value if failure
*/
int sort(void *t, size_t num_elements, int(*cmp)(void *a, void *b), int size_of);

#endif
