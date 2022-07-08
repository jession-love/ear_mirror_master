/**
  ******************************************************************************
  * @file       sdk\include\chip\txw80x\io_function.h
  * @author     HUGE-IC Application Team
  * @version    V1.0.0
  * @date       2022-01-11
  * @brief      This file contains all the GPIO functions.
  * @copyright  Copyright (c) 2016-2022 HUGE-IC
  ******************************************************************************
  * @attention
  * Only used for txw80x.
  * 
  *
  *
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __IO_FUNCTION_H
#define __IO_FUNCTION_H

#ifdef __cplusplus
extern "C" {
#endif

#include "typesdef.h"

/** @addtogroup Docxygenid_GPIO_enum
  * @{
  */

/**
  * @brief Enumeration constant for GPIO command.
  * @note
  *       Enum number start from 0x101.
  */
enum gpio_cmd {
    /*! GPIO cmd afio set
     */
    GPIO_CMD_AFIO_SET = 0x101,

    /*! GPIO cmd iomap output
     */
    GPIO_CMD_IOMAP_OUT_FUNC,

    /*! GPIO cmd iomap input
     */
    GPIO_CMD_IOMAP_IN_FUNC,

    /*! GPIO cmd iomap inout
     */
    GPIO_CMD_IOMAP_INOUT_FUNC,

    /*! GPIO pin driver strength config
     */
    GPIO_CMD_DRIVER_STRENGTH,

    /*! GPIO adc analog config
     */
    GPIO_CMD_ADC_ANALOG,
    
    /*! GPIO tk analog config
     */
    GPIO_CMD_TK_ANALOG,
};


/**
  * @brief Enumeration constant for GPIO afio set.
  */
enum gpio_afio_set{
    /*! gpio cmd afio 0
     */
    GPIO_AF_0  = 0,
    
    /*! gpio cmd afio 1
     */
    GPIO_AF_1 ,

    /*! gpio cmd afio 2
     */
    GPIO_AF_2 ,

    /*! gpio cmd afio 3
     */
    GPIO_AF_3 ,
};

/**
  * @breif : Enumeration constant for GPIO pull level.
  */
enum gpio_pull_level {
    /*! gpio pull level : NONE
     */
    GPIO_PULL_LEVEL_NONE = 0,
    
    /*! gpio pull level : 4.7K
     */
    GPIO_PULL_LEVEL_4_7K ,
    
    /*! gpio pull level : 10K
     */
    GPIO_PULL_LEVEL_10K  ,
    
    /*! gpio pull level : 100k
     */
    GPIO_PULL_LEVEL_100K ,
};

/**
  * @breif : Enumeration constant for GPIO driver strength
  */
enum pin_driver_strength {
    /*! Compatible version: V4
     */
    GPIO_DS_4MA ,
    
    /*! Compatible version: V4
     */
    GPIO_DS_8MA ,
    
    /*! Compatible version: V4
     */
    GPIO_DS_12MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_16MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_20MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_24MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_28MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_32MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_36MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_40MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_44MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_48MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_52MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_56MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_60MA,
    
    /*! Compatible version: V4
     */
    GPIO_DS_64MA,
} ;

/** @defgroup Enumeration constant for GPIO IO map output function selection
  * @{
  */
