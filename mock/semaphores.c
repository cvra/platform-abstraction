#include <stdlib.h>
#include "platform/semaphores.h"

semaphore_t *platform_semaphore_create(int count)
{
    semaphore_t *sem;
    sem = malloc(sizeof(semaphore_t));

    sem->count = count;
    sem->acquired_count = 0;

    return sem;
}

void platform_semaphore_delete(semaphore_t *sem)
{
    free(sem);
}

void platform_semaphore_take(semaphore_t *sem)
{
    sem->count--;
    sem->acquired_count++;
}

void platform_semaphore_release(semaphore_t *sem)
{
    sem->count ++;
}
