#include <ccache.h>
#include <test.h>
#include <cstring.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <ctype.h>

#define CACHE_FILE "./.ccache"

static size_t get_available_memory(void)
{
    const char * const meminfo = "/proc/meminfo";
    const char * const token = "MemAvailable:";
    char buffer [BIT(12)] = {0}; /* 4KB buffer */
    String *string;
    int fd;
    size_t line;
    char *ptr;
    size_t ram;

    fd = open(meminfo, O_RDONLY);
    if (fd == -1)
        return 0;

    if (read(fd, buffer, ARRAY_SIZE(buffer)) == -1)
        return 0;

    close(fd);

    string = string_create_from_str(buffer);
    line = (size_t)string_find_str_first(string, token);

    ptr = &(string_get_str(string)[line]);
    while (!isdigit(*ptr))
        ++ptr;

    /* killobytes to bytes */
    ram = (size_t)strtoul(ptr, NULL, 10) * BIT(10);

    string_destroy(string);
    return ram;
}

test_f test_create_in_ram(void)
{
    const size_t ram = get_available_memory();
    size_t size;
    CCache *cache = ccache_create(ram - 1);


    /* in RAM */
    T_EXPECT(access(CACHE_FILE, F_OK), -1);
    T_ERROR(cache == NULL);

    T_CHECK(ccache_get_array(cache, &size) != NULL);
    T_ASSERT(size, ram - 1);

    ccache_destroy(cache);
}

test_f test_create_in_file(void)
{
    const size_t ram = get_available_memory();
    size_t size;
    CCache *cache = ccache_create(ram + 1);

    /* in file */
    T_CHECK(access(CACHE_FILE, F_OK) != -1);
    T_ERROR(cache == NULL);

    T_CHECK(ccache_get_array(cache, &size) != NULL);
    T_ASSERT(size, ram + 1);

    ccache_destroy(cache);

    /* check cleanup */
    T_EXPECT(access(CACHE_FILE, F_OK), -1);
}

test_f test_read_write_in_ram(void)
{
    const size_t ram = get_available_memory();
    void *array;
    size_t size;
    CCache *cache = ccache_create(ram - 1);

    /* in RAM */
    T_EXPECT(access(CACHE_FILE, F_OK), -1);
    T_ERROR(cache == NULL);

    T_CHECK(ccache_get_array(cache, &size) != NULL);
    T_ASSERT(size, ram - 1);

    array = ccache_get_array(cache, &size);
    ((int *)array)[0] = 10;
    ((char *)array)[10] = 'a';
    ((double *)array)[20] = 3.14;

    array = ccache_get_array(cache, &size);
    T_ASSERT(((int *)array)[0], 10);
    T_ASSERT(((char *)array)[10], 'a');
    T_ASSERT(((double *)array)[20], 3.14);

    ccache_destroy(cache);
}

test_f test_read_write_in_file(void)
{
    const size_t ram = get_available_memory();
    void *array;
    size_t size;
    CCache *cache = ccache_create(ram + 1);

    /* in file */
    T_CHECK(access(CACHE_FILE, F_OK) != -1);
    T_ERROR(cache == NULL);

    T_CHECK(ccache_get_array(cache, &size) != NULL);
    T_ASSERT(size, ram + 1);

    array = ccache_get_array(cache, &size);
    ((int *)array)[0] = 10;
    ((char *)array)[10] = 'a';
    ((double *)array)[20] = 3.14;

    array = ccache_get_array(cache, &size);
    T_ASSERT(((int *)array)[0], 10);
    T_ASSERT(((char *)array)[10], 'a');
    T_ASSERT(((double *)array)[20], 3.14);

    ccache_destroy(cache);

    T_EXPECT(access(CACHE_FILE, F_OK), -1);
}

void test(void)
{
    TEST(test_create_in_ram());
    TEST(test_create_in_file());
    TEST(test_read_write_in_ram());
    TEST(test_read_write_in_file());
}

int main(void)
{
    TEST_INIT("CCACHE");
    test();
    TEST_SUMMARY();
}