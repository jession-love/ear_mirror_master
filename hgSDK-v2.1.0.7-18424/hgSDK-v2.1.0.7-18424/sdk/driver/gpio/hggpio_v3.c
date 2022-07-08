// * @file    hggpio_v3.c
// * @author  
// * @brief   This file contains all the hggpio_v3 driver functions.
// Revision History
// V1.0.0  23/02/2022  first release

#include "typesdef.h"
#include "errno.h"
#include "list.h"
#include "dev.h"
#include "osal/irq.h"
#include "hal/gpio.h"
#include "dev/gpio/hggpio_v3.h"
#include "hggpio_v3_hw.h"

/* mode define */
#define _GPIO_PULL_NONE  0x1
#define _GPIO_PULL_UP    0x2
#define _GPIO_PULL_DOWN  0x4

#define _GPIO_OPENDRAIN_PULL_MASK 0x80
#define _GPIO_OPENDRAIN_PULL_NONE (_GPIO_OPENDRAIN_PULL_MASK | _GPIO_PULL_NONE)
#define _GPIO_OPENDRAIN_PULL_UP   (_GPIO_OPENDRAIN_PULL_MASK | _GPIO_PULL_UP  )
#define _GPIO_OPENDRAIN_PULL_DOWN (_GPIO_OPENDRAIN_PULL_MASK | _GPIO_PULL_DOWN)

#define _GPIO_OPENDRAIN_DROP_MASK 0x40
#define _GPIO_OPENDRAIN_DROP_NONE (_GPIO_OPENDRAIN_DROP_MASK | _GPIO_PULL_NONE)
#define _GPIO_OPENDRAIN_DROP_DOWN (_GPIO_OPENDRAIN_DROP_MASK | _GPIO_PULL_DOWN)


#define GPIO_PULL_LEVEL_MASK  0xF


static inline uint32 hggpio_v3_pin_num(struct hggpio_v3 *gpio, uint32 pin)
{
    return pin - gpio->pin_num[0];
}

static int32 hggpio_v3_set_pull(struct hggpio_v3_hw *reg, uint32 pin, enum gpio_pin_mode pull_mode, enum gpio_pull_level level)
{
    int32  ret               = RET_OK;
    uint32 l_pin_pos         = (pin * 4);
    uint32 h_pin_pos         = (pin & 0x07) * 4;
    uint32 pull_level_to_reg = 0;

    if (pin >= HGGPIO_V3_MAX_PINS) {
        return -EINVAL;
    }

    switch (level) {
        case (GPIO_PULL_LEVEL_NONE):
            pull_level_to_reg = 0;
            break;
        case (GPIO_PULL_LEVEL_4_7K):
            pull_level_to_reg = 1;
            break;
        case (GPIO_PULL_LEVEL_10K ):
            pull_level_to_reg = 2;
            break;
        case (GPIO_PULL_LEVEL_100K):
            pull_level_to_reg = 4;
            break;
        
        default:
            return -EINVAL;
    }


    sysctrl_unlock();

    if (pin < 8) {
        if (_GPIO_PULL_NONE == (pull_mode & 0xF)) {
                reg->PUDL &= ~(GPIO_PULL_LEVEL_MASK << l_pin_pos);
                reg->PUPL &= ~(GPIO_PULL_LEVEL_MASK << l_pin_pos);
        }else if (_GPIO_PULL_DOWN & pull_mode) {
                reg->PUDL  =  (reg->PUDL & ~ (GPIO_PULL_LEVEL_MASK << l_pin_pos)) | (pull_level_to_reg << l_pin_pos);
                reg->PUPL &= ~(GPIO_PULL_LEVEL_MASK << l_pin_pos);
        } else if (_GPIO_PULL_UP & pull_mode) {
                reg->PUDL &= ~(GPIO_PULL_LEVEL_MASK << l_pin_pos);
                reg->PUPL  =  (reg->PUPL & ~ (GPIO_PULL_LEVEL_MASK << l_pin_pos)) | (pull_level_to_reg << l_pin_pos);
        }
    } else {
        if (_GPIO_PULL_NONE == (pull_mode & 0xF)) {
                reg->PUDH &= ~(GPIO_PULL_LEVEL_MASK << h_pin_pos);
                reg->PUPH &= ~(GPIO_PULL_LEVEL_MASK << h_pin_pos);
        }else if (_GPIO_PULL_DOWN & pull_mode) {
                reg->PUDH  =  (reg->PUDH & ~ (GPIO_PULL_LEVEL_MASK << h_pin_pos)) | (pull_level_to_reg << h_pin_pos);
                reg->PUPH &= ~(GPIO_PULL_LEVEL_MASK << h_pin_pos);
        } else if (_GPIO_PULL_UP & pull_mode) {
                reg->PUDH &= ~(GPIO_PULL_LEVEL_MASK << h_pin_pos);
                reg->PUPH  =  (reg->PUPH & ~ (GPIO_PULL_LEVEL_MASK << h_pin_pos)) | (pull_level_to_reg << h_pin_pos);
        }
    }

    sysctrl_lock();
    
    return ret;
}

