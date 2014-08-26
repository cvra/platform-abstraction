
#include "../panic.h"
#include "../semaphore.h"

void os_semaphore_init(semaphore_t *sem, uint32_t count)
{
    OS_ERR err;

    OSSemCreate(&sem->ucos_sem, "semaphore", count, &err);

    if (err != OS_ERR_NONE) {
        PANIC("Semaphore init: %d", err);
    }
}

void os_semaphore_take(semaphore_t *sem)
{
    OS_ERR err;

    OSSemPend(&sem->ucos_sem, 0, OS_OPT_PEND_BLOCKING, NULL, &err);

    if (err != OS_ERR_NONE) {
        PANIC("Semaphore take: %d", err);
    }
}

bool os_semaphore_try(semaphore_t *sem)
{
    OS_ERR err;

    OSSemPend(&sem->ucos_sem, 0, OS_OPT_PEND_NON_BLOCKING, NULL, &err);

    switch (err) {
        case OS_ERR_NONE:
            return true;
        case OS_ERR_PEND_WOULD_BLOCK:
            return false;
        default:
            PANIC("Semaphore try: %d", err);
            return false;
    }
}

bool os_semaphore_try_timeout(semaphore_t *sem, uint32_t timeout)
{
    OS_ERR err;

    OSSemPend(&sem->ucos_sem, (OS_TICK)timeout, OS_OPT_PEND_NON_BLOCKING, NULL, &err);

    switch (err) {
        case OS_ERR_NONE:
            return true;
        case OS_ERR_TIMEOUT:
            return false;
        default:
            PANIC("Semaphore try: %d", err);
            return false;
    }
}

void os_semaphore_release(semaphore_t *sem)
{
    OS_ERR err;

    OSSemPost(&sem->ucos_sem, OS_OPT_POST_1, &err);

    if (err != OS_ERR_NONE) {
        PANIC("Semaphore release: %d", err);
    }
}
