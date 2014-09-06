#include "CppUTest/TestHarness.h"

extern "C" {
#include "../semaphore.h"
}

TEST_GROUP(SemaphoreMockTestGroup)
{
    semaphore_t sem;
};

TEST(SemaphoreMockTestGroup, CanCreateSemaphoreWithCount)
{
    os_semaphore_init(&sem, 1);
    CHECK_EQUAL(1, sem.count);
}

TEST(SemaphoreMockTestGroup, CanTakeSemaphore)
{
    os_semaphore_init(&sem, 1);

    os_semaphore_wait(&sem);
    CHECK_EQUAL(0, sem.count);
    CHECK_EQUAL(1, sem.acquired_count);
}

TEST(SemaphoreMockTestGroup, CanTrySemaphore)
{
    os_semaphore_init(&sem, 1);

    CHECK_TRUE(os_semaphore_try(&sem));
    CHECK_EQUAL(0, sem.count);
    CHECK_FALSE(os_semaphore_try(&sem));
    CHECK_EQUAL(0, sem.count);
    CHECK_EQUAL(1, sem.acquired_count);
}

TEST(SemaphoreMockTestGroup, CanTryTimeoutSemaphore)
{
    os_semaphore_init(&sem, 1);

    CHECK_TRUE(os_semaphore_try_timeout(&sem, 42000));
    CHECK_EQUAL(0, sem.count);
    CHECK_FALSE(os_semaphore_try_timeout(&sem, 69000));
    CHECK_EQUAL(0, sem.count);
    CHECK_EQUAL(1, sem.acquired_count);
}

TEST(SemaphoreMockTestGroup, CanSignalSemaphore)
{
    os_semaphore_init(&sem, 1);
    os_semaphore_signal(&sem);

    CHECK_EQUAL(2, sem.count);
}


