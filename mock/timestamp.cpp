#include "CppUTestExt/MockSupport.h"
extern "C" {
#include "../timestamp.h"
}

uint32_t os_timestamp_get(void)
{
    return mock().actualCall("os_timestamp_get").returnIntValue();
}
