#include "CppUTest/TestHarness.h"

extern "C" {
#include "../threading.h"
}

TEST_GROUP(ThreadingTestGroup)
{
};

TEST(ThreadingTestGroup, CanAllocateStack)
{
    /* Check that a 2048 byte stack really is 2048 bytes. */
    THREAD_STACK mystack[2048];

    CHECK_EQUAL(2048, sizeof(mystack));
}
