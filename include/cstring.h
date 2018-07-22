#ifndef CSTRING_H
#define CSTRING_H

/*
    Implementation of string from char *, like std :: string

    Author: Michal Kukowski
    email: michalkukowski10@gmail.com

    LICENCE: GPL 3.0
*/

#include <sys/types.h>
#include <stdbool.h>
#include <iterators.h>
#include <common.h>

typedef struct String String;
typedef struct String_iterator String_iterator;

IT_FUNC(String, string)

/*
    Create Empty String

    PARAMS
    NO PARAMS

    RETURN
    NULL iff failure
    Pointer to String
*/
String *string_create(void);

/*
    Destroy String

    PARAMS
    @IN string

    RETURN
    This is a void function
*/
void string_destroy(String *string);

/*
    Get char* from String

    PARAMS
    @IN string - pointer to String

    RETURN
    NULL iff failure (e.g empty string)
    char* iff success
*/
char *string_get_str(const String *string);

/*
    Get length of String

    PARAMS
    @IN string - pointer to String

    RETURN
    -1 iff failure
    String Length iff success
*/
ssize_t string_get_length(const String *string);

/*
    Test if string is empty

    PARAMS
    @IN string - pointer to String

    RETURN
    true iff string is empty
    false iff string is not empty
*/
bool string_is_empty(const String *string);

/*
    Reverse in place whole string

    PARAMS
    @IN str - string to reverse

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_reverse(String *str);

/*
    Create string (string representation of variable)
    from variable of primitive type

    PARAMS
    @IN c / str/ n -> variable

    RETURN
    NULL iff failure
    Pointer to new String
*/
String *string_create_from_c(char c);
String *string_create_from_str(const char *str);
String *string_create_from_i(int n);
String *string_create_from_ui(unsigned int n);
String *string_create_from_l(long n);
String *string_create_from_ul(unsigned long n);
String *string_create_from_ll(long long n);
String *string_create_from_ull(unsigned long long n);
String *string_create_from_f(float n);
String *string_create_from_d(double n);
String *string_create_from_ld(long double n);

/*
    Create new String with concatenated string from s1 and s2

    PARAMS
    @IN s1 - 1st string
    @IN s2 - 2nd string

    RETURN
    NULL iff failure
    Pointer to new String iff success

*/
String *string_concat(const String *s1, const String *s2);

/*
    Get char from @index

    PARAMS
    @IN string - pointer to String
    @IN index - char index

    RETURN
    -1 iff failure
    char at index @index

*/
char string_get_char(const String *string, size_t index);

/*
    Set char at @index

    PARAMS
    @IN string - pointer to String
    @IN index - char index
    @IN c - char

    RETURN
    Non-zero value iff failure
    0 iff success
*/
int string_set_char(String *string, size_t index, char c);

/*
    Convert String to upper case

    PARAMS
    @IN string - pointer to string

    RETURN
    Non-zero value iff failure
    0 iff success
*/
int string_toupper(String *string);

/*
    Convert String to lower case

    PARAMS
    @IN string - pointer to string

    RETURN
    Non-zero value iff failure
    0 iff success
*/
int string_tolower(String *string);

/*
    Remove whitespaces at begining and at the end of string

    PARAMS
    @IN string - pointer to string

    RETURN
    Non-zero value iff failure
    0 iff success
*/
int string_trim(String *string);

/*
    Compare Strings

    PARAMS
    @IN s1 - 1st string
    @IN s2 - 2nd string

    RETURN
    < 0 iff s1 < s2
    = 0 iff s1 == s2
    > 0 iff s1 > s2
*/
int string_cmp(const String *s1, const String *s2);

/*
    Get substring from string from [begin; end]
    so whole string is 0, len - 1

    PARAMS
    @IN string - pointer to string
    @IN begin - begin of substring, index in string
    @IN end - end of substring, index in string

    RETURN
    NULL iff failure
    Pointer to new string (substring of @string) iff success
*/
String *string_substr(const String *string, size_t begin, size_t end);

/*
    Find first Occurrence of pattern(char, char* or String) in string

    PARAMS
    @IN string / s - pointer to String
    @IN c / str / s2 - pattern to find

    RETURN
    -1 iff pattern doesn't exist in string
    position of begining pattern in string
*/
ssize_t string_find_c_first(const String *string, char c);
ssize_t string_find_str_first(const String * ___restrict___ string, const char * ___restrict___ str);
ssize_t string_find_string_first(const String *s1, const String *s2);
ssize_t string_find_c_last(const String *string, char c);
ssize_t string_find_str_last(const String * ___restrict___ string, const char * ___restrict___ str);
ssize_t string_find_string_last(const String *s1, const String *s2);

/*
    Find first / last Occurrence of pattern(char, char* or String) in substring
    of string in range [begin; end]

    PARAMS
    @IN string / s - pointer to String
    @IN c / str / s2 - pattern to find
    @IN begin - begin of substr
    @IN end - end of substr

    RETURN
    -1 iff pattern doesn't exist in string
    position of begining pattern in string
*/
ssize_t string_find_c_first_substr(const String *string, char c, size_t begin, size_t end);
ssize_t string_find_str_first_substr(const String * ___restrict___ string, const char * ___restrict___ str, size_t begin, size_t end);
ssize_t string_find_string_first_substr(const String *s1, const String *s2, size_t begin, size_t end);
ssize_t string_find_c_last_substr(const String *string, char c, size_t begin, size_t end);
ssize_t string_find_str_last_substr(const String * ___restrict___ string, const char * ___restrict___ str, size_t begin, size_t end);
ssize_t string_find_string_last_substr(const String *s1, const String *s2, size_t begin, size_t end);

