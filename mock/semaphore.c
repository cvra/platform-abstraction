#include <assert.h>
#include "../semaphore.h"
#include "../xmalloc.h"

void os_semaphore_init(semaphore_t *sem, uint32_t count)
{
    sem->count = count;
    sem->acquired_count = 0;
}

void os_semaphore_wait(semaphore_t *sem)
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

void os_semaphore_signal(semaphore_t *sem)
{
    sem->count ++;
}
