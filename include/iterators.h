#ifndef ITERATORS_H
#define ITERATORS_H

/*
    Framework to create iteratos for structures

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0

*/

/*
    How to use:

    1. Let Struct be the name of your struct for which you want to create iterator
       Example:
            typedef struct Mystruct
            {
                int f1;
                int f2;
            }Mystruct;

    2. In header file create iterator structure with name: Struct_iterator
       Example:
            typedef struct Mystruct_iterator
            {
                int pos;
            }Mystruct_iterator;

    3. In header file use macro: IT_FUNC to create definision of needed functions
       Example:
            IT_FUNC(Mystruct, mystruct)

    4. In src file implement all needed functions defined in IT_FUNC macro
*/


/*
    Functions:

    Struct is name of your struct pass to macros as "PREFIX"

    1)
        Init iterator

        PARAMS
        @IN struct - pointer to struct
    	@IN mode - iterator init mode

        RETURN:
        %NULL iff failure
        %Pointer to new iterator iff success

        Struct *Struct_iterator_create(Struct *struct, ITI_MODE mode);


    2)
        Init iterator

        PARAMS
        @IN struct - pointer to struct
        @IN iterator - pointer to iterator
    	@IN mode - iterator init mode

        RETURN:
        %0 iff success
        %Non-zero value iff failure

        int Struct_iterator_init(Struct *struct, Struct_iterator *iterator, ITI_MODE mode);

    3)

        Deallocate iterator memory

        PARAMS
        @iterator - pointer to iterator

        RETURN:
        This is void function

        void Struct_iterator_destroy(Struct_iterator *iterator);

    4)

        Go to the next value

        PARAMS
        @IN iterator - pointer iterator

        RETURN:
        %0 iff success
        %Non-zero value iff failure

        int Struct_iterator_next(Struct_iterator *iterator);

    5)

        Go to the prev value

        PARAMS
        @IN iterator - pointer iterator

        RETURN:
        %0 iff success
        %Non-zero value iff failure

        int Struct_iterator_prev(Struct_iterator *iterator);

    6)

        Check the end of iterator

        PARAMS
        @IN iterator - pointer to iterator

        RETURN:
        %FALSE iff not end
        %TRUE iff end

        bool Struct_iterator_end(Struct_iterator *iterator);

    7)

        Get data from iterator (Pass addr of data as val)

        PARAMS
        @IN - pointer iterator
        @OUT val - pointer to value ( (void *)&struct )

        RETURN:
        %0 iff success
        %Non-zero value iff failure

        int Struct_iterator_get_data(Struct_iterator *iterator, void *val);

    8)

        Get node from iterator (Pass addr of node as node)

        PARAMS
        @IN - pointer iterator
        @OUT node - pointer to node ( (void *)&struct )

        RETURN:
        %0 iff success
        %Non-zero value iff failure

        int Struct_iterator_get_node(Struct_iterator *iterator, void *node);


    Macros:

    @PARAMS
        __struct --> instance of your struct
        __node  --> instance of your node (or NULL)
        __data  --> instance of your data (or NULL)

    1)
        for_each(__struct, __node, __data)  ---> from BEGIN to END

    2)
        for_each_prev(__struct, __node, __data) --> from END to BEGIN

    3)
        for_each_root(__struct, __node, __data) --> from ROOT to END

    4)
        for_each_root_prev(__struct, __node, __data) --> from ROOT to BEGIN

    5)
        for_each_data(__struct, __data) --> from BEGIN to END, return only data

    6)
        for_each_data_prev(__struct, __data) --> from END to BEGIN, return only data

    7)
        for_each_data_root(__struct, __data) --> from ROOT to END, return only data

    8)
        for_each_data_root_prev(__struct, __data) --> from ROOT to BEGIN, return only data

    9)
        for_each_node(__struct, __node) --> from BEGIN to END, return only node

    10)
        for_each_node_prev(__struct, __node) --> from END to BEGIN, return only node

    11)
        for_each_node_root(__struct, __node) --> from ROOT to END, return only node

    12)
        for_each_node_root_prev(__struct, __node) --> from ROOT to BEGIN, return only node


*/

