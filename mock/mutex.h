#ifndef MUTEX_MOCK_H_
#define MUTEX_MOCK_H_

#include <stdbool.h>

typedef struct {
    bool acquired;
    int acquired_count;
} mutex_t;

#endif
