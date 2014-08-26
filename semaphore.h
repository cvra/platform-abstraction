#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __unix__
#include "mock/semaphore.h"
#else
#include "ucos-iii/semaphore.h"
#endif

/** Initializes the semaphore with given count value. */
void os_semaphore_init(semaphore_t *sem, uint32_t count);

/** Takes the semaphore if available, blocks if not available. */
void os_semaphore_take(semaphore_t *sem);

/** Takes the semaphore non-blocking */
bool os_semaphore_try(semaphore_t *sem);

/** Acquire the semaphore blocking with timeout [us] */
bool os_semaphore_try_timeout(semaphore_t *sem, uint32_t timeout);

/** Releases (posts) the semaphore. */
void os_semaphore_release(semaphore_t *sem);

#endif
