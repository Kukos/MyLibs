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

        Struct *Struct_iterator_create(Struct *struct, iti_mode_t mode);


    2)
        Init iterator

        PARAMS
        @IN struct - pointer to struct
        @IN iterator - pointer to iterator
    	@IN mode - iterator init mode

        RETURN:
        %0 iff success
        %Non-zero value iff failure

        int Struct_iterator_init(Struct *struct, Struct_iterator *iterator, iti_mode_t mode);

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
        __type   --> typeof(__struct)
        __node   --> instance of your node
        __data   --> instance of your data

    1)
        for_each(__struct, __type, __node, __data)  ---> from BEGIN to END

    2)
        for_each_prev(__struct, __type, __node, __data) --> from END to BEGIN

    3)
        for_each_root(__struct, __type, __node, __data) --> from ROOT to END

    4)
        for_each_root_prev(__struct, __type, __node, __data) --> from ROOT to BEGIN

    5)
        for_each_data(__struct, __type, __data) --> from BEGIN to END, return only data

    6)
        for_each_data_prev(__struct, __type, __data) --> from END to BEGIN, return only data

    7)
        for_each_data_root(__struct, __type, __data) --> from ROOT to END, return only data

    8)
        for_each_data_root_prev(__struct, __type, __data) --> from ROOT to BEGIN, return only data

    9)
        for_each_node(__struct, __type, __node) --> from BEGIN to END, return only node

    10)
        for_each_node_prev(__struct, __type, __node) --> from END to BEGIN, return only node

    11)
        for_each_node_root(__struct, __type, __node) --> from ROOT to END, return only node

    12)
        for_each_node_root_prev(__struct, __type, __node) --> from ROOT to BEGIN, return only node
*/

#include <stdbool.h>
#include <compiler.h>

#define IT_STRUCT_NAME(PREFIX) concat(PREFIX, _iterator)

typedef char iti_mode_t;
#define ITI_BEGIN   0
#define ITI_END     1
#define ITI_ROOT    2

#define IT_FUNC(STRUCT, PREFIX) \
    IT_STRUCT_NAME(STRUCT) *concat(PREFIX, _iterator_create)(STRUCT *, iti_mode_t); \
    int concat(PREFIX, _iterator_init)(STRUCT *, IT_STRUCT_NAME(STRUCT) *, iti_mode_t); \
    void concat(PREFIX, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *); \
    int concat(PREFIX, _iterator_next)(IT_STRUCT_NAME(STRUCT) *); \
    int concat(PREFIX, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *); \
    bool concat(PREFIX, _iterator_end)(IT_STRUCT_NAME(STRUCT) *); \
    int concat(PREFIX, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *, void *); \
    int concat(PREFIX, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *, void *); \
    ___inline___ IT_STRUCT_NAME(STRUCT) *concat(STRUCT, _iterator_create)(STRUCT *s, iti_mode_t mode); \
    ___inline___ int concat(STRUCT, _iterator_init)(STRUCT *s, IT_STRUCT_NAME(STRUCT) *it, iti_mode_t mode); \
    ___inline___ void concat(STRUCT, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_destroy_int)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_next)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ bool concat(STRUCT, _iterator_end)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *s, void *d); \
    ___inline___ int concat(STRUCT, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *s, void *n); \
    \
    ___inline___ IT_STRUCT_NAME(STRUCT) *concat(STRUCT, _iterator_create)(STRUCT *s, iti_mode_t mode) \
    { \
        return concat(PREFIX, _iterator_create)(s, mode); \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_init)(STRUCT *s, IT_STRUCT_NAME(STRUCT) *it, iti_mode_t mode) \
    { \
        return concat(PREFIX, _iterator_init)(s, it, mode); \
    } \
    \
    ___inline___ void concat(STRUCT, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        concat(PREFIX, _iterator_destroy)(s); \
    } \
    ___inline___ int concat(STRUCT, _iterator_destroy_int)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        concat(PREFIX, _iterator_destroy)(s); \
        return 0; \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_next)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_next)(s); \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_prev)(s); \
    } \
    \
    ___inline___ bool concat(STRUCT, _iterator_end)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_end)(s); \
    } \
    ___inline___ int concat(STRUCT, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *s, void *d) \
    { \
            return concat(PREFIX, _iterator_get_data)(s, d); \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *s, void *n) \
    { \
            return concat(PREFIX, _iterator_get_node)(s, n); \
    }

#define for_each(__struct, __type, __node, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_BEGIN); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _next)(______it) \
            )

#define for_each_prev(__struct, __type, __node, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_END); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _prev)(______it) \
            )

#define for_each_root(__struct, __type, __node, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_ROOT); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _next)(______it) \
            )


#define for_each_root_prev(__struct, __type, __node, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_ROOT); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _prev)(______it) \
            )


#define for_each_data(__struct, __type, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_BEGIN); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _next)(______it) \
            )

#define for_each_data_prev(__struct, __type, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_END); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _prev)(______it) \
            )

#define for_each_data_root(__struct, __type, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_ROOT); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _next)(______it) \
            )


#define for_each_data_root_prev(__struct, __type, __data) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_ROOT); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_data)(______it, (void *)&(__data)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _prev)(______it) \
            )


#define for_each_node(__struct, __type, __node) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_BEGIN); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _next)(______it) \
            )

#define for_each_node_prev(__struct, __type, __node) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_END); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _prev)(______it) \
            )

