
// @file    pin_function.c
// @author  wangying
// @brief   This file contains all the mars pin functions.

// Revision History
// V1.0.0  06/01/2019  First Release, copy from 4001a project
// V1.0.1  07/05/2019  add lmac pin-func
// V1.0.2  07/06/2019  add sdio pull-up regs config
// V1.0.3  07/09/2019  add agc/rx-bw/lo-freq-idx gpio control
// V1.0.4  07/18/2019  change gpio-agc default index to 5
// V1.0.5  07/19/2019  uart1 only init tx
// V1.0.6  07/23/2019  add uart-rx pull-up resistor config
// V1.0.7  07/24/2019  switch-en1 disable; delete fdd/tdd macro-def switch
// V1.0.8  07/26/2019  not use pb17 for mac debug for it is used to reset ext-rf
// V1.0.9  07/29/2019  add function dbg_pin_func()
// V1.1.0  02/11/2020  add spi-pin-function
// V1.1.1  02/27/2020  fix uart1 pin-function
// V1.2.0  03/02/2020  add uart0-pin code and rf-pin code
// V1.2.1  03/26/2020  fix vmode pin
// V1.2.2  04/14/2020  use pa2 as lmac debug1:rx_req

#include "sys_config.h"
#include "typesdef.h"
#include "errno.h"
#include "list.h"
#include "dev.h"
#include "devid.h"
#include "hal/gpio.h"

__weak void user_pin_func(int dev_id, int request) {};

/** 
  * @brief  Configure the GPIO pin driver strength.
  * @param  pin       : which pin to set.\n
  *                     This parameter can be Px_y where x can be (A..C) and y can be (0..15)\n
  *                     But the value of the y only be (0..5) when x is C.
  * @strength         : Driver strength to configure the GPIO pin, reference @ref gpio_private_pin_driver_strength.
  * @return
  *         - RET_OK  : Configure the GPIO pin driver strength successfully.
  *         - RET_ERR : Configure the GPIO pin driver strength unsuccessfully.
  */
int32 gpio_driver_strength(uint32 pin, enum pin_driver_strength strength)
{
    struct gpio_device *gpio = gpio_get(pin);

    if (gpio && gpio->ioctl) {
        return gpio->ioctl(gpio, pin, GPIO_CMD_DRIVER_STRENGTH, strength, 0);
    }
    return RET_ERR;
}


/** 
  * @brief  Configure the GPIO module AFIO.
  * @param  pin       : which pin to set.\n
  *                     This parameter can be Px_y where x can be (A..C) and y can be (0..15)\n
  *                     But the value of the y only be (0..5) when x is C.
  * @afio             : AFIO value, reference @ref gpio_private_afio_set.
  * @return
  *         - RET_OK  : GPIO module configure AFIO successfully.
  *         - RET_ERR : GPIO module configure AFIO unsuccessfully.
  */
int32 gpio_set_altnt_func(uint32 pin, enum gpio_afio_set afio)
{
    struct gpio_device *gpio = gpio_get(pin);

    if (gpio && gpio->ioctl) {
        return gpio->ioctl(gpio, pin, GPIO_CMD_AFIO_SET, afio, 0);
    }
    return RET_ERR;
}


/** 
  * @brief  Configure the GPIO module IOMAP_OUTPUT.
  * @param  pin       : which pin to set.\n
  *                     This parameter can be Px_y where x can be (A..C) and y can be (0..15)\n
  *                     But the value of the y only be (0..5) when x is C.
  * @func_sel         : IOMAP_OUTPUT function value, reference @ref gpio_private_iomap_out_func.
  * @return
  *         - RET_OK  : GPIO module configure IOMAP_OUTPUT successfully.
  *         - RET_ERR : GPIO module configure IOMAP_OUTPUT unsuccessfully.
  */
int32 gpio_iomap_output(uint32 pin, enum gpio_iomap_out_func func_sel)
{

    struct gpio_device *gpio = gpio_get(pin);

    if (gpio && gpio->ioctl) {
        return gpio->ioctl(gpio, pin, GPIO_CMD_IOMAP_OUT_FUNC, func_sel, 0);
    }

    return RET_ERR;
}


/** 
  * @brief  Configure the GPIO module IOMAP_INPUT.
  * @param  pin       : which pin to set.\n
  *                     This parameter can be Px_y where x can be (A..C) and y can be (0..15)\n
  *                     But the value of the y only be (0..5) when x is C.
  * @func_sel         : IOMAP_INPUT function value, reference @ref gpio_private_iomap_in_func.
  * @return
  *         - RET_OK  : GPIO module configure IOMAP_INPUT successfully.
  *         - RET_ERR : GPIO module configure IOMAP_INPUT unsuccessfully.
  */
