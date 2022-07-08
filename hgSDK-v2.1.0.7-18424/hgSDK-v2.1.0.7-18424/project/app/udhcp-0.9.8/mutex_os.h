#ifndef MUTEX_OS_H
#define MUTEX_OS_H
#include "sys_arch.h"
struct mutex {
  sys_sem_t mutex;
};

static inline void mutex_init(struct mutex *lock)
{
  if(NULL == lock) return ;

  //xSemaphoreCreateMutexStatic( &lock->mutex );
  lock->mutex = csi_kernel_sem_new(1,1);
  

}

static inline void mutex_lock(struct mutex *lock)
{
  //xSemaphoreTake( &lock->mutex, -1 ); // todo: interrupt
  csi_kernel_sem_wait(lock->mutex,portMAX_DELAY);
}

static inline void mutex_unlock(struct mutex *lock)
{
  //xSemaphoreGive( &lock->mutex );
	csi_kernel_sem_post(lock->mutex);
}

typedef void* wait_event_t;
typedef void* SemaphoreHandle_t;

#endif