static int32 hggpio_v3_set_driver_strength(struct hggpio_v3_hw *reg, uint32 pin, enum pin_driver_strength drive)
{
    int32  ret = RET_OK;
    uint32 pin_pos = ((pin & 0x7) * 4);
    uint32 drive_to_reg = 0;

    if (pin >= HGGPIO_V3_MAX_PINS) {
        return -EINVAL;
    }

    switch (drive) {
        case (GPIO_DS_4MA) :
            drive_to_reg = 0;
            break;
        case (GPIO_DS_8MA) :
            drive_to_reg = 1;
            break;
        case (GPIO_DS_12MA):
            drive_to_reg = 2;
            break;
        case (GPIO_DS_16MA):
            drive_to_reg = 3;
            break;
        case (GPIO_DS_20MA):
            drive_to_reg = 4;
            break;
        case (GPIO_DS_24MA):
            drive_to_reg = 5;
            break;
        case (GPIO_DS_28MA):
            drive_to_reg = 6;
            break;
        case (GPIO_DS_32MA):
            drive_to_reg = 7;
            break;
        case (GPIO_DS_36MA):
            drive_to_reg = 8;
            break;
        case (GPIO_DS_40MA):
            drive_to_reg = 9;
            break;
        case (GPIO_DS_44MA):
            drive_to_reg = 10;
            break;
        case (GPIO_DS_48MA):
            drive_to_reg = 11;
            break;
        case (GPIO_DS_52MA):
            drive_to_reg = 12;
            break;
        case (GPIO_DS_56MA):
            drive_to_reg = 13;
            break;
        case (GPIO_DS_60MA):
            drive_to_reg = 14;
            break;
        case (GPIO_DS_64MA):
            drive_to_reg = 15;
            break;

        default: 
            return -EINVAL;
    }

    sysctrl_unlock();
    switch(pin >> 3) {
        case 0:
            reg->OSPEEDL = (reg->OSPEEDL & ~(0xF << pin_pos)) | (drive_to_reg << pin_pos);
            break;
        case 1:
            reg->OSPEEDH = (reg->OSPEEDH & ~(0xF << pin_pos)) | (drive_to_reg << pin_pos);
            break;
        default:
            ret = -EINVAL;
        break;
    }
    sysctrl_lock();
    
    return ret;
}



