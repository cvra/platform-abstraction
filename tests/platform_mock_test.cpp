#include "CppUTest/TestHarness.h"

extern "C" {
#include "../semaphores.h"
}

TEST_GROUP(SemaphoreMockTestGroup)
{
    semaphore_t *sem;

    void teardown()
    {
        platform_semaphore_delete(sem);
    }

};

TEST(SemaphoreMockTestGroup, CanCreateSemaphoreWithCount)
{
    sem = platform_semaphore_create(1);
    CHECK_EQUAL(1, sem->count);
}

TEST(SemaphoreMockTestGroup, CanTakeSemaphore)
{
    sem = platform_semaphore_create(1);

    platform_semaphore_take(sem);
    CHECK_EQUAL(0, sem->count);
    CHECK_EQUAL(1, sem->acquired_count);
}

TEST(SemaphoreMockTestGroup, CanReleaseSemaphore)
{
    sem = platform_semaphore_create(1);
    platform_semaphore_release(sem);

    CHECK_EQUAL(2, sem->count);
}
