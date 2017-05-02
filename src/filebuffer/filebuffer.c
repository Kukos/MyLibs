#include <filebuffer.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <log.h>
#include <compiler.h>
#include <common.h>
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>

__inline__ static bool is_aligned(size_t size)
{
	/* pagesize is always power of 2 */
	return !(CAST_TO_BOOL(size & ((size_t)getpagesize())));
}

__inline__ static size_t align_size(size_t size)
{
	/* page size is always power of two */
	size_t page_size = (size_t)getpagesize();

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

file_buffer *file_buffer_create(int fd, int protect_flag)
{
	file_buffer *fb;
	struct stat ft;

	TRACE("");

	if (fd < 0)
		ERROR("fd < 0\n", NULL, "");

    fb = (file_buffer *)malloc(sizeof(file_buffer));
    if (fb == NULL)
		ERROR("malloc error\n", NULL, "");

    fb->fd = fd;
    fb->protect_flag = protect_flag;

    /* read size of file ( in bytes ) */
    if (fstat(fd, &ft) == -1)
	{
		FREE(fb);
		ERROR("fstat error\n", NULL, "");
	}

    fb->size = ft.st_size;
	fb->mapped_size = align_size(fb->size);

    if ((fb->buffer = (char *)mmap(NULL, fb->mapped_size,
			 protect_flag, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		FREE(fb);
		ERROR("mmap error\n", NULL, "");
	}

    return fb;
}

file_buffer *file_buffer_create_from_path(const char *path, int protect_flag, int open_flag)
{
	int fd;

	TRACE("");

	if (path == NULL)
		ERROR("path == NULL\n", NULL, "");

	fd = open(path, open_flag, 0644);
	return file_buffer_create(fd, protect_flag);
}

int file_buffer_destroy(file_buffer *fb)
{
	TRACE("");

    if (fb == NULL)
		return 1;

    /* before detach sunchronize */
    if (file_buffer_synch(fb))
		ERROR("msync error\n", 1, "");

    /* unmap file */
    if (munmap((void *)fb->buffer, fb->mapped_size) == -1)
       	ERROR("munmap error\n", 1, "");

    FREE(fb);

    return 0;
}

int file_buffer_append(file_buffer *fb, const char *data)
{
	size_t length;
    off_t new_size;

	TRACE("");

    assert(fb == NULL || data == NULL);

	length = strlen(data);
	new_size = fb->size + length;

	/* resize file */
    if (ftruncate(fb->fd, new_size) == -1)
        ERROR("ftruncate error\n", 1, "");

	/* realloc mapped file in RAM */
	if (new_size > fb->mapped_size)
    	if ((fb->buffer = (char *)mremap((void *)fb->buffer, fb->size,
	 			new_size, MREMAP_MAYMOVE)) == MAP_FAILED)
     		ERROR("mremap error\n", 1, "");

	/* write data to buffer */
    if (memcpy((void *)(fb->buffer + fb->size), data, length) == NULL)
		ERROR("memcpy error\n", 1, "");

    fb->size = new_size;

    return 0;
}

int file_buffer_synch(file_buffer *fb)
{
	TRACE("");

    assert(fb == NULL);

    if ((msync((void *)fb->buffer, fb->size, MS_SYNC)) == -1)
        ERROR("msync error\n", 1, "");

    return 0;
}

char *file_buffer_get_buff(file_buffer *fb)
{
	assert(fb == NULL);

	return fb->buffer;
}

ssize_t file_buffer_get_size(file_buffer *fb)
{
	assert(fb == NULL);

	return (size_t)fb->size;
}