static int32 hggpio_v3_set_mode(struct hggpio_v3_hw *reg, uint32 pin, enum gpio_pin_mode mode, enum gpio_pull_level level)
{

    uint32 pin_bit  = 1UL << pin;
    uint32 mode_tmp = 0;

    if (pin >= HGGPIO_V3_MAX_PINS) {
        return -EINVAL;
    }
    
    switch (mode) {
        
        case GPIO_PULL_NONE:
            mode_tmp = _GPIO_PULL_NONE;
            SYSCTRL_REG_OPT(reg->OTYPE  &= ~pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 &= ~pin_bit);
            break;
            
        case GPIO_PULL_UP  :
            mode_tmp = _GPIO_PULL_UP;
            SYSCTRL_REG_OPT(reg->OTYPE  &= ~pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 &= ~pin_bit);
            break;
            
        case GPIO_PULL_DOWN: 
            mode_tmp = _GPIO_PULL_DOWN;
            SYSCTRL_REG_OPT(reg->OTYPE  &= ~pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 &= ~pin_bit);
            break;
            
        case GPIO_OPENDRAIN_PULL_NONE:
            mode_tmp = _GPIO_OPENDRAIN_PULL_NONE;
            SYSCTRL_REG_OPT(reg->OTYPE  |=  pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 &= ~pin_bit);
            break;

        case GPIO_OPENDRAIN_PULL_UP:
            mode_tmp = _GPIO_OPENDRAIN_PULL_UP;
            SYSCTRL_REG_OPT(reg->OTYPE  |=  pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 &= ~pin_bit);
            break;
            
        case GPIO_OPENDRAIN_DROP_NONE:
            mode_tmp = _GPIO_OPENDRAIN_DROP_NONE;
            SYSCTRL_REG_OPT(reg->OTYPE  &= ~pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 |=  pin_bit);
            break;

        case GPIO_OPENDRAIN_DROP_DOWN:
            mode_tmp = _GPIO_OPENDRAIN_DROP_DOWN;
            SYSCTRL_REG_OPT(reg->OTYPE  &= ~pin_bit);
            SYSCTRL_REG_OPT(reg->OTYPE1 |=  pin_bit);
            break;
            
        default:
            return -EINVAL;
    }

    return hggpio_v3_set_pull(reg, pin, mode_tmp, level);
}


///////////////////////////////attch function///////////////////////
static int32 hggpio_v3_iomap_output(struct gpio_device *gpio, uint32 pin, enum gpio_iomap_out_func iomap_out_func_sel) {

    int32 ret_val = RET_OK;
    struct hggpio_v3 *dev        = (struct hggpio_v3*)gpio;
    struct hggpio_v3_hw *hw      = (struct hggpio_v3_hw *)dev->hw;
    uint32 iomap_out_pin_pos     = 0;
    uint32 mode_pin_pos          = 0;
    volatile uint32 *operate_ptr = &(hw->IOFUNCOUTCON0);

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    iomap_out_pin_pos = ((pin & 0x3) << 3);
    mode_pin_pos      = pin *  2;
    operate_ptr      += pin >> 2;

    sysctrl_unlock();

    if (HG_GPIOC_BASE == dev->hw) {
        if ((6 == pin) || (7 == pin)) {
            //PC6->DP; PC7->DM change to GPIO
            SYSCTRL->SYS_CON1 &= ~ BIT(18); //USB20_PHY reset
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->USB20_PHY_CFG3 |= (0xf << 0);
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->SYS_CON1 |= BIT(18);
        }
    }

    /* clear simulation enablement for corresponding pins */
    hw->AIOEN &= ~(1 << pin);

    //Config IOFUNCOUTCONx Reg
    *operate_ptr = (*operate_ptr & ~(0xFF << iomap_out_pin_pos)) | ((iomap_out_func_sel & 0x000000FF) << iomap_out_pin_pos);

    //Config MODE
    hw->MODE = (hw->MODE & ~(0x3 << mode_pin_pos)) | (0x01 << mode_pin_pos);
    
    sysctrl_lock();
    return ret_val;
}

