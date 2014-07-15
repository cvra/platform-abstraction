#ifndef CRITICALSECTION_MOCK_H_
#define CRITICALSECTION_MOCK_H_

#include <stdint.h>
#include <assert.h>

#define CPU_CRITICAL_ENTER() {critical_entered(); }
#define CPU_CRITICAL_EXIT() {critical_exited(); }
#define CPU_SR_ALLOC() {}

extern void (*critical_entered)(void);
extern void (*critical_exited)(void);

#endif
