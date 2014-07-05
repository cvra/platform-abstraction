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
    int *array;

    /* Find max value of size_t */
    static const size_t max_size = (size_t) -1;

    void setup()
    {
        UT_PTR_SET(panic, panic_counter);
        panic_count = 0;
        array = NULL;
    }

    void teardown()
    {
        xfree(array);
    }
};

TEST(MallocTestGroup, CanAllocateMemory)
{
    array = (int *)xmalloc(10*sizeof(int));
    array[9] = 42;
    CHECK_EQUAL(42, array[9]);
}

TEST(MallocTestGroup, CreatingHugeAmountOfMemoryFails)
{
    xmalloc(max_size);
    CHECK_EQUAL(1, panic_count);
}

TEST(MallocTestGroup, ZeroSizeDoesntCrash)
{
    xmalloc(0);
    CHECK_EQUAL(0, panic_count);
}

TEST(MallocTestGroup, XReallocWorksToo)
{
    int *array = (int *)xmalloc(10*sizeof(int));
    array[9] = 42;
    array = (int *)xrealloc(array, 100);
    array[99] = 43;

    CHECK_EQUAL(42, array[9]);
    CHECK_EQUAL(43, array[99]);
}

TEST(MallocTestGroup, XReallocCanFailOnHugeSize)
{
    xrealloc(NULL, max_size);
    CHECK_EQUAL(1, panic_count);
}
