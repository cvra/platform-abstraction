#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "criticalsection_capturing_mock.h"

extern "C" {
#include "../criticalsection.h"
}

void mock_critical_entered(void)
{
    mock().actualCall("CPU_CRITICAL_ENTER");
}

void mock_critical_exited(void)
{
    mock().actualCall("CPU_CRITICAL_EXIT");
}

void criticalsection_use_capturing_mock()
{
    UT_PTR_SET(critical_entered, mock_critical_entered);
    UT_PTR_SET(critical_exited, mock_critical_exited);
}


TEST_GROUP(CriticalSectionMockTestGroup)
{
    void setup(void)
    {
        criticalsection_use_capturing_mock();
        mock().strictOrder();
    }

    void teardown(void)
    {
        mock().clear();
    }

};

TEST(CriticalSectionMockTestGroup, EnterCriticalIsCapturedCorrectly)
{
    CRITICAL_SECTION_ALLOC();
    mock().expectOneCall("CPU_CRITICAL_ENTER");

    CRITICAL_SECTION_ENTER();

    mock().checkExpectations();
}

TEST(CriticalSectionMockTestGroup, ExitCriticalIsCaptured)
{
    CRITICAL_SECTION_ALLOC();
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    CRITICAL_SECTION_EXIT();

    mock().checkExpectations();
}

TEST(CriticalSectionMockTestGroup, BlockWorksToo)
{
    CRITICAL_SECTION_ALLOC();
    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    CRITICAL_SECTION() {

    }
    mock().checkExpectations();
}

TEST(CriticalSectionMockTestGroup, NestedWorksToo)
{
    CRITICAL_SECTION_ALLOC();
    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("A");
    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("B");
    mock().expectOneCall("CPU_CRITICAL_EXIT");
    mock().expectOneCall("C");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    CRITICAL_SECTION() {
        mock().actualCall("A");
        CRITICAL_SECTION() {
            mock().actualCall("B");
        }
        mock().actualCall("C");
    }
    mock().checkExpectations();
}

TEST(CriticalSectionMockTestGroup, CanBreakFromSection)
{
    CRITICAL_SECTION_ALLOC();
    mock().expectOneCall("CPU_CRITICAL_ENTER");
    mock().expectOneCall("CPU_CRITICAL_EXIT");

    CRITICAL_SECTION() {
        break;
        FAIL("Should not get there !");
    }

    mock().checkExpectations();
}

