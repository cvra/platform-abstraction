#ifndef MALLOC_H_
#define MALLOC_H_

#include <stdlib.h>

/** Safe wrapper for malloc.
 *
 * In case of an out of memory error, it will panic().
 * This allows the developper to skip checking for error in cases where it is non recoverable.
 */
void* xmalloc(size_t size);

/** Wrapper for free(). Should be used every time. */
void xfree(void *p);


#endif
