#ifndef FILE_BUFFER_H
#define FILE_BUFFER_H

/*
	Simple file buffer wchich mapped file to RAM

	Author: Michal Kukowski
	email: michalkukowski10@gmail.com

	LICENCE: GPL 3.0
*/

#include <sys/mman.h>
#include <stddef.h> /* size_t */
#include <sys/types.h> /* ssize_t */
#include <fcntl.h>
#include <common.h>

/*
	Buffer use mmap so you can pass those flags as protect_flag

       PROT_EXEC  Pages may be executed.

       PROT_READ  Pages may be read.

       PROT_WRITE Pages may be written.

       PROT_NONE  Pages may not be accessed.

    example:
        int fd = open("path_to_file", O_RDWR, 0644);
        File_buffer  *fb = file_buffer_create(fd, PROT_READ | PROT_WRITE);

        or equivalently:
        File_buffer  *fb = file_buffer_create_from_path("path_to_file", PROT_READ | PROT_WRITE, O_RDWR);

*/

typedef struct File_buffer File_buffer;

/*
    MAP file to RAM with flag

	PARAMS
	@IN fd - file descriptor
	@IN protect_flag - flags

	RETURN:
	%NULL iff failure
	%Pointer to File_buffer iff success
*/
File_buffer *file_buffer_create(int fd, int protect_flag);

/*
    MAP file to RAM with flag in 64 bits MODE

	PARAMS
	@IN fd - file descriptor
	@IN protect_flag - flags

	RETURN:
	%NULL iff failure
	%Pointer to File_buffer iff success
*/
File_buffer *file_buffer64_create(int fd, int protect_flag);

/*
    MAP file to RAM with flag

	PARAMS
	@IN path - path to file
	@IN protect_flag - flags
	@IN open_flag - flags to open file

	RETURN:
	%NULL iff failure
	%Pointer to File_buffer iff success
*/
File_buffer *file_buffer_create_from_path(const char *path, int protect_flag, int open_flag);

/*
    MAP file to RAM with flag in 64 bits MODE

	PARAMS
	@IN path - path to file
	@IN protect_flag - flags
	@IN open_flag - flags to open file

	RETURN:
	%NULL iff failure
	%Pointer to File_buffer iff success
*/
File_buffer *file_buffer64_create_from_path(const char *path, int protect_flag, int open_flag);

/*
    Detach file and destroy structure

	PARAMS
	@IN fb - pointer to file buffer

	RETURN:
	%0 iff success
	%Non-zero value iff failure
*/
int file_buffer_destroy(File_buffer *fb);

/*
    Add to the end of mapped file the content data

	PARAMS
	@IN File_buffer - pointer to file buffer
	@IN data - data in char format ( expected '/0' at the end )

	RETURN:
	%0 iff success
	%Non-zero value iff failure
*/
int file_buffer_append(File_buffer * ___restrict___ fb, const char * ___restrict___ data);

/*
    Add to the end of mapped file the content data in 64 bits MODE

	PARAMS
	@IN File_buffer - pointer to file buffer
	@IN data - data in char format ( expected '/0' at the end )

	RETURN:
	%0 iff success
	%Non-zero value iff failure
*/
int file_buffer64_append(File_buffer * ___restrict___ fb, const char * ___restrict___ data);

/*
    Synchronize mapped file with true file on disk

	PARAMS
	@IN fb - pointer to file buffer

	REUTRN:
	%0 iff success
	%Non-zero value iff failure
*/
int file_buffer_synch(const File_buffer *fb);

/*
	Get Buffer

	PARAMS
	@IN fb - pointer to file buffer

	RETURN:
	%Pointer to buffer iff success
	%NULL iff failure
*/
char *file_buffer_get_buff(const File_buffer *fb);

/*
	Get Size of buffered file

	PARAMS
	@IN fb - pointer to file buffer

	RETURN:
	%Size iff success
	%-1 iff failure
*/
ssize_t file_buffer_get_size(const File_buffer *fb);

#endif
