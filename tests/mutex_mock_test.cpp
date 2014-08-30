#include "CppUTest/TestHarness.h"

extern "C" {
#include "../mutex.h"
}

TEST_GROUP(MutexMockTestGroup)
{
    mutex_t mutex;

    void setup(void)
    {
        os_mutex_init(&mutex);
    }
};

TEST(MutexMockTestGroup, CanCreateMutex)
{
    // Put some garbage values
    mutex.acquired = 1;
    os_mutex_init(&mutex);
    CHECK_FALSE(mutex.acquired);
    CHECK_EQUAL(0, mutex.acquired_count)
}

TEST(MutexMockTestGroup, CanTakeMutex)
{
    os_mutex_take(&mutex);
    CHECK_TRUE(mutex.acquired);
    CHECK_EQUAL(1, mutex.acquired_count);
}

TEST(MutexMockTestGroup, CanTryMutex)
{
    CHECK_TRUE(os_mutex_try(&mutex));
    CHECK_TRUE(mutex.acquired);
    CHECK_FALSE(os_mutex_try(&mutex));
    CHECK_EQUAL(1, mutex.acquired_count);
}

TEST(MutexMockTestGroup, CanTryTimeoutMutex)
{
    CHECK_TRUE(os_mutex_try_timeout(&mutex, 42000));
    CHECK_TRUE(mutex.acquired);
    CHECK_FALSE(os_mutex_try_timeout(&mutex, 69000));
    CHECK_EQUAL(1, mutex.acquired_count);
}

TEST(MutexMockTestGroup, CanReleaseMutex)
{
    os_mutex_take(&mutex);
    os_mutex_release(&mutex);

    CHECK_FALSE(mutex.acquired);
}

