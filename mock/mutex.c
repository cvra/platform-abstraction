#include <stdlib.h>
#include <assert.h>
#include "../mutex.h"

mutex_t *os_mutex_create(void)
{
    mutex_t *mutex = malloc(sizeof(mutex_t));

    mutex->acquired = false;
    mutex->acquired_count = 0;

    return mutex;
}

void os_mutex_delete(mutex_t *mutex)
{
    free(mutex);
}

void os_mutex_take(mutex_t *mutex)
{
    assert(mutex->acquired == false);
    mutex->acquired = true;
    mutex->acquired_count++;
}

void os_mutex_release(mutex_t *mutex)
{
    assert(mutex->acquired == true);
    mutex->acquired = false;
}
