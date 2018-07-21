#include <cstring.h>
#include <stdlib.h>
#include <compiler.h>
#include <common.h>
#include <log.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <darray.h>

struct String
{
    char *str;
    size_t len;
    size_t allocated;
};

struct String_iterator
{
    char *str;
    ssize_t index;
};

#define INIT_BYTES  BIT(7)

/* Macro to calculate size to alloc ( power of two ) from bytes */
#define __string_calc_size(bytes) \
    __extension__ \
    ({ \
        size_t __size = BIT(LOG2_long((unsigned long)(bytes - 1)) + 1); \
        __size < INIT_BYTES ? INIT_BYTES : __size; \
    })

/* macro to create string from primitive type (variable n) and with printf format (fmt) */
#define __string_create_from(n, fmt, bytes) \
    do { \
        String *s; \
        ssize_t ret; \
        \
        TRACE(); \
        \
        s = __string_create(__string_calc_size(bytes)); \
        if (s == NULL) \
            ERROR("string_create error\n", NULL); \
        \
        ret = snprintf(s->str, s->allocated, fmt, n); \
        if (ret < 0 || ret >= (ssize_t)s->allocated) \
        { \
            string_destroy(s); \
            ERROR("snprintf error\n", NULL); \
        } \
        \
        s->len += (size_t)ret; \
        \
        return s; \
    } while (0)

/* Macro to check params for deletion */
#define __string_delete_check_params(string, pos, chars) \
    do { \
        if (string == NULL) \
            ERROR("string == NULL\n", 1); \
        \
        if (pos >= string->len) \
            ERROR("Invalid pos\n", 1); \
        \
        if (chars > string->len) \
            ERROR("String is to short\n", 1); \
        \
        if (chars == 0) \
            return 0; \
    } while (0)


/*
    If string need bigger / smaller memory chunk, this function
    realloc private data of @s object to another mem chunk

    PARAMS
    @IN s - pointer to String
    @IN new_len - new len of @s string

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static ___inline___ int __string_realloc(String *s, size_t new_len);

/*
    Delete @bytes from String, realloc to packed in another smaller mem chunk if needed

    PARAMS
    @IN s - pointer to String
    @IN pos - position from we delete chars
    @IN bytes - bytes to cut from string

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static ___inline___ int _string_delete(String *string, size_t pos, size_t chars);

/*
    Delete @bytes from String
    PARAMS
    @IN s - pointer to String
    @IN pos - position from we delete chars
    @IN bytes - bytes to cut from string

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static ___inline___ int _string_delete_wo_realloc(String *string, size_t pos, size_t chars);

/*
    Create string with @bytes size

    PARAMS
    @IN bytes - allocated size

    RETURN
    NULL iff failure
    Pointer to new String iff success
*/
static ___inline___ String *__string_create(size_t bytes);

/*
   Private function of _string_delete, so check upper description
*/
static ___inline___ int __string_delete(String *s, size_t pos, size_t bytes);

/*
   Private function of _string_delete_wo_realloc, so check upper description
*/
static ___inline___ int __string_delete_wo_realloc(String *s, size_t pos, size_t bytes);

/*
    Insert @str to @string in @pos

    PARAMS
    @IN s - pointer to String
    @IN str - C legacy string
    @IN pos - position to insert
    @IN len - str len

    RETURN
    0 iff success
    Non-zero value iff failure
*/
static ___inline___ int __string_insert(String * ___restrict___ s, const char * ___restrict___ str, size_t pos, size_t len);

/*
    Create states for Knuth-Morris-Pratt algorithm

    PARAMS
    @IN pattern - pattern

    RETURN
    NULL iff failure
    States iff success
*/
static ssize_t *kmp_states_create(const char *pattern);

/*
    Knuth-Morris-Pratt algorithm

    PARAMS
    @IN text - text
    @IN pattern - pattern
    @IN first - want first occurrence ?

    RETURN
    -1 iff text hasn't pattern
    index of pattern begining in text iff success
*/
static ssize_t kmp(const char *text, const char *pattern, bool first);

/*
    Find pattern in string

    PARAMS
    @IN string - pointer to string
    @IN pattern - pattern to find
    @IN begin - begin string pos
    @IN end - end string pos
    @IN first - want you first occurrence ?

    RETURN
    -1 iff text hasn't pattern
    index of pattern begining in text iff success
*/
static ___inline___ ssize_t __string_find_str(const String * ___restrict___ string, const char * ___restrict___ pattern, size_t begin, size_t end, bool first);

/*
    Find pattern in string

    PARAMS
    @IN string - pointer to string
    @IN c - char to find
    @IN begin - begin string pos
    @IN end - end string pos
    @IN first - want you first occurrence ?

    RETURN
    -1 iff text hasn't pattern
    index of pattern begining in text iff success
*/
static ___inline___ ssize_t __string_find_c(const String *string, char c, size_t begin, size_t end, bool first);

