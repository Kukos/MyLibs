#include <test.h>
#include <cstring.h>
#include <string.h>

test_f test_create(void)
{
    String *s;

    char c = 'C';
    char *str = "kukos";
    int _int = -3;
    unsigned int _uint = 3;
    long _long = -11;
    unsigned long _ulong = 11;
    long long _llong = -17;
    unsigned long long _ullong = 17;
    float _float = -3.14;
    double _double = 3.1415;
    long double _ldouble = 3.141592;

    char *expected;

    s = string_create();
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), 0);
    T_CHECK(string_get_str(s) != NULL);
    string_destroy(s);

    expected = "C";
    s = string_create_from_c(c);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = str;
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "-3";
    s = string_create_from_i(_int);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "3";
    s = string_create_from_ui(_uint);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "-11";
    s = string_create_from_l(_long);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "11";
    s = string_create_from_ul(_ulong);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "-17";
    s = string_create_from_ll(_llong);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "17";
    s = string_create_from_ull(_ullong);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "-3.140000";
    s = string_create_from_f(_float);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "3.141500";
    s = string_create_from_d(_double);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "3.141592";
    s = string_create_from_ld(_ldouble);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);
}

test_f test_concat(void)
{
    String *s1;
    String *s2;
    String *s3;

    char *expected;
    char *str = "Kukos";
    long _long = 7;
    double _double = 3.14;

    expected = str;
    s1 = string_create_from_str(str);
    T_ERROR(s1 == NULL);
    T_EXPECT(string_get_length(s1), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s1), expected), 0);

    str = " jest masterem C";
    expected = str;
    s2 = string_create_from_str(str);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);

    str = "Kukos jest masterem C";
    expected = str;
    s3 = string_concat(s1, s2);
    T_ERROR(s3 == NULL);
    T_EXPECT(string_get_length(s3), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s3), expected), 0);

    string_destroy(s1);
    string_destroy(s2);
    string_destroy(s3);

    expected = "7";
    s1 = string_create_from_l(_long);
    T_ERROR(s1 == NULL);
    T_EXPECT(string_get_length(s1), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s1), expected), 0);

    expected = "3.140000";
    s2 = string_create_from_d(_double);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);

    expected = "73.140000";
    s3 = string_concat(s1, s2);
    T_ERROR(s3 == NULL);
    T_EXPECT(string_get_length(s3), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s3), expected), 0);

    string_destroy(s1);
    string_destroy(s2);
    string_destroy(s3);
}

test_f test_get_set_char(void)
{
    String *s;
    char *expected;
    char *str = "Kukos";
    int i;

    s = string_create();
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), 0);
    T_CHECK(string_get_str(s) != NULL);
    T_CHECK(string_get_char(s, 0) != 0);
    string_destroy(s);

    expected = str;
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    for (i = 0; i < string_get_length(s); ++i)
    {
        T_EXPECT(string_get_char(s, i), expected[i]);
        T_EXPECT(string_set_char(s, i, expected[i] + 1), 0);
        T_EXPECT(string_get_char(s, i), expected[i] + 1);
    }

    T_CHECK(string_get_char(s, string_get_length(s)) != 0);
    T_CHECK(string_set_char(s, string_get_length(s), 'C') != 0);
    string_destroy(s);
}