#include <stdbool.h>
#include <compiler.h>

#define IT_STRUCT_NAME(PREFIX) concat(PREFIX, _iterator)

#define ITI_MODE    char
#define ITI_BEGIN   0
#define ITI_END     1
#define ITI_ROOT    2

#define IT_FUNC(STRUCT, PREFIX) \
    IT_STRUCT_NAME(STRUCT) *concat(PREFIX, _iterator_create)(STRUCT *, ITI_MODE); \
    int concat(PREFIX, _iterator_init)(STRUCT *, IT_STRUCT_NAME(STRUCT) *, ITI_MODE); \
    void concat(PREFIX, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *); \
    int concat(PREFIX, _iterator_next)(IT_STRUCT_NAME(STRUCT) *); \
    int concat(PREFIX, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *); \
    bool concat(PREFIX, _iterator_end)(IT_STRUCT_NAME(STRUCT) *); \
    int concat(PREFIX, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *, void *); \
    int concat(PREFIX, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *, void *); \
    \
    __inline__ IT_STRUCT_NAME(STRUCT) *concat(STRUCT, _iterator_create)(STRUCT *s, ITI_MODE mode) \
    { \
        return concat(PREFIX, _iterator_create)(s, mode); \
    } \
    \
    __inline__ int concat(STRUCT, _iterator_init)(STRUCT *s, IT_STRUCT_NAME(STRUCT) *it, ITI_MODE mode) \
    { \
        return concat(PREFIX, _iterator_init)(s, it, mode); \
    } \
    \
    __inline__ void concat(STRUCT, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        concat(PREFIX, _iterator_destroy)(s); \
    } \
    \
    __inline__ int concat(STRUCT, _iterator_next)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_next)(s); \
    } \
    \
    __inline__ int concat(STRUCT, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_prev)(s); \
    } \
    \
    __inline__ bool concat(STRUCT, _iterator_end)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_end)(s); \
    } \
    __inline__ int concat(STRUCT, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *s, void *d) \
    { \
        if (d != NULL) \
            return concat(PREFIX, _iterator_get_data)(s, d); \
    } \
    \
    __inline__ int concat(STRUCT, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *s, void *n) \
    { \
        if (n != NULL) \
            return concat(PREFIX, _iterator_get_node)(s, n); \
    } \
    \

#define for_each(__struct, __node, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_BEGIN)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_next(&______it); \
            )

#define for_each_prev(__struct, __node, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_END)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_prev(&______it); \
            )

#define for_each_root(__struct, __node, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_ROOT)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_next(&______it); \
            )

#define for_each_root_prev(__struct, __node, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_ROOT)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_prev(&______it); \
            )

#define for_each_data(__struct, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_BEGIN)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_next(&______it); \
            )

#define for_each_data_prev(__struct, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_END)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_prev(&______it); \
            )

#define for_each_data_root(__struct, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_ROOT)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_next(&______it); \
            )

#define for_each_data_root_prev(__struct, __data) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_ROOT)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_data(&______it, __data), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_prev(&______it); \
            )


#define for_each_node(__struct, __node) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_BEGIN)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_next(&______it); \
            )

#define for_each_node_prev(__struct, __node) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_END)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_prev(&______it); \
            )

#define for_each_node_root(__struct, __node) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_ROOT)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_next(&______it); \
            )

#define for_each_node_root_prev(__struct, __node) \
        for (   __unused__ IT_STRUCT_NAME(typeof(__struct)) ______it, \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_init(__struct, &______it, ITI_ROOT)), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node); \
                !concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_end(&______it); \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_get_node(&______it, __node), \
                concat(IT_STRUCT_NAME(typeof(__struct)), _iterator_prev(&______it); \
            )


#endif
