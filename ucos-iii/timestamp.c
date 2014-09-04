
#include <stdint.h>
#include "../criticalsection.h"
#include "threading.h"
#include <os_cfg_app.h>

/* Note:
 * For the timestamps to be precise, the defines CPU_CFG_CPU_CORE_FREQ and
 * OS_CFG_TICK_RATE_HZ must fulfill following conditions:
 *  - CPU_CFG_CPU_CORE_FREQ must be a mutiple of 1MHz.
 *  - OS_CFG_TICK_RATE_HZ must be divisor of 1MHz.
 */

/* SysTick reload value register */
#define REG_SYSTICK_RVR             (*(volatile uint32_t *)0xE000E014)

/* SysTick current value register */
#define REG_SYSTICK_CVR             (*(volatile uint32_t *)0xE000E018)

uint32_t os_timestamp_get(void)
{
    uint32_t cur, ticks, timestamp;

    CRITICAL_SECTION_ALLOC();

    CRITICAL_SECTION_ENTER();

    /* read systick timer current value */
    cur = REG_SYSTICK_CVR;

    /* read timestamp counter */
    ticks = os_sys_ticks;

    CRITICAL_SECTION_EXIT();

    /* calculate time in microseconds */
    timestamp = ticks * (1000000 / OS_CFG_TICK_RATE_HZ);

    /* SysTick is counting down, subtract from reload value */
    cur = REG_SYSTICK_RVR - cur;

    /* add current timer value */
    timestamp += cur / (CPU_CFG_CPU_CORE_FREQ / 1000000);

    return timestamp;
}
