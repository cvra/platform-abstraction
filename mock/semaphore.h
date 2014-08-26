#ifndef SEMAPHORE_MOCK_H_
#define SEMAPHORE_MOCK_H_

/** A semaphore structure. The implementation is public to allow to inspect
 * state when in tests. */
typedef struct {
    uint32_t count;
    uint32_t acquired_count;
} semaphore_t;

#endif
