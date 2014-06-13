#include <stdlib.h>
#include <assert.h>
#include "../semaphores.h"

semaphore_t *os_semaphore_create(int count)
{
    semaphore_t *sem;
    sem = malloc(sizeof(semaphore_t));

    sem->count = count;
    sem->acquired_count = 0;

    return sem;
}

void os_semaphore_delete(semaphore_t *sem)
{
    free(sem);
}

void os_semaphore_take(semaphore_t *sem)
{
    assert(sem->count > 0);
    sem->count--;
    sem->acquired_count++;
}

bool os_semaphore_try(semaphore_t *sem){
    if (sem->count < 1) {
        return false;
    } else {
        sem->count--;
        sem->acquired_count++;
        return true;
    }
}

void os_semaphore_release(semaphore_t *sem)
{
    sem->count ++;
}
