#include "CppUTest/TestHarness.h"

extern "C" {
#include "../threading.h"
}

TEST_GROUP(ThreadingAPITestGroup)
{
};

TEST(ThreadingAPITestGroup, CanAllocateStackStatically)
{
    THREAD_STACK(mystack, 2048);
    CHECK_EQUAL(sizeof(thread_stack_t), sizeof(mystack));
}

TEST(ThreadingAPITestGroup, StaticAllocationFillsSizeField)
{
    THREAD_STACK(mystack, 2048);
    CHECK_EQUAL(2048, mystack.size);
}

TEST(ThreadingAPITestGroup, CanAccessMiddleOfStack)
{
    THREAD_STACK(mystack, 2048);
    mystack.stack[1024] = 42; /* should not crash */
    CHECK_EQUAL(mystack.stack[1024], 42);
}

TEST(ThreadingAPITestGroup, CanDynamicallyAllocateStack)
{
    thread_stack_t *mystack = thread_stack_create(2048);
    CHECK_EQUAL(2048, mystack->size);

    thread_stack_delete(mystack);
}

TEST(ThreadingAPITestGroup, CanAccessDynamicallyAllocatedStack)
{
    thread_stack_t *mystack = thread_stack_create(2048);
    mystack->stack[1024] = 42;
    CHECK_EQUAL(42, mystack->stack[1024]);
    thread_stack_delete(mystack);
}
