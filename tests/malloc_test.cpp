#include "CppUTest/TestHarness.h"

extern "C" {
#include "../xmalloc.h"
#include "../panic.h"
}


static int panic_count;
static void panic_counter(const char *file, int line, const char *msg)
{
    panic_count++;
}

TEST_GROUP(MallocTestGroup) {
    void setup()
    {
        UT_PTR_SET(panic, panic_counter);
        panic_count = 0;
    }
};

TEST(MallocTestGroup, CanAllocateMemory)
{
    int *array = (int *)xmalloc(100);
    array[99] = 42;
    CHECK_EQUAL(42, array[99]);
    xfree(array);
}

TEST(MallocTestGroup, CreatingHugeAmountOfMemoryFails)
{
    /* Find max value of size_t */
    size_t max_size = (size_t) -1;

    xmalloc(max_size);
    CHECK_EQUAL(1, panic_count);
}

TEST(MallocTestGroup, ZeroSizeDoesntCrash)
{
    void *ptr = xmalloc(0);
    POINTERS_EQUAL(NULL, ptr);
    CHECK_EQUAL(0, panic_count);
}
