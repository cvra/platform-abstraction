#ifndef MUTEX_MOCK_H_
#define MUTEX_MOCK_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool acquired;
    uint32_t acquired_count;
} mutex_t;

#endif