/*
    Make a copy of @string

    PARAMS
    @IN string - pointer to String

    RETURN
    Copy of @string iff success
    NULL iff failure
*/
String *string_clone(const String *string);

/*
    Delete @chars from @string starting from @pos

    PARAMS
    @IN string - pointer to string
    @IN pos - starting position
    @IN chars - num of chars to delete

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_delete(String *string, size_t pos, size_t chars);

/*
    Cut String (Delete last @chars)

    PARAMS
    @IN string - pointer to string
    @IN chars - num of chars to delete

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_cut(String *string, size_t chars);

/*
    Append char / char * / String at the end of @string

    PARAMS
    @IN string / s1 - pointer to String
    @IN c / str / s2 - data to append

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_append_c(String *string, char c);
int string_append_str(String * ___restrict___ string, const char * ___restrict___ str);
int string_append_string(String *s1, const String *s2);

/*
    Insert char / char * / string at pos @pos

    PARAMS
    @IN string / s1 - pointer to String
    @IN c / str / c2 - data to insert
    @IN pos - @pos to insert

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_insert_c(String *string, char c, size_t pos);
int string_insert_str(String * ___restrict___ string, const char * ___restrict___ str, size_t pos);
int string_insert_string(String *s1, const String *s2, size_t pos);

/*
    Remove first / last / all occurrence of
    char / char * / string in @string

    PARAMS
    @IN string - pointer to String
    @IN c / str / pattern - pattern to remove

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_remove_c_first(String *string, char c);
int string_remove_c_last(String *string, char c);
int string_remove_c_all(String *string, char c);
int string_remove_str_first(String * ___restrict___ string, const char * ___restrict___ str);
int string_remove_str_last(String * ___restrict___ string, const char * ___restrict___ str);
int string_remove_str_all(String * ___restrict___ string, const char * ___restrict___ str);
int string_remove_string_first(String *string, const String *pattern);
int string_remove_string_last(String *string, const String *pattern);
int string_remove_string_all(String *string, const String *pattern);

/*
    Replace first / last / all  occurrence of char by char / char * /string

    PARAMS
    @IN string - pointer to String
    @IN c1 - char to replace
    @IN c2 / str / string2 - replacing pattern

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_replace_c_by_c_first(String *string, char c1, char c2);
int string_replace_c_by_str_first(String * ___restrict___ string, char c1, const char * ___restrict___ str);
int string_replace_c_by_string_first(String *string, char c1, const String *string2);
int string_replace_c_by_c_last(String *string, char c1, char c2);
int string_replace_c_by_str_last(String * ___restrict___ string, char c1, const char * ___restrict___ str);
int string_replace_c_by_string_last(String *string, char c1, const String *string2);
int string_replace_c_by_c_all(String *string, char c1, char c2);
int string_replace_c_by_str_all(String * ___restrict___ string, char c1, const char * ___restrict___ str);
int string_replace_c_by_string_all(String *string, char c1, const String *string2);

/*
    Replace first / last / all  occurrence of char * by char / char * /string

    PARAMS
    @IN string - pointer to String
    @IN str1 - legacy string to replace
    @IN c / str2 / string2 - replacing pattern

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_replace_str_by_c_first(String * ___restrict___ string, const char * ___restrict___ str1, char c);
int string_replace_str_by_str_first(String * ___restrict___ string, const char *str1, const char *str2);
int string_replace_str_by_string_first(String *string, const char * ___restrict___ str1, const String *string2);
int string_replace_str_by_c_last(String * ___restrict___ string, const char * ___restrict___ str1, char c2);
int string_replace_str_by_str_last(String * ___restrict___ string, const char *str1, const char *str);
int string_replace_str_by_string_last(String *string, const char * ___restrict___ str1, const String *string2);
int string_replace_str_by_c_all(String * ___restrict___ string, const char * ___restrict___ str1, char c2);
int string_replace_str_by_str_all(String * ___restrict___ string, const char *str1, const char *str);
int string_replace_str_by_string_all(String *string, const char * ___restrict___ str1, const String *string2);


/*
    Replace first / last / all occurrene @string1 by char / char * / string

    PARAMS
    @IN string - pointer to String
    @IN string1 - pointer to String to replace
    @IN c / str / string2 - replacing pattern

    RETURN
    0 iff success
    Non-zero value iff failure
*/
int string_replace_string_by_c_first(String *string, const String *string1, char c);
int string_replace_string_by_str_first(String *string, const String *string1, const char * ___restrict___ str);
int string_replace_string_by_string_first(String *string, const String *string1, const String *string2);
int string_replace_string_by_c_last(String *string, const String *string1, char c);
int string_replace_string_by_str_last(String *string, const String *string1, const char * ___restrict___ str);
int string_replace_string_by_string_last(String *string, const String *string1, const String *string2);
int string_replace_string_by_c_all(String *string, const String *string1, char c);
int string_replace_string_by_str_all(String *string, const String *string1, const char * ___restrict___ str);
int string_replace_string_by_string_all(String *string, const String *string1, const String *string2);

/*
    Split String to array of String wchich are separated by char / char * / String
    in orginal @string

    PARAMS
    @IN string - string to split
    @IN c / str / string - delimeter
    @OUT size - size of array of String after splitting

    RETURN
    NULL iff failure
    Pointer to array iff success
*/
String **string_split_c(const String *string, char c, size_t *size);
String **string_split_str(const String * ___restrict___ string, const char * ___restrict___ str, size_t * ___restrict___ size);
String **string_split_string(const String *string, const String *string2, size_t *size);

#endif