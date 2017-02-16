#include <log.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef DEBUG_MODE

void __log__(const char *msg, ...)
{
	va_list args;

    va_start (args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
}

void __trace_call__(const char *msg, ...)
{
    va_list args;

    va_start (args, msg);
	vfprintf(stderr, msg, args);
	va_end(args);
}

#else

/* empty function, because we have not enable debugs */
void __log__(const char *msg, ...) { }
void __trace_call__(const char *msg, ...) { }

#endif
