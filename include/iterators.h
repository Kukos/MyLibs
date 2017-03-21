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
        %NULL if failure
        %Pointer to new iterator if success

        Struct *Struct_iterator_create(Struct *struct, ITI_MODE mode);


    2)
        Init iterator

        PARAMS
        @IN struct - pointer to struct
        @IN iterator - pointer to iterator
    	@IN mode - iterator init mode

        RETURN:
        %0 if success
        %Non-zero value if failure

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
        %0 if success
        %Non-zero value if failure

        int Struct_iterator_next(Struct_iterator *iterator);

    5)

        Go to the prev value

        PARAMS
        @IN iterator - pointer iterator

        RETURN:
        %0 if success
        %Non-zero value if failure

        int Struct_iterator_prev(Struct_iterator *iterator);

    6)

        Check the end of iterator

        PARAMS
        @IN iterator - pointer to iterator

        RETURN:
        %FALSE if not end
        %TRUE if end

        bool Struct_iterator_end(Struct_iterator *iterator);

    7)

        Get data from iterator (Pass addr of data as val)

        PARAMS
        @IN - pointer iterator
        @OUT val - pointer to value ( (void *)&struct )

        RETURN:
        %0 if success
        %Non-zero value if failure

        int Struct_iterator_get_data(Struct_iterator *iterator,void *val);
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
    int concat(PREFIX, _iterator_get_data)(IT_STRUCT_NAME(STRUCT) *, void *);


#endif
