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
#define CRITICAL_SECTION_ALLOC() int16_t __critctrl; int16_t __critical_alloc; CPU_SR_ALLOC();

#define CRITICAL_SECTION() for(__critctrl = -1; __critctrl < 2 && sizeof(__critical_alloc)>0; __critctrl++) \
                                                    if(__critctrl < 0) \
        {    CPU_CRITICAL_ENTER(); } \
        else if(__critctrl > 0) \
        {   CPU_CRITICAL_EXIT(); __critctrl = 0; break; }\
        else for(__critical_alloc = 0; __critical_alloc<1; __critical_alloc++)


#define CRITICAL_SECTION_ENTER() {if(sizeof(__critical_alloc)>0){CPU_CRITICAL_ENTER();(void) __critctrl;}}

#define CRITICAL_SECTION_EXIT() {if(sizeof(__critical_alloc)>0){CPU_CRITICAL_EXIT();}}

#ifdef __cplusplus
}
#endif

#endif