static int32 hggpio_v3_iomap_input(struct gpio_device *gpio, uint32 pin, enum gpio_iomap_in_func iomap_in_func_sel) {

    struct hggpio_v3 *dev     = (struct hggpio_v3*)gpio;
    struct hggpio_v3_hw *hw   = (struct hggpio_v3_hw *)dev->hw;
    int32 ret_val             = RET_OK;
    uint32 pin_num_temp       = 0;
    uint32 pin_pos            = (iomap_in_func_sel & 0x3) << 3;
    volatile uint32 *operate_ptr = &(SYSCTRL->IOFUNCINCON0);

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    pin_num_temp = pin;

    sysctrl_unlock();

    if (HG_GPIOC_BASE == dev->hw) {
        if ((6 == pin) || (7 == pin)) {
            //PC6->DP; PC7->DM change to GPIO
            SYSCTRL->SYS_CON1 &= ~ BIT(18); //USB20_PHY reset
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->USB20_PHY_CFG3 |= (0xf << 0);
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->SYS_CON1 |= BIT(18);
        }
    }

    if (hw) {
        hw->AIOEN &= ~(1 << pin);
    }

    if (0xF000 != (0xF000 & iomap_in_func_sel)) {
        
        //Config IOMAP_INPUT
        if (HG_GPIOA_BASE == (uint32)dev->hw) {
            pin_num_temp = pin_num_temp + 0x00;
        } else if (HG_GPIOB_BASE == (uint32)dev->hw) {
            pin_num_temp = pin_num_temp + 0x10;
        } else if (HG_GPIOC_BASE == (uint32)dev->hw) {
            pin_num_temp = pin_num_temp + 0x20;
        } else if ((!dev->hw) && (!pin)) {
            pin_num_temp = 0x28;
        } else if ((!dev->hw) && ( pin)) {
            pin_num_temp = 0x29;
        }
        
        operate_ptr  = operate_ptr + (iomap_in_func_sel >> 2);
        *operate_ptr = (*operate_ptr & ~(0xFF << pin_pos)) | pin_num_temp << pin_pos;
    }

    //Config input mode
    if (hw) {
        hw->MODE = hw->MODE & ~(0x03 << (pin * 2));
    }

    sysctrl_lock();

    return ret_val;

}

static int32 hggpio_v3_iomap_inout(struct gpio_device *gpio, uint32 pin, enum gpio_iomap_in_func iomap_in_func_sel, enum gpio_iomap_out_func iomap_out_func_sel) {

    if (RET_OK != hggpio_v3_iomap_input(gpio,  pin,  iomap_in_func_sel)) {
        return RET_ERR;
    }
    if (RET_OK != hggpio_v3_iomap_output(gpio, pin,  iomap_out_func_sel)) {
        return RET_ERR;
    }

    return RET_OK;
}

static int32 hggpio_v3_dir(struct gpio_device *gpio,uint32 pin, enum gpio_pin_direction direction)
{
    if (GPIO_DIR_INPUT != direction && GPIO_DIR_OUTPUT != direction) {
        return -EINVAL;
    }

    if (GPIO_DIR_INPUT == direction) {
        if (RET_OK != hggpio_v3_iomap_input(gpio,  pin, GPIO_IOMAP_INPUT)) {
            return RET_ERR;
        }
    } else {
        if (RET_OK != hggpio_v3_iomap_output(gpio, pin, GPIO_IOMAP_OUTPUT)) {
            return RET_ERR;
        }
    }

    return RET_OK;
}

#if HGGPIO_V3_DIR_ATOMIC_EN
static int32 hggpio_v3_dir_atomic(struct gpio_device *gpio,uint32 pin, enum gpio_pin_direction direction)
{
#if defined(__CORTEX_M) && ((__CORTEX_M == 0x03) || (__CORTEX_M == 0x04))
    if (RET_OK != hggpio_v3_dir(gpio, pin, direction)) {
        return RET_ERR;
    }
#else
    uint32 flags = disable_irq();
    if (RET_OK != hggpio_v3_dir(gpio, pin, direction)) {
        return RET_ERR;
    }
    enable_irq(flags);
#endif
    return RET_OK;
}
#endif

