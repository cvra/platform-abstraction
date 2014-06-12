#include <stdlib.h>
#include <assert.h>
#include "../semaphores.h"

semaphore_t *os_semaphore_create(int count)
{
    semaphore_t *sem;
    sem = malloc(sizeof(semaphore_t));

    sem->count = count;
    sem->max_count = count;
    sem->acquired_count = 0;

    return sem;
}

void os_semaphore_delete(semaphore_t *sem)
{
    free(sem);
}

void os_semaphore_take(semaphore_t *sem)
{
    sem->count--;
    sem->acquired_count++;
}

void os_semaphore_release(semaphore_t *sem)
{
    sem->count ++;
    assert(sem->count <= sem->max_count);
}
