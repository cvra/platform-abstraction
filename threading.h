#ifndef THREADING_H_
#define THREADING_H_

#include <stdlib.h>

typedef unsigned char THREAD_STACK;

#ifdef __unix__
#include "threading_linux.h"
#endif

thread_t* thread_create(THREAD_STACK *stack, size_t mystack, unsigned int priority);
void thread_run(thread_t *self, void (*func)(void *), void *context);
void thread_destroy(thread_t *self);


#endif
