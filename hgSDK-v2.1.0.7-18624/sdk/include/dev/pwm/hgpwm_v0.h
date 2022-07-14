#ifndef _HGPWM_V0_H
#define _HGPWM_V0_H
#include "hal/pwm.h"

#ifdef __cplusplus
extern "C" {
#endif

#define HGPWM_MAX_PWM_CHANNEL 10

enum hgpwm_v0_func_cmd {
    HGPWM_V0_FUNC_CMD_MASK                           = BIT(31),
    HGPWM_V0_FUNC_CMD_INIT                           = HGPWM_V0_FUNC_CMD_MASK | 1,
    HGPWM_V0_FUNC_CMD_DEINIT                         = HGPWM_V0_FUNC_CMD_MASK | 2,
    HGPWM_V0_FUNC_CMD_START                          = HGPWM_V0_FUNC_CMD_MASK | 3,
    HGPWM_V0_FUNC_CMD_STOP                           = HGPWM_V0_FUNC_CMD_MASK | 4,
    /* IOCTL CMD */
    HGPWM_V0_FUNC_CMD_IOCTL_SET_PERIOD_DUTY          = HGPWM_V0_FUNC_CMD_MASK | 5,
    HGPWM_V0_FUNC_CMD_IOCTL_GET_PERIOD_DUTY          = HGPWM_V0_FUNC_CMD_MASK | 6,
    HGPWM_V0_FUNC_CMD_IOCTL_SET_SINGLE_INCREAM       = HGPWM_V0_FUNC_CMD_MASK | 7,
    HGPWM_V0_FUNC_CMD_IOCTL_SET_INCREAM_DECREASE     = HGPWM_V0_FUNC_CMD_MASK | 8,
    /* IRQ_FLAG */
    HGPWM_V0_FUNC_CMD_REQUEST_IRQ_COMPARE            = HGPWM_V0_FUNC_CMD_MASK | 9,
    HGPWM_V0_FUNC_CMD_REQUEST_IRQ_PERIOD             = HGPWM_V0_FUNC_CMD_MASK | 10,

    HGPWM_V0_FUNC_CMD_RELEASE_IRQ                    = HGPWM_V0_FUNC_CMD_MASK | 11,
};

struct hgpwm_v0_config {
    uint32       func_cmd;
    uint32       period;
    uint32       duty;
    pwm_irq_hdl  irq_hdl;
    uint32       irq_data;
};

struct hgpwm_v0 {
    struct pwm_device  dev;
    void              *channel[HGPWM_MAX_PWM_CHANNEL];
    uint32             opened[HGPWM_MAX_PWM_CHANNEL];
};



int32 hgpwm_v0_attach(uint32 dev_id, struct hgpwm_v0 *pwm);


#ifdef __cplusplus
}
#endif
#endif
