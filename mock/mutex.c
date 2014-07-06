#include <assert.h>
#include "../mutex.h"
#include "../xmalloc.h"

mutex_t *os_mutex_create(void)
{
    mutex_t *mutex = xmalloc(sizeof(mutex_t));

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

bool os_mutex_try(mutex_t *mutex)
{
    if(mutex->acquired) {
        return false;
    } else {
        mutex->acquired = true;
        mutex->acquired_count++;
        return true;
    }
}

bool os_mutex_try_timeout(mutex_t *mutex, uint32_t timeout)
{
    assert(timeout >= 0);
    return os_mutex_try(mutex);
}

void os_mutex_release(mutex_t *mutex)
{
    assert(mutex->acquired == true);
    mutex->acquired = false;
}
