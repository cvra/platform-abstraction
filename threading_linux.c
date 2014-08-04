#include "threading.h"
#include "xmalloc.h"

thread_t* thread_create(THREAD_STACK *stack, size_t mystack, unsigned int priority)
{
    thread_t *self;

    self = xmalloc(sizeof(thread_t));

    return self;
}

void thread_run(thread_t *self, void (*func)(void *), void *context)
{
    pthread_create(&self->pthread, NULL, func, context);
}

void thread_destroy(thread_t *self)
{
    pthread_join(self->pthread, NULL);
    xfree(self);
}
