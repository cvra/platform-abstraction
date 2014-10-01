#ifndef CRITICALSECTION_H_
#define CRITICALSECTION_H_

#include <stdlib.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __unix__
#include "mock/criticalsection.h"
#else
#include <cpu.h>
#endif


/** Takes care of allocs for critical section */
#define CRITICAL_SECTION_ALLOC() CPU_SR_ALLOC();

#define CRITICAL_SECTION_ENTER() CPU_CRITICAL_ENTER();

#define CRITICAL_SECTION_EXIT() CPU_CRITICAL_EXIT();

#ifdef __cplusplus
}
#endif

#endif
