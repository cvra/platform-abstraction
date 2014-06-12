#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_

#ifdef __unix__
#include "platform/mock/semaphores.h"
#endif

/** Creates a semaphore with given count value. */
semaphore_t *platform_semaphore_create(int count);

#endif