static ___inline___ int __string_realloc(String *s, size_t new_len)
{
    size_t size;

    TRACE();

    if (s == NULL)
        ERROR("s == NULL\n", 1);

    size = __string_calc_size(new_len);
    if (size != s->allocated && size >= INIT_BYTES)
    {
        s->str = (char *)realloc(s->str, size);
        if (s->str == NULL)
            ERROR("realloc error\n", 1);

        s->allocated = size;
    }

    return 0;
}

static ___inline___ int _string_delete(String *string, size_t pos, size_t chars)
{
    TRACE();

    __string_delete_check_params(string, pos, chars);

    return __string_delete(string, pos, chars);
}

static ___inline___ int _string_delete_wo_realloc(String *string, size_t pos, size_t chars)
{
    TRACE();

    __string_delete_check_params(string, pos, chars);

    return __string_delete_wo_realloc(string, pos, chars);
}

static ___inline___ String *__string_create(size_t bytes)
{
    String *s;

    TRACE();

    s = malloc(sizeof(String));
    if (s == NULL)
        ERROR("malloc error\n", NULL);

    s->str = (char *)malloc(bytes);
    if (s->str == NULL)
    {
        FREE(s);
        ERROR("malloc error\n", NULL);
    }

    s->len = 0;
    s->allocated = bytes;

    return s;
}

static ___inline___ int __string_delete(String *s, size_t pos, size_t bytes)
{
    size_t new_len;

    TRACE();

    if (bytes == 0)
        return 0;

    if (bytes > s->len)
        ERROR("Cannot cut bytes, string is to short\n", 1);

    new_len = s->len - bytes;

    if (pos + bytes < s->len)
        if (memmove((void *)(s->str + pos), (void *)(s->str + pos + bytes), new_len - pos) == NULL)
            ERROR("memmove error\n", 1);

    __string_realloc(s, new_len);

    s->len = new_len;
    s->str[s->len] = '\0';

    return 0;
}

static ___inline___ int __string_delete_wo_realloc(String *s, size_t pos, size_t bytes)
{
    size_t new_len;

    TRACE();

    if (bytes == 0)
        return 0;

    if (bytes > s->len)
        ERROR("Cannot cut bytes, string is to short\n", 1);

    new_len = s->len - bytes;

    if (pos + bytes < s->len)
        if (memmove((void *)(s->str + pos), (void *)(s->str + pos + bytes), new_len - pos) == NULL)
            ERROR("memmove error\n", 1);

    s->len = new_len;
    s->str[s->len] = '\0';

    return 0;
}

static ___inline___ int __string_insert(String * ___restrict___ s, const char * ___restrict___ str, size_t pos, size_t len)
{
    size_t new_len;

    TRACE();

    if (len == 0)
        return 0;

    if (pos > s->len)
        ERROR("Invalid pos\n", 1);

    new_len = s->len + len;
    __string_realloc(s, new_len);

    if (pos < s->len)
        if (memmove((void *)(s->str + pos + len), (void *)(s->str + pos), s->len - pos) == NULL)
            ERROR("memmove error\n", 1);

    if (memcpy((void *)(s->str + pos), (void *)str, len) == NULL)
        ERROR("memcpy error\n", 1);

    s->len += len;
    s->str[s->len] = '\0';

    return 0;
}

static ssize_t *kmp_states_create(const char *pattern)
{
    ssize_t *states;
    size_t i;
    size_t len;
    ssize_t longest_prefix;

    TRACE();

    if(pattern == NULL)
        ERROR("pattern == NULL\n", NULL);

    len = strlen(pattern);
    if (len == 0)
        ERROR("String is empty\n", NULL);

    states = (ssize_t *)malloc(sizeof(ssize_t) * len);
    if (states == NULL)
        ERROR("malloc error\n", NULL);

    /* start without prefix */
    longest_prefix = -1;

    /* 1 char has empty char as longest prefix */
    states[0] = longest_prefix;

    for (i = 1; i < len; ++i)
    {
		/* go back while pattern doesn't match */
      	while (longest_prefix > -1 && pattern[longest_prefix + 1] != pattern[i])
            longest_prefix = states[longest_prefix];

        /*
            here longest_prefix == 0 || pattern[i] == pattern[longest_prefix]
            if pattern matches we can extends prefix
        */
        if (pattern[i] == pattern[longest_prefix + 1])
            ++longest_prefix;

        /* we save state as longest_prefix */
        states[i] = longest_prefix;
    }

    return states;
}

ssize_t kmp(const char *text, const char *pattern, bool first)
{
    ssize_t *states;
	ssize_t text_len;
	ssize_t pattern_len;
	ssize_t i;
	ssize_t matches;
    ssize_t end = -1;

	TRACE();

    if (text == NULL)
        ERROR("text == NULL\n", -1);

    if (pattern == NULL)
        ERROR("pattern == NULL\n", -1);

    states = kmp_states_create(pattern);
    if(states == NULL)
        ERROR("kmp_states_create error\n", -1);

    text_len = (ssize_t)strlen(text);
    pattern_len = (ssize_t)strlen(pattern);

    matches = -1;
    for (i = 0; i < text_len; ++i)
    {
        /* go back while pattern doesn't match text*/
        while (matches > -1 && pattern[matches + 1] != text[i])
		    matches = states[matches];

        /* if matches then we can extends matched string*/
		if (pattern[matches + 1] == text[i])
			++matches;

		/* we find pattern in text */
		if (matches == pattern_len - 1)
        {
            end = i - pattern_len + 1;
            if (first)
                break;
            else /* else needed beacuse newest compiler has warn here */
        	    matches = states[matches];
		}
    }

    FREE(states);

    return end;
}

