#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#include "mock/semaphore.h"
#else
#include "ucos-iii/semaphore.h"
#endif

/** Initializes the semaphore with given count value. */
void os_semaphore_init(semaphore_t *sem, uint32_t count);

/** Decrements the semaphore if available, blocks if not available. */
void os_semaphore_wait(semaphore_t *sem);

/** Decrements the semaphore non-blocking.
 *
 * @return true if the semaphore was decremented, false if it would have blocked.
 */
bool os_semaphore_try(semaphore_t *sem);

/** Tries to decrements the semaphore before a given timeout (in us).
 *
 * @return true if it was decremented, false if it timed out.
 */
bool os_semaphore_try_timeout(semaphore_t *sem, uint32_t timeout);

/** Increments the semaphore. */
void os_semaphore_signal(semaphore_t *sem);

#ifdef __cplusplus
}
#endif

#endif
