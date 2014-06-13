#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdbool.h>

#ifdef __unix__
#include "mock/mutex.h"
#endif

/** Creates a new mutex. */
mutex_t *os_mutex_create(void);

/** Frees the memory and OS structures used by a mutex. */
void os_mutex_delete(mutex_t *mutex);

/** Doesn't return until the mutex is acquired. */
void os_mutex_take(mutex_t *mutex);

/** Acquire the mutex non-blocking */
bool os_mutex_try(mutex_t *mutex);

/** Acquire the mutex blocking with timeout [ms] */
bool os_mutex_try_timeout(mutex_t *mutex, float timeout);

/** Release a mutex. */
void os_mutex_release(mutex_t *mutex);

#endif