test_f test_to_lower_upper(void)
{
    String *s;
    char *expected;
    char *str = "Kukos95";

    expected = str;
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "KUKOS95";
    T_EXPECT(string_toupper(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "kukos95";
    T_EXPECT(string_tolower(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_trim(void)
{
    String *s;
    char *expected = "Kukos95";
    char *str = "Kukos95";

    expected = str;
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_trim(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    str = "    Kukos95";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_trim(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    str = "Kukos95     ";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_trim(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    str = "         Kukos95         ";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_trim(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);

    expected = "K  u  k  o  s  9  5";
    str = "         K  u  k  o  s  9  5         ";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);
    T_EXPECT(string_trim(s), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);
}

test_f test_cmp(void)
{
    String *s1;
    String *s2;
    char *str1 = "Kukos";
    char *str2 = "C Master";

    s1 = string_create_from_str(str1);
    T_ERROR(s1 == NULL);

    s2 = string_create_from_str(str2);
    T_ERROR(s2 == NULL);

    T_CHECK(string_cmp(s1, s2) > 0);
    T_CHECK(string_cmp(s2, s1) < 0);

    string_destroy(s1);
    string_destroy(s2);
}

test_f test_substr(void)
{
    String *s;
    String *s2;
    char *str = "Kukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    expected = "Kukos";
    s2 = string_substr(s, 0, strlen(expected) - 1);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);
    string_destroy(s2);

    expected = "Kuk";
    s2 = string_substr(s, 0, strlen(expected) -1);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);
    string_destroy(s2);

    expected = "kos";
    s2 = string_substr(s, 2, strlen(str) -1);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);
    string_destroy(s2);

    expected = "ko";
    s2 = string_substr(s, 2, strlen(str) -2);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);
    string_destroy(s2);

    expected = "k";
    s2 = string_substr(s, 2, 2);
    T_ERROR(s2 == NULL);
    T_EXPECT(string_get_length(s2), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s2), expected), 0);
    string_destroy(s2);

    string_destroy(s);
}

test_f test_find_c(void)
{
    String *s;
    char *str = "Kukos Master C Kukos";

    char c[] = {'z', 'K', 'k', 'C', ' '};
    ssize_t expt_c_f[] = {-1, 0, 2, 13, 5};
    ssize_t expt_c_l[] = {-1, 15, 17, 13, 14};

    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(expt_c_f); ++i)
        T_EXPECT(string_find_c_first(s, c[i]), expt_c_f[i]);

    for (i = 0; i < ARRAY_SIZE(expt_c_l); ++i)
        T_EXPECT(string_find_c_last(s, c[i]), expt_c_l[i]);

    string_destroy(s);
}

test_f test_find_str(void)
{
    String *s;
    char *str = "Kukos Master C Kukos";

    char *strr[] = {"ukos", "er ", "Ala", "koss", "C"};
    ssize_t expt_str_f[] = {1, 10, -1, -1, 13};
    ssize_t expt_str_l[] = {16, 10, -1, -1, 13};

    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(expt_str_f); ++i)
        T_EXPECT(string_find_str_first(s, strr[i]), expt_str_f[i]);

    for (i = 0; i < ARRAY_SIZE(expt_str_l); ++i)
        T_EXPECT(string_find_str_last(s, strr[i]), expt_str_l[i]);


    string_destroy(s);
}

