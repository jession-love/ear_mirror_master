#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/led.h"


int32 led_init(struct led_device *led, enum led_work_mode work_mode, enum led_seg_cnt seg_cnt, enum led_com_cnt com_cnt) {

    if(led && led->init) {
        return led->init(led, work_mode, seg_cnt, com_cnt);
    }

    return RET_ERR;
}

int32 led_open(struct led_device *led, uint32 num, enum led_com_cnt pos) {

    if(led && led->open) {
        return led->open(led, num, pos);
    }

    return RET_ERR;
}

int32 led_close(struct led_device *led) {

    if(led && led->close) {
        return led->close(led);
    }

    return RET_ERR;
}

int32 led_ioctl(struct led_device *led, enum led_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2) {

    if(led && led->ioctl) {
        return led->ioctl(led, ioctl_cmd, param1, param2);
    }

    return RET_ERR;
}

int32 led_request_irq(struct led_device *led, enum led_irq_flag irq_flag, led_irq_hdl irq_hdl, uint32 irq_data) {

    if(led && led->request_irq) {
        return led->request_irq(led, irq_flag, irq_hdl, irq_data);
    }

    return RET_ERR;
}

int32 led_release_irq(struct led_device *led, enum led_irq_flag irq_flag) {

    if(led && led->release_irq) {
        return led->release_irq(led, irq_flag);
    }

    return RET_ERR;
}

