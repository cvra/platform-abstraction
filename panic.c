#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void panic_impl(const char *file, int line, const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);

    printf("PANIC (%s:%d): ", file, line);
    vprintf(msg, ap);

    va_end(ap);

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
    abort();
#endif
}

void (*panic)(const char *file, int line, const char *msg, ...) = panic_impl;
