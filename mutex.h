#ifndef MUTEX_H_
#define MUTEX_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __unix__
#include "mock/mutex.h"
#else
#include "ucos-iii/mutex.h"
#endif

/** Initializes the mutex. */
void os_mutex_init(mutex_t *mutex);

/** Doesn't return until the mutex is acquired. */
void os_mutex_take(mutex_t *mutex);

/** Acquire the mutex non-blocking */
bool os_mutex_try(mutex_t *mutex);

/** Acquire the mutex blocking with timeout [us] */
bool os_mutex_try_timeout(mutex_t *mutex, uint32_t timeout);

/** Release a mutex. */
void os_mutex_release(mutex_t *mutex);

#endif
