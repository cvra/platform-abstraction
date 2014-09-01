#include <string.h>
#include <os.h>
#include <os_cfg_app.h>
#include "../threading.h"
#include "../panic.h"
#include "../mutex.h"
#include "../xmalloc.h"

/* default newlib context */
static struct _reent *default_newlib_reent;

/* mutex to guard malloc access */
static mutex_t malloc_mutex;
static bool os_running = false;

void os_init(void)
{
    CPU_Init();

    OS_ERR err;
    OSInit(&err);

    if (err != OS_ERR_NONE) {
       PANIC("Failed to init uCOS-III");
    }

    default_newlib_reent = _impure_ptr;

    os_mutex_init(&malloc_mutex);
}

void os_run(void)
{
    OS_ERR err;

    bool enable = true;
    /* switch tasks every system timer ticks */
    OS_TICK default_time_quanta = 1;

    /* enable round robin scheduling for tasks with same priority */
    OSSchedRoundRobinCfg(enable, default_time_quanta, &err);

    if (err != OS_ERR_NONE) {
       PANIC("Failed to configure scheduler");
    }

    /* init context switch timer for frequency OS_CFG_TICK_RATE_HZ */
    OS_CPU_SysTickInit(CPU_CFG_CPU_CORE_FREQ / OS_CFG_TICK_RATE_HZ);

    os_running = true;
    OSStart(&err);

    if (err != OS_ERR_NONE) {
        PANIC("Failed to start uCOS-III");
    }
}

void os_thread_create(os_thread_t *thread, void (*fn)(void *), void *stack, size_t stack_size, const char *name, unsigned int prio, void *arg)
{
    OS_ERR err;

    /* initialize newlib's reentrancy structure */
    _REENT_INIT_PTR(&thread->newlib_reent);

    /* uCOS-III priority: from 2 up to OS_CFG_PRIO_MAX - 3 */
    prio += 2;
    if (prio > OS_CFG_PRIO_MAX - 3) {
        PANIC("Invalid thread priority: %u", prio);
    }

    /* need to allocate the stack? */
    if (stack == NULL) {
        stack = xmalloc(stack_size);
        thread->dyn_alloc_stack = true;
    } else {
        thread->dyn_alloc_stack = false;
    }
    thread->stack_base = stack;
    thread->stack_size = stack_size;

    /* use default time quanta */
    OS_TICK time_quanta = 0;
    /* message queues are not (yet) in use */
    OS_MSG_QTY nb_msg_queue = 0;

    /* create a uCOS-III task */
    OSTaskCreate(&thread->ucos_tcb,          /* Address of TCB assigned to the task          */
                 (CPU_CHAR *)name,           /* Name you want to give the task               */
                 fn,                         /* Address of the task itself                   */
                 arg,                        /* Argument pointer                             */
                 prio,                       /* Priority you want to assign to the task      */
                 stack,                      /* Base address of task’s stack                 */
                 0,                          /* Watermark limit for stack growth, (not used) */
                 stack_size/sizeof(CPU_STK), /* Stack size in number of CPU_STK elements     */
                 nb_msg_queue,               /* Size of task message queue                   */
                 time_quanta,                /* Time quanta (in number of ticks)             */
                 thread,                     /* Extension pointer                            */
                 OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP, /* Options                       */
                 &err);                                     /* Error code                    */

    if (err != OS_ERR_NONE) {
        PANIC("Failed to create thread");
    }
}

/* Thread waits us microseconds or less. */
void os_thread_sleep_us(uint32_t us)
{
    OS_ERR err;
    CPU_INT16U hr = 0;
    CPU_INT16U min = 0;
    CPU_INT16U sec = 0;
    CPU_INT32U ms = us / 1000;

    if (ms == 0) {
        return;
    }

    /* uCOS-III will round to the nearest tick, even if that means no delay. */
    OSTimeDlyHMSM(hr, min, sec, ms, OS_OPT_TIME_HMSM_NON_STRICT, &err);
}