test_f test_find_string(void)
{
    String *s;
    String *temp;
    char *str = "Kukos Master C Kukos";

    char c[] = {'z', 'K', 'k', 'C', ' '};
    ssize_t expt_c_f[] = {-1, 0, 2, 13, 5};
    ssize_t expt_c_l[] = {-1, 15, 17, 13, 14};

    char *strr[] = {"ukos", "er ", "Ala", "koss", "C"};
    ssize_t expt_str_f[] = {1, 10, -1, -1, 13};
    ssize_t expt_str_l[] = {16, 10, -1, -1, 13};

    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(expt_c_f); ++i)
    {
        temp = string_create_from_c(c[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_first(s, temp), expt_c_f[i]);

        string_destroy(temp);
    }

    for (i = 0; i < ARRAY_SIZE(expt_c_l); ++i)
    {
        temp = string_create_from_c(c[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_last(s, temp), expt_c_l[i]);

        string_destroy(temp);
    }

    for (i = 0; i < ARRAY_SIZE(expt_str_f); ++i)
    {
        temp = string_create_from_str(strr[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_first(s, temp), expt_str_f[i]);

        string_destroy(temp);
    }

    for (i = 0; i < ARRAY_SIZE(expt_str_l); ++i)
    {
        temp = string_create_from_str(strr[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_last(s, temp), expt_str_l[i]);

        string_destroy(temp);
    }

    string_destroy(s);
}

test_f test_find_substr_c(void)
{
     String *s;

    char *str = "Kukos Master C Kukos";
    size_t begin = 2;
    size_t end = 17;

    char c[] = {'z', 'K', 'k', 'C', ' '};
    ssize_t expt_c_f[] = {-1, 15, 2, 13, 5};
    ssize_t expt_c_l[] = {-1, 15, 17, 13, 14};

    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(expt_c_f); ++i)
        T_EXPECT(string_find_c_first_substr(s, c[i], begin, end), expt_c_f[i]);

    for (i = 0; i < ARRAY_SIZE(expt_c_l); ++i)
        T_EXPECT(string_find_c_last_substr(s, c[i], begin, end), expt_c_l[i]);

    string_destroy(s);
}

test_f test_find_substr_str(void)
{
    String *s;
    char *str = "Kukos Master C Kukos";
    size_t begin = 2;
    size_t end = 17;

    char *strr[] = {"kos", "er ", "Ala", "koss", "C"};
    ssize_t expt_str_f[] = {2, 10, -1, -1, 13};
    ssize_t expt_str_l[] = {2, 10, -1, -1, 13};

    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(expt_str_f); ++i)
        T_EXPECT(string_find_str_first_substr(s, strr[i], begin, end), expt_str_f[i]);

    for (i = 0; i < ARRAY_SIZE(expt_str_l); ++i)
        T_EXPECT(string_find_str_last_substr(s, strr[i], begin, end), expt_str_l[i]);

    string_destroy(s);
}

test_f test_find_substr_string(void)
{
    String *s;
    String *temp;
    char *str = "Kukos Master C Kukos";
    size_t begin = 2;
    size_t end = 17;

    char c[] = {'z', 'K', 'k', 'C', ' '};
    ssize_t expt_c_f[] = {-1, 15, 2, 13, 5};
    ssize_t expt_c_l[] = {-1, 15, 17, 13, 14};

    char *strr[] = {"kos", "er ", "Ala", "koss", "C"};
    ssize_t expt_str_f[] = {2, 10, -1, -1, 13};
    ssize_t expt_str_l[] = {2, 10, -1, -1, 13};

    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(expt_c_f); ++i)
    {
        temp = string_create_from_c(c[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_first_substr(s, temp, begin, end), expt_c_f[i]);

        string_destroy(temp);
    }

    for (i = 0; i < ARRAY_SIZE(expt_c_l); ++i)
    {
        temp = string_create_from_c(c[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_last_substr(s, temp, begin, end), expt_c_l[i]);

        string_destroy(temp);
    }

    for (i = 0; i < ARRAY_SIZE(expt_str_f); ++i)
    {
        temp = string_create_from_str(strr[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_first_substr(s, temp, begin, end), expt_str_f[i]);

        string_destroy(temp);
    }

    for (i = 0; i < ARRAY_SIZE(expt_str_l); ++i)
    {
        temp = string_create_from_str(strr[i]);
        T_ERROR(temp == NULL);

        T_EXPECT(string_find_string_last_substr(s, temp, begin, end), expt_str_l[i]);

        string_destroy(temp);
    }

    string_destroy(s);
}

test_f test_clone(void)
{
    String *s;
    String *copy;
    char *str = "Kukos";

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    copy = string_clone(s);
    T_ERROR(copy == NULL);

    T_EXPECT(string_get_length(copy), string_get_length(s));
    T_EXPECT(string_cmp(copy, s), 0);

    string_destroy(s);
    string_destroy(copy);
}

test_f test_cut(void)
{
    String *s;
    char *str = "Kukos";
    char *expected = str;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_cut(s, 0), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_CHECK(string_cut(s, 8) != 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "Kuk";
    T_EXPECT(string_cut(s, 2), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "";
    T_EXPECT(string_cut(s, 3), 0);
    T_EXPECT(string_get_length(s), strlen(expected));

    string_destroy(s);
}

test_f test_delete(void)
{
    String *s;
    char *str = "Kukos C Master";
    char *expected = str;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_delete(s, 0, 0), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_CHECK(string_delete(s, 20, 0) != 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "KukosMaster";
    T_EXPECT(string_delete(s, 5, 3), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "Master";
    T_EXPECT(string_delete(s, 0, 5), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "Maste";
    T_EXPECT(string_delete(s, 5, 1), 0);
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    expected = "";
    T_EXPECT(string_delete(s, 0, 5), 0);
    T_EXPECT(string_get_length(s), strlen(expected));

    string_destroy(s);
}

test_f test_append_c(void)
{
    String *s;
    char cc[] = {'K', 'u', 'k', 'o', 's'};
    char *expected;

    int i;

    s = string_create();
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(cc); ++i)
        T_EXPECT(string_append_c(s, cc[i]), 0);

    expected = "Kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_append_str(void)
{
    String *s;
    char *str[] = {"Kukos", " ", "Master", " ", "C"};
    char *expected;

    int i;

    s = string_create();
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(str); ++i)
        T_EXPECT(string_append_str(s, str[i]), 0);

    expected = "Kukos Master C";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_append_string(void)
{
    String *s;
    String *temp;
    char *expected;
    char *str[] = {"Kukos", " ", "Master", " ", "C"};
    int i;

    s = string_create();
    T_ERROR(s == NULL);
    for (i = 0; i < ARRAY_SIZE(str); ++i)
    {
        temp = string_create_from_str(str[i]);
        T_ERROR(temp == NULL);
        T_EXPECT(string_append_string(s, temp), 0);
        string_destroy(temp);
    }

    expected = "Kukos Master C";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_insert_c(void)
{
    String *s;
    char cc[] = {'K', 'o', 'u', 'k', 's'};
    size_t cpos[] = {0, 1, 1, 2, 4};
    char *expected;

    int i;

    s = string_create();
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(cc); ++i)
        T_EXPECT(string_insert_c(s, cc[i], cpos[i]), 0);

    expected = "Kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_insert_str(void)
{
    String *s;
    char *str[] = {"C", " " , "Kuk", "os",  " ", "Ma", "st", "er"};
    size_t spos[] = {0, 0, 0, 3, 5, 6, 8, 10};
    char *expected;

    int i;

    s = string_create();
    T_ERROR(s == NULL);

    for (i = 0; i < ARRAY_SIZE(str); ++i)
        T_EXPECT(string_insert_str(s, str[i], spos[i]), 0);

    expected = "Kukos Master C";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_insert_string(void)
{
    String *s;
    String *temp;
    char *expected;
    char *str[] = {"C", " " , "Kuk", "os",  " ", "Ma", "st", "er"};
    size_t spos[] = {0, 0, 0, 3, 5, 6, 8, 10};
    int i;

    s = string_create();
    T_ERROR(s == NULL);
    for (i = 0; i < ARRAY_SIZE(str); ++i)
    {
        temp = string_create_from_str(str[i]);
        T_ERROR(temp == NULL);
        T_EXPECT(string_insert_string(s, temp, spos[i]), 0);
        string_destroy(temp);
    }

    expected = "Kukos Master C";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_for_each(void)
{
    String *s;
    char *str = "Kukos C Master";
    char *node;
    char data;
    int i;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    i = 0;
    for_each(s, String, node, data)
    {
        T_ASSERT(*node, str[i]);
        T_ASSERT(data, str[i]);
        ++i;
    }

    i = string_get_length(s) - 1;
    for_each_prev(s, String, node, data)
    {
        T_ASSERT(*node, str[i]);
        T_ASSERT(data, str[i]);
        --i;
    }

    i = 0;
    for_each_data(s, String, data)
    {
        T_ASSERT(data, str[i]);
        ++i;
    }

    i = string_get_length(s) - 1;
    for_each_data_prev(s, String, data)
    {
        T_ASSERT(data, str[i]);
        --i;
    }

    i = 0;
    for_each_node(s, String, node)
    {
        T_ASSERT(*node, str[i]);
        ++i;
    }

    i = string_get_length(s) - 1;
    for_each_node_prev(s, String, node)
    {
        T_ASSERT(*node, str[i]);
        --i;
    }

    string_destroy(s);
}

test_f test_remove_c(void)
{
    String *s;
    char *str = "Kukkos C Masttter kuuukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_remove_c_first(s, 'k'), 0);
    expected = "Kukos C Masttter kuuukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_remove_c_last(s, 'k'), 0);
    expected = "Kukos C Masttter kuuuos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_remove_c_all(s, 'k'), 0);

    expected = "Kuos C Masttter uuuos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_remove_c_all(s, 'u'), 0);
    expected = "Kos C Masttter os";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    str = "kkkkk";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_remove_c_all(s, 'k'), 0);
    expected = "";
    T_EXPECT(string_get_length(s), strlen(expected));

    string_destroy(s);
}

test_f test_remove_str(void)
{
    String *s;
    char *str = "Kukkos C Masttter Kukkos kuuukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_remove_str_first(s, "kos"), 0);
    expected = "Kuk C Masttter Kukkos kuuukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_remove_str_last(s, "ko"), 0);
    expected = "Kuk C Masttter Kukkos kuuus";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_remove_str_all(s, "Kuk"), 0);
    expected = " C Masttter kos kuuus";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_remove_str_all(s, "u"), 0);
    expected = " C Masttter kos ks";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    str = "kukkukkukkukkuk";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_remove_str_all(s, "kuk"), 0);
    expected = "";
    T_EXPECT(string_get_length(s), strlen(expected));

    string_destroy(s);
}

test_f test_remove_string(void)
{
    String *s;
    String *temp;
    char *str = "Kukkos C Masttter Kukkos kuuukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("kos");
    T_ERROR(temp == NULL);
    T_EXPECT(string_remove_string_first(s, temp), 0);
    expected = "Kuk C Masttter Kukkos kuuukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(temp);

    temp = string_create_from_str("ko");
    T_ERROR(temp == NULL);
    T_EXPECT(string_remove_string_last(s, temp), 0);
    expected = "Kuk C Masttter Kukkos kuuus";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(temp);

    temp = string_create_from_str("Kuk");
    T_ERROR(temp == NULL);
    T_EXPECT(string_remove_string_all(s, temp), 0);
    expected = " C Masttter kos kuuus";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(temp);

    temp = string_create_from_str("u");
    T_ERROR(temp == NULL);
    T_EXPECT(string_remove_string_all(s, temp), 0);
    expected = " C Masttter kos ks";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(temp);

    string_destroy(s);

    str = "kukkukkukkukkuk";
    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("kuk");
    T_ERROR(temp == NULL);
    T_EXPECT(string_remove_str_all(s, "kuk"), 0);
    expected = "";
    T_EXPECT(string_get_length(s), strlen(expected));
    string_destroy(temp);

    string_destroy(s);
}

test_f test_replace_c_c(void)
{
    String *s;
    char *str = "Kukos C kkkukkkos kukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_c_by_c_first(s, 'k', 'l'), 0);
    expected = "Kulos C kkkukkkos kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_c_by_c_last(s, 'k', 'l'), 0);
    expected = "Kulos C kkkukkkos kulos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_c_by_c_all(s, 'k', 'l'), 0);
    expected = "Kulos C lllulllos lulos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    s = string_create_from_str("CCCCCCCCCC");
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_c_by_c_all(s, 'C', 'K'), 0);
    expected = "KKKKKKKKKK";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);
    string_destroy(s);
}

test_f test_replace_c_str(void)
{
    String *s;
    char *str = "Kukos C kkkukkkos kukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_c_by_str_first(s, 'k', "CCC"), 0);
    expected = "KuCCCos C kkkukkkos kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_c_by_str_last(s, 'k', "CCC"), 0);
    expected = "KuCCCos C kkkukkkos kuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_c_by_str_all(s, 'k', "CCC"), 0);
    expected = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    s = string_create_from_str("CCCCCCCCCC");
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_c_by_str_all(s, 'C', "KKKK"), 0);
    expected = "KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_replace_c_string(void)
{
    String *s;
    String *temp;
    char *str = "Kukos C kkkukkkos kukos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("CCC");
    T_ERROR(temp == NULL);
    T_EXPECT(string_replace_c_by_string_first(s, 'k', temp), 0);
    expected = "KuCCCos C kkkukkkos kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_c_by_string_last(s, 'k', temp), 0);
    expected = "KuCCCos C kkkukkkos kuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_c_by_string_all(s, 'k', temp), 0);
    expected = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);

    s = string_create_from_str("CCCCCCCCCC");
    T_ERROR(s == NULL);

    temp = string_create_from_str("KKKK");
    T_ERROR(temp == NULL);

    T_EXPECT(string_replace_c_by_string_all(s, 'C', temp), 0);
    expected = "KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);
}

test_f test_replace_str_c(void)
{
    String *s;
    char *str = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_str_by_c_first(s, "CCC", 'k'), 0);
    expected = "Kukos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_str_by_c_last(s, "CCC", 'k'), 0);
    expected = "Kukos C CCCCCCCCCuCCCCCCCCCos CCCukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_str_by_c_all(s, "CCC", 'k'), 0);
    expected = "Kukos C kkkukkkos kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    s = string_create_from_str("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_str_by_c_all(s, "KKKK", 'C'), 0);
    expected = "CCCCCCCCCC";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_replace_str_str(void)
{
    String *s;
    char *str = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_str_by_str_first(s, "CCC", "kk"), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_str_by_str_last(s, "CCC", "kk"), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_str_by_str_all(s, "CCC", "kk"), 0);
    expected = "Kukkos C kkkkkkukkkkkkos kkukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    s = string_create_from_str("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_str_by_str_all(s, "KKKK", "CC"), 0);
    expected = "CCCCCCCCCCCCCCCCCCCC";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
}

test_f test_replace_str_string(void)
{
    String *s;
    String *temp;
    char *str = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("kk");
    T_ERROR(temp == NULL);

    T_EXPECT(string_replace_str_by_string_first(s, "CCC", temp), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_str_by_string_last(s, "CCC", temp), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_str_by_string_all(s, "CCC", temp), 0);
    expected = "Kukkos C kkkkkkukkkkkkos kkukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);

    s = string_create_from_str("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    T_ERROR(s == NULL);

    temp = string_create_from_str("CC");
    T_ERROR(temp == NULL);

    T_EXPECT(string_replace_str_by_string_all(s, "KKKK", temp), 0);
    expected = "CCCCCCCCCCCCCCCCCCCC";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);
}

test_f test_replace_string_c(void)
{
    String *s;
    String *temp;
    char *str = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("CCC");
    T_ERROR(temp == NULL);

    T_EXPECT(string_replace_string_by_c_first(s, temp, 'k'), 0);
    expected = "Kukos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_string_by_c_last(s, temp, 'k'), 0);
    expected = "Kukos C CCCCCCCCCuCCCCCCCCCos CCCukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_string_by_c_all(s, temp, 'k'), 0);
    expected = "Kukos C kkkukkkos kukos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);

    s = string_create_from_str("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    T_ERROR(s == NULL);

    temp = string_create_from_str("KKKK");
    T_ERROR(temp == NULL);

    T_EXPECT(string_replace_string_by_c_all(s, temp, 'C'), 0);
    expected = "CCCCCCCCCC";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);
}

test_f test_replace_string_str(void)
{
    String *s;
    String *temp;

    char *str = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("CCC");
    T_ERROR(temp == NULL);

    T_EXPECT(string_replace_string_by_str_first(s, temp, "kk"), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_string_by_str_last(s, temp, "kk"), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_string_by_str_all(s, temp, "kk"), 0);
    expected = "Kukkos C kkkkkkukkkkkkos kkukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);

    s = string_create_from_str("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    T_ERROR(s == NULL);

    T_EXPECT(string_replace_str_by_str_all(s, "KKKK", "CC"), 0);
    expected = "CCCCCCCCCCCCCCCCCCCC";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);
}

test_f test_replace_string_string(void)
{
    String *s;
    String *temp;
    String *rep;
    char *str = "KuCCCos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    char *expected;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    temp = string_create_from_str("CCC");
    T_ERROR(temp == NULL);

    rep = string_create_from_str("kk");
    T_ERROR(rep == NULL);

    T_EXPECT(string_replace_string_by_string_first(s, temp, rep), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCuCCCos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_string_by_string_last(s, temp, rep), 0);
    expected = "Kukkos C CCCCCCCCCuCCCCCCCCCos CCCukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    T_EXPECT(string_replace_string_by_string_all(s, temp, rep), 0);
    expected = "Kukkos C kkkkkkukkkkkkos kkukkos";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);
    string_destroy(rep);

    s = string_create_from_str("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK");
    T_ERROR(s == NULL);

    temp = string_create_from_str("KKKK");
    T_ERROR(temp == NULL);

    rep = string_create_from_str("CC");
    T_ERROR(rep == NULL);

    T_EXPECT(string_replace_string_by_string_all(s, temp, rep), 0);
    expected = "CCCCCCCCCCCCCCCCCCCC";
    T_EXPECT(string_get_length(s), strlen(expected));
    T_EXPECT(strcmp(string_get_str(s), expected), 0);

    string_destroy(s);
    string_destroy(temp);
    string_destroy(rep);
}

