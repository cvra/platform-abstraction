#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdbool.h>

#ifdef __unix__
#include "mock/semaphores.h"
#endif

/** Creates a semaphore with given count value. */
semaphore_t *os_semaphore_create(int count);

/** Frees the memory and operating system structures used by a semaphore. */
void os_semaphore_delete(semaphore_t *sem);

/** Takes a semaphore if available, blocks if not available. */
void os_semaphore_take(semaphore_t *sem);

/** Takes a semaphore non-blocking */
bool os_semaphore_try(semaphore_t *sem);

/** Releases (posts) a semaphore. */
void os_semaphore_release(semaphore_t *sem);

#endif
