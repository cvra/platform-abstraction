#ifndef CRITICALSECTION_MOCK_H_
#define CRITICALSECTION_MOCK_H_

#include <stdint.h>
#include <assert.h>

#define CPU_CRITICAL_ENTER() { ++__mock_critical_depth; } 
#define CPU_CRITICAL_EXIT() { --__mock_critical_depth; }
#define CPU_SR_ALLOC() int16_t __mock_critical_depth = 0;

#define mock_critsec_get_depth() (__mock_critical_depth)
#define mock_critsec_is_critical() (__mock_critical_depth > 0 )

#endif
