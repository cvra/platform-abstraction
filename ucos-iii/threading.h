#ifndef UCOS_III_THREADING_H_
#define UCOS_III_THREADING_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <errno.h>
#include <os.h>

#define THREAD_STACK __attribute((section(".noinit"))) uint8_t

typedef struct {
    OS_TCB ucos_tcb;
    struct _reent newlib_reent;
    bool dyn_alloc_stack;
    void *stack_base;
    size_t stack_size;
} os_thread_t;

#endif
