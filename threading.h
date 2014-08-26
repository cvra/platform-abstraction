#ifndef THREADING_H_
#define THREADING_H_

#include <stdint.h>
#include <stddef.h>

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

/** Sleep for n milliseconds */
void os_thread_sleep_ms(uint32_t millisec);

#endif /* THREADING_H_ */
