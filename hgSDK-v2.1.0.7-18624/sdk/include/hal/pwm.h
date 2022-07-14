#ifndef _HAL_PWM_H_
#define _HAL_PWM_H_

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @brief PWM channel type
  */
enum pwm_channel {

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_0,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_1,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_2,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_3,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_4,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_5,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_6,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_7,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_8,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_9,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_CHANNEL_10,
};

/**
  * @brief PWM irq_flag type
  */
enum pwm_irq_flag {

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_IRQ_FLAG_COMPARE,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_IRQ_FLAG_PERIOD,
};

/**
  * @brief PWM ioctl_cmd type
  */
enum pwm_ioctl_cmd {

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_IOCTL_CMD_SET_PERIOD_DUTY,
    
    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_IOCTL_CMD_SET_SINGLE_INCREAM,

    /*! Compatible version: V0;
     *@ Describe:
     * 
     */
    PWM_IOCTL_CMD_SET_INCREAM_DECREASE,
};


/* User interrupt handle */
typedef void (*pwm_irq_hdl)(uint32 irq, uint32 irq_data);

/* PWM api for user */
struct pwm_device {
    struct dev_obj dev;
    int32(*init)(struct pwm_device *pwm, enum pwm_channel channel, uint32 period_us, uint32 h_duty_us);
    int32(*deinit)(struct pwm_device *pwm, enum pwm_channel channel);
    int32(*start)(struct pwm_device *pwm, enum pwm_channel channel);
    int32(*stop)(struct pwm_device *pwm, enum pwm_channel channel);
    int32(*ioctl)(struct pwm_device *pwm, enum pwm_channel channel, enum pwm_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2);
    int32(*request_irq)(struct pwm_device *pwm, enum pwm_channel channel, enum pwm_irq_flag irq_flag, pwm_irq_hdl irq_hdl, uint32 data);
    int32(*release_irq)(struct pwm_device *pwm, enum pwm_channel channel);
};



/* PWM API functions */



/**
 * @brief pwm_init
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_init(struct pwm_device *pwm, enum pwm_channel channel, uint32 period_us, uint32 h_duty_us);

/**
 * @brief pwm_deinit
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_deinit(struct pwm_device *pwm, enum pwm_channel channel);

/**
 * @brief pwm_start
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_start(struct pwm_device *pwm, enum pwm_channel channel);

/**
 * @brief pwm_stop
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_stop(struct pwm_device *pwm, enum pwm_channel channel);

/**
 * @brief pwm_ioctl
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_ioctl(struct pwm_device *pwm, enum pwm_channel channel, enum pwm_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2);

/**
 * @brief pwm_request_irq
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_request_irq(struct pwm_device *pwm, enum pwm_channel channel, enum pwm_irq_flag irq_flag, pwm_irq_hdl irq_hdl, uint32 data);

/**
 * @brief pwm_release_irq
 *
 * @Compatible version: V0;
 *
 * @note  .
 *
 * @param pwm       The address of pwm device in the RTOS.
 * @param .
 * @param .
 *
 * @return
 *     - RET_OK              Success
 *     - RET_ERR             Fail
 */
int32 pwm_release_irq(struct pwm_device *pwm, enum pwm_channel channel);

#ifdef __cplusplus
}
#endif
#endif