static ___inline___ ssize_t __string_find_str(const String * ___restrict___ string, const char * ___restrict___ pattern, size_t begin, size_t end, bool first)
{
    ssize_t ret;
    char c;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    if (end < begin)
        ERROR("end < begin\n", -1);

    if (begin > string->len || end > string->len)
        ERROR("begin or end grater than string len\n", -1);

    c = string->str[end];
    string->str[end] = '\0';
    ret = kmp(string->str + begin, pattern, first);
    string->str[end] = c;

    if (ret == -1)
      return -1;

    return ret + (ssize_t)begin;
}

static ___inline___ ssize_t __string_find_c(const String *string, char c, size_t begin, size_t end, bool first)
{
    ssize_t i;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    if (end < begin)
        ERROR("end < begin\n", -1);

    if (begin >= string->len || end > string->len)
        ERROR("begin or end grater than string len\n", -1);

    if (first)
    {
        for (i = (ssize_t)begin; i <= (ssize_t)end; ++i)
            if (string->str[i] == c)
                return i;
    }
    else
    {
        for (i = (ssize_t)end; i >= (ssize_t)begin; --i)
            if (string->str[i] == c)
                return i;
    }

    return -1;
}

String *string_create(void)
{
    TRACE();

    return __string_create(INIT_BYTES);
}

void string_destroy(String *string)
{
    TRACE();

    if (string == NULL)
        return;

    FREE(string->str);
    FREE(string);
}

bool string_is_empty(const String *string)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", true);

    return string->len == 0;
}

char *string_get_str(const String *string)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", NULL);

    return string->str;
}

ssize_t string_get_length(const String *string)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    return (ssize_t)string->len;
}

String *string_create_from_c(char c)
{
   __string_create_from(c, "%c", INIT_BYTES);
}

String *string_create_from_str(const char *str)
{
    if (str == NULL)
        ERROR("str == NULL\n", NULL);

   __string_create_from(str, "%s", strlen(str));
}

String *string_create_from_i(int n)
{
    __string_create_from(n, "%d", INIT_BYTES);
}

String *string_create_from_ui(unsigned int n)
{
    __string_create_from(n, "%u", INIT_BYTES);
}

String *string_create_from_l(long n)
{
    __string_create_from(n, "%ld", INIT_BYTES);
}

String *string_create_from_ul(unsigned long n)
{
    __string_create_from(n, "%lu", INIT_BYTES);
}

String *string_create_from_ll(long long n)
{
    __string_create_from(n, "%lld", INIT_BYTES);
}

String *string_create_from_ull(unsigned long long n)
{
    __string_create_from(n, "%llu", INIT_BYTES);
}

String *string_create_from_f(float n)
{
    __string_create_from(n, "%f", INIT_BYTES);
}

String *string_create_from_d(double n)
{
    __string_create_from(n, "%lf", INIT_BYTES);
}

String *string_create_from_ld(long double n)
{
    __string_create_from(n, "%Lf", INIT_BYTES);
}

String *string_concat(const String *s1, const String *s2)
{
    String *s;
    size_t bytes;
    ssize_t ret;

    TRACE();

    if (string_is_empty(s1) || string_is_empty(s2))
        ERROR("One of string is empty\n", NULL);

    bytes = __string_calc_size(s1->len + s2->len);

    s = __string_create(bytes);
    if (s == NULL)
        ERROR("__string_create error\n", NULL);

    ret = snprintf(s->str, s->allocated, "%s%s", s1->str, s2->str);
    if (ret < 0 || ret >= (ssize_t)s->allocated)
    {
        FREE(s);
        ERROR("snprintf error\n", NULL);
    }

    s->len += (size_t)ret;

    return s;
}

char string_get_char(const String *string, size_t index)
{
    TRACE();

    if (index >= string->len)
        ERROR("Incorrect index\n", -1);

    return string->str[index];
}

int string_set_char(String *string, size_t index, char c)
{
    TRACE();

    if (index >= string->len)
        ERROR("Incorrect index\n", 1);

    string->str[index] = c;

    return 0;
}

int string_toupper(String *string)
{
    size_t i;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    for (i = 0; i < string->len; ++i)
        string->str[i] = (char)toupper((char)string->str[i]);

    return 0;
}

int string_tolower(String *string)
{
    size_t i;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    for (i = 0; i < string->len; ++i)
        string->str[i] = (char)tolower((char)string->str[i]);

    return 0;
}

