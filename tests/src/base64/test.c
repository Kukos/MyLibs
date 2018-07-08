#include <test.h>
#include <base64.h>
#include <string.h>

#define TEST_BASE_STR(str) \
    do { \
        char *enc_str; \
        char *dec_str; \
        size_t size; \
        \
        enc_str = base64_encode(str, sizeof(str), &size); \
        T_ERROR(enc_str == NULL); \
        T_CHECK(size != 0); \
        \
        dec_str = base64_decode(enc_str, size, &size); \
        T_ERROR(dec_str == NULL) \
        T_CHECK(size >= sizeof(str)); \
        \
        T_EXPECT(strcmp(str, dec_str), 0); \
        \
        FREE(enc_str); \
        FREE(dec_str); \
    } while (0);


test_f test_coding_alphabet(void)
{
    const char str[] = "qwertyuioplkjhgfdsazxcvbnm";
    const char str2[] = "QWERTYUIOPLKJHGFDSAZXCVBNM";
    const char str3[] = "qMwNeBrVtCyXuZiAoSpDlFkGjHhJgKfLdPsOaIzUxYcTvRbEnWmQ";

    TEST_BASE_STR(str);
    TEST_BASE_STR(str2);
    TEST_BASE_STR(str3);
}

test_f test_coding_numeric(void)
{
    const char str[] = "1234567890";
    const char str2[] = "1qwer2tyu3io4pl5kjh6gfd7saz8xcv9bn0m";
    const char str3[] = "0QW1ERT2YU3IOPL4K5JHGF6DSA7ZXCV8BNM9";
    const char str4[] = "1qM2wNe3BrV4tCy5XuZi6AoSpD7lFkGj8Hh9JgKfLdPs0OaIzUxYcTvRbEnWmQ";

    TEST_BASE_STR(str);
    TEST_BASE_STR(str2);
    TEST_BASE_STR(str3);
    TEST_BASE_STR(str4);
}

test_f test_coding_schars(void)
{
    const char str[] = "12+3-4567890";
    const char str2[] = "1qwe/r2t'yu3io[4pl5kjh6gfd]7saz8xcv9bn0m";
    const char str3[] = "0QW1=ERT2+YU3I|OPL?4K5%%JHGF6.DSA7,ZXCV8BNM9";
    const char str4[] = "1qM2w~Ne3`BrV4t@Cy5X!uZi6A#oSpD7l$FkGj8Hh9Jg^KfLdPs&0OaI*zUxY(cTv)RbE_nWmQ";

    TEST_BASE_STR(str);
    TEST_BASE_STR(str2);
    TEST_BASE_STR(str3);
    TEST_BASE_STR(str4);
}

test_f test_coding_words(void)
{
    const char str[] = "C   Master .";
    const char str2[] = "Word1. Word2 and Word3!     And word 4?!    ";

    TEST_BASE_STR(str);
    TEST_BASE_STR(str2);
}

void test(void)
{
    TEST(test_coding_alphabet());
    TEST(test_coding_numeric());
    TEST(test_coding_schars());
    TEST(test_coding_words());
}


int main(void)
{
    TEST_INIT("BASE 64");
    test();
    TEST_SUMMARY();
}