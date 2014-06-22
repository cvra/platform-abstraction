#include "threading.h"

thread_stack_t *thread_stack_create(size_t size)
{
    thread_stack_t *stack = malloc(sizeof(thread_stack_t));
    stack->size = size;
    stack->stack = malloc(stack->size);

    return stack;
}