static int32 hggpio_v3_mode(struct gpio_device *gpio, uint32 pin, enum gpio_pin_mode mode, enum gpio_pull_level level)
{
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);

    sysctrl_unlock();

    if (HG_GPIOC_BASE == dev->hw) {
        if ((6 == pin) || (7 == pin)) {
            //PC6->DP; PC7->DM change to GPIO
            SYSCTRL->SYS_CON1 &= ~ BIT(18); //USB20_PHY reset
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->USB20_PHY_CFG3 |= (0xf << 0);
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->SYS_CON1 |= BIT(18);
        }
    }

    sysctrl_lock();

    
    
    return hggpio_v3_set_mode(hw, pin, mode, level);
}

#if HGGPIO_V3_DRIVER_STRENGTH_EN
static int32 hggpio_v3_driver_strength(struct gpio_device *gpio, uint32 pin, enum pin_driver_strength strength)
{
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    return hggpio_v3_set_driver_strength(hw, pin, strength);
}
#endif

static int32 hggpio_v3_get(struct gpio_device *gpio, int32 pin)
{
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);

    sysctrl_unlock();

    if (HG_GPIOC_BASE == dev->hw) {
        if ((6 == pin) || (7 == pin)) {
            //PC6->DP; PC7->DM change to GPIO
            SYSCTRL->SYS_CON1 &= ~ BIT(18); //USB20_PHY reset
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->USB20_PHY_CFG3 |= (0xf << 0);
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->SYS_CON1 |= BIT(18);
        }
    }

    sysctrl_lock();

    return (hw->IDAT & (1UL << pin)) ? 1 : 0;
}

static int32 hggpio_v3_set(struct gpio_device *gpio, uint32 pin, int32 val)
{
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);

    sysctrl_unlock();

    if (HG_GPIOC_BASE == dev->hw) {
        if ((6 == pin) || (7 == pin)) {
            //PC6->DP; PC7->DM change to GPIO
            SYSCTRL->SYS_CON1 &= ~ BIT(18); //USB20_PHY reset
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->USB20_PHY_CFG3 |= (0xf << 0);
            __NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
            SYSCTRL->SYS_CON1 |= BIT(18);
        }
    }

    sysctrl_lock();

    if (val) {
        hw->ODAT |=  (1UL << pin);
    } else {
        hw->ODAT &= ~(1UL << pin);
    }

    return RET_OK;
}

#if HGGPIO_V3_LOCK_EN
static int32 hggpio_v3_lock(struct gpio_device *gpio, uint32 pin)
{
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;
    uint32 tmp = 0x00010000;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);

    tmp |= (1UL << pin);
    /* Set LCKK bit */
    hw->LCK = tmp;
    /* Reset LCKK bit */
    hw->LCK = (1UL << pin);
    /* Set LCKK bit */
    hw->LCK = tmp;
    /* Read LCKK bit*/
    tmp = hw->LCK;
    /* Read LCKK bit*/
    tmp = hw->LCK;
    if (0 == (tmp & 0x00010000)) {
        return RET_ERR;
    }

    return RET_OK;
}
#endif

#if HGGPIO_V3_DEBUNCE_EN
static int32 hggpio_v3_debunce(struct gpio_device *gpio, uint32 pin, int32 enable)
{
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);
    if (enable) {
        hw->DEBEN |= (1UL << pin);
    } else {
        hw->DEBEN &= ~(1UL << pin);
    }

    return RET_OK;
}
#endif

#if HGGPIO_V3_TOGGLE_EN
static int32 hggpio_v3_toggle(struct gpio_device *gpio, uint32 pin)
{
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);

    hw->TGL = (1UL << pin);

    return RET_OK;
}
#endif

#if HGGPIO_V3_SET_ATOMIC_EN
static int32 hggpio_v3_set_atomic(struct gpio_device *gpio, uint32 pin, int32 value)
{
#if defined(__CORTEX_M) && ((__CORTEX_M == 0x03) || (__CORTEX_M == 0x04))
    struct hggpio_v3 *dev   = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);
    REG_BIT_BAND((uint32)&hw->ODAT, pin) = value;
