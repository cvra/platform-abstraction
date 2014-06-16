#ifndef CRITICALSECTION_H_
#define CRITICALSECTION_H_

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#ifdef __unix__
#define CPU_CRITICAL_ENTER()
#define CPU_CRITICAL_EXIT()
#define CPU_SR_ALLOC()
#else
#include <cpu.h>
#endif

/** Takes care of necessary allocs for critical section
 *  \warning Call after all variable initializations and before all functions
 */
#define CRITICAL_SECTION_ALLOC() int16_t __critctrl; int16_t __critsection; CPU_SR_ALLOC();

/** Critical section macro
 * \warn Will generate a compile-time bug if CRITICAL_SECTION_ALLOC hasn't been called beforehand
 */
#define CRITICAL_SECTION() assert(sizeof(__critctrl)>0); for(__critctrl = -1; __critctrl < 2; __critctrl++) \
    if(__critctrl < 0) \
    { CPU_CRITICAL_ENTER(); } \
    else if(__critctrl > 0) \
    { CPU_CRITICAL_EXIT(); break; }\
    else for(__critsection = 0; __critsection<1; __critsection++) 
    
#endif
