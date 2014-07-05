#include "xmalloc.h"
#include "panic.h"

void* xmalloc(size_t size)
{
    void *result;

    if (size == 0) {
        return NULL;
    }

    result = malloc(size);

    if (result == NULL) {
        PANIC("out of memory!");
    }

    return result;
}

void xfree(void *p)
{
    free(p);
}
