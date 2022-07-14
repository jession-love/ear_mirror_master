#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/pwm.h"


int32 pwm_init(struct pwm_device *pwm, enum pwm_channel channel, uint32 period_us, uint32 h_duty_us) {

    if(pwm && pwm->init) {
        return pwm->init(pwm, channel, period_us, h_duty_us);
    }

    return RET_ERR;
}

int32 pwm_deinit(struct pwm_device *pwm, enum pwm_channel channel) {

    if(pwm && pwm->deinit) {
        return pwm->deinit(pwm, channel);
    }

    return RET_ERR;
}

int32 pwm_start(struct pwm_device *pwm, enum pwm_channel channel) {

    if(pwm && pwm->start) {
        return pwm->start(pwm, channel);
    }

    return RET_ERR;
}

int32 pwm_stop(struct pwm_device *pwm, enum pwm_channel channel) {

    if(pwm && pwm->stop) {
        return pwm->stop(pwm, channel);
    }

    return RET_ERR;
}

int32 pwm_ioctl(struct pwm_device *pwm, enum pwm_channel channel, enum pwm_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2) {

    if(pwm && pwm->ioctl) {
        return pwm->ioctl(pwm, channel, ioctl_cmd, param1, param2);
    }

    return RET_ERR;
}

int32 pwm_request_irq(struct pwm_device *pwm, enum pwm_channel channel, enum pwm_irq_flag irq_flag, pwm_irq_hdl irq_hdl, uint32 data) {

    if(pwm && pwm->request_irq) {
        return pwm->request_irq(pwm, channel, irq_flag, irq_hdl, data);
    }

    return RET_ERR;
}

int32 pwm_release_irq(struct pwm_device *pwm, enum pwm_channel channel) {

    if(pwm && pwm->release_irq) {
        return pwm->release_irq(pwm, channel);
    }

    return RET_ERR;
}

