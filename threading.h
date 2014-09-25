#ifndef THREADING_H_
#define THREADING_H_

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __unix__
#include "mock/threading.h"
#else
#include "ucos-iii/threading.h"
#endif

/** Initializes OS. */
void os_init(void);

/** Starts scheduling the created threads. Never returns */
void os_run(void);

/** Creates a new thread. */
void os_thread_create(os_thread_t *thread, void (*fn)(void *), void *stack, size_t stack_size, const char *name, unsigned int prio, void *arg);

/** Sleep for us microseconds or less */
void os_thread_sleep_us(uint32_t us);

/** Sleep for at least us microseconds */
void os_thread_sleep_least_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* THREADING_H_ */
