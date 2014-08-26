#include <string.h>
#include <os.h>
#include "../threading.h"
#include "../panic.h"
#include "../mutex.h"
#include "../xmalloc.h"

/* default newlib context */
struct _reent *default_newlib_reent;

/* mutex to guard malloc access */
mutex_t malloc_mutex;


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

    OSSchedRoundRobinCfg(true, 1, &err);

    if (err != OS_ERR_NONE) {
       PANIC("Failed to configure scheduler");
    }

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
        PANIC("Invalid thread priority");
    }

    if (stack == NULL) {
        stack = xmalloc(stack_size);
        thread->dyn_alloc_stack = true;
    } else {
        thread->dyn_alloc_stack = false;
    }
    thread->stack_base = stack;
    thread->stack_size = stack_size;

    /* create a uCOS-III task */
    OSTaskCreate(&thread->ucos_tcb,          /* Address of TCB assigned to the task      */
                 (CPU_CHAR *)name,           /* Name you want to give the task           */
                 fn,                         /* Address of the task itself               */
                 arg,                        /* Argument pointer                         */
                 prio,                       /* Priority you want to assign to the task  */
                 stack,                      /* Base address of task’s stack             */
                 0,                          /* Watermark limit for stack growth         */
                 stack_size/sizeof(CPU_STK), /* Stack size in number of CPU_STK elements */
                 0,                          /* Size of task message queue               */
                 0,                          /* Time quanta (in number of ticks)         */
                 thread,                     /* Extension pointer                        */
                 OS_OPT_TASK_STK_CHK | OS_OPT_TASK_SAVE_FP, /* Options                   */
                 &err);                                     /* Error code                */

    if (err != OS_ERR_NONE) {
        PANIC("Failed to create thread");
    }
}

void os_thread_sleep(uint32_t millisec)
{
    OS_ERR err;
    OSTimeDlyHMSM(0, 0, 0, millisec, OS_OPT_TIME_HMSM_NON_STRICT, &err);
}

/* __malloc_lock() may be called several times before __malloc_unlock()
   uCOS-III mutexes can be taken multiple times by the owner. */
void __malloc_lock(void)
{
    os_mutex_take(&malloc_mutex);
}

void __malloc_unlock(void)
{
    os_mutex_release(&malloc_mutex);
}


/* OS hooks */

void OSTaskReturnHook(OS_TCB *p_tcb)
{
    os_thread_t *t = (os_thread_t *)p_tcb->ExtPtr;

    if (t == NULL) {
        return;
    }

    if (t->dyn_alloc_stack) {
        xfree(t->stack_base);
    }
}

void OSTaskSwHook(void)
{
#if OS_CFG_TASK_PROFILE_EN > 0u
    CPU_TS  ts;
#endif
#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    CPU_TS  int_dis_time;
#endif

#if (defined(TRACE_CFG_EN) && (TRACE_CFG_EN > 0u))
    /* Record the event. */
    TRACE_OS_TASK_SWITCHED_IN(OSTCBHighRdyPtr);
#endif

#if OS_CFG_TASK_PROFILE_EN > 0u
    ts = OS_TS_GET();
    if (OSTCBCurPtr != OSTCBHighRdyPtr) {
        OSTCBCurPtr->CyclesDelta  = ts - OSTCBCurPtr->CyclesStart;
        OSTCBCurPtr->CyclesTotal += (OS_CYCLES)OSTCBCurPtr->CyclesDelta;
    }
    OSTCBHighRdyPtr->CyclesStart = ts;
#endif

#ifdef  CPU_CFG_INT_DIS_MEAS_EN
    /* Keep track of per-task interrupt disable time */
    int_dis_time = CPU_IntDisMeasMaxCurReset();
    if (OSTCBCurPtr->IntDisTimeMax < int_dis_time) {
        OSTCBCurPtr->IntDisTimeMax = int_dis_time;
    }
#endif

#if OS_CFG_SCHED_LOCK_TIME_MEAS_EN > 0u
    /* Keep track of per-task scheduler lock time */
    if (OSTCBCurPtr->SchedLockTimeMax < OSSchedLockTimeMaxCur) {
        OSTCBCurPtr->SchedLockTimeMax = OSSchedLockTimeMaxCur;
    }
    OSSchedLockTimeMaxCur = (CPU_TS)0;
#endif

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

void OSInitHook(void)
{
    OS_CPU_ExceptStkBase = (CPU_STK *)(OSCfg_ISRStkBasePtr + OSCfg_ISRStkSize);
    OS_CPU_ExceptStkBase = (CPU_STK *)((CPU_STK)(OS_CPU_ExceptStkBase) & 0xFFFFFFF8);
}

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
