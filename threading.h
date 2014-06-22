#ifndef THREADING_H_
#define THREADING_H_

#include <stdlib.h>
#include <stdint.h>

/** Structure representing a thread stack. */
typedef struct {
    size_t size;
    uint8_t *stack;
} thread_stack_t;

/** Statically allocates some space and fills the given stack structure. */
#define THREAD_STACK(var, s) uint8_t __stack_ ## var [s];  \
    thread_stack_t var = {s, __stack_ ## var };

/** Dynamically allocates the memory for a stack on the heap. */
thread_stack_t *thread_stack_create(size_t size);

/** Frees the memory allocated by thread_stack_create. */
void thread_stack_delete(thread_stack_t *stack);


#endif
