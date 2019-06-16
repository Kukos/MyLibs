#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <execinfo.h>
#include <common.h>
#include <compiler.h>
#include <string.h>
#include <stdlib.h>
#include <log.h>

#define MAX_TRACE   100

#define NLEN    30
#define ALEN    20
#define OLEN    10

/*
    Print functions from backtrace

    PARAMS
    @IN str - string from backtrace_symbols

    RETURN
    THIS is a void function
*/
static ___inline___ void print_func_trace(const char *str);

static ___inline___ void print_func_trace(const char *str)
{
    const char * const spaces = "                                                                         ";
    char *name;
    char *off;
    char *addr;

    char *end_off;
    char *end_addr;

    const char * const empty = "";
    const char * const empty_name = "()";

    FILE *fd;
    FILE *file;

    fd = log_get_fd();
    file = log_get_file();

    name = strchr(str, '(');
    if (name == NULL || *(name + 1) == ')')
        name = (char *)empty_name;
    else
        ++name;

    off = strchr(str, '+');
    if (off == NULL)
        off = (char *)empty;
    else
        ++off;

    addr = strchr(str, '[');
    if (addr == NULL)
        addr = (char *)empty;
    else
        ++addr;

    end_addr = strchr(str, ']');
    if (end_addr != NULL)
        *end_addr = '\0';

    end_off = strchr(str, ')');
    if (end_off != NULL)
        *end_off = '\0';

    if (off != empty)
        *(off - 1) = '\0';

    fprintf(stderr, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
    fprintf(stderr, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
    fprintf(stderr, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);

    if (fd != NULL && fd != stderr)
    {
        fprintf(fd, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
        fprintf(fd, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
        fprintf(fd, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);
    }

    if (file != NULL)
    {
        fprintf(file, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
        fprintf(file, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
        fprintf(file, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);
    }
}

void stack_trace(void)
{
    void *t[MAX_TRACE];
    int size;
    char **strings;
    int i;

    size = backtrace(t, MAX_TRACE);
    strings = backtrace_symbols(t, size);
    if (strings == NULL)
        return;

    for (i = 1; i < size; ++i)
        print_func_trace(strings[i]);

    FREE(strings);
}

void __assert_msg(const char *msg, ...)
{
    va_list args;
    FILE *fd;
    FILE *file;

    fd = log_get_fd();
    file = log_get_file();

    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);

    if (fd != NULL && fd != stderr)
    {
        va_start(args, msg);
        vfprintf(fd, msg, args);
        va_end(args);
    }

    if (file != NULL)
    {
        va_start(args, msg);
        vfprintf(file, msg, args);
        va_end(args);
    }
}
