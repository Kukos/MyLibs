#ifndef TEMPLATE_ARRAY_H
#define TEMPLATE_ARRAY_H

/*
    Implementation template for array of simple type

    using: declare function like ARRAY_EQUAL(int)
    then you can use it by bool array_equal_int(const int *t, size_t len);

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE GPL3.0
*/

#include <stdbool.h>
#include <compiler.h>
#include <stddef.h>
#include <sys/types.h>

#define ARRAY_IT_NAME concat(__i, __LINE__)

/* foreach */
#define for_each_array(t, len, ptr, data)           for (size_t ARRAY_IT_NAME = 0; ARRAY_IT_NAME < len && ((data = t[ARRAY_IT_NAME]) || true) && ((ptr = &((typeof(ptr))t)[ARRAY_IT_NAME]) || true); ++ARRAY_IT_NAME)
#define for_each_prev_array(t, len, ptr, data)      for (size_t ARRAY_IT_NAME = 1; ARRAY_IT_NAME <= len && ((data = t[len - ARRAY_IT_NAME]) || true) && ((ptr = &((typeof(ptr))t)[len - ARRAY_IT_NAME]) || true); ++ARRAY_IT_NAME)
#define for_each_data_array(t, len, data)           for (size_t ARRAY_IT_NAME = 0; ARRAY_IT_NAME < len && ((data = t[ARRAY_IT_NAME]) || true); ++ARRAY_IT_NAME)
#define for_each_data_prev_array(t, len, data)      for (size_t ARRAY_IT_NAME = 1; ARRAY_IT_NAME <= len && ((data = t[len - ARRAY_IT_NAME]) || true); ++ARRAY_IT_NAME)
#define for_each_ptr_array(t, len, ptr)             for (size_t ARRAY_IT_NAME = 0; ARRAY_IT_NAME < len && ((ptr = &((typeof(ptr))t)[ARRAY_IT_NAME]) || true); ++ARRAY_IT_NAME)
#define for_each_ptr_prev_array(t, len, ptr)        for (size_t ARRAY_IT_NAME = 1; ARRAY_IT_NAME <= len && ((ptr = &((typeof(ptr))t)[len - ARRAY_IT_NAME]) || true); ++ARRAY_IT_NAME)

/*
    Macro create static function for cmp whole array (type must by simple type)
*/
#define ARRAY_EQUAL(type) \
    static ___inline___ ___unused___ bool concat(array_equal_, type)(const type *t1, const type *t2, size_t size) \
    { \
        size_t i; \
        \
        if (t1 == NULL || t2 == NULL) \
            return false; \
        \
        for (i = 0; i < size; ++i) \
            if (t1[i] != t2[i]) \
                return false; \
        \
        return true; \
    }

/*
    Macro create static function for reversing whole array (type must by simple type)
*/
#define ARRAY_REVERSE(type) \
    static ___inline___ ___unused___ void concat(array_reverse_, type)(type *t, size_t size) \
    { \
        size_t i; \
        \
        if (t == NULL) \
            return; \
        \
        for (i = 0; i < (size >> 1); ++i) \
            SWAP(t[i], t[size - i - 1]); \
    }

/*
    Macro create static function to get index of min and also min value
*/
#define ARRAY_MIN(type) \
    static ___inline___ ___unused___ ssize_t concat(array_min_, type)(const type *t, size_t len, type *min) \
    { \
        type m; \
        size_t i; \
        size_t index; \
    \
        if (t == NULL || len == 0) \
            return -1; \
        \
        m = t[0]; \
        index = 0; \
        for (i = 1; i < len; ++i) \
            if (m > t[i]) \
            { \
                m = t[i]; \
                index = i; \
            } \
        *min = m; \
        return index; \
    } \

/*
    Macro create static function to get index of min and also min value
*/
#define ARRAY_MAX(type) \
    static ___inline___ ___unused___ ssize_t concat(array_max_, type)(const type *t, size_t len, type *max) \
    { \
        type m; \
        size_t i; \
        size_t index; \
        \
        if (t == NULL || len == 0) \
            return -1; \
        \
        m = t[0]; \
        index = 0; \
        for (i = 1; i < len; ++i) \
            if (m < t[i]) \
            { \
                m = t[i]; \
                index = i; \
            } \
        *max = m; \
        return index; \
    } \

/*
    Macro return sum of array
*/
#define ARRAY_SUM(type) \
    static ___inline___ ___unused___ type concat(array_sum_, type)(const type *t, size_t len) \
    { \
        type sum = 0; \
        type data; \
        \
        if (t == NULL || len == 0) \
            return 0; \
        \
        for_each_data_array(t, len, data) \
            sum += data; \
        \
        return sum; \
    }

/*
    Macro return product of array
*/
#define ARRAY_PRODUCT(type) \
    static ___inline___ ___unused___ type concat(array_product_, type)(const type *t, size_t len) \
    { \
        type product = 1; \
        type data; \
        \
        if (t == NULL || len == 0) \
            return 0; \
        \
        for_each_data_array(t, len, data) \
            product *= data; \
        \
        return product; \
    }

/*
    Macro check partial ordering of array
*/
#define ARRAY_IS_SORTED(type) \
    static ___inline___ ___unused___ bool concat(array_is_sorted_, type)(const type *t, size_t len) \
    { \
        type *ptr; \
        if (t == NULL || len == 0) \
            return true; \
        \
        for_each_ptr_array(t, len - 1, ptr) \
            if (*(ptr + 1) < *ptr) \
                return false; \
        \
        return true; \
    }

/*
    Macro check reverse partial ordering of array
*/
#define ARRAY_IS_REVERSE_SORTED(type) \
    static ___inline___ ___unused___ bool concat(array_is_reverse_sorted_, type)(const type *t, size_t len) \
    { \
        type *ptr; \
        if (t == NULL || len == 0) \
            return true; \
        \
        for_each_ptr_array(t, len - 1, ptr) \
            if (*(ptr + 1) > *ptr) \
                return false; \
        \
        return true; \
    }

#endif