#include "CppUTest/TestHarness.h"

extern "C" {
#include "../threading.h"
}

static int thread_ran = 0;

void mythread(void *context)
{
    thread_ran++;
}

TEST_GROUP(ThreadingTestGroup)
{
    static const int prio = 10;
    THREAD_STACK mystack[2048];
};

TEST(ThreadingTestGroup, CanAllocateStack)
{
    /* Check that a 2048 byte stack really is 2048 bytes. */
    CHECK_EQUAL(2048, sizeof(mystack));
}

TEST(ThreadingTestGroup, StartingThreadMakesItRun)
{
    thread_ran = 0;
    thread_t *t = thread_create(mystack, sizeof(mystack), prio);
    thread_run(t, mythread, NULL);
    thread_destroy(t);

    CHECK_TRUE(thread_ran);
}
