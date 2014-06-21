#include "CppUTest/TestHarness.h"

extern "C" {
#include "../semaphores.h"
#include "../mutex.h"
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


TEST_GROUP(MutexMockTestGroup)
{
    mutex_t *mutex;

    void setup(void)
    {
        mutex = os_mutex_create();
    }

    void teardown(void)
    {
        os_mutex_delete(mutex);
    }
};

TEST(MutexMockTestGroup, CanCreateMutex)
{
    CHECK_FALSE(mutex->acquired);
    CHECK_EQUAL(0, mutex->acquired_count)
}

TEST(MutexMockTestGroup, CanTakeMutex)
{
    os_mutex_take(mutex);
    CHECK_TRUE(mutex->acquired);
    CHECK_EQUAL(1, mutex->acquired_count);
}

TEST(MutexMockTestGroup, CanTryMutex)
{
    CHECK_TRUE(os_mutex_try(mutex));
    CHECK_TRUE(mutex->acquired);
    CHECK_FALSE(os_mutex_try(mutex));
    CHECK_EQUAL(1, mutex->acquired_count);
}

TEST(MutexMockTestGroup, CanTryTimeoutMutex)
{
    CHECK_TRUE(os_mutex_try_timeout(mutex, 42000));
    CHECK_TRUE(mutex->acquired);
    CHECK_FALSE(os_mutex_try_timeout(mutex, 69000));
    CHECK_EQUAL(1, mutex->acquired_count);
}

TEST(MutexMockTestGroup, CanReleaseMutex)
{
    os_mutex_take(mutex);
    os_mutex_release(mutex);

    CHECK_FALSE(mutex->acquired);
}

TEST_GROUP(CriticalSectionMockTestGroup)
{};


TEST(CriticalSectionMockTestGroup, CanAlloc)
{
    CRITICAL_SECTION_ALLOC();

    CHECK_FALSE(mock_critsec_is_critical());
}

TEST(CriticalSectionMockTestGroup, CanEnterCritical)
{
    CRITICAL_SECTION_ALLOC();
    CRITICAL_SECTION_ENTER();

    CHECK_EQUAL(mock_critsec_get_depth(),1);
}

TEST(CriticalSectionMockTestGroup, CanExitCritical)
{
    CRITICAL_SECTION_ALLOC();
    CRITICAL_SECTION_ENTER();

    CHECK_EQUAL(mock_critsec_get_depth(),1);

    CRITICAL_SECTION_EXIT();

    CHECK_EQUAL(mock_critsec_get_depth(),0);
}

TEST(CriticalSectionMockTestGroup, CanNestCritical)
{
    CRITICAL_SECTION_ALLOC();

    CRITICAL_SECTION_ENTER();
    CRITICAL_SECTION_ENTER();
    
    CHECK_EQUAL(mock_critsec_get_depth(),2);

    CRITICAL_SECTION_EXIT();

    CHECK_EQUAL(mock_critsec_get_depth(),1);

    CRITICAL_SECTION_EXIT();

    CHECK_EQUAL(mock_critsec_get_depth(),0);
}

TEST(CriticalSectionMockTestGroup, CanUseCriticalSectionBlock)
{
    CRITICAL_SECTION_ALLOC();

    CRITICAL_SECTION() {
        CHECK_EQUAL(mock_critsec_get_depth(),1);
    }

    CHECK_EQUAL(mock_critsec_get_depth(),0);
}

TEST(CriticalSectionMockTestGroup, CanNestCriticalSectionBlock)
{
    CRITICAL_SECTION_ALLOC();

    CRITICAL_SECTION() {

        CRITICAL_SECTION() {
            CHECK_EQUAL(mock_critsec_get_depth(),2);
        }

        CHECK_EQUAL(mock_critsec_get_depth(),1);
    }

    CHECK_EQUAL(mock_critsec_get_depth(),0);
}
