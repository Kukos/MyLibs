#ifndef FILE_BUFFER_H
#define FILE_BUFFER_H

/*
	Simple file buffer wchich mapped file to RAM

	Author: Michal Kukowski
	email: michalkukowski10@gmail.com

	LICENCE: GPL3
*/

#ifndef _GNU_SOURCE
	#define _GNU_SOURCE /* needed to use feature.h */
#endif

#include<sys/mman.h>

/*
	Buffer use mmap so you can pass those flags as protect_flag

       PROT_EXEC  Pages may be executed.

       PROT_READ  Pages may be read.

       PROT_WRITE Pages may be written.

       PROT_NONE  Pages may not be accessed.

*/

typedef struct file_buffer
{
    char 			*buffer;
    int 			size;
    int 			fd; /* file descriptor of buffered file */
    int 			protect_flag;

}file_buffer;

/*
    MAP file to RAM with flag

	PARAMS
	@IN fd - file descriptor
	@IN protect_flag - flags

	RETURN:
	%NULL if failure
	%Pointer to file_buffer if success
*/
file_buffer *file_buffer_create(int fd, int protect_flag);

/*
    Detached file and destroy structure

	PARAMS
	@IN fb - pointer to file buffer

	RETURN:
	%0 if success
	%Non-zero value if failure
*/
int file_buffer_destroy(file_buffer *fb);

/*
    Add to end of mapped file the content data

	PARAMS
	@IN file_buffer - pointer to file buffer
	@IN data - data in char format ( expected '/0' at the end )

	RETURN:
	%0 if success
	%Non-zero value if failure
*/
int file_buffer_append(file_buffer *fb, const char *data);

/*
    Synchronized mapped file with true file on disk

	PARAMS
	@IN fb - pointer to file buffer

	REUTRN:
	%0 if success
	%Non-zero value if failure
*/
int file_buffer_synch(file_buffer *fb);

#endif
