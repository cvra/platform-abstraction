#include <stdio.h>
#include <stdlib.h>

void panic_impl(const char *msg)
{
    printf("PANIC: %s\n", msg);

#ifdef __unix__
    abort();

#endif
}

void (*panic)(const char *msg) = panic_impl;