#else
    uint32 flags = disable_irq();
    hggpio_v3_set(gpio, pin, value);
    enable_irq(flags);
#endif
    return RET_OK;
}
#endif

/**********************************************************************************/
/*******************     GPIO EXTERNAL FUNCTIONAL     *****************************/
/**********************************************************************************/
#if HGGPIO_V3_ANALOG_EN
static int32 hggpio_v3_analog(struct gpio_device *gpio, uint32 pin, int32 enable)
{
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    
    sysctrl_unlock();
    if (enable) {
        hw->MODE = (hw->MODE &~ (0x3 << pin*2)) | (0x3 << pin*2);;
    } else {
        hw->MODE = (hw->MODE & ~(3UL << (pin*2)));
    }
    hw->AIOEN |= (1UL << (pin));
    sysctrl_lock();
    
    return RET_OK;
}
#endif

#if HGGPIO_V3_INPUT_LAG_EN
static int32 hggpio_v3_input_lag(struct gpio_device *gpio, uint32 pin, int32 enable)
{
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);

    sysctrl_unlock();
    if (enable) {
        hw->HY |=  (1UL << pin);
    } else {
        hw->HY &= ~(1UL << pin);
    }
    sysctrl_lock();
    
    return RET_OK;
}
#endif

static int32 hggpio_v3_afio(struct gpio_device *gpio, uint32 pin, enum gpio_afio_set afio)
{
    uint32 pin_loc;
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);
    
    pin_loc = (pin & 0x7) * 4;
    
    sysctrl_unlock();
    
    hw->AIOEN &= ~(1UL << (pin));

    /* set alternate function mode */
    hw->MODE = (hw->MODE & ~(3UL << (pin*2))) | (2UL << (pin*2));

    if (pin < 8) {
        hw->AFRL = (hw->AFRL & ~(0xFUL << (pin_loc))) | (afio << (pin_loc));
    } else {
        hw->AFRH = (hw->AFRH & ~(0xFUL << (pin_loc))) | (afio << (pin_loc));
    }

    sysctrl_lock();
    
    return RET_OK;
}

static int32 hggpio_v3_int(struct gpio_device *gpio, uint32 pin, enum gpio_irq_event evt)
{
    uint32 pin_bit = 0;
    int32  ret_val = RET_OK;
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin > HGGPIO_V3_MAX_PINS) {
        return -EINVAL;
    }

    pin_bit = (1 << pin);

    sysctrl_unlock();

    switch(evt) {
        case (GPIO_IRQ_EVENT_NONE): {
            hw->IMK  &= ~pin_bit;
            hw->TRG0 &= ~pin_bit;
            hw->TRG1 &= ~pin_bit;
        };break;
        case (GPIO_IRQ_EVENT_ALL): {
            hw->IMK  |=  pin_bit;
            hw->TRG0 &= ~pin_bit;
            hw->TRG1 &= ~pin_bit;
        };break;
        case (GPIO_IRQ_EVENT_RISE): {
            hw->IMK  |=  pin_bit;
            hw->TRG0 &= ~pin_bit;
            hw->TRG1 |=  pin_bit;
        };break;
        case (GPIO_IRQ_EVENT_FALL): {
            hw->IMK  |=  pin_bit;
            hw->TRG0 |=  pin_bit;
            hw->TRG1 &= ~pin_bit;
        };break;
        default: {
            ret_val = -ENOTSUPP;
            break;
        }
    }

    sysctrl_lock();

    return RET_OK;
}

