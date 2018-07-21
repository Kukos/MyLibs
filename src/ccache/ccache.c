#ifndef _GNU_SOURCE
    #define _GNU_SOURCE /* needed to use feature.h */
#endif

#include <ccache.h>
#include <unistd.h>
#include <common.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <log.h>
#include <stdlib.h>
#include <cstring.h>
#include <ctype.h>
#include <string.h>

struct CCache
{
    int fd;
    char *cache;
    size_t len;
    size_t mmap_size;
};

static const char * const PATH_TO_CACHE = "./.ccache";

static ___inline___ bool is_aligned(size_t size);
static ___inline___ size_t align_size(size_t size);
static size_t get_available_memory(void);

static ___inline___ bool is_aligned(size_t size)
{
	TRACE();

	/* pagesize is always power of 2 */
	return !(CAST_TO_BOOL(size & ((size_t)getpagesize())));
}

static ___inline___ size_t align_size(size_t size)
{
    /* page size is always power of two */
    size_t page_size = (size_t)getpagesize();

    TRACE();

    if (size == 0)
        return page_size;

    if (is_aligned(size))
        return size;

    /*
        A little magic here, but it is rly fast
        Example:
        1010 1100 -> to 1011 0000
        size & 1111 -> 0000 1100
        so ~(size & 1111) -> 1111 0011
        so 1111 0011 & 1111 -> 0000 0011
        so size | 0000 0011 -> 1010 1111
        so ++size -> 1011 0000
    */
    size |= (~(size & (page_size - 1)) & (page_size - 1));
    ++size;

    return size;
}

static size_t get_available_memory(void)
{
    const char * const meminfo = "/proc/meminfo";
    const char * const token = "MemAvailable:";
    char buffer [BIT(12)] = {0}; /* 1KB buffer */
    String *string;
    int fd;
    size_t line;
    char *ptr;
    size_t ram;

    fd = open(meminfo, O_RDONLY);
    if (fd == -1)
        ERROR("open error\n", 0);

    if (read(fd, buffer, ARRAY_SIZE(buffer)) == -1)
        ERROR("read error\n", 0);

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

CCache *ccache_create(size_t size)
{
    CCache *cache;
    int fd;
    __off64_t asize;

    TRACE();

    cache = (CCache *)malloc(sizeof(CCache));
    if (cache == NULL)
        ERROR("malloc error\n", NULL);

    asize = (__off64_t)align_size(size);

    cache->fd = -1;
    cache->mmap_size = (size_t)asize;
    cache->len = size;

    if (size < get_available_memory())
    {
        cache->cache = mmap64(NULL, (size_t)asize, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (cache->cache == MAP_FAILED)
        {
            FREE(cache);
            ERROR("mmap64 error\n", NULL);
        }

        return cache;
    }

    fd = open64(PATH_TO_CACHE, O_CREAT | O_TRUNC | O_LARGEFILE | O_RDWR, 0644);
    if (fd == -1)
    {
        FREE(cache);
        ERROR("creat64 error\n", NULL);
    }

    if (ftruncate64(fd, asize) == -1)
    {
        FREE(cache);
        close(fd);
        ERROR("ftruncate64 error\n", NULL);
    }

    cache->fd = fd;
    cache->cache = mmap64(NULL, (size_t)asize, PROT_EXEC | PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (cache->cache == MAP_FAILED)
    {
        FREE(cache);
        close(fd);

        ERROR("mmap64 error\n", NULL);
    }

    return cache;
}

void ccache_destroy(CCache *cache)
{
    TRACE();

    if (cache == NULL)
        return;

    munmap(cache->cache, cache->mmap_size);

    if (cache->fd != -1)
    {
        close(cache->fd);
        remove(PATH_TO_CACHE);
    }

    FREE(cache);
}

ssize_t ccache_get_size(CCache *cache)
{
    if (cache == NULL)
        ERROR("cache == NULL\n", -1);

    return (ssize_t)cache->len;
}

void *ccache_get_array(CCache *cache, size_t *size)
{
    if (cache == NULL)
        ERROR("cache == NULL\n", NULL);

    if (size != NULL)
        *size = cache->len;

    return (void *)cache->cache;
}