int string_trim(String *string)
{
    ssize_t i;
    size_t begin;
    size_t end;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    /* Calculate spaces to delete from begining */
    i = 0;
    begin = 0;
    while (i < (ssize_t)string->len && isspace(string->str[i]))
    {
        ++i;
        ++begin;
    }

    /* Calculate spaces to delete from end */
    i = (ssize_t)(string->len - 1);
    end = 0;
    while (i >= 0  && isspace(string->str[i]))
    {
        --i;
        ++end;
    }

    /* delete whitespaces */
    if (__string_delete(string, 0, begin))
        ERROR("__string_delete error\n", 1);

    if (__string_delete(string, string->len - end, end))
         ERROR("__string_delete error\n", 1);

    return 0;
}

int string_cmp(const String *s1, const String *s2)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", 0);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", 0);

    return strcmp(s1->str, s2->str);
}

String *string_substr(const String *string, size_t begin, size_t end)
{
    String *s;
    size_t bytes;
    ssize_t ret;

    TRACE();

    if (string == NULL)
        ERROR("string == NLL\n", NULL);

    if (end < begin)
        ERROR("end < begin\n", NULL);

    if (begin >= string->len || end >= string->len)
        ERROR("begin or end grater than string len\n", NULL);

    bytes = __string_calc_size(end - begin + 1);

    s = __string_create(bytes);
    if (s == NULL)
        ERROR("__string_create error\n", NULL);

    ret = snprintf(s->str, s->allocated, "%.*s", (int)(end - begin + 1), string->str + begin);
    if (ret < 0 || ret >= (ssize_t)s->allocated)
    {
        FREE(s);
        ERROR("snprintf error\n", NULL);
    }

    s->len = (size_t)ret;

    return s;
}

ssize_t string_find_c_first(const String *string, char c)
{
    if (string == NULL)
        ERROR("string == NULL\n", -1);

    if (string_is_empty(string))
        ERROR("string is empty\n", -1);

    return __string_find_c(string, c, 0, string->len, true);
}

ssize_t string_find_str_first(const String * ___restrict___ string, const char * ___restrict___ str)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    if (str == NULL)
        ERROR("str == NULL\n", -1);

    if (string_is_empty(string))
        ERROR("string is empty\n", -1);

    if (strlen(str) == 0)
        ERROR("pattern is empty\n", -1);

    return __string_find_str(string, str, 0, string->len, true);
}

ssize_t string_find_string_first(const String *s1, const String *s2)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", -1);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", -1);

    if (string_is_empty(s1))
        ERROR("string is empty\n", -1);

    if (string_is_empty(s2))
        ERROR("pattern is empty\n", -1);

    return __string_find_str(s1, s2->str, 0, s1->len, true);
}

ssize_t string_find_c_last(const String *string, char c)
{
    if (string == NULL)
        ERROR("string == NULL\n", -1);

    if (string_is_empty(string))
        ERROR("string is empty\n", -1);

    return __string_find_c(string, c, 0, string->len, false);
}

ssize_t string_find_str_last(const String * ___restrict___ string, const char * ___restrict___ str)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    if (str == NULL)
        ERROR("str == NULL\n", -1);

    if (string_is_empty(string))
        ERROR("string is empty\n", -1);

    if (strlen(str) == 0)
        ERROR("pattern is empty\n", -1);

    return __string_find_str(string, str, 0, string->len, false);
}

ssize_t string_find_string_last(const String *s1, const String *s2)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", -1);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", -1);

    if (string_is_empty(s1))
        ERROR("string is empty\n", -1);

    if (string_is_empty(s2))
        ERROR("pattern is empty\n", -1);

    return __string_find_str(s1, s2->str, 0, s1->len, false);
}

ssize_t string_find_c_first_substr(const String *string, char c, size_t begin, size_t end)
{
    ssize_t ret;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    ret = __string_find_c(string, c, begin, end + 1, true);
    if (ret == -1)
        ERROR("__string_find_c\n", -1);

    return ret;
}

ssize_t string_find_str_first_substr(const String * ___restrict___ string, const char * ___restrict___ str, size_t begin, size_t end)
{
    ssize_t ret;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    ret = __string_find_str(string, str, begin, end + 1, true);
    if (ret == -1)
        ERROR("__string_find_str\n", -1);

    return ret;
}

ssize_t string_find_string_first_substr(const String *s1, const String *s2, size_t begin, size_t end)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", -1);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", -1);

    return string_find_str_first_substr(s1, s2->str, begin, end + 1);
}

ssize_t string_find_c_last_substr(const String *string, char c, size_t begin, size_t end)
{
     ssize_t ret;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    ret = __string_find_c(string, c, begin, end + 1, false);
    if (ret == -1)
        ERROR("__string_find_c\n", -1);

    return ret;
}

ssize_t string_find_str_last_substr(const String * ___restrict___ string, const char * ___restrict___ str, size_t begin, size_t end)
{
    ssize_t ret;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", -1);

    ret = __string_find_str(string, str, begin, end + 1, false);
    if (ret == -1)
        ERROR("__string_find_str\n", -1);

    return ret;
}

ssize_t string_find_string_last_substr(const String *s1, const String *s2, size_t begin, size_t end)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", -1);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", -1);

    return string_find_str_last_substr(s1, s2->str, begin, end + 1);
}

