#ifndef _GNU_SOURCE
	#define _GNU_SOURCE /* needed to use feature.h */
#endif

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
#include <fcntl.h>
#include <stdbool.h>

struct File_buffer
{
    char 			*buffer;
    size_t 			size; /* size visible by user */
	size_t			mapped_size; /* mapped size including padding */
    int 			fd; /* file descriptor of buffered file */
    int 			protect_flag;
    bool            has_private_file; /* when fb created by path, file is open by FB */
    bool            mode64; /* mode 64 or not ? */

};

___inline___ static bool is_aligned(size_t size);
___inline___ static size_t align_size(size_t size);

___inline___ static bool is_aligned(size_t size)
{
	TRACE();

	/* pagesize is always power of 2 */
	return !(CAST_TO_BOOL(size & ((size_t)getpagesize())));
}

___inline___ static size_t align_size(size_t size)
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

File_buffer *file_buffer_create(int fd, int protect_flag)
{
	File_buffer *fb;
	struct stat ft;

	TRACE();

	if (fd < 0)
		ERROR("fd < 0\n", NULL);

    fb = (File_buffer *)malloc(sizeof(File_buffer));
    if (fb == NULL)
		ERROR("malloc error\n", NULL);

    fb->fd = fd;
    fb->protect_flag = protect_flag;

    /* read size of file ( in bytes ) */
    if (fstat(fd, &ft) == -1)
	{
		FREE(fb);
		ERROR("fstat error\n", NULL);
	}

    fb->size = (size_t)ft.st_size;
	fb->mapped_size = align_size(fb->size);
    fb->has_private_file = false;
    fb->mode64 = false;

    if ((fb->buffer = (char *)mmap(NULL, fb->mapped_size,
			 protect_flag, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		FREE(fb);
		ERROR("mmap error\n", NULL);
	}

    return fb;
}

File_buffer *file_buffer_create_from_path(const char *path, int protect_flag, int open_flag)
{
    File_buffer *fb;
	int fd;

	TRACE();

	if (path == NULL)
		ERROR("path == NULL\n", NULL);

	fd = open(path, open_flag, 0644);
	fb = file_buffer_create(fd, protect_flag);
    if (fb == NULL)
        ERROR("file_buffer_create error\n", NULL);

    fb->has_private_file = true;
    return fb;
}

int file_buffer_destroy(File_buffer *fb)
{
	TRACE();

    if (fb == NULL)
		return 1;

    /* before detach sunchronize */
    if (file_buffer_synch(fb))
		ERROR("msync error\n", 1);

    /* unmap file */
    if (munmap((void *)fb->buffer, fb->mapped_size) == -1)
       	ERROR("munmap error\n", 1);

    if (fb->has_private_file)
        close(fb->fd);

    FREE(fb);

    return 0;
}

int file_buffer_append(File_buffer * ___restrict___ fb, const char * ___restrict___ data)
{
	size_t length;
    off_t new_size;
	size_t new_aligned_size;

	TRACE();

	if (fb == NULL || data == NULL)
		ERROR("fb == NULL || data == NULL\n", 1);

    if (fb->mode64)
        ERROR("file_buffer works in 64bits mode, incorrect functions\n", 1);

	length = strlen(data);
	if (length == 0)
		ERROR("data has 0 len\n", 1);

	new_size = (off_t)(fb->size + length);

	/* resize file */
    if (ftruncate(fb->fd, new_size) == -1)
        ERROR("ftruncate error\n", 1);

	new_aligned_size = align_size((size_t)new_size);

	/* realloc mapped file in RAM */
	if (new_aligned_size > fb->mapped_size)
	{
    	if ((fb->buffer = (char *)mremap((void *)fb->buffer, fb->mapped_size, new_aligned_size, MREMAP_MAYMOVE)) == MAP_FAILED)
     		ERROR("mremap error\n", 1);

		fb->mapped_size = new_aligned_size;
	}

	/* write data to buffer */
    if (memcpy((void *)(fb->buffer + fb->size), data, length) == NULL)
		ERROR("memcpy error\n", 1);

    fb->size = (size_t)new_size;

    return 0;
}

int file_buffer_synch(const File_buffer *fb)
{
	TRACE();

    if (fb == NULL)
		ERROR("fb == NULL\n", 1);

    if ((msync((void *)fb->buffer, fb->size, MS_SYNC)) == -1)
        ERROR("msync error\n", 1);

    return 0;
}

char *file_buffer_get_buff(const File_buffer *fb)
{
	TRACE();

	if (fb == NULL)
		ERROR("fb == NULL\n", NULL);

	return fb->buffer;
}

ssize_t file_buffer_get_size(const File_buffer *fb)
{
	TRACE();

	if (fb == NULL)
		ERROR("fb == NULL\n", -1);

	return (ssize_t)fb->size;
}

/**** FILE BUFFER 64 ****/

File_buffer *file_buffer64_create(int fd, int protect_flag)
{
	File_buffer *fb;
	struct stat64 ft;

	TRACE();

	if (fd < 0)
		ERROR("fd < 0\n", NULL);

    fb = (File_buffer *)malloc(sizeof(File_buffer));
    if (fb == NULL)
		ERROR("malloc error\n", NULL);

    fb->fd = fd;
    fb->protect_flag = protect_flag;

    /* read size of file ( in bytes ) */
    if (fstat64(fd, &ft) == -1)
	{
		FREE(fb);
		ERROR("fstat error\n", NULL);
	}

    fb->size = (size_t)ft.st_size;
	fb->mapped_size = align_size(fb->size);
    fb->has_private_file = false;
    fb->mode64 = true;

    if ((fb->buffer = (char *)mmap64(NULL, fb->mapped_size, protect_flag, MAP_SHARED, fd, 0)) == MAP_FAILED)
	{
		FREE(fb);
		ERROR("mmap error\n", NULL);
	}

    return fb;
}

File_buffer *file_buffer64_create_from_path(const char *path, int protect_flag, int open_flag)
{
    File_buffer *fb;
	int fd;

	TRACE();

	if (path == NULL)
		ERROR("path == NULL\n", NULL);

	fd = open64(path, open_flag, 0644);
	fb = file_buffer64_create(fd, protect_flag);
    if (fb == NULL)
        ERROR("file_buffer_create error\n", NULL);

    fb->has_private_file = true;
    return fb;
}

int file_buffer64_append(File_buffer * ___restrict___ fb, const char * ___restrict___ data)
{
	size_t length;
    off_t new_size;
	size_t new_aligned_size;

	TRACE();

	if (fb == NULL || data == NULL)
		ERROR("fb == NULL || data == NULL\n", 1);

    if (!fb->mode64)
        ERROR("file_buffer works in 32bits mode, incorrect functions\n", 1);

	length = strlen(data);
	if (length == 0)
		ERROR("data has 0 len\n", 1);

	new_size = (off_t)(fb->size + length);

	/* resize file */
    if (ftruncate64(fb->fd, new_size) == -1)
        ERROR("ftruncate error\n", 1);

	new_aligned_size = align_size((size_t)new_size);

	/* realloc mapped file in RAM */
	if (new_aligned_size > fb->mapped_size)
	{
    	if ((fb->buffer = (char *)mremap((void *)fb->buffer, fb->mapped_size, new_aligned_size, MREMAP_MAYMOVE)) == MAP_FAILED)
     		ERROR("mremap error\n", 1);

		fb->mapped_size = new_aligned_size;
	}

	/* write data to buffer */
    if (memcpy((void *)(fb->buffer + fb->size), data, length) == NULL)
		ERROR("memcpy error\n", 1);

    fb->size = (size_t)new_size;

    return 0;
}