enum gpio_iomap_out_func{
    GPIO_IOMAP_OUTPUT                    = 0x00, 
    GPIO_IOMAP_OUT_COMP_DOUT_DIG0        = 0x01, 
    GPIO_IOMAP_OUT_COMP_DOUT_DIG1        = 0x02, 
    GPIO_IOMAP_OUT_GRANT_BLE_SWITCH_O    = 0x03, 
    GPIO_IOMAP_OUT_GRANT_BLE             = 0x04, 
    GPIO_IOMAP_OUT_GRANT_WIFI_SWITCH_O   = 0x05, 
    GPIO_IOMAP_OUT_RF_SWITCH_EN1         = 0x06, 
    GPIO_IOMAP_OUT_RF_SWITCH_EN0         = 0x07, 
    GPIO_IOMAP_OUT_ANTENNA_SEL           = 0x08, 
    GPIO_IOMAP_OUT_PA_EN                 = 0x09, 
    GPIO_IOMAP_OUT_RF_EXT_LNA_EN         = 0x0A, 
    GPIO_IOMAP_OUT_RF_TX_EN_FEM          = 0x0B, 
    GPIO_IOMAP_OUT_RF_RX_EN_FEM          = 0x0C, 
    GPIO_IOMAP_OUT_UART4_TX              = 0x0D, 
    GPIO_IOMAP_OUT_UART5_TX              = 0x0E, 
    GPIO_IOMAP_OUT_UART1_RTS_RE_O        = 0x0F, 
    GPIO_IOMAP_OUT_UART1_CTS_DE_OUT      = 0x10, 
    GPIO_IOMAP_OUT_UART1_OUT             = 0x11, 
    GPIO_IOMAP_OUT_UART0_RTS_RE_O        = 0x12, 
    GPIO_IOMAP_OUT_UART0_CTS_DE_OUT      = 0x13, 
    GPIO_IOMAP_OUT_UART0_OUT             = 0x14, 
    GPIO_IOMAP_OUT_STMR3_PWM_OUT         = 0x15, 
    GPIO_IOMAP_OUT_STMR2_PWM_OUT         = 0x16, 
    GPIO_IOMAP_OUT_STMR1_PWM_OUT         = 0x17, 
    GPIO_IOMAP_OUT_STMR0_PWM_OUT         = 0x18, 
    GPIO_IOMAP_OUT_SUPTMR5_PWMOUT        = 0x19, 
    GPIO_IOMAP_OUT_SUPTMR4_PWMOUT        = 0x1A, 
    GPIO_IOMAP_OUT_SUPTMR3_PWMOUT        = 0x1B, 
    GPIO_IOMAP_OUT_SUPTMR2_PWMOUT        = 0x1C, 
    GPIO_IOMAP_OUT_SUPTMR1_PWMOUT        = 0x1D, 
    GPIO_IOMAP_OUT_SUPTMR0_PWMOUT        = 0x1E, 
    GPIO_IOMAP_OUT_SDHOST_SCLK_O         = 0x1F, 
    GPIO_IOMAP_OUT_TMR3_PWM_OUT          = 0x20, 
    GPIO_IOMAP_OUT_TMR2_PWM_OUT          = 0x21, 
    GPIO_IOMAP_OUT_TMR1_PWM_OUT          = 0x22, 
    GPIO_IOMAP_OUT_TMR0_PWM_OUT          = 0x23, 
    GPIO_IOMAP_OUT_LED_TMR0_PWM_OUT      = 0x24, 
    GPIO_IOMAP_OUT_LED_TMR1_PWM_OUT      = 0x25, 
    GPIO_IOMAP_OUT_LED_TMR2_PWM_OUT      = 0x26, 
    GPIO_IOMAP_OUT_LED_TMR3_PWM_OUT      = 0x27, 
    GPIO_IOMAP_OUT_PDM_MCLK              = 0x28, 
    GPIO_IOMAP_OUT_QSPI_NSS1_OUT         = 0x29, 
    GPIO_IOMAP_OUT_SPI0_NSS_OUT          = 0x2A, 
    GPIO_IOMAP_OUT_SPI0_SCK_OUT          = 0x2B, 
    GPIO_IOMAP_OUT_SPI0_IO0_OUT          = 0x2C, 
    GPIO_IOMAP_OUT_SPI0_IO1_OUT          = 0x2D, 
    GPIO_IOMAP_OUT_SPI0_IO2_OUT          = 0x2E, 
    GPIO_IOMAP_OUT_SPI0_IO3_OUT          = 0x2F, 
    GPIO_IOMAP_OUT_SPI1_NSS_OUT          = 0x30, 
    GPIO_IOMAP_OUT_SPI1_SCK_OUT          = 0x31, 
    GPIO_IOMAP_OUT_SPI1_IO0_OUT          = 0x32, 
    GPIO_IOMAP_OUT_SPI1_IO1_OUT          = 0x33, 
    GPIO_IOMAP_OUT_SPI1_IO2_OUT          = 0x34, 
    GPIO_IOMAP_OUT_SPI1_IO3_OUT          = 0x35, 
    GPIO_IOMAP_OUT_SPI2_NSS_OUT          = 0x36, 
    GPIO_IOMAP_OUT_SPI2_SCK_OUT          = 0x37, 
    GPIO_IOMAP_OUT_SPI2_IO0_OUT          = 0x38, 
    GPIO_IOMAP_OUT_SPI2_IO1_OUT          = 0x39, 
    GPIO_IOMAP_OUT_SPI2_IO2_OUT          = 0x3A, 
    GPIO_IOMAP_OUT_SPI2_IO3_OUT          = 0x3B, 
    GPIO_IOMAP_OUT_SPI5_SCK_OUT          = 0x3C, 
    GPIO_IOMAP_OUT_SPI5_IO0_OUT          = 0x3D, 
    GPIO_IOMAP_OUT_SPI5_IO1_OUT          = 0x3E, 
    GPIO_IOMAP_OUT_SPI6_SCK_OUT          = 0x3F, 
    GPIO_IOMAP_OUT_SPI6_IO0_OUT          = 0x40, 
    GPIO_IOMAP_OUT_SPI6_IO1_OUT          = 0x41, 
    GPIO_IOMAP_OUT_SDHOST_CMD_OUT        = 0x42, 
    GPIO_IOMAP_OUT_SDHOST_DAT0_OUT       = 0x43, 
    GPIO_IOMAP_OUT_SDHOST_DAT1_OUT       = 0x44, 
    GPIO_IOMAP_OUT_SDHOST_DAT2_OUT       = 0x45, 
    GPIO_IOMAP_OUT_SDHOST_DAT3_OUT       = 0x46, 
    GPIO_IOMAP_OUT_IIS0_MCLK_OUT         = 0x47, 
    GPIO_IOMAP_OUT_IIS0_WSCLK_OUT        = 0x48, 
    GPIO_IOMAP_OUT_IIS0_BCLK_OUT         = 0x49, 
    GPIO_IOMAP_OUT_IIS0_DO               = 0x4A, 
    GPIO_IOMAP_OUT_IIS1_MCLK_OUT         = 0x4B, 
    GPIO_IOMAP_OUT_IIS1_WSCLK_OUT        = 0x4C, 
    GPIO_IOMAP_OUT_IIS1_BCLK_OUT         = 0x4D, 
    GPIO_IOMAP_OUT_IIS1_DO               = 0x4E, 
    GPIO_IOMAP_OUT_CLK_TO_IO             = 0x4F, 
};


