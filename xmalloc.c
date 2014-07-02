#include "xmalloc.h"

void *xmalloc(size_t size)
{
    void *result;

    result = malloc(size);

    /* TODO if error : panic() */

    return result;
}

void xfree(void *p)
{
    free(p);
}