/* Thread waits at least us microseconds. */
void os_thread_sleep_least_us(uint32_t us)
{
    OS_ERR err;

    if (us == 0) {
        return;
    }

    /* round up to gurarantee a wait period of us microseconds */
    OS_TICK dly = (OS_TICK) (us - 1) * OS_CFG_TICK_RATE_HZ / 1000000  + 1;

    OSTimeDly(dly, OS_OPT_TIME_DLY, &err);
}

/* __malloc_lock() may be called several times before __malloc_unlock()
   uCOS-III mutexes can be taken multiple times by the owner. */
void __malloc_lock(void)
{
    if (os_running) {
        os_mutex_take(&malloc_mutex);
    }
}

void __malloc_unlock(void)
{
    if (os_running) {
        os_mutex_release(&malloc_mutex);
    }
}


/* OS hooks */

void OSTaskReturnHook(OS_TCB *p_tcb)
{
    os_thread_t *t = (os_thread_t *)p_tcb->ExtPtr;

    /* if t == uCOS-III internal task */
    if (t == NULL) {
        return;
    }

    /* need to free the stack? */
    if (t->dyn_alloc_stack) {
        xfree(t->stack_base);
    }
}

static inline void task_profile_update(void)
{
#if OS_CFG_TASK_PROFILE_EN > 0u
    CPU_TS  ts;
    /* Keep track of per-task running time */
    ts = OS_TS_GET();
    /* check if the running task changes */
    if (OSTCBCurPtr != OSTCBHighRdyPtr) {
        OSTCBCurPtr->CyclesDelta  = ts - OSTCBCurPtr->CyclesStart;
        OSTCBCurPtr->CyclesTotal += (OS_CYCLES)OSTCBCurPtr->CyclesDelta;
    }
    OSTCBHighRdyPtr->CyclesStart = ts;
#endif
}

static inline void int_disable_time_update(void)
{
#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_TS  int_dis_time;
    /* Keep track of per-task interrupt disable time */
    int_dis_time = CPU_IntDisMeasMaxCurReset();
    if (OSTCBCurPtr->IntDisTimeMax < int_dis_time) {
        OSTCBCurPtr->IntDisTimeMax = int_dis_time;
    }
#endif
}

static inline void sched_lock_time_update(void)
{
#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
    /* Keep track of per-task scheduler lock time */
    if (OSTCBCurPtr->SchedLockTimeMax < OSSchedLockTimeMaxCur) {
        OSTCBCurPtr->SchedLockTimeMax = OSSchedLockTimeMaxCur;
    }
    OSSchedLockTimeMaxCur = (CPU_TS)0;
#endif
}

static inline void trace_task_switch(void)
{
#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN > 0u))
    /* Record the event. */
    TRACE_OS_TASK_SWITCHED_IN(OSTCBHighRdyPtr);
#endif
}

static inline void newlib_context_switch(void)
{
    /* switch newlib context */
    os_thread_t *t = (os_thread_t *)OSTCBHighRdyPtr->ExtPtr;
    if (t != NULL) {
        _impure_ptr = &t->newlib_reent;
    } else {
        /* ExtPtr in uCOS-III internal tasks is initialized with NULL.
           Those tasks share the same newlib context, since it isn't used anyway */
        _impure_ptr = default_newlib_reent;
    }
}

void OSTaskSwHook(void)
{
    task_profile_update();
    int_disable_time_update();
    sched_lock_time_update();
    trace_task_switch();
    newlib_context_switch();
}

void OSInitHook(void)
{
    /* setup 8 byte aligned ISR stack pointer */
    OS_CPU_ExceptStkBase = (CPU_STK *)(OSCfg_ISRStkBasePtr + OSCfg_ISRStkSize);
    OS_CPU_ExceptStkBase = (CPU_STK *)((CPU_STK)(OS_CPU_ExceptStkBase) & 0xFFFFFFF8);
}


/* unused hooks, need to be implemented though */

void OSIdleTaskHook(void)
{

}

void OSTaskCreateHook(OS_TCB *p_tcb)
{

}

void OSTimeTickHook(void)
{

}

void OSTaskDelHook(OS_TCB *p_tcb)
{

}
