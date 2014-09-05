#include "timestamp.h"

uint32_t os_timestamp_diff_and_update(uint32_t *ts)
{
    uint32_t diff, now;
    now = os_timestamp_get();
    diff = now - *ts;
    *ts = now;
    return diff;
}
