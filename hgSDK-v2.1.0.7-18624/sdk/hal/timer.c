#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/timer_device.h"

int32 timer_device_open(struct timer_device *timer, enum timer_type type, uint32 flags)
{
    if (timer && timer->open) {
        return timer->open(timer, type, flags);
    }
    return RET_ERR;
}

int32 timer_device_close(struct timer_device *timer)
{
    if (timer && timer->close) {
        return timer->close(timer);
    }
    return RET_ERR;
}

int32 timer_device_start(struct timer_device *timer, uint32 tmo_us, timer_cb_hdl cb, uint32 cb_data)
{
    if (timer && timer->start) {
        return timer->start(timer, tmo_us, cb, cb_data);
    }
    return RET_ERR;
}

int32 timer_device_stop(struct timer_device *timer)
{
    if (timer && timer->stop) {
        return timer->stop(timer);
    }
    return RET_ERR;
}

int32 timer_device_ioctl(struct timer_device *timer, uint32 cmd, uint32 param1, uint32 param2)
{
    if(timer && timer->ioctl) {
        return timer->ioctl(timer, cmd, param1, param2);
    }
    return RET_ERR;
}

int32 timer_request_irq(struct timer_device *timer, uint32 irq_flag, timer_cb_hdl cb, uint32 cb_data)
{
    if(timer && timer->request_irq) {
        return timer->request_irq(timer, irq_flag, cb, cb_data);
    }
    return RET_ERR;
}

int32 timer_release_irq(struct timer_device *timer, uint32 irq_flag)
{
    if(timer && timer->release_irq) {
        return timer->release_irq(timer, irq_flag);
    }
    return RET_ERR;
}

