#include "CppUTest/TestHarness.h"

extern "C" {
#include "../semaphores.h"
}

TEST_GROUP(SemaphoreMockTestGroup)
{
    semaphore_t *sem;

    void teardown()
    {
        os_semaphore_delete(sem);
    }

};

TEST(SemaphoreMockTestGroup, CanCreateSemaphoreWithCount)
{
    sem = os_semaphore_create(1);
    CHECK_EQUAL(1, sem->count);
    CHECK_EQUAL(1, sem->max_count);
}

TEST(SemaphoreMockTestGroup, CanTakeSemaphore)
{
    sem = os_semaphore_create(1);

    os_semaphore_take(sem);
    CHECK_EQUAL(0, sem->count);
    CHECK_EQUAL(1, sem->acquired_count);
}

TEST(SemaphoreMockTestGroup, CanReleaseSemaphore)
{
    sem = os_semaphore_create(1);
    os_semaphore_take(sem);
    os_semaphore_release(sem);

    CHECK_EQUAL(1, sem->count);
}
