#include "CppUTest/TestHarness.h"

extern "C" {
#include "../semaphores.h"
}

TEST_GROUP(SemaphoreMockTestGroup)
{
    semaphore_t *sem;

    void teardown(void)
    {
        os_semaphore_delete(sem);
    }

};

TEST(SemaphoreMockTestGroup, CanCreateSemaphoreWithCount)
{
    sem = os_semaphore_create(1);
    CHECK_EQUAL(1, sem->count);
}

TEST(SemaphoreMockTestGroup, CanTakeSemaphore)
{
    sem = os_semaphore_create(1);

    os_semaphore_take(sem);
    CHECK_EQUAL(0, sem->count);
    CHECK_EQUAL(1, sem->acquired_count);
}

TEST(SemaphoreMockTestGroup, CanTrySemaphore)
{
    sem = os_semaphore_create(1);

    CHECK_TRUE(os_semaphore_try(sem));
    CHECK_EQUAL(0, sem->count);
    CHECK_FALSE(os_semaphore_try(sem));
    CHECK_EQUAL(0, sem->count);
    CHECK_EQUAL(1, sem->acquired_count);
}

TEST(SemaphoreMockTestGroup, CanTryTimeoutSemaphore)
{
    sem = os_semaphore_create(1);

    CHECK_TRUE(os_semaphore_try_timeout(sem, 42000));
    CHECK_EQUAL(0, sem->count);
    CHECK_FALSE(os_semaphore_try_timeout(sem, 69000));
    CHECK_EQUAL(0, sem->count);
    CHECK_EQUAL(1, sem->acquired_count);
}

TEST(SemaphoreMockTestGroup, CanReleaseSemaphore)
{
    sem = os_semaphore_create(1);
    os_semaphore_release(sem);

    CHECK_EQUAL(2, sem->count);
}