int32 gpio_iomap_input(uint32 pin, enum gpio_iomap_in_func func_sel)
{

    struct gpio_device *gpio = gpio_get(pin);

    if (gpio && gpio->ioctl) {
        return gpio->ioctl(gpio, pin, GPIO_CMD_IOMAP_IN_FUNC, func_sel, 0);
    }

    return RET_ERR;
}


/** 
  * @brief  Configure the GPIO module IOMAP_INOUT.
  * @param  pin       : which pin to set.\n
  *                     This parameter can be Px_y where x can be (A..C) and y can be (0..15)\n
  *                     But the value of the y only be (0..5) when x is C.
  * @in_func_sel      : IOMAP_INPUT function value, reference @ref gpio_private_iomap_in_func.
  * @out_func_sel     : IOMAP_OUTPUT function value, reference @ref gpio_private_iomap_out_func.
  * @return
  *         - RET_OK  : GPIO module configure IOMAP_INOUT successfully.
  *         - RET_ERR : GPIO module configure IOMAP_INOUT unsuccessfully.
  */
int32 gpio_iomap_inout(uint32 pin, enum gpio_iomap_in_func in_func_sel, enum gpio_iomap_out_func out_func_sel)
{

    struct gpio_device *gpio = gpio_get(pin);

    if (gpio && gpio->ioctl) {
        return gpio->ioctl(gpio, pin, GPIO_CMD_IOMAP_INOUT_FUNC, in_func_sel, out_func_sel);
    }

    return RET_ERR;
}

