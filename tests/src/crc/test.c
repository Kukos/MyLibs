#include <crc.h>
#include <test.h>

typedef struct MyStruct
{
    char c;
    int i;
    double d;
    char *str;
} MyStruct;

static uint64_t crc8_wrapper(const void *data, size_t size)
{
    return (uint64_t)crc8(data, size);
}

static uint64_t crc16_wrapper(const void *data, size_t size)
{
    return (uint64_t)crc16(data, size);
}

static uint64_t crc32_wrapper(const void *data, size_t size)
{
    return (uint64_t)crc32(data, size);
}

static uint64_t crc64_wrapper(const void *data, size_t size)
{
    return (uint64_t)crc64(data, size);
}

test_f test_general(uint64_t (*crc_f)(const void *data, size_t size))
{
    char c1 = 'K';
    char c2 = c1;
    
    int i1 = 123456;
    int i2 = i1;

    double d1 = 1234.5678;
    double d2 = d1;

    char str1[] = "qwertyuioplkjhgfdsazxcvbnm";
    char str2[] = "qwertyuioplkjhgfdsazxcvbnm";

    MyStruct ms1 = {.c = c1, .i = i1, .d = d1, .str = str1};
    MyStruct ms2 = ms1;

    uint64_t crc1;
    uint64_t crc2;

    crc1 = crc_f((const void *)&c1, sizeof(c1));
    crc2 = crc_f((const void *)&c2, sizeof(c2));
    T_ASSERT(crc1, crc2);

    crc1 = crc_f((const void *)&i1, sizeof(i1));
    crc2 = crc_f((const void *)&i2, sizeof(i2));
    T_ASSERT(crc1, crc2);

    crc1 = crc_f((const void *)&d1, sizeof(d1));
    crc2 = crc_f((const void *)&d2, sizeof(d2));
    T_ASSERT(crc1, crc2);

    crc1 = crc_f((const void *)&str1, sizeof(str1));
    crc2 = crc_f((const void *)&str2, sizeof(str2));
    T_ASSERT(crc1, crc2);

    crc1 = crc_f((const void *)&ms1, sizeof(ms1));
    crc2 = crc_f((const void *)&ms2, sizeof(ms2));
    T_ASSERT(crc1, crc2);
}

test_f test_crc8(void)
{
    test_general(crc8_wrapper);
}

test_f test_crc16(void)
{
    test_general(crc16_wrapper);
}

test_f test_crc32(void)
{
    test_general(crc32_wrapper);
}

test_f test_crc64(void)
{
    test_general(crc64_wrapper);
}

void test(void)
{
    TEST(test_crc8());
    TEST(test_crc16());
    TEST(test_crc32());
    TEST(test_crc64());
}

int main(void)
{
    TEST_INIT("CRC");
    test();
    TEST_SUMMARY();
}