test_f test_split_c(void)
{
    String *s;

    char c[] = {' ', '_'};

    char *str = " Kuk_os _C_ Mas te _r_";
    char *expt[5][5] =  {
                            {"Kuk_os", "_C_", "Mas", "te", "_r_"},
                            {" Kuk", "os ", "C", " Mas te ", "r"}
                        };

    String **array;
    size_t size;

    int i;
    int j;

    s = string_create_from_str(str);
    T_ERROR(s == NULL);

    for (j = 0; j < ARRAY_SIZE(c); ++j)
    {
        array = string_split_c(s, c[j], &size);
        T_CHECK(array != NULL);
        T_ASSERT(size, ARRAY_SIZE(expt[j]));

        for (i = 0; i < size; ++i)
        {
            T_EXPECT(string_get_length(array[i]), strlen(expt[j][i]));
            T_EXPECT(strcmp(string_get_str(array[i]), expt[j][i]), 0);
            string_destroy(array[i]);
        }

        FREE(array);
    }

    string_destroy(s);
}

test_f test_split_str(void)
{
    String *s;

    char *str[] = { " Kuk_os _C_ Mas te _r_", "CCKukkukosCCkukCkukCCMasCCteCCkukrkuk"};
    char *strr[] = {" ", "_", "CC", "kuk"};

    char *expt[5][5] =  {
                            {"Kuk_os", "_C_", "Mas", "te", "_r_"},
                            {" Kuk", "os ", "C", " Mas te ", "r"},
                            {"Kukkukos", "kukCkuk", "Mas", "te", "kukrkuk"},
                            {"CCKuk", "osCC", "C", "CCMasCCteCC", "r"}
                        };

    String **array;
    size_t size;

    int i;
    int j;
    int k;

    for (k = 0; k < ARRAY_SIZE(str); ++k)
    {
        s = string_create_from_str(str[k]);
        T_ERROR(s == NULL);
        for (j = (k << 1); j < ARRAY_SIZE(str) + (k << 1); ++j)
        {
            array = string_split_str(s, strr[j], &size);
            T_CHECK(array != NULL);
            T_ASSERT(size, ARRAY_SIZE(expt[j]));

            for (i = 0; i < size; ++i)
            {
                T_EXPECT(string_get_length(array[i]), strlen(expt[j][i]));
                T_EXPECT(strcmp(string_get_str(array[i]), expt[j][i]), 0);
                string_destroy(array[i]);
            }
            FREE(array);
        }
        string_destroy(s);
    }
}

