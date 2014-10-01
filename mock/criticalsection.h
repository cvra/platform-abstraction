#ifndef CRITICALSECTION_MOCK_H_
#define CRITICALSECTION_MOCK_H_

#include <stdint.h>
#include <assert.h>

#define CPU_CRITICAL_ENTER() {critical_entered(); (void) __critical_section;}
#define CPU_CRITICAL_EXIT() {critical_exited(); (void) __critical_section;}
#define CPU_SR_ALLOC() void* __critical_section;

extern void (*critical_entered)(void);
extern void (*critical_exited)(void);

#endif
