#ifndef SEMAPHORES_MOCK_H_
#define SEMAPHORES_MOCK_H_

/** A semaphore structure. The implementation is public to allow to inspect
 * state when in tests. */
typedef struct {
    int count;
    int acquired_count;
} semaphore_t;

#endif
