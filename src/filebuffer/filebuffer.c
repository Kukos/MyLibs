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

/* size to map empty file */
#define FAKE_FILE_SIZE	sizeof(char)

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

    if (fb->size == 0)
    {
		/* map virtual "fake" file */
        if ((fb->buffer = (char *)mmap(NULL, FAKE_FILE_SIZE,
			 	protect_flag, MAP_SHARED, fd, 0)) == MAP_FAILED)
		{
			FREE(fb);
			ERROR("mmap error\n", NULL, "");
		}
    }
    else
        if ((fb->buffer = (char *)mmap(NULL, fb->size, protect_flag,
				MAP_SHARED, fd, 0)) == MAP_FAILED)
		{
			FREE(fb);
			ERROR("mmap error\n", NULL, "");
		}

    return fb;
}

int file_buffer_destroy(file_buffer *fb)
{
	TRACE("");

    if (fb == NULL)
		return 1;

    /* before detach sunchronize */
    if ((msync((void *)fb->buffer, fb->size, MS_SYNC)) == -1)
         ERROR("msync error\n", 1, "");

    /* unmap file */
    if (munmap((void *)fb->buffer,
	 		fb->size == 0 ? FAKE_FILE_SIZE : fb->size) == -1)
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
    if ((fb->buffer = (char *)mremap((void *)fb->buffer, fb->size,
	 		new_size, MREMAP_MAYMOVE)) == MAP_FAILED)
     	ERROR("mremap error\n", 1, "");

	/* write data to buffer */
    if (memcpy((void *)(fb->buffer + fb->size), data, length) == NULL)
		ERROR("memcpy error\n", 1, "");

    fb->size = (int)new_size;

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