String *string_clone(const String *string)
{
    String *s;
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", NULL);

    s = __string_create(string->allocated);
    if (s == NULL)
        ERROR("__string_create error\n", NULL);

    s->len = string->len;
    if (memcpy((void *)s->str, (void *)string->str, string->len + 1) == NULL)
    {
        string_destroy(s);
        ERROR("memcpy error\n", NULL);
    }

    return s;
}

int string_delete(String *string, size_t pos, size_t chars)
{
    TRACE();

    return _string_delete(string, pos, chars);
}

int string_cut(String *string, size_t chars)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (chars > string->len)
        ERROR("String is to short\n", 1);

    if (chars == 0)
        return 0;

    return __string_delete(string, string->len - chars, chars);
}

int string_append_c(String *string, char c)
{
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    buf[0] = c;
    buf[1] = '\0';

    return __string_insert(string, buf, string->len, 1);
}

int string_append_str(String * ___restrict___ string, const char * ___restrict___ str)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    return __string_insert(string, str, string->len, strlen(str));
}

int string_append_string(String *s1, const String *s2)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", 1);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", 1);

    return __string_insert(s1, s2->str, s1->len, s2->len);
}

int string_insert_c(String *string, char c, size_t pos)
{
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (pos > string->len)
        ERROR("Invalid pos\n", 1);

    buf[0] = c;
    buf[1] = '\0';

    return __string_insert(string, buf, pos, 1);
}

int string_insert_str(String * ___restrict___ string, const char * ___restrict___ str, size_t pos)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (pos > string->len)
        ERROR("Invalid pos\n", 1);

    return __string_insert(string, str, pos, strlen(str));
}

int string_insert_string(String *s1, const String *s2, size_t pos)
{
    TRACE();

    if (s1 == NULL)
        ERROR("s1 == NULL\n", 1);

    if (s2 == NULL)
        ERROR("s2 == NULL\n", 1);

    if (pos > s1->len)
        ERROR("Invalid pos\n", 1);

    return __string_insert(s1, s2->str, pos, s2->len);
}

int string_remove_c_first(String *string, char c)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    pos = string_find_c_first(string, c);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    return string_delete(string, (size_t)pos, 1);
}

int string_remove_c_last(String *string, char c)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    pos = string_find_c_last(string, c);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    return string_delete(string, (size_t)pos, 1);
}

int string_remove_c_all(String *string, char c)
{
    ssize_t pos;
    size_t offset;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);


    offset = 0;
    while (offset < string->len && !string_is_empty(string) && ((pos = __string_find_c(string, c, offset, string->len, true)) != -1))
    {
        offset = (size_t)pos;
        if (_string_delete_wo_realloc(string, (size_t)pos, 1))
            ERROR("string_delete error\n", 1);
    }

    return __string_realloc(string, string->len);
}

int string_remove_str_first(String * ___restrict___ string, const char * ___restrict___ str)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    pos = string_find_str_first(string, str);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    return string_delete(string, (size_t)pos, strlen(str));
}

int string_remove_str_last(String * ___restrict___ string, const char * ___restrict___ str)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    pos = string_find_str_last(string, str);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    return string_delete(string, (size_t)pos, strlen(str));
}

int string_remove_str_all(String * ___restrict___ string, const char * ___restrict___ str)
{
    ssize_t pos;
    size_t offset;
    size_t str_len;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    str_len = strlen(str);

    offset = 0;
    while (offset < string->len && !string_is_empty(string) && ((pos = __string_find_str(string, str, offset, string->len, true)) != -1))
    {
        offset = (size_t)pos;

        if (_string_delete_wo_realloc(string, (size_t)pos, str_len))
            ERROR("string_delete error\n", 1);
    }

    return __string_realloc(string, string->len);
}

int string_remove_string_first(String *string, const String *pattern)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (pattern == NULL)
        ERROR("pattern == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    return string_remove_str_first(string, pattern->str);
}

int string_remove_string_last(String *string, const String *pattern)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (pattern == NULL)
        ERROR("pattern == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    return string_remove_str_last(string, pattern->str);
}

int string_remove_string_all(String *string, const String *pattern)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (pattern == NULL)
        ERROR("pattern == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    return string_remove_str_all(string, pattern->str);
}

int string_replace_c_by_c_first(String *string, char c1, char c2)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (c1 == c2)
        return 0;

    pos = string_find_c_first(string, c1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    return string_set_char(string, (size_t)pos, c2);
}

int string_replace_c_by_c_last(String *string, char c1, char c2)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (c1 == c2)
        return 0;

    pos = string_find_c_last(string, c1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    return string_set_char(string, (size_t)pos, c2);
}

int string_replace_c_by_c_all(String *string, char c1, char c2)
{
    ssize_t pos;
    size_t offset;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (c1 == c2)
        return 0;

    offset = 0;
    while (offset < string->len && !string_is_empty(string) && ((pos = __string_find_c(string, c1, offset, string->len, true)) != -1))
    {
        offset = (size_t)pos + 1;

        if (string_set_char(string, (size_t)pos, c2))
            ERROR("string_set_char error\n", 1);
    }

    return 0;
}

int string_replace_c_by_str_first(String *  ___restrict___ string, char c1, const char * ___restrict___ str)
{
    ssize_t pos;
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str) == 0)
        ERROR("Please use remove instead of replace by empty string\n", 1);

    buf[0] = c1;
    buf[1] = '\0';

    if (strcmp(buf, str) == 0)
        return 0;

    pos = string_find_c_first(string, c1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    if (_string_delete(string, (size_t)pos, 1))
        ERROR("_string_delete error\n", 1);

    return string_insert_str(string, str, (size_t)pos);
}

int string_replace_c_by_str_last(String * ___restrict___ string, char c1, const char * ___restrict___ str)
{
    ssize_t pos;
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str) == 0)
        ERROR("Please use remove instead of replace by empty string\n", 1);

    buf[0] = c1;
    buf[1] = '\0';

    if (strcmp(buf, str) == 0)
        return 0;

    pos = string_find_c_last(string, c1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    if (_string_delete(string, (size_t)pos, 1))
        ERROR("_string_delete error\n", 1);

    return string_insert_str(string, str, (size_t)pos);
}

int string_replace_c_by_str_all(String * ___restrict___ string, char c1, const char * ___restrict___ str)
{
    ssize_t pos;
    size_t offset;

    char buf[2];
    size_t str_len;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str == NULL)
        ERROR("str == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    str_len = strlen(str);
    if (str_len == 0)
        ERROR("Please use remove instead of replace by empty string\n", 1);

    buf[0] = c1;
    buf[1] = '\0';

    if (strcmp(buf, str) == 0)
        return 0;

    offset = 0;
    while (offset < string->len && !string_is_empty(string) && ((pos = __string_find_c(string, c1, offset, string->len, true)) != -1))
    {
        if (_string_delete(string, (size_t)pos, 1))
            ERROR("_string_delete error\n", 1);

        offset = (size_t)pos;

        if (string_insert_str(string, str, (size_t)pos))
            ERROR("string_set_char error\n", 1);
    }

    return 0;
}

int string_replace_c_by_string_first(String *string, char c1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (string_is_empty(string2))
        ERROR("Please use remove instead of replace by empty string\n", 1);

    return string_replace_c_by_str_first(string, c1, string2->str);
}

int string_replace_c_by_string_last(String *string, char c1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (string_is_empty(string2))
        ERROR("Please use remove instead of replace by empty string\n", 1);

    return string_replace_c_by_str_last(string, c1, string2->str);
}

int string_replace_c_by_string_all(String *string, char c1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (string_is_empty(string2))
        ERROR("Please use remove instead of replace by empty string\n", 1);

    return string_replace_c_by_str_all(string, c1, string2->str);
}

