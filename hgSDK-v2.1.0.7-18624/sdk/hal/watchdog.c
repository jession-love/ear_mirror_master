#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/watchdog.h"


void watchdog_device_open(struct watchdog_device *watchdog, uint32 flags)
{
    if (watchdog && watchdog->open) 
    {
        watchdog->open(watchdog,flags);
    }
}
void watchdog_device_start(struct watchdog_device *watchdog,uint32 timeout_s)
{
    if (watchdog && watchdog->start) 
    {
        watchdog->start(watchdog,timeout_s);
    }
} 
void watchdog_device_stop(struct watchdog_device *watchdog)
{
    if (watchdog && watchdog->stop) 
    {
        watchdog->stop(watchdog);
    }
}
void watchdog_device_feed(struct watchdog_device *watchdog)
{
    if (watchdog && watchdog->feed) 
    {
        watchdog->feed(watchdog);
    }
}  
int32 watchdog_device_request_irq(struct watchdog_device *watchdog, wdt_irq_hdl irqhdl)
{
    if (watchdog && watchdog->request_irq) {
        return watchdog->request_irq(watchdog,irqhdl);
    }
    return RET_ERR;
}


