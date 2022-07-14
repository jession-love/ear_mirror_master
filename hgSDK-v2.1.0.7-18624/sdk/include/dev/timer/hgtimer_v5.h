#ifndef _HGTIMER_V5_H
#define _HGTIMER_V5_H
#include "hal/timer_device.h"


#ifdef __cplusplus
extern "C" {
#endif


/*-----timer lower layer: counter mode-----*/
#define HGTIMER_V5_TYPE_ONCE       BIT(0)
#define HGTIMER_V5_TYPE_PERIODIC   BIT(1)
#define HGTIMER_V5_TYPE_COUNTER    BIT(2)


/*----timer lower layer: PWM function cmd----*/
#define HGTIMER_V5_PWM_FUNC_CMD_INIT                          BIT(31) | 1
#define HGTIMER_V5_PWM_FUNC_CMD_DEINIT                        BIT(31) | 2
#define HGTIMER_V5_PWM_FUNC_CMD_START                         BIT(31) | 3
#define HGTIMER_V5_PWM_FUNC_CMD_STOP                          BIT(31) | 4
/* IOCTL CMD */                                        
#define HGTIMER_V5_PWM_FUNC_CMD_IOCTL_SET_PERIOD_DUTY         BIT(31) | 5
#define HGTIMER_V5_PWM_FUNC_CMD_IOCTL_SET_SINGLE_INCREAM      BIT(31) | 6
#define HGTIMER_V5_PWM_FUNC_CMD_IOCTL_SET_INCREAM_DECREASE    BIT(31) | 7

/* IRQ_FLAG */                                         
#define HGTIMER_V5_PWM_FUNC_CMD_REQUEST_IRQ_COMPARE           BIT(31) | 8
#define HGTIMER_V5_PWM_FUNC_CMD_REQUEST_IRQ_PERIOD            BIT(31) | 9
                                                       
#define HGTIMER_V5_PWM_FUNC_CMD_RELEASE_IRQ                   BIT(31) | 10



/*----timer lower layer: cap function cmd----*/
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_INIT                   BIT(30) | 1
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_DEINIT                 BIT(30) | 2
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_START                  BIT(30) | 3
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_STOP                   BIT(30) | 4
///* IOCTL CMD */                                        
//
///* IRQ_FLAG */                                         
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_REQUEST_IRQ_CAPTURE    BIT(30) | 7
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_REQUEST_IRQ_OVERFLOW   BIT(30) | 8
//                                                       
//#define HGTIMER_V5_CAPTURE_FUNC_CMD_RELEASE_IRQ            BIT(30) | 9


struct hgtimer_v5 {
    struct timer_device   dev;
    uint32                hw;
    timer_cb_hdl          _counter_irq_hdl;
    pwm_irq_hdl           _pwm_irq_hdl;
    //capture_irq_hdl       _capture_irq_hdl;
    uint32                irq_data;
    uint32                irq_num;
    uint32                opened;
    uint32                type;
    uint32                counter_once_en  :1,
                          counter_period_en:1,
                          counter_en       :1,
                          pwm_en           :1,
                          cap_en           :1,
                          ir_en            :1;
};



int32 hgtimer_v5_attach(uint32 dev_id, struct hgtimer_v5 *timer);


#ifdef __cplusplus
}
#endif
#endif