int string_replace_str_by_c_first(String * ___restrict___ string, const char * ___restrict___ str1, char c)
{
    ssize_t pos;
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    buf[0] = c;
    buf[1] = '\0';

    if (strcmp(buf, str1) == 0)
        return 0;

    pos = string_find_str_first(string, str1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    if (_string_delete(string, (size_t)pos, strlen(str1)))
        ERROR("_string_delete error\n", 1);

    return string_insert_c(string, c, (size_t)pos);
}

int string_replace_str_by_c_last(String * ___restrict___ string, const char * ___restrict___ str1, char c)
{
    ssize_t pos;
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    buf[0] = c;
    buf[1] = '\0';
    if (strcmp(buf, str1) == 0)
        return 0;

    pos = string_find_str_last(string, str1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    if (_string_delete(string, (size_t)pos, strlen(str1)))
        ERROR("_string_delete error\n", 1);

    return string_insert_c(string, c, (size_t)pos);
}

int string_replace_str_by_c_all(String * ___restrict___ string, const char * ___restrict___ str1, char c)
{
    ssize_t pos;
    size_t len;
    size_t offset;
    char buf[2];

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    len = strlen(str1);
    if (len == 0)
        ERROR("str1 is empty\n", 1);

    buf[0] = c;
    buf[1] = '\0';
    if (strcmp(buf, str1) == 0)
        return 0;

    offset = 0;
    while (offset < string->len && !string_is_empty(string) && ((pos = __string_find_str(string, str1, offset, string->len, true)) != -1))
    {
        offset = (size_t)pos + 1;
        if (_string_delete(string, (size_t)pos, len))
            ERROR("_string_delete error\n", 1);

        if (string_insert_c(string, c, (size_t)pos))
            ERROR("string_insert_c error\n", 1);
    }

    return 0;
}

int string_replace_str_by_str_first(String * ___restrict___ string, const char *str1, const char *str2)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (str2 == NULL)
        ERROR("str2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    if (strlen(str2) == 0)
        ERROR("Please use remove instead of replace by empty string\n", 1);

    if (strcmp(str1, str2) == 0)
        return 0;

    pos = string_find_str_first(string, str1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    if (_string_delete(string, (size_t)pos, strlen(str1)))
        ERROR("_string_delete error\n", 1);

    return string_insert_str(string, str2, (size_t)pos);
}

int string_replace_str_by_str_last(String * ___restrict___ string, const char *str1, const char *str2)
{
    ssize_t pos;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (str2 == NULL)
        ERROR("str2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    if (strlen(str2) == 0)
        ERROR("Please use remove instead of replace by empty string\n", 1);

    if (strcmp(str1, str2) == 0)
        return 0;

    pos = string_find_str_last(string, str1);
    if (pos == -1)
        ERROR("Pattern doesn't match in string\n", 1);

    if (_string_delete(string, (size_t)pos, strlen(str1)))
        ERROR("_string_delete error\n", 1);

    return string_insert_str(string, str2, (size_t)pos);
}

int string_replace_str_by_str_all(String * ___restrict___ string, const char *str1, const char *str2)
{
    ssize_t pos;
    size_t len1;
    size_t len2;
    size_t offset;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (str2 == NULL)
        ERROR("str2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    len1 = strlen(str1);
    if (len1 == 0)
        ERROR("str1 is empty\n", 1);

    len2 = strlen(str2);
    if (len2 == 0)
        ERROR("Please use remove instead of replace by empty string\n", 1);

    if (strcmp(str1, str2) == 0)
        return 0;

    offset = 0;
    while (offset < string->len && !string_is_empty(string) && ((pos = __string_find_str(string, str1, offset, string->len, true)) != -1))
    {
        offset = (size_t)pos;

        if (_string_delete(string, (size_t)pos, len1))
            ERROR("_string_delete error\n", 1);

        if (string_insert_str(string, str2, (size_t)pos))
            ERROR("string_insert_str error\n", 1);

        offset += (size_t)ABS((ssize_t)(len1 - len2));
    }

    return 0;
}

int string_replace_str_by_string_first(String *string, const char * ___restrict___ str1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    if (string_is_empty(string2))
        ERROR("Please use remove instead of replace by empty string\n", 1);

    if (strcmp(str1, string2->str) == 0)
        return 0;

    return string_replace_str_by_str_first(string, str1, string2->str);
}

int string_replace_str_by_string_last(String *string, const char * ___restrict___ str1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    if (string_is_empty(string2))
        ERROR("Please use remove instead of replace by empty string\n", 1);

    if (strcmp(str1, string2->str) == 0)
        return 0;

    return string_replace_str_by_str_last(string, str1, string2->str);
}

int string_replace_str_by_string_all(String *string, const char * ___restrict___ str1, const String *string2)
{
    TRACE();

     if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (str1 == NULL)
        ERROR("str1 == NULL\n", 1);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", 1);

    if (string_is_empty(string))
        ERROR("String is empty\n", 1);

    if (strlen(str1) == 0)
        ERROR("str1 is empty\n", 1);

    if (string_is_empty(string2))
        ERROR("Please use remove instead of replace by empty string\n", 1);

    if (strcmp(str1, string2->str) == 0)
        return 0;

    return string_replace_str_by_str_all(string, str1, string2->str);
}

int string_replace_string_by_c_first(String *string, const String *string1, char c)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_c_first(string, string1->str, c);
}

int string_replace_string_by_c_last(String *string, const String *string1, char c)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_c_last(string, string1->str, c);
}

int string_replace_string_by_c_all(String *string, const String *string1, char c)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_c_all(string, string1->str, c);
}

int string_replace_string_by_str_first(String *string, const String *string1, const char * ___restrict___ str)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_str_first(string, string1->str, str);
}

int string_replace_string_by_str_last(String *string, const String *string1, const char * ___restrict___ str)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_str_last(string, string1->str, str);
}

int string_replace_string_by_str_all(String *string, const String *string1, const char * ___restrict___ str)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_str_all(string, string1->str, str);
}

int string_replace_string_by_string_first(String *string, const String *string1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_string_first(string, string1->str, string2);
}

int string_replace_string_by_string_last(String *string, const String *string1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_string_last(string, string1->str, string2);
}

int string_replace_string_by_string_all(String *string, const String *string1, const String *string2)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", 1);

    if (string1 == NULL)
        ERROR("string1 == NULL\n", 1);

    return string_replace_str_by_string_all(string, string1->str, string2);
}

String **string_split_c(const String *string, char c, size_t *size)
{
    ssize_t pos;
    size_t i;
    Darray *darray;
    String **array;
    String *temp;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", NULL);

    if (size == NULL)
        ERROR("size == NULL\n", NULL);

    if (string_is_empty(string))
        ERROR("string is empty\n", NULL);

    darray = darray_create(DARRAY_UNSORTED, 0, sizeof(String *), NULL, NULL);
    if (darray == NULL)
        ERROR("darray == NULL\n", NULL);

    i = 0;
    while (i < string->len)
    {
        pos = __string_find_c(string, c, i, string->len, true);
        if (pos > 0 && pos < (ssize_t)string->len)
        {
            temp = string_substr(string, i, (size_t)pos - 1);
            if (temp == NULL)
                ERROR("string_substr error\n", NULL);

            if (darray_insert(darray, (void *)&temp))
                ERROR("darray insert error\n", NULL);
        }
        else if (pos == -1)
        {
            temp = string_substr(string, i, string->len - 1);
            if (temp == NULL)
                ERROR("string_substr error\n", NULL);

            if (darray_insert(darray, (void *)&temp))
                ERROR("darray insert error\n", NULL);

            break;
        }

        i = (size_t)pos + 1;
    }

    array = (String **)malloc(sizeof(String *) * (size_t)darray_get_num_entries(darray));
    if (array == NULL)
        ERROR("malloc error\n", NULL);

    i = 0;
    for_each_data(darray, Darray, temp)
        array[i++] = temp;

    *size = i;

    darray_destroy(darray);

    return array;
}