test_f test_split_string(void)
{
    String *s;
    String *temp;

    char *str[] = { " Kuk_os _C_ Mas te _r_", "CCKukkukosCCkukCkukCCMasCCteCCkukrkuk"};
    char *strr[] = {" ", "_", "CC", "kuk"};

    char *expt[5][5] =  {
                            {"Kuk_os", "_C_", "Mas", "te", "_r_"},
                            {" Kuk", "os ", "C", " Mas te ", "r"},
                            {"Kukkukos", "kukCkuk", "Mas", "te", "kukrkuk"},
                            {"CCKuk", "osCC", "C", "CCMasCCteCC", "r"}
                        };

    String **array;
    size_t size;

    int i;
    int j;
    int k;

    for (k = 0; k < ARRAY_SIZE(str); ++k)
    {
        s = string_create_from_str(str[k]);
        T_ERROR(s == NULL);
        for (j = (k << 1); j < ARRAY_SIZE(str) + (k << 1); ++j)
        {
            temp = string_create_from_str(strr[j]);
            T_ERROR(temp == NULL);

            array = string_split_string(s, temp, &size);
            T_CHECK(array != NULL);
            T_ASSERT(size, ARRAY_SIZE(expt[j]));

            for (i = 0; i < size; ++i)
            {
                T_EXPECT(string_get_length(array[i]), strlen(expt[j][i]));
                T_EXPECT(strcmp(string_get_str(array[i]), expt[j][i]), 0);
                string_destroy(array[i]);
            }
            FREE(array);

            string_destroy(temp);
        }
        string_destroy(s);
    }
}

