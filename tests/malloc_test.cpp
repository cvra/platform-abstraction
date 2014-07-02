#include "CppUTest/TestHarness.h"

extern "C" {
#include "../xmalloc.h"
}

TEST_GROUP(MallocTestGroup) {

};

TEST(MallocTestGroup, CanAllocateMemory)
{
    int *array = (int *)xmalloc(100);
    array[99] = 42;
    CHECK_EQUAL(42, array[99]);
    xfree(array);
}
