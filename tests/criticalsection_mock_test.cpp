#include "CppUTest/TestHarness.h"

extern "C" {
#include "../criticalsection.h"
}


TEST_GROUP(CriticalSectionMockTestGroup)
{
};


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