static int uart_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_UART0_DEVID:
            if (request) {
                gpio_iomap_output(PIN_UART0_TX, GPIO_IOMAP_OUT_UART0_OUT);
                gpio_iomap_input(PIN_UART0_RX, GPIO_IOMAP_IN_UART0_IN);
                gpio_set_mode(PIN_UART0_RX, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
            } else {
                gpio_set_dir(PIN_UART0_TX, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_UART0_RX, GPIO_DIR_INPUT);
            }
            break;
        case HG_UART1_DEVID: //if use uart1 for print, just init tx
            if (request) {
                gpio_iomap_input(PIN_UART1_RX, GPIO_IOMAP_IN_UART1_IN);
                gpio_iomap_output(PIN_UART1_TX, GPIO_IOMAP_OUT_UART1_OUT);
                gpio_set_mode(PIN_UART1_RX, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
            } else {
                gpio_set_dir(PIN_UART1_RX, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_UART1_TX, GPIO_DIR_INPUT);
            }
            break;
        case (HG_UART4_DEVID):
            if (request) {
                gpio_iomap_output(PIN_UART4_TX, GPIO_IOMAP_OUT_UART4_TX);
                gpio_iomap_input(PIN_UART4_RX, GPIO_IOMAP_IN_UART4_RX);
            } else {
                gpio_set_dir(PIN_UART4_TX, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_UART4_RX, GPIO_DIR_INPUT);
            }
            break;
        case (HG_UART5_DEVID):
            if (request) {
                gpio_iomap_output(PIN_UART5_TX, GPIO_IOMAP_OUT_UART5_TX);
                gpio_iomap_input(PIN_UART5_RX, GPIO_IOMAP_IN_UART5_RX);
            } else {
                gpio_set_dir(PIN_UART5_TX, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_UART5_RX, GPIO_DIR_INPUT);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}

static int gmac_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_GMAC_DEVID:
            if (request) {
                gpio_set_altnt_func(PIN_GMAC_RMII_REF_CLKIN, 0);
                gpio_set_altnt_func(PIN_GMAC_RMII_RXD0, 0);
                gpio_set_altnt_func(PIN_GMAC_RMII_RXD1, 0);
                gpio_set_altnt_func(PIN_GMAC_RMII_TXD0, 0);
                gpio_set_altnt_func(PIN_GMAC_RMII_TXD1, 0);
                gpio_set_altnt_func(PIN_GMAC_RMII_CRS_DV, 0);
                gpio_set_altnt_func(PIN_GMAC_RMII_TX_EN, 0);
                gpio_set_dir(PIN_GMAC_RMII_MDIO, GPIO_DIR_OUTPUT);
                gpio_set_dir(PIN_GMAC_RMII_MDC, GPIO_DIR_OUTPUT);
            } else {
                gpio_set_dir(PIN_GMAC_RMII_REF_CLKIN, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_RXD0, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_RXD1, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_TXD0, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_TXD1, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_CRS_DV, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_TX_EN, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_MDIO, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_GMAC_RMII_MDC, GPIO_DIR_INPUT);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


static int sdio_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_SDIOSLAVE_DEVID:
            if (request) {
                gpio_set_altnt_func(PIN_SDCLK, 0);
                gpio_set_altnt_func(PIN_SDCMD, 0);
                gpio_set_mode(PIN_SDCMD, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
                gpio_set_mode(PIN_SDCLK, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
                gpio_set_altnt_func(PIN_SDDAT0, 0);
                gpio_set_mode(PIN_SDDAT0, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
                gpio_set_altnt_func(PIN_SDDAT1, 0);
                gpio_set_mode(PIN_SDDAT1, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
                gpio_set_altnt_func(PIN_SDDAT2, 0);
                gpio_set_mode(PIN_SDDAT2, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);
                gpio_set_altnt_func(PIN_SDDAT3, 0);
                gpio_set_mode(PIN_SDDAT3, GPIO_PULL_UP, GPIO_PULL_LEVEL_100K);

                gpio_ioctl(PIN_SDCMD, GPIO_INPUT_DELAY_ON_OFF, 1, 0);
                gpio_ioctl(PIN_SDCLK, GPIO_INPUT_DELAY_ON_OFF, 1, 0);
                gpio_ioctl(PIN_SDDAT0, GPIO_INPUT_DELAY_ON_OFF, 1, 0);
                gpio_ioctl(PIN_SDDAT1, GPIO_INPUT_DELAY_ON_OFF, 1, 0);
                gpio_ioctl(PIN_SDDAT2, GPIO_INPUT_DELAY_ON_OFF, 1, 0);
                gpio_ioctl(PIN_SDDAT3, GPIO_INPUT_DELAY_ON_OFF, 1, 0);

                gpio_driver_strength(PIN_SDCLK,  GPIO_DS_8MA);
                gpio_driver_strength(PIN_SDCMD,  GPIO_DS_8MA);
                gpio_driver_strength(PIN_SDDAT0, GPIO_DS_8MA);
                gpio_driver_strength(PIN_SDDAT1, GPIO_DS_8MA);
                gpio_driver_strength(PIN_SDDAT2, GPIO_DS_8MA);
                gpio_driver_strength(PIN_SDDAT3, GPIO_DS_8MA);
            } else {
                gpio_set_dir(PIN_SDCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SDCMD, GPIO_DIR_INPUT);
                gpio_set_mode(PIN_SDCMD, GPIO_PULL_UP, GPIO_PULL_LEVEL_NONE);
                gpio_set_dir(PIN_SDDAT0, GPIO_DIR_INPUT);
                gpio_set_mode(PIN_SDDAT0, GPIO_PULL_UP, GPIO_PULL_LEVEL_NONE);
                gpio_set_dir(PIN_SDDAT1, GPIO_DIR_INPUT);
                gpio_set_mode(PIN_SDDAT1, GPIO_PULL_UP, GPIO_PULL_LEVEL_NONE);
                gpio_set_dir(PIN_SDDAT2, GPIO_DIR_INPUT);
                gpio_set_mode(PIN_SDDAT2, GPIO_PULL_UP, GPIO_PULL_LEVEL_NONE);
                gpio_set_dir(PIN_SDDAT3, GPIO_DIR_INPUT);
                gpio_set_mode(PIN_SDDAT3, GPIO_PULL_UP, GPIO_PULL_LEVEL_NONE);
                gpio_ioctl(PIN_SDCMD, GPIO_INPUT_DELAY_ON_OFF, 0, 0);
                gpio_ioctl(PIN_SDCLK, GPIO_INPUT_DELAY_ON_OFF, 0, 0);
                gpio_ioctl(PIN_SDDAT0, GPIO_INPUT_DELAY_ON_OFF, 0, 0);
                gpio_ioctl(PIN_SDDAT1, GPIO_INPUT_DELAY_ON_OFF, 0, 0);
                gpio_ioctl(PIN_SDDAT2, GPIO_INPUT_DELAY_ON_OFF, 0, 0);
                gpio_ioctl(PIN_SDDAT3, GPIO_INPUT_DELAY_ON_OFF, 0, 0);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}

static int qspi_pin_func(int dev_id, int request)
{
    int ret = RET_OK;
    static int req_cs1 = 0;

    switch (dev_id) {
        case HG_QSPI_DEVID:
            if (request) {
//                gpio_set_altnt_func(PIN_QSPI_CS,  4);
//                gpio_set_altnt_func(PIN_QSPI_CLK, 4);
//                gpio_set_altnt_func(PIN_QSPI_IO0, 4);
//                gpio_set_altnt_func(PIN_QSPI_IO1, 4);
//                gpio_set_altnt_func(PIN_QSPI_IO2, 4);
//                gpio_set_altnt_func(PIN_QSPI_IO3, 4);
                if (request == 2) {
                    //gpio_set_altnt_func(PIN_QSPI_CS1, 4);
                    req_cs1 = 1;
                }
            } else {
                //gpio_set_dir(PIN_QSPI_CS,  GPIO_DIR_INPUT);
                //gpio_set_dir(PIN_QSPI_CLK, GPIO_DIR_INPUT);
                //gpio_set_dir(PIN_QSPI_IO0, GPIO_DIR_INPUT);
                //gpio_set_dir(PIN_QSPI_IO1, GPIO_DIR_INPUT);
                //gpio_set_dir(PIN_QSPI_IO2, GPIO_DIR_INPUT);
                //gpio_set_dir(PIN_QSPI_IO3, GPIO_DIR_INPUT);
                if (req_cs1) {
                    req_cs1 = 0;
                    //gpio_set_dir(PIN_QSPI_CS1,  GPIO_DIR_INPUT);
                }
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


static int spi_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_SPI0_DEVID:
            if (request) {
                gpio_iomap_inout(PIN_SPI0_CS, GPIO_IOMAP_IN_SPI0_NSS_IN, GPIO_IOMAP_OUT_SPI0_NSS_OUT);
                gpio_iomap_inout(PIN_SPI0_CLK, GPIO_IOMAP_IN_SPI0_SCK_IN, GPIO_IOMAP_OUT_SPI0_SCK_OUT);
                gpio_iomap_inout(PIN_SPI0_IO0, GPIO_IOMAP_IN_SPI0_IO0_IN, GPIO_IOMAP_OUT_SPI0_IO0_OUT);
                gpio_iomap_inout(PIN_SPI0_IO1, GPIO_IOMAP_IN_SPI0_IO1_IN, GPIO_IOMAP_OUT_SPI0_IO1_OUT);
                gpio_iomap_inout(PIN_SPI0_IO2, GPIO_IOMAP_IN_SPI0_IO2_IN, GPIO_IOMAP_OUT_SPI0_IO2_OUT);
                gpio_iomap_inout(PIN_SPI0_IO3, GPIO_IOMAP_IN_SPI0_IO3_IN, GPIO_IOMAP_OUT_SPI0_IO3_OUT);
            } else {
                gpio_set_dir(PIN_SPI0_CS, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI0_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI0_IO0, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI0_IO1, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI0_IO2, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI0_IO3, GPIO_DIR_INPUT);
            }
            break;
        case HG_SPI1_DEVID:
            if (request) {
                gpio_iomap_inout(PIN_SPI1_CS, GPIO_IOMAP_IN_SPI1_NSS_IN, GPIO_IOMAP_OUT_SPI1_NSS_OUT);
                gpio_iomap_inout(PIN_SPI1_CLK, GPIO_IOMAP_IN_SPI1_SCK_IN, GPIO_IOMAP_OUT_SPI1_SCK_OUT);
                gpio_iomap_inout(PIN_SPI1_IO0, GPIO_IOMAP_IN_SPI1_IO0_IN, GPIO_IOMAP_OUT_SPI1_IO0_OUT);
                gpio_iomap_inout(PIN_SPI1_IO1, GPIO_IOMAP_IN_SPI1_IO1_IN, GPIO_IOMAP_OUT_SPI1_IO1_OUT);
            } else {
                gpio_set_dir(PIN_SPI1_CS, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI1_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI1_IO0, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI1_IO1, GPIO_DIR_INPUT);
            }
            break;
        case (HG_SPI5_DEVID):
            if (request) {
                gpio_iomap_inout(PIN_SPI5_CLK, GPIO_IOMAP_IN_SPI5_SCK_IN, GPIO_IOMAP_OUT_SPI5_SCK_OUT);
                gpio_iomap_inout(PIN_SPI5_IO0, GPIO_IOMAP_IN_SPI5_IO0_IN, GPIO_IOMAP_OUT_SPI5_IO0_OUT);
                gpio_iomap_inout(PIN_SPI5_IO1, GPIO_IOMAP_IN_SPI5_IO1_IN, GPIO_IOMAP_OUT_SPI5_IO1_OUT);
            } else {
                gpio_set_dir(PIN_SPI5_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI5_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI5_CLK, GPIO_DIR_INPUT);
            }
            break;
        case (HG_SPI6_DEVID):
            if (request) {
                gpio_iomap_inout(PIN_SPI6_CLK, GPIO_IOMAP_IN_SPI6_SCK_IN, GPIO_IOMAP_OUT_SPI6_SCK_OUT);
                gpio_iomap_inout(PIN_SPI6_IO0, GPIO_IOMAP_IN_SPI6_IO0_IN, GPIO_IOMAP_OUT_SPI6_IO0_OUT);
                gpio_iomap_inout(PIN_SPI6_IO1, GPIO_IOMAP_IN_SPI6_IO1_IN, GPIO_IOMAP_OUT_SPI6_IO1_OUT);
            } else {
                gpio_set_dir(PIN_SPI6_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI6_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SPI6_CLK, GPIO_DIR_INPUT);
            }
            break;
        case HG_SPI2_DEVID:
        case HG_SPI3_DEVID:
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}

static int iis_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_IIS0_DEVID:
            if (request) {
                gpio_iomap_inout(PIN_IIS0_MCLK, GPIO_IOMAP_IN_IIS0_MCLK_IN, GPIO_IOMAP_OUT_IIS0_MCLK_OUT);
                gpio_iomap_inout(PIN_IIS0_BCLK, GPIO_IOMAP_IN_IIS0_BCLK_IN, GPIO_IOMAP_OUT_IIS0_BCLK_OUT);
                gpio_iomap_inout(PIN_IIS0_WCLK, GPIO_IOMAP_IN_IIS0_WSCLK_IN, GPIO_IOMAP_OUT_IIS0_WSCLK_OUT);
                gpio_iomap_inout(PIN_IIS0_DATA, GPIO_IOMAP_IN_IIS0_DI, GPIO_IOMAP_OUT_IIS0_DO);
            } else {
                gpio_set_dir(PIN_IIS0_MCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIS0_BCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIS0_WCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIS0_DATA, GPIO_DIR_INPUT);
            }
            break;
        case HG_IIS1_DEVID:
            if (request) {
                gpio_iomap_inout(PIN_IIS1_MCLK, GPIO_IOMAP_IN_IIS1_MCLK_IN, GPIO_IOMAP_OUT_IIS1_MCLK_OUT);
                gpio_iomap_inout(PIN_IIS1_BCLK, GPIO_IOMAP_IN_IIS1_BCLK_IN, GPIO_IOMAP_OUT_IIS1_BCLK_OUT);
                gpio_iomap_inout(PIN_IIS1_WCLK, GPIO_IOMAP_IN_IIS1_WSCLK_IN, GPIO_IOMAP_OUT_IIS1_WSCLK_OUT);
                gpio_iomap_inout(PIN_IIS1_DATA, GPIO_IOMAP_IN_IIS1_DI, GPIO_IOMAP_OUT_IIS1_DO);
            } else {
                gpio_set_dir(PIN_IIS1_MCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIS1_BCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIS1_WCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIS1_DATA, GPIO_DIR_INPUT);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}

static int pdm_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_PDM_DEVID:
            if (request) {
                gpio_iomap_output(PIN_PDM_MCLK, GPIO_IOMAP_OUT_PDM_MCLK);
                gpio_iomap_input(PIN_PDM_DATA,  GPIO_IOMAP_IN_PDM_DATA);
            } else {
                gpio_set_dir(PIN_PDM_MCLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_PDM_DATA, GPIO_DIR_INPUT);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


static int led_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_LED_DEVID:
            if (request) {
                gpio_set_altnt_func(PIN_LED_SEG0, GPIO_AF_0);     //seg0
                gpio_set_altnt_func(PIN_LED_SEG1, GPIO_AF_0);     //seg1
                gpio_set_altnt_func(PIN_LED_SEG2, GPIO_AF_0);     //seg2
                gpio_set_altnt_func(PIN_LED_SEG3, GPIO_AF_0);     //seg3
                gpio_set_altnt_func(PIN_LED_SEG4, GPIO_AF_0);     //seg4
                gpio_set_altnt_func(PIN_LED_SEG5, GPIO_AF_0);    //seg5
                gpio_set_altnt_func(PIN_LED_SEG6, GPIO_AF_0);    //seg6
                gpio_set_altnt_func(PIN_LED_SEG7, GPIO_AF_0);    //seg7
//                gpio_set_altnt_func(PIN_LED_SEG8, GPIO_AF_0);    //seg8
//                gpio_set_altnt_func(PIN_LED_SEG9 , GPIO_AF_0);    //seg9
//                gpio_set_altnt_func(PIN_LED_SEG10,GPIO_AF_0);    //seg10
//                gpio_set_altnt_func(PIN_LED_SEG11,GPIO_AF_0);    //seg11

                gpio_set_altnt_func(PIN_LED_COM0,  GPIO_AF_0);    //com0
                gpio_set_altnt_func(PIN_LED_COM1,  GPIO_AF_0);    //com1
                gpio_set_altnt_func(PIN_LED_COM2,  GPIO_AF_0);    //com2
//                gpio_set_altnt_func(PIN_LED_COM3,  GPIO_AF_0);    //com3
//                gpio_set_altnt_func(PIN_LED_COM4,  GPIO_AF_1);    //com4
//                gpio_set_altnt_func(PIN_LED_COM5,  GPIO_AF_1);    //com5
//                gpio_set_altnt_func(PIN_LED_COM6,  GPIO_AF_1);    //com6
//                gpio_set_altnt_func(PIN_LED_COM7,  GPIO_AF_1);    //com7

            } else {
                gpio_set_dir(PIN_LED_SEG0, GPIO_DIR_INPUT);     //seg0
                gpio_set_dir(PIN_LED_SEG1, GPIO_DIR_INPUT);     //seg1
                gpio_set_dir(PIN_LED_SEG2, GPIO_DIR_INPUT);     //seg2
                gpio_set_dir(PIN_LED_SEG3, GPIO_DIR_INPUT);     //seg3
                gpio_set_dir(PIN_LED_SEG4, GPIO_DIR_INPUT);     //seg4
                gpio_set_dir(PIN_LED_SEG5, GPIO_DIR_INPUT);    //seg5
                gpio_set_dir(PIN_LED_SEG6, GPIO_DIR_INPUT);    //seg6
                gpio_set_dir(PIN_LED_SEG7, GPIO_DIR_INPUT);    //seg7
//                gpio_set_dir(PIN_LED_SEG8, GPIO_DIR_INPUT);    //seg8
//                gpio_set_dir(PIN_LED_SEG9, GPIO_DIR_INPUT);    //seg9
//                gpio_set_dir(PIN_LED_SEG10,GPIO_DIR_INPUT);    //seg10
//                gpio_set_dir(PIN_LED_SEG11,GPIO_DIR_INPUT);    //seg11

                gpio_set_dir(PIN_LED_COM0,  GPIO_DIR_INPUT);    //com0
//                gpio_set_dir(PIN_LED_COM1,  GPIO_DIR_INPUT);    //com1
//                gpio_set_dir(PIN_LED_COM2,  GPIO_DIR_INPUT);    //com2
//                gpio_set_dir(PIN_LED_COM3,  GPIO_DIR_INPUT);    //com3
//                gpio_set_dir(PIN_LED_COM4,  GPIO_DIR_INPUT);    //com4
//                gpio_set_dir(PIN_LED_COM5,  GPIO_DIR_INPUT);    //com5
//                gpio_set_dir(PIN_LED_COM6,  GPIO_DIR_INPUT);    //com6
//                gpio_set_dir(PIN_LED_COM7,  GPIO_DIR_INPUT);    //com7
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}

static int iic_pin_func(int dev_id, int request)
{
    int ret = RET_OK;
    switch (dev_id) {
        case HG_I2C1_DEVID:
            if (request) {
                gpio_iomap_inout(PIN_IIC1_SCL, GPIO_IOMAP_IN_SPI1_SCK_IN, GPIO_IOMAP_OUT_SPI1_SCK_OUT);
                gpio_iomap_inout(PIN_IIC1_SDA, GPIO_IOMAP_IN_SPI1_IO0_IN, GPIO_IOMAP_OUT_SPI1_IO0_OUT);
                gpio_set_mode(PIN_IIC1_SCL, GPIO_OPENDRAIN_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                gpio_set_mode(PIN_IIC1_SDA, GPIO_OPENDRAIN_PULL_UP, GPIO_PULL_LEVEL_4_7K);
            } else {
                gpio_set_dir(PIN_IIC1_SCL, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIC1_SDA, GPIO_DIR_INPUT);
            }
            break;
        case HG_I2C2_DEVID:
            if (request) {
                gpio_iomap_inout(PIN_IIC2_SCL, GPIO_IOMAP_IN_SPI2_SCK_IN, GPIO_IOMAP_OUT_SPI2_SCK_OUT);
                gpio_iomap_inout(PIN_IIC2_SDA, GPIO_IOMAP_IN_SPI2_IO0_IN, GPIO_IOMAP_OUT_SPI2_IO0_OUT);
                gpio_set_mode(PIN_IIC2_SCL, GPIO_OPENDRAIN_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                gpio_set_mode(PIN_IIC2_SDA, GPIO_OPENDRAIN_PULL_UP, GPIO_PULL_LEVEL_4_7K);
            } else {
                gpio_set_dir(PIN_IIC2_SCL, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_IIC2_SDA, GPIO_DIR_INPUT);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


static int adc_pin_func(int dev_id, int request) {

    #define ADC_FLAG_REQUEST BIT(26)
    #define ADC_FLAG_SUSPEND BIT(27)
    #define ADC_FLAG_RELEASE BIT(28)

    struct gpio_device *ptr = NULL;
    uint32 flag = request & (0xFF << 24);
    uint32 adc_pin = (request&0xFFFFFF);

    switch (dev_id) {
        case (HG_ADC_DEVID):
            
            switch (flag) {
                /* release */
                case (ADC_FLAG_RELEASE):
                    gpio_ioctl(adc_pin, GPIO_CMD_ADC_ANALOG, 0, 0);
                    gpio_set_dir(adc_pin, GPIO_DIR_INPUT);
                    break;
                /* suspend */
                case (ADC_FLAG_SUSPEND):
                    gpio_ioctl(adc_pin, GPIO_CMD_ADC_ANALOG, 0, 0);
                    break;
                case (ADC_FLAG_REQUEST):
                    gpio_ioctl(adc_pin, GPIO_CMD_ADC_ANALOG, 1, 0);
                    ptr = gpio_get(adc_pin);
                    if (HG_GPIOA_DEVID == ptr->dev.dev_id) {
                        return BIT(25);
                    }
                    break;
                default:
                    return RET_ERR;
                    break;
            }
            break;
        case (HG_ADC1_DEVID):
            
            switch (flag) {
                /* release */
                case (ADC_FLAG_RELEASE):
                    gpio_ioctl(adc_pin, GPIO_CMD_ADC_ANALOG, 0, 0);
                    gpio_set_dir(adc_pin, GPIO_DIR_INPUT);
                    break;
                /* suspend */
                case (ADC_FLAG_SUSPEND):
                    gpio_ioctl(adc_pin, GPIO_CMD_ADC_ANALOG, 0, 0);
                    break;
                case (ADC_FLAG_REQUEST):
                    gpio_ioctl(adc_pin, GPIO_CMD_ADC_ANALOG, 1, 0);
                    ptr = gpio_get(adc_pin);
                    if (HG_GPIOA_DEVID == ptr->dev.dev_id) {
                        return BIT(25);
                    }
                    break;
                default:
                    return RET_ERR;
                    break;
            }
            break;
    }

    return RET_OK;
}

static int timer_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_TIMER0_DEVID:
            switch (request) {
                /* 0: none */
                case (0):
                    gpio_set_dir(PIN_PWM_CHANNEL_0, GPIO_DIR_INPUT);
                    gpio_set_dir(PIN_CAPTURE_CHANNEL_0, GPIO_DIR_INPUT);
                    break;
                /* 1: pwm */
                case (1):
                    gpio_iomap_output(PIN_PWM_CHANNEL_0, GPIO_IOMAP_OUT_TMR0_PWM_OUT);
                    break;
                /* 2: capture */
                case (2):
                    gpio_iomap_input(PIN_CAPTURE_CHANNEL_0, GPIO_IOMAP_IN_TMR0_CAP);
                    break;
                default:
                    ret = EINVAL;
                    break;
            }
            break;
        case HG_TIMER1_DEVID:
            switch (request) {
                /* 0: none */
                case (0):
                    gpio_set_dir(PIN_PWM_CHANNEL_1, GPIO_DIR_INPUT);
                    gpio_set_dir(PIN_CAPTURE_CHANNEL_1, GPIO_DIR_INPUT);
                    break;
                /* 1: pwm */
                case (1):
                    gpio_iomap_output(PIN_PWM_CHANNEL_1, GPIO_IOMAP_OUT_TMR1_PWM_OUT);
                    break;
                /* 2: capture */
                case (2):
                    gpio_iomap_input(PIN_CAPTURE_CHANNEL_1, GPIO_IOMAP_IN_TMR1_CAP);
                    break;
                default:
                    ret = EINVAL;
                    break;
            }
            break;
        case HG_LED_TIMER0_DEVID:
            switch (request) {
                /* 0: none */
                case (0):
                    gpio_set_dir(PIN_PWM_CHANNEL_2, GPIO_DIR_INPUT);
                    break;
                /* 1: pwm */
                case (1):
                    gpio_iomap_output(PIN_PWM_CHANNEL_2, GPIO_IOMAP_OUT_LED_TMR0_PWM_OUT);
                    break;
                default:
                    ret = EINVAL;
                    break;
            }
            break;
        case HG_SUPTMR0_DEVID:
            switch (request) {
                /* 0: none */
                case (0):
                    gpio_set_dir(PIN_PWM_CHANNEL_3, GPIO_DIR_INPUT);
                    break;
                /* 1: pwm */
                case (1):
                    gpio_iomap_output(PIN_PWM_CHANNEL_3, GPIO_IOMAP_OUT_SUPTMR0_PWMOUT);
                    break;
                default:
                    ret = EINVAL;
                    break;
            }
            break;

        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


static int dvp_pin_func(int dev_id, int request)
{
    int ret = RET_OK;
    switch (dev_id) {
        case HG_DVP_DEVID:
            gpio_set_altnt_func(PIN_DVP_HSYNC, 1);
            gpio_set_altnt_func(PIN_DVP_VSYNC, 1);
            gpio_set_altnt_func(PIN_DVP_PCLK , 1);
            gpio_set_altnt_func(PIN_DVP_MCLK , 1);
            gpio_set_altnt_func(PIN_DVP_DATA0, 1);
            gpio_set_altnt_func(PIN_DVP_DATA1, 1);
            gpio_set_altnt_func(PIN_DVP_DATA2, 1);
            gpio_set_altnt_func(PIN_DVP_DATA3, 1);
            gpio_set_altnt_func(PIN_DVP_DATA4, 1);
            gpio_set_altnt_func(PIN_DVP_DATA5, 1);
            gpio_set_altnt_func(PIN_DVP_DATA6, 1);
            gpio_set_altnt_func(PIN_DVP_DATA7, 1);
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


static int sdh_pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    switch (dev_id) {
        case HG_SDC_HOST_DEVID:
            if (request) {
                gpio_set_mode(PIN_SDH_CLK, GPIO_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                gpio_set_mode(PIN_SDH_CMD, GPIO_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                gpio_set_mode(PIN_SDH_DAT0, GPIO_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                if (request == 4) {
                    gpio_set_mode(PIN_SDH_DAT1, GPIO_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                    gpio_set_mode(PIN_SDH_DAT2, GPIO_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                    gpio_set_mode(PIN_SDH_DAT3, GPIO_PULL_UP, GPIO_PULL_LEVEL_4_7K);
                }

                gpio_driver_strength(PIN_SDH_CLK, GPIO_DS_28MA);
                gpio_driver_strength(PIN_SDH_CMD, GPIO_DS_28MA);
                gpio_driver_strength(PIN_SDH_DAT0, GPIO_DS_28MA);
                if (request == 4) {
                    gpio_driver_strength(PIN_SDH_DAT1, GPIO_DS_28MA);
                    gpio_driver_strength(PIN_SDH_DAT2, GPIO_DS_28MA);
                    gpio_driver_strength(PIN_SDH_DAT3, GPIO_DS_28MA);
                }

                gpio_iomap_output(PIN_SDH_CLK, GPIO_IOMAP_OUT_SDHOST_SCLK_O);
                gpio_iomap_inout(PIN_SDH_CMD, GPIO_IOMAP_IN_SDHOST_CMD_IN, GPIO_IOMAP_OUT_SDHOST_CMD_OUT);
                gpio_iomap_inout(PIN_SDH_DAT0, GPIO_IOMAP_IN_SDHOST_DAT0_IN, GPIO_IOMAP_OUT_SDHOST_DAT0_OUT);

                if (request == 4) {
                    gpio_iomap_inout(PIN_SDH_DAT1, GPIO_IOMAP_IN_SDHOST_DAT1_IN, GPIO_IOMAP_OUT_SDHOST_DAT1_OUT);
                    gpio_iomap_inout(PIN_SDH_DAT2, GPIO_IOMAP_IN_SDHOST_DAT2_IN, GPIO_IOMAP_OUT_SDHOST_DAT2_OUT);
                    gpio_iomap_inout(PIN_SDH_DAT3, GPIO_IOMAP_IN_SDHOST_DAT3_IN, GPIO_IOMAP_OUT_SDHOST_DAT3_OUT);
                }

            } else {
                gpio_set_dir(PIN_SDH_CLK, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SDH_CMD, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SDH_DAT0, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SDH_DAT1, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SDH_DAT2, GPIO_DIR_INPUT);
                gpio_set_dir(PIN_SDH_DAT3, GPIO_DIR_INPUT);
            }
            break;
        default:
            ret = EINVAL;
            break;
    }
    return ret;
}


int pin_func(int dev_id, int request)
{
    int ret = RET_OK;

    sysctrl_unlock();

    switch (dev_id) {
        case HG_UART0_DEVID:
        case HG_UART1_DEVID:
        case HG_UART4_DEVID:
        case HG_UART5_DEVID:
            ret = uart_pin_func(dev_id, request);
            break;
        case HG_GMAC_DEVID:
            ret = gmac_pin_func(dev_id, request);
            break;
        case HG_SDIOSLAVE_DEVID:
            ret = sdio_pin_func(dev_id, request);
            break;
        case HG_SPI0_DEVID:
        case HG_SPI2_DEVID:
        case HG_SPI1_DEVID:
        case HG_SPI3_DEVID:
        case HG_SPI5_DEVID:
        case HG_SPI6_DEVID:
            ret = spi_pin_func(dev_id, request);
            break;
        case HG_I2C0_DEVID:
        case HG_I2C1_DEVID:
        case HG_I2C2_DEVID:
        case HG_I2C3_DEVID:
            ret = iic_pin_func(dev_id, request);
            break;
        case HG_DVP_DEVID:
            ret = dvp_pin_func(dev_id, request);
            break;
        case HG_QSPI_DEVID:
            ret = qspi_pin_func(dev_id, request);
            break;
        case HG_SDC_HOST_DEVID:
            ret = sdh_pin_func(dev_id, request);
            break;
        case HG_IIS0_DEVID:
        case HG_IIS1_DEVID:
            ret = iis_pin_func(dev_id, request);
            break;
        case HG_PDM_DEVID:
            ret = pdm_pin_func(dev_id, request);
            break;
        case HG_LED_DEVID:
            ret = led_pin_func(dev_id, request);
            break;
        case HG_TIMER0_DEVID:
        case HG_TIMER1_DEVID:
        case HG_LED_TIMER0_DEVID:
        case HG_SUPTMR0_DEVID:
            ret = timer_pin_func(dev_id, request);
            break;
        case HG_ADC_DEVID:
        case HG_ADC1_DEVID:
            ret = adc_pin_func(dev_id, request);
            break;
        default:
            break;
    }

    user_pin_func(dev_id, request);
    sysctrl_lock();
    return ret;
}

