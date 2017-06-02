#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <execinfo.h>
#include <common.h>
#include <compiler.h>
#include <string.h>
#include <stdlib.h>

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
static ___inline___ void print_func_trace(char *str);

static ___inline___ void print_func_trace(char *str)
{
    const char *spaces = "                                                                         ";
    char *name;
    char *off;
    char *addr;

    char *end_off;
    char *end_addr;

    char *empty = (char *)"";
    char *empty_name = (char *)"()";

    name = strchr((const char *)str, '(');
    if (name == NULL || *(name + 1) == ')')
        name = empty_name;
    else
        ++name;

    off =  strchr((const char *)str, '+');
    if (off == NULL)
        off = empty;
    else
        ++off;

    addr = strchr((const char *)str, '[');
    if (addr == NULL)
        addr = empty;
    else
        ++addr;

    end_addr = strchr((const char *)str, ']');
    if (end_addr != NULL)
        *end_addr = '\0';

    end_off = strchr((const char *)str, ')');
    if (end_off != NULL)
        *end_off = '\0';

    if (off != empty)
        *(off - 1) = '\0';

    (void)fprintf(stderr, "NAME = %s%.*s", name, strlen(name) > NLEN ? 0 : (int)(NLEN - strlen(name)), spaces);
    (void)fprintf(stderr, "ADDR = %s%.*s", addr, strlen(addr) > ALEN ? 0 : (int)(ALEN - strlen(addr)), spaces);
    (void)fprintf(stderr, "OFF = %s%.*s\n", off, strlen(off) > OLEN ? 0 : (int)(OLEN - strlen(off)), spaces);
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

    va_start(args, msg);
    (void)vfprintf(stderr, msg, args);
    va_end(args);
}