#if HGGPIO_V3_ADC_ANALOG_INPUT_EN
static int32 hggpio_v3_adc_analog_input(struct gpio_device *gpio, uint32 pin, int32 value) {

    uint32 pin_bit = 0;
    struct hggpio_v3 *dev = (struct hggpio_v3*)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    pin_bit = (1 << pin);

    sysctrl_unlock();

    if (value) {
        hw->MODE   = (hw->MODE &~ (0x3 << pin*2)) | (0x3 << pin*2);
        hw->AIOEN &= ~pin_bit;
        hw->ADC_AIOEN |= pin_bit;
    } else {
        hw->ADC_AIOEN &= ~pin_bit;
        hw->MODE = (hw->MODE &~ (0x3 << pin*2));
    }

    sysctrl_lock();

    return RET_OK;

}
#endif

#if HGGPIO_V3_TK_ANALOG_INPUT_EN
static int32 hggpio_v3_tk_analog_input(struct gpio_device *gpio, uint32 pin, int32 value) {

    uint32 pin_bit = 0;
    struct hggpio_v3 *dev = (struct hggpio_v3*)gpio;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    pin_bit = (1 << pin);

    sysctrl_unlock();

    if (value) {
        hw->MODE   = (hw->MODE &~ (0x3 << pin*2)) | (0x3 << pin*2);
        //hw->AIOEN &= ~pin_bit;
        //hw->TK_AIOEN |= pin_bit;
    } else {
        //hw->TK_AIOEN &= ~pin_bit;
        hw->MODE = (hw->MODE &~ (0x3 << pin*2));
    }

    sysctrl_lock();

    return RET_OK;
}
#endif

static int32 hggpio_v3_ioctl(struct gpio_device *gpio, uint32 pin, int32 cmd, int32 param1, int32 param2)
{
    int32 ret_val = RET_OK;
    
    switch (cmd) {
        #if HGGPIO_V3_INPUT_LAG_EN
            case GPIO_INPUT_DELAY_ON_OFF:
                ret_val = hggpio_v3_input_lag(gpio, pin, param1);
                break;
        #endif
        
        #if HGGPIO_V3_LOCK_EN
            case GPIO_LOCK:
                ret_val = hggpio_v3_lock(gpio, pin);
                break;
        #endif
        
        #if HGGPIO_V3_DEBUNCE_EN
            case GPIO_DEBUNCE:
                ret_val = hggpio_v3_debunce(gpio, pin, param1);
                break;
        #endif
        
        #if HGGPIO_V3_TOGGLE_EN
            case GPIO_OUTPUT_TOGGLE:
                ret_val = hggpio_v3_toggle(gpio, pin);
                break;
        #endif

        #if HGGPIO_V3_DIR_ATOMIC_EN
            case GPIO_DIR_ATOMIC:
                ret_val = hggpio_v3_dir_atomic(gpio, pin, param1);
                break;
        #endif

        #if HGGPIO_V3_SET_ATOMIC_EN
            case GPIO_VALUE_ATOMIC:
                ret_val = hggpio_v3_set_atomic(gpio, pin, param1);
                break;
        #endif

        #if HGGPIO_V3_ADC_ANALOG_INPUT_EN
            case GPIO_CMD_ADC_ANALOG:
                ret_val = hggpio_v3_adc_analog_input(gpio, pin, param1);
                break;
        #endif

        #if HGGPIO_V3_TK_ANALOG_INPUT_EN
            case GPIO_CMD_TK_ANALOG:
                ret_val = hggpio_v3_tk_analog_input(gpio, pin, param1);
                break;
        #endif

            case GPIO_CMD_DRIVER_STRENGTH:
                ret_val = hggpio_v3_driver_strength(gpio, pin, param1);
                break;

            case GPIO_CMD_AFIO_SET:
                ret_val = hggpio_v3_afio(gpio, pin, param1);
                break;

            case GPIO_CMD_IOMAP_OUT_FUNC:
                ret_val = hggpio_v3_iomap_output(gpio, pin, param1);
                break;

            case GPIO_CMD_IOMAP_IN_FUNC:
                ret_val = hggpio_v3_iomap_input(gpio, pin, param1);
                break;

            case GPIO_CMD_IOMAP_INOUT_FUNC:
                ret_val = hggpio_v3_iomap_inout(gpio, pin, param1, param2);
                break;

            case GPIO_GENERAL_ANALOG:
                ret_val = hggpio_v3_analog(gpio, pin, param1);
                break;
        default:
            ret_val = -ENOTSUPP;
            break;
    }
    return ret_val;
}

