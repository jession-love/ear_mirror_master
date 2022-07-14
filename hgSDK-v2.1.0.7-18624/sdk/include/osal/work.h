
#ifndef _OS_TASK_WORK_H_
#define _OS_TASK_WORK_H_

#include "typesdef.h"
#include "errno.h"
#include "list.h"
#include "osal/string.h"
#include "osal/timer.h"
#include "osal/mutex.h"
#include "osal/task.h"
#include "osal/semaphore.h"

#ifdef __cplusplus
extern "C" {
#endif

struct os_work;
typedef int32 (*os_work_func_t)(struct os_work *work);
typedef void (*os_run_func_t)(uint32 param1, uint32 parma2, uint32 param3);

struct os_work {
    struct list_head list;
    struct os_workqueue *wkq;
    uint32 running: 1, alloc : 1, delay: 1, pri: 5, magic: 24;
    uint32 schedule;
    uint32 expired;
    os_work_func_t func;
};

struct os_workqueue {
    char  *name;
    uint16 priority;
    uint16 stack_size;
    uint32 init: 1, wait_sema: 1, rev: 30;
    struct os_timer  timer;
    struct os_task   task;
    struct list_head works;
    struct list_head delay_works;
    struct os_semaphore sema;
    struct os_work *runwk;
};

#define OS_WORK_INIT(work, function, priority) do{\
        (work)->func = (os_work_func_t)(function);\
        (work)->pri  = ((priority)&0x1f);\
    }while (0)

int32 os_workqueue_init(struct os_workqueue *wkq, char *name, uint16 priority, uint16 stack_size);
void os_work_schedule(struct os_workqueue *wkq, struct os_work *work);
void os_work_schedule_delay(struct os_workqueue *wkq, struct os_work *work, uint32 delay_ms);
void os_work_cancle(struct os_work *work, uint8 sync);
int32 os_run_func(os_run_func_t func, uint32 param1, uint32 param2, uint32 param3);
int32 os_run_func_delay(os_run_func_t func, uint32 param1, uint32 param2, uint32 delay_ms);
int32 os_run_work(struct os_work *work);
int32 os_run_work_delay(struct os_work *work, uint32 delay_ms);

#endif