#define for_each_node_root(__struct, __type, __node) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_ROOT); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _next)(______it) \
            )


#define for_each_node_root_prev(__struct, __type, __node) \
        for (   ___unused___ IT_STRUCT_NAME(__type) *______it = \
                concat(IT_STRUCT_NAME(__type), _create)(__struct, ITI_ROOT); \
                ( \
                    !concat(IT_STRUCT_NAME(__type), _end)(______it) && \
                    !concat(IT_STRUCT_NAME(__type), _get_node)(______it, (void *)&(__node)) \
                ) || concat(IT_STRUCT_NAME(__type), _destroy_int(______it)); \
                concat(IT_STRUCT_NAME(__type), _prev)(______it) \
            )

/* FOR CONTAINERS FRAMEWORKS LIKE TREE / ULIST / SLIST ... */
/* Like IT_FUNC but most functions are inline and containers framework friendly */
#define IT_FUNC_CONTAINER(STRUCT, PREFIX) \
    ___inline___ void *concat(PREFIX, _iterator_get_iterator)(IT_STRUCT_NAME(STRUCT) *it); \
    ___inline___ void concat(PREFIX, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *it); \
    ___inline___ int concat(PREFIX, _iterator_next)(IT_STRUCT_NAME(STRUCT) *it); \
    ___inline___ int concat(PREFIX, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *it); \
    ___inline___ bool concat(PREFIX, _iterator_end)(IT_STRUCT_NAME(STRUCT) *it); \
    ___inline___ int concat(PREFIX, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *it, void *data); \
    ___inline___ int concat(PREFIX, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *it, void *node); \
    ___inline___ IT_STRUCT_NAME(STRUCT) *concat(STRUCT, _iterator_create)(STRUCT *s, iti_mode_t mode); \
    ___inline___ int concat(STRUCT, _iterator_init)(STRUCT *s, IT_STRUCT_NAME(STRUCT) *it, iti_mode_t mode); \
    ___inline___ void concat(STRUCT, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_destroy_int)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_next)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ bool concat(STRUCT, _iterator_end)(IT_STRUCT_NAME(STRUCT) *s); \
    ___inline___ int concat(STRUCT, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *s, void *d); \
    ___inline___ int concat(STRUCT, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *s, void *n); \
    \
    ___inline___ void *concat(PREFIX, _iterator_get_iterator)(IT_STRUCT_NAME(STRUCT) *it) \
    { \
        if (it == NULL) \
            return NULL; \
        \
        return it->____iterator; \
    } \
     \
    ___inline___ void concat(PREFIX, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *it) \
    { \
        if (it == NULL) \
            return; \
        \
        it->____destroy(concat(PREFIX, _iterator_get_iterator)(it)); \
        FREE(it); \
    } \
    \
    ___inline___ int concat(PREFIX, _iterator_next)(IT_STRUCT_NAME(STRUCT) *it) \
    { \
        if (it == NULL) \
            return 1; \
        \
        return it->____next(concat(PREFIX, _iterator_get_iterator)(it)); \
    } \
    \
    ___inline___ int concat(PREFIX, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *it) \
    { \
        if (it == NULL) \
            return 1;\
        \
        return it->____prev(concat(PREFIX, _iterator_get_iterator)(it)); \
    } \
    \
    ___inline___ bool concat(PREFIX, _iterator_end)(IT_STRUCT_NAME(STRUCT) *it) \
    { \
        if (it == NULL) \
            return true; \
        \
        return it->____end(concat(PREFIX, _iterator_get_iterator)(it)); \
    } \
    \
    ___inline___ int concat(PREFIX, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *it, void *data) \
    { \
        if (it == NULL) \
            return 1; \
        \
        return it->____get_data(concat(PREFIX, _iterator_get_iterator)(it), data); \
    } \
    ___inline___ int concat(PREFIX, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *it, void *node) \
    { \
        if (it == NULL) \
            return 1; \
        \
        return it->____get_node(concat(PREFIX, _iterator_get_iterator)(it), node); \
    } \
    \
      ___inline___ IT_STRUCT_NAME(STRUCT) *concat(STRUCT, _iterator_create)(STRUCT *s, iti_mode_t mode) \
    { \
        return concat(PREFIX, _iterator_create)(s, mode); \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_init)(STRUCT *s, IT_STRUCT_NAME(STRUCT) *it, iti_mode_t mode) \
    { \
        return concat(PREFIX, _iterator_init)(s, it, mode); \
    } \
    \
    ___inline___ void concat(STRUCT, _iterator_destroy)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        concat(PREFIX, _iterator_destroy)(s); \
    } \
    ___inline___ int concat(STRUCT, _iterator_destroy_int)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        concat(PREFIX, _iterator_destroy)(s); \
        return 0; \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_next)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_next)(s); \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_prev)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_prev)(s); \
    } \
    \
    ___inline___ bool concat(STRUCT, _iterator_end)(IT_STRUCT_NAME(STRUCT) *s) \
    { \
        return concat(PREFIX, _iterator_end)(s); \
    } \
    ___inline___ int concat(STRUCT, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *s, void *d) \
    { \
            return concat(PREFIX, _iterator_get_data)(s, d); \
    } \
    \
    ___inline___ int concat(STRUCT, _iterator_get_node)(IT_STRUCT_NAME(STRUCT) *s, void *n) \
    { \
            return concat(PREFIX, _iterator_get_node)(s, n); \
    }

#endif
