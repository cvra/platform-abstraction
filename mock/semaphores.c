#include <assert.h>
#include "../semaphores.h"
#include "../xmalloc.h"

semaphore_t *os_semaphore_create(uint32_t count)
{
    semaphore_t *sem;
    sem = xmalloc(sizeof(semaphore_t));

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

bool os_semaphore_try_timeout(semaphore_t *sem, uint32_t timeout)
{
    assert(timeout >= 0);
    return os_semaphore_try(sem);
}

void os_semaphore_release(semaphore_t *sem)
{
    sem->count ++;
}
