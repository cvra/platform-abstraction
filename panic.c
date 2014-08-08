#include <stdio.h>
#include <stdlib.h>

void panic_impl(const char *file, int line, const char *msg, ...)
{
    printf("PANIC (%s:%d): %s\n", file, line, msg);

#ifdef __unix__
    abort();
#endif
}

void (*panic)(const char *file, int line, const char *msg, ...) = panic_impl;