String **string_split_str(const String *  ___restrict___ string, const char * ___restrict___ str, size_t * ___restrict___ size)
{
    ssize_t pos;
    size_t i;
    size_t len;
    Darray *darray;
    String **array;
    String *temp;

    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", NULL);

    if (str == NULL)
        ERROR("str == NULL\n", NULL);

    if (size == NULL)
        ERROR("size == NULL\n", NULL);

    if (string_is_empty(string))
        ERROR("string is empty\n", NULL);

    len = strlen(str);
    if (len == 0)
        ERROR("len == 0\n", NULL);

    darray = darray_create(DARRAY_UNSORTED, 0, sizeof(String *), NULL, NULL);
    if (darray == NULL)
        ERROR("darray == NULL\n", NULL);

    i = 0;
    while (i < string->len)
    {
        pos = __string_find_str(string, str, i, string->len, true);
        if (pos > 0 && pos < (ssize_t)string->len)
        {
            temp = string_substr(string, i, (size_t)pos - 1);
            if (temp == NULL)
                ERROR("string_substr error\n", NULL);

            if (darray_insert(darray, (void *)&temp))
                ERROR("darray insert error\n", NULL);
        }
        else if (pos == -1)
        {
            temp = string_substr(string, i, string->len - 1);
            if (temp == NULL)
                ERROR("string_substr error\n", NULL);

            if (darray_insert(darray, (void *)&temp))
                ERROR("darray insert error\n", NULL);

            break;
        }

        i = (size_t)pos + len;
    }

    array = (String **)malloc(sizeof(String *) * (size_t)darray_get_num_entries(darray));
    if (array == NULL)
        ERROR("malloc error\n", NULL);

    i = 0;
    for_each_data(darray, Darray, temp)
        array[i++] = temp;

    *size = i;

    darray_destroy(darray);

    return array;
}

String **string_split_string(const String *string, const String *string2, size_t *size)
{
    TRACE();

    if (string == NULL)
        ERROR("string == NULL\n", NULL);

    if (string2 == NULL)
        ERROR("string2 == NULL\n", NULL);

    if (size == NULL)
        ERROR("size == NULL\n", NULL);

    if (string_is_empty(string))
        ERROR("string is empty\n", NULL);

    if (string_is_empty(string2))
        ERROR("string2 is empty\n", NULL);

    return string_split_str(string, string2->str, size);
}

int string_reverse(String *str)
{
    size_t i;

    if (str == NULL)
        ERROR("string == NULL\n", 1);

    if (string_is_empty(str))
        return 0;

    for (i = 0; i < str->len / 2; ++i)
        SWAP(str->str[i], str->str[str->len - i - 1]);

    return 0;
}

String_iterator *string_iterator_create(const String *s, iti_mode_t mode)
{
    String_iterator *it;

    TRACE();

    if (s == NULL)
        ERROR("s == NULL\n", NULL);

    if (mode != ITI_BEGIN && mode != ITI_END)
        ERROR("Incorrect mode\n", NULL);

    if (string_is_empty(s))
        return NULL;

    it = (String_iterator *)malloc(sizeof(String_iterator));
    if (it == NULL)
        ERROR("malloc error\n", NULL);

    it->str = s->str;
    if (mode == ITI_BEGIN)
        it->index = 0;
    else
        it->index = (ssize_t)(s->len - 1);

    return it;
}

void string_iterator_destroy(String_iterator *it)
{
    TRACE();

    if (it == NULL)
        return;

    FREE(it);
}

int string_iterator_next(String_iterator *it)
{
    TRACE();

    if (it == NULL)
        ERROR("it == NULL", 1);

    ++it->index;

    return 0;
}

int string_iterator_prev(String_iterator *it)
{
    TRACE();

    if (it == NULL)
        ERROR("it == NULL", 1);

    --it->index;

    return 0;
}

int string_iterator_get_data(const String_iterator * ___restrict___ it, void * ___restrict___ val)
{
    TRACE();

    if (it == NULL)
        ERROR("it == NULL", 1);

    *(char *)val = it->str[it->index];

    return 0;
}

int string_iterator_get_node(const String_iterator * ___restrict___ it, void * ___restrict___ node)
{
    TRACE();

    if (it == NULL)
        ERROR("it == NULL", 1);

    *(void **)node = (it->str + it->index);

    return 0;
}

bool string_iterator_end(const String_iterator *it)
{
    TRACE();

    if (it == NULL)
        ERROR("it == NULL", 1);

    return it->index == -1 || it->str[it->index] == '\0';
}