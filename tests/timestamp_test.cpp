#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

extern "C" {
#include "../timestamp.h"
}

TEST_GROUP(TimestampTestGroup)
{
    void teardown()
    {
        mock().clear();
    }
};

TEST(TimestampTestGroup, CanGetTime)
{
    uint32_t time;

    mock().expectOneCall("os_timestamp_get").andReturnValue(42);
    time = os_timestamp_get();
    CHECK_EQUAL(42, time);
}

TEST(TimestampTestGroup, CanUpdateTime)
{
    uint32_t time = 0;
    mock().expectOneCall("os_timestamp_get").andReturnValue(42);
    mock().expectOneCall("os_timestamp_get").andReturnValue(100);

    os_timestamp_diff_and_update(&time);
    CHECK_EQUAL(42, time);

    os_timestamp_diff_and_update(&time);
    CHECK_EQUAL(100, time);
}

TEST(TimestampTestGroup, CanGetDifference)
{
    uint32_t time=1000, diff;
    mock().expectOneCall("os_timestamp_get").andReturnValue(2500);

    diff = os_timestamp_diff_and_update(&time);
    CHECK_EQUAL(1500, diff)
}


