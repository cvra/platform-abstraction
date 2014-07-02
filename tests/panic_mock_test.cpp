#include "CppUTest/TestHarness.h"
#include <cstring>

extern "C" {
#include "../panic.h"
}

#define ERROR_MAX_LEN 1024

static char error[ERROR_MAX_LEN];

static void panic_dummy(const char *msg)
{
    strncpy(error, msg, ERROR_MAX_LEN);
}

TEST_GROUP(PanicTestGroup) {

    void setup(void)
    {
        UT_PTR_SET(panic, panic_dummy);
        memset(error, 0, ERROR_MAX_LEN);
    }
};

TEST(PanicTestGroup, CanGetPanicMessage)
{
    STRCMP_EQUAL("", error);
    panic("lol");
    STRCMP_EQUAL("lol", error);
}
