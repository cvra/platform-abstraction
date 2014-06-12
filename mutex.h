#ifndef MUTEX_H_
#define MUTEX_H_

#ifdef __unix__
#include "mock/mutex.h"
#endif

/** Creates a new mutex. */
mutex_t *os_mutex_create(void);

/** Frees the memory and OS structures used by a mutex. */
void os_mutex_delete(mutex_t *mutex);

/** Doesn't return until the mutex is acquired. */
void os_mutex_take(mutex_t *mutex);

/** Release a mutex. */
void os_mutex_release(mutex_t *mutex);

#endif
