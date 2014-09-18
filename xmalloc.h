#ifndef MALLOC_H_
#define MALLOC_H_

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Safe wrapper for malloc.
 *
 * In case of an out of memory error, it will panic().
 * This allows the developper to skip checking for error in cases where it is non recoverable.
 *
 * @note xmalloc also panics if size == 0.
 */
void* xmalloc(size_t size);

/** Wrapper for free(). Should be used every time. */
void xfree(void *p);

/** Wrapper for realloc() similar to xmalloc. */
void* xrealloc(void *p, size_t size);

#ifdef __cplusplus
}
#endif

#endif