/**********************************************************************************/
/*******************     GPIO    IRQ   FUNCTIONAL     *****************************/
/**********************************************************************************/
static void hggpio_v3_irq_handler(void *data)
{
    int32 i = 0;
    struct hggpio_v3 *dev = (struct hggpio_v3 *)data;
    struct hggpio_v3_hw *hw = (struct hggpio_v3_hw *)dev->hw;
    enum gpio_irq_event evt;
    uint32 pending  = hw->PND;
    uint32 trg0     = hw->TRG0;
    uint32 trg1     = hw->TRG1;
    hw->PNDCLR = 0xFF;

    for (i = 0; i < HGGPIO_V3_MAX_PINS; i++) {
        if (pending & BIT(i)) {
            if ((trg0 & BIT(i)) && (BIT(i) != (trg1 & BIT(i)))) {
                evt = GPIO_IRQ_EVENT_FALL;
            } else if ((trg1 & BIT(i)) && (BIT(i) != (trg0 & BIT(i)))) {
                evt = GPIO_IRQ_EVENT_RISE;
            } else {
                evt = GPIO_IRQ_EVENT_ALL;
            }
            if (dev->irq_hdl[i]) {
                dev->irq_hdl[i](dev->pin_id[i], evt);
            }
        }
    }
}

static int32 hggpio_v3_request_pin_irq(struct gpio_device *gpio, uint32 pin, gpio_irq_hdl handler, uint32 data, enum gpio_irq_event evt)
{
    int32 ret = RET_OK;
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    dev->irq_hdl[pin] = handler;
    dev->pin_id[pin]  = data;
    ret = request_irq(dev->irq_num, hggpio_v3_irq_handler, (void *)dev);
    ASSERT(ret == RET_OK);
    hggpio_v3_int(gpio, pin, evt);
    irq_enable(dev->irq_num);
    return RET_OK;
}

static int32 hggpio_v3_release_pin_irq(struct gpio_device *gpio, uint32 pin,enum gpio_irq_event evt)
{
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }
    pin = hggpio_v3_pin_num(dev, pin);
    dev->irq_hdl[pin] = NULL;
    dev->pin_id[pin]  = NULL;
    return hggpio_v3_int(gpio, pin, 0);
}

static int32 hggpio_v3_enable_irq(struct gpio_device *gpio, uint32 pin, uint32 enable)
{
    struct hggpio_v3 *dev = (struct hggpio_v3 *)gpio;

    if (pin < dev->pin_num[0] || pin > dev->pin_num[1]) {
        return -EINVAL;
    }

    pin = hggpio_v3_pin_num(dev, pin);
    if (enable) {
        irq_enable(dev->irq_num);
    } else {
        irq_disable(dev->irq_num);
    }
    return RET_OK;
}

int32 hggpio_v3_attach(uint32 dev_id, struct hggpio_v3 *gpio)
{
    int32 i = 0;
    
    gpio->dev.mode            = hggpio_v3_mode;
    gpio->dev.dir             = hggpio_v3_dir;
    gpio->dev.set             = hggpio_v3_set;
    gpio->dev.get             = hggpio_v3_get;
    gpio->dev.ioctl           = hggpio_v3_ioctl;
    gpio->dev.request_pin_irq = hggpio_v3_request_pin_irq;
    gpio->dev.release_pin_irq = hggpio_v3_release_pin_irq;

    for (i = 0; i < HGGPIO_V3_MAX_PINS; i++) {
        gpio->irq_hdl[i] = NULL;
    }
    irq_disable(gpio->irq_num);
    dev_register(dev_id, (struct dev_obj *)gpio);
    
    return RET_OK;
}