/** @defgroup Enumeration constant for GPIO IO map input function selection
  * @{
  */
enum gpio_iomap_in_func{
    GPIO_IOMAP_INPUT               = 0xF000,
    GPIO_IOMAP_IN_TMR0_CAP         = 0x0000,
    GPIO_IOMAP_IN_TMR0_SYNCI       ,
    GPIO_IOMAP_IN_TMR1_CAP         ,
    GPIO_IOMAP_IN_TMR2_CAP         ,
    GPIO_IOMAP_IN_TMR3_CAP         ,
    GPIO_IOMAP_IN_PDM_DATA         ,
    GPIO_IOMAP_IN_PTA_REQ          ,
    GPIO_IOMAP_IN_PTA_PRI          ,
    GPIO_IOMAP_IN_FREQ_IND         ,
    GPIO_IOMAP_IN_STMR0_CAP        ,
    GPIO_IOMAP_IN_STMR1_CAP        ,
    GPIO_IOMAP_IN_STMR2_CAP        ,
    GPIO_IOMAP_IN_STMR3_CAP        ,
    GPIO_IOMAP_IN_PORT_WKUP_IN0    ,
    GPIO_IOMAP_IN_PORT_WKUP_IN1    ,
    GPIO_IOMAP_IN_PORT_WKUP_IN2    ,
    GPIO_IOMAP_IN_PORT_WKUP_IN3    ,
    GPIO_IOMAP_IN_UART0_IN         ,
    GPIO_IOMAP_IN_UART0_CTS_DE_IN  ,
    GPIO_IOMAP_IN_UART1_IN         ,
    GPIO_IOMAP_IN_UART1_CTS_DE_IN  ,
    GPIO_IOMAP_IN_FB_IN_EXT_PA     ,
    GPIO_IOMAP_IN_UART4_RX         ,
    GPIO_IOMAP_IN_UART5_RX         ,
    GPIO_IOMAP_IN_SPI0_NSS_IN      ,
    GPIO_IOMAP_IN_SPI0_SCK_IN      ,
    GPIO_IOMAP_IN_SPI0_IO0_IN      ,
    GPIO_IOMAP_IN_SPI0_IO1_IN      ,
    GPIO_IOMAP_IN_SPI0_IO2_IN      ,
    GPIO_IOMAP_IN_SPI0_IO3_IN      ,
    GPIO_IOMAP_IN_SPI1_NSS_IN      ,
    GPIO_IOMAP_IN_SPI1_SCK_IN      ,
    GPIO_IOMAP_IN_SPI1_IO0_IN      ,
    GPIO_IOMAP_IN_SPI1_IO1_IN      ,
    GPIO_IOMAP_IN_SPI1_IO2_IN      ,
    GPIO_IOMAP_IN_SPI1_IO3_IN      ,
    GPIO_IOMAP_IN_SPI2_NSS_IN      ,
    GPIO_IOMAP_IN_SPI2_SCK_IN      ,
    GPIO_IOMAP_IN_SPI2_IO0_IN      ,
    GPIO_IOMAP_IN_SPI2_IO1_IN      ,
    GPIO_IOMAP_IN_SPI2_IO2_IN      ,
    GPIO_IOMAP_IN_SPI2_IO3_IN      ,
    GPIO_IOMAP_IN_SPI5_SCK_IN      ,
    GPIO_IOMAP_IN_SPI5_IO0_IN      ,
    GPIO_IOMAP_IN_SPI5_IO1_IN      ,
    GPIO_IOMAP_IN_SPI6_SCK_IN      ,
    GPIO_IOMAP_IN_SPI6_IO0_IN      ,
    GPIO_IOMAP_IN_SPI6_IO1_IN      ,
    GPIO_IOMAP_IN_SDHOST_CMD_IN    ,
    GPIO_IOMAP_IN_SDHOST_DAT0_IN   ,
    GPIO_IOMAP_IN_SDHOST_DAT1_IN   ,
    GPIO_IOMAP_IN_SDHOST_DAT2_IN   ,
    GPIO_IOMAP_IN_SDHOST_DAT3_IN   ,
    GPIO_IOMAP_IN_IIS0_MCLK_IN     ,
    GPIO_IOMAP_IN_IIS0_WSCLK_IN    ,
    GPIO_IOMAP_IN_IIS0_BCLK_IN     ,
    GPIO_IOMAP_IN_IIS0_DI          ,
    GPIO_IOMAP_IN_IIS1_MCLK_IN     ,
    GPIO_IOMAP_IN_IIS1_WSCLK_IN    ,
    GPIO_IOMAP_IN_IIS1_BCLK_IN     ,
    GPIO_IOMAP_IN_IIS1_DI          ,
}; 


/** @} Docxygenid_IO_function_enum*/

int32 gpio_driver_strength(uint32 pin, enum pin_driver_strength strength);
int32 gpio_set_altnt_func(uint32 pin, enum gpio_afio_set afio); 
int32 gpio_iomap_output(uint32 pin, enum gpio_iomap_out_func func_sel);
int32 gpio_iomap_input(uint32 pin, enum gpio_iomap_in_func func_sel);
int32 gpio_iomap_inout(uint32 pin, enum gpio_iomap_in_func in_func_sel, enum gpio_iomap_out_func out_func_sel);


#ifdef __cplusplus
}
#endif
#endif

/*************************** (C) COPYRIGHT 2016-2022 HUGE-IC ***** END OF FILE *****/

