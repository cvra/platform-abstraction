#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

/** Returns the time since the boot of the system in microseconds. */
uint32_t os_timestamp_get(void);

/** Updates the given timestamp and returns the difference. */
uint32_t os_timestamp_diff_and_update(uint32_t *ts);

#endif
