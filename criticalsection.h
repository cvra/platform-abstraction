#ifndef CRITICALSECTION_H_
#define CRITICALSECTION_H_

#include <stdlib.h>
#include <stdint.h>

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
#define CRITICAL_SECTION_ALLOC() int16_t __critctrl; int16_t __critical_alloc; CPU_SR_ALLOC();

/** Critical section macro
 * \warn Will generate a compile-time bug if CRITICAL_SECTION_ALLOC hasn't been called beforehand
 */
#define CRITICAL_SECTION() for(__critctrl = -1; __critctrl < 2 && sizeof(__critical_alloc)>0; __critctrl++) \
                                                    if(__critctrl < 0) \
        {    CPU_CRITICAL_ENTER(); } \
        else if(__critctrl > 0) \
        {   CPU_CRITICAL_EXIT(); break; }\
        else for(__critical_alloc = 0; __critical_alloc<1; __critical_alloc++) 
  

/** Critical section enter macro
 * \warn Will generate a compile-time bug if CRITICAL_SECTION_ALLOC hasn't been called beforehand
 */
#define CRITICAL_SECTION_ENTER() {if(sizeof(__critical_alloc)>0){CPU_CRITICAL_ENTER();}}

/** Critical section exit macro
 * \warn Will generate a compile-time bug if CRITICAL_SECTION_ALLOC hasn't been called beforehand
 */
#define CRITICAL_SECTION_EXIT() {if(sizeof(__critical_alloc)>0){CPU_CRITICAL_EXIT();}}

int main(int argc, char** argv)
{
    uint16_t roll = 20;

    CRITICAL_SECTION_ALLOC();

    printf("Isn't critical... yet! \n");

    CRITICAL_SECTION_ENTER() 
        printf("Rolled a %d.. Critical printf ! \n",roll);
    CRITICAL_SECTION_EXIT()

    printf("Isn't critical... anymore! \n");

    CRITICAL_SECTION() {
        printf("Rolled a %d.. Critical printf again ! \n",roll);
    }
    
    return 0;
}
    
#endif
