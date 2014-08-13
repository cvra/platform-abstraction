#include "CppUTest/TestHarness.h"
#include <cstring>
#include <cstdio>

extern "C" {
#include "../panic.h"
}

#define ERROR_MAX_LEN 1024

static char error[ERROR_MAX_LEN];
static int line;

static void panic_dummy(const char *file, int l, const char *msg, ...)
{
    line = l;
    va_list ap;
    va_start(ap, msg);
    vsprintf(error, msg, ap);
    va_end(ap);
}

TEST_GROUP(PanicTestGroup) {

    void setup(void)
    {
        UT_PTR_SET(panic, panic_dummy);
        memset(error, 0, ERROR_MAX_LEN);
        line = 0;
    }
};

TEST(PanicTestGroup, CanGetPanicMessage)
{
    STRCMP_EQUAL("", error);
    PANIC("lol");
    CHECK_EQUAL(__LINE__-1, line);
    STRCMP_EQUAL("lol", error);
}

TEST(PanicTestGroup, CanUseVariableArguments)
{
    PANIC("foobar %d", 123);
    STRCMP_EQUAL("foobar 123", error);
}