test_f test_empty(void)
{
    String *s;
    String *s2;
    String *s3;

    char c = 'C';
    char c2 = 'D';
    char *str = "Kukos";
    char *str2 = "Master";

    size_t size;

    s = string_create();
    T_ERROR(s == NULL);

    s2 = string_create();
    T_ERROR(s2 == NULL);

    s3 = string_create();
    T_ERROR(s3 == NULL);

    T_EXPECT(string_get_char(s, 0), -1);
    T_CHECK(string_set_char(s, 0, c) != 0);
    T_EXPECT(string_tolower(s), 0);
    T_EXPECT(string_toupper(s), 0);
    T_EXPECT(string_trim(s), 0);
    T_EXPECT(string_substr(s, 0, 1), NULL);
    T_EXPECT(string_concat(s, s2), NULL);
    T_CHECK(string_delete(s, 0, 1) != 0);
    T_CHECK(string_cut(s, 1) != 0);
    T_CHECK(string_get_str(s) != NULL);

    string_destroy(string_clone(s));

    T_EXPECT(string_find_c_first(s, c), -1);
    T_EXPECT(string_find_c_last(s, c), -1);
    T_EXPECT(string_find_str_first(s, str), -1);
    T_EXPECT(string_find_str_last(s, str), -1);
    T_EXPECT(string_find_string_first(s, s2), -1);
    T_EXPECT(string_find_string_last(s, s2), -1);

    T_EXPECT(string_find_c_first_substr(s, c, 0, 0), -1);
    T_EXPECT(string_find_c_last_substr(s, c, 0, 0), -1);
    T_EXPECT(string_find_str_first_substr(s, str, 0, 0), -1);
    T_EXPECT(string_find_str_last_substr(s, str, 0, 0), -1);
    T_EXPECT(string_find_string_first_substr(s, s2, 0, 0), -1);
    T_EXPECT(string_find_string_last_substr(s, s2, 0, 0), -1);

    T_CHECK(string_remove_c_first(s, c) != 0);
    T_CHECK(string_remove_c_last(s, c) != 0);
    T_CHECK(string_remove_c_all(s, c) != 0);
    T_CHECK(string_remove_str_first(s, str) != 0);
    T_CHECK(string_remove_str_last(s, str) != 0);
    T_CHECK(string_remove_str_all(s, str) != 0);
    T_CHECK(string_remove_string_first(s, s2) != 0);
    T_CHECK(string_remove_string_last(s, s2) != 0);
    T_CHECK(string_remove_string_all(s, s2) != 0);

    T_CHECK(string_replace_c_by_c_first(s, c, c2) != 0);
    T_CHECK(string_replace_c_by_c_last(s, c, c2) != 0);
    T_CHECK(string_replace_c_by_c_all(s, c, c2) != 0);
    T_CHECK(string_replace_c_by_str_first(s, c, str) != 0);
    T_CHECK(string_replace_c_by_str_last(s, c, str) != 0);
    T_CHECK(string_replace_c_by_str_all(s, c, str) != 0);
    T_CHECK(string_replace_c_by_string_first(s, c, s2) != 0);
    T_CHECK(string_replace_c_by_string_last(s, c, s2) != 0);
    T_CHECK(string_replace_c_by_string_all(s, c, s2) != 0);

    T_CHECK(string_replace_str_by_c_first(s, str2, c2) != 0);
    T_CHECK(string_replace_str_by_c_last(s, str2, c2) != 0);
    T_CHECK(string_replace_str_by_c_all(s, str2, c2) != 0);
    T_CHECK(string_replace_str_by_str_first(s, str2, str) != 0);
    T_CHECK(string_replace_str_by_str_last(s, str2, str) != 0);
    T_CHECK(string_replace_str_by_str_all(s, str2, str) != 0);
    T_CHECK(string_replace_str_by_string_first(s, str2, s2) != 0);
    T_CHECK(string_replace_str_by_string_last(s, str2, s2) != 0);
    T_CHECK(string_replace_str_by_string_all(s, str2, s2) != 0);

    T_CHECK(string_replace_string_by_c_first(s, s3, c2) != 0);
    T_CHECK(string_replace_string_by_c_last(s, s3, c2) != 0);
    T_CHECK(string_replace_string_by_c_all(s, s3, c2) != 0);
    T_CHECK(string_replace_string_by_str_first(s, s3, str) != 0);
    T_CHECK(string_replace_string_by_str_last(s, s3, str) != 0);
    T_CHECK(string_replace_string_by_str_all(s, s3, str) != 0);
    T_CHECK(string_replace_string_by_string_first(s, s3, s2) != 0);
    T_CHECK(string_replace_string_by_string_last(s, s3, s2) != 0);
    T_CHECK(string_replace_string_by_string_all(s, s3, s2) != 0);

    T_EXPECT(string_split_c(s, c, &size), NULL);
    T_EXPECT(string_split_str(s, str, &size), NULL);
    T_EXPECT(string_split_string(s, s2, &size), NULL);

    string_destroy(s);
    string_destroy(s2);
    string_destroy(s3);
}

