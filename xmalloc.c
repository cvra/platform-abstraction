#include "xmalloc.h"
#include "panic.h"

void *xmalloc(size_t size)
{
    void *result;

    result = malloc(size);

    if (result == NULL) {
        panic("out of memory!");
    }

    return result;
}

void xfree(void *p)
{
    free(p);
}