void test(void)
{
    TEST(test_create());
    TEST(test_concat());
    TEST(test_get_set_char());
    TEST(test_to_lower_upper());
    TEST(test_trim());
    TEST(test_cmp());
    TEST(test_substr());
    TEST(test_find_c());
    TEST(test_find_str());
    TEST(test_find_string());
    TEST(test_find_substr_c());
    TEST(test_find_substr_str());
    TEST(test_find_substr_string());
    TEST(test_clone());
    TEST(test_cut());
    TEST(test_delete());
    TEST(test_append_c());
    TEST(test_append_str());
    TEST(test_append_string());
    TEST(test_insert_c());
    TEST(test_insert_str());
    TEST(test_insert_string());
    TEST(test_for_each());
    TEST(test_remove_c());
    TEST(test_remove_str());
    TEST(test_remove_string());
    TEST(test_replace_c_c());
    TEST(test_replace_c_str());
    TEST(test_replace_c_string());
    TEST(test_replace_str_c());
    TEST(test_replace_str_str());
    TEST(test_replace_str_string());
    TEST(test_replace_string_c());
    TEST(test_replace_string_str());
    TEST(test_replace_string_string());
    TEST(test_split_c());
    TEST(test_split_str());
    TEST(test_split_string());
    TEST(test_empty());
}

int main(void)
{
    TEST_INIT("CSTRING");
    test();
    TEST_SUMMARY;
}