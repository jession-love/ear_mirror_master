/**
  ******************************************************************************
  * @file    sysctrl.h
  * @author  HUGE-IC Application Team
  * @version V1.0.0
  * @date    2021.01.14
  * @brief   This file contains all the PowerDomain firmware functions.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2021 HUGE-IC</center></h2>
  *
  *
  * Revision History
  * V1.0.0  2021.01.14  First Release
  *
  ******************************************************************************
  */

// Define to prevent recursive inclusion //////////////////////////////
#ifndef __SYSCTRL_H__
#define __SYSCTRL_H__


#include "typesdef.h"
#include "pmu.h"
#include "misc.h"

#ifdef __cplusplus
extern "C" {
#endif


/** @defgroup SYSCTRL_Exported_Constants
  * @{
  */

typedef struct
{
    __IO uint32 SYS_KEY;
    __IO uint32 SYS_CON0;
    __IO uint32 SYS_CON1;
    __IO uint32 SYS_CON2;
    __IO uint32 SYS_CON3;
    __IO uint32 SYS_CON4;
    __IO uint32 SYS_CON5;
    __IO uint32 SYS_CON6;
    __IO uint32 SYS_CON7;
    __IO uint32 SYS_CON8;
    __IO uint32 SYS_CON9;
    __IO uint32 SYS_CON10;
    __IO uint32 SYS_CON11;
    __IO uint32 SYS_CON12;
    __IO uint32 SYS_CON13;
    __IO uint32 SYS_CON14;
    __IO uint32 SYS_CON15;
    __IO uint32 CLK_CON0;
    __IO uint32 CLK_CON1;
    __IO uint32 CLK_CON2;
    __IO uint32 CLK_CON3;
    __IO uint32 CLK_CON4;
    __IO uint32 CLK_CON5;
    __IO uint32 CHIP_ID;
    __IO uint32 CLK_CON6;
    __IO uint32 CLK_CON7;
    __IO uint32 SRAM0_PD_CON;
    __IO uint32 AIP_CON0;
    __IO uint32 AIP_CON1;
    __IO uint32 IO_MAP;
    __IO uint32 EFUSE_CON;
    __IO uint32 SYS_ERR0;
    __IO uint32 SYS_ERR1;
    __IO uint32 HOSC_MNT;
    __IO uint32 WK_CTRL;
    __IO uint32 LP_CTRL;
    __IO uint32 MBIST_CTRL;
    __IO uint32 MPE[6];
    __IO uint32 MPE_PND[6];
    __IO uint32 MODE_REG;
    __IO uint32 RESERVED00[8];
    __IO uint32 USB20_PHY_CFG0;
    __IO uint32 USB20_PHY_CFG1;
    __IO uint32 USB20_PHY_CFG2;
    __IO uint32 USB20_PHY_CFG3;
    __IO uint32 USB20_PHY_DBG0;
    __IO uint32 USB20_PHY_DBG1;
         uint32 RESERVED0[16];
    __IO uint32 IOFUNCINCON0;
    __IO uint32 IOFUNCINCON1;
    __IO uint32 IOFUNCINCON2;
    __IO uint32 IOFUNCINCON3;
    __IO uint32 IOFUNCINCON4;
    __IO uint32 IOFUNCINCON5;
    __IO uint32 IOFUNCINCON6;
    __IO uint32 IOFUNCINCON7;
    __IO uint32 IOFUNCINCON8;
    __IO uint32 IOFUNCINCON9;
    __IO uint32 IOFUNCINCON10;
    __IO uint32 IOFUNCINCON11;
    __IO uint32 IOFUNCINCON12;
    __IO uint32 IOFUNCINCON13;
    __IO uint32 IOFUNCINCON14;
    __IO uint32 IOFUNCINCON15;
    __IO uint32 IOFUNCMASK0;
    __IO uint32 QSPI_MAP_CTL;
    __IO uint32 QSPI_ENCDEC_CON0;
    __IO uint32 QSPI_ENCDEC_CON1;
    __IO uint32 TRNG;
    __IO uint32 DCRC_TRIM;
} SYSCTRL_TypeDef;

#define SYSCTRL                 ((SYSCTRL_TypeDef    *) HG_SYSCTRL_BASE)

#if SYS_KEY_OPT_EN
#define sysctrl_unlock()                        do { SYSCTRL->SYS_KEY = 0x3fac87e4; } while(0)
#define sysctrl_lock()                          do { SYSCTRL->SYS_KEY = 0; } while(0)
#define SYSCTRL_REG_OPT_INIT()                  do { sysctrl_lock(); } while(0)
#define SYSCTRL_REG_OPT(expr)                   do { uint32 flag=disable_irq(); sysctrl_unlock(); expr; sysctrl_lock(); enable_irq(flag); } while(0)
#define EFUSE_REG_OPT(expr)                     do { uint32 flag=disable_irq(); SYSCTRL->SYS_KEY = 0x4c5de9b3; expr; sysctrl_lock(); enable_irq(flag); } while(0)
#else
#define sysctrl_unlock()
#define sysctrl_lock()
#define SYSCTRL_REG_OPT_INIT()                  do { SYSCTRL->SYS_KEY = 0x3fac87e4; } while(0)
#define SYSCTRL_REG_OPT(expr)                   do { expr; } while(0)
#define EFUSE_REG_OPT(expr)                     do { uint32 flag=disable_irq(); SYSCTRL->SYS_KEY = 0x4c5de9b3; expr; SYSCTRL->SYS_KEY = 0x3fac87e4; enable_irq(flag); } while(0)
#endif

#define SYSCTRL_REG_BITS_S0S1(reg, bits)       do { SYSCTRL_REG_OPT( reg &= ~(bits); __NOP();__NOP();__NOP(); (reg) |= (bits); ); } while(0)
#define SYSCTRL_REG_BITS_S1S0(reg, bits)       do { SYSCTRL_REG_OPT( reg |= (bits); __NOP();__NOP();__NOP(); (reg) &= ~(bits); ); } while(0)
#define SYSCTRL_REG_SET_BITS(reg, bits)        do { SYSCTRL_REG_OPT( reg |=  (bits); ); } while(0)
#define SYSCTRL_REG_CLR_BITS(reg, bits)        do { SYSCTRL_REG_OPT( reg &= ~(bits); ); } while(0)
#define SYSCTRL_REG_SET_VALUE(reg, bit_mask, val, bit_pos)  do { SYSCTRL_REG_OPT( reg = ((reg) & ~(bit_mask)) | (((val) << (bit_pos)) & (bit_mask)); ); } while(0)
#define SYSCTRL_REG_BIT_FUN(fun_name, reg, bits)\
__STATIC_INLINE void fun_name##_en(void) { SYSCTRL_REG_SET_BITS(reg, bits); }\
__STATIC_INLINE void fun_name##_dis(void) { SYSCTRL_REG_CLR_BITS(reg, bits); }

enum iis_clk_src {
    IIS_CLK_NONE, 
    IIS_CLK_SYSPLL, 
    IIS_CLK_USBPLL, 
    IIS_CLK_XOSC, 
};
/* SYS_CON0 */
#define sysctrl_iis1_clk_sel(iis_clk_src)         SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON0, BIT(31)|BIT(30), iis_clk_src, 30) 
#define sysctrl_rssi_ck_sel_rssi_done() SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON0, BIT(29))
#define sysctrl_rssi_ck_sel_rssi_en()   SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON0, BIT(29))
#define sysctrl_rssi_sel_rssi_done()    SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON0, BIT(28))
#define sysctrl_rssi_sel_rssi_en()      SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON0, BIT(28))
#define sysctrl_dbg_seg9_sel(n)         SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON0, 0xFF00000, n, 20) 
#define sysctrl_dbg_seg8_sel(n)         SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON0, 0xFF000, n, 12)

#define sysctrl_iis0_clk_sel(iis_clk_src)         SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON0, BIT(11)|BIT(10), iis_clk_src, 10)
#define sysctrl_mac_pa_en()             SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON0, BIT(9))
#define sysctrl_mac_pa_dis()            SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON0, BIT(9))
#define sysctrl_qspi_dlychain_cfg(n)    SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON0, 0x1F0, n, 4)
#define sysctrl_qspi_clkin_dis()        SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON0, BIT(3))
#define sysctrl_qspi_clkin_en()         SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON0, BIT(3))
#define sysctrl_mjpeg_reset()           SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON0, BIT(2))
#define sysctrl_iis1_reset()            SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON0, BIT(1))
#define sysctrl_wdt_at_lp_gate_en()     SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON0, BIT(0))
#define sysctrl_wdt_at_lp_gate_dis()    SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON0, BIT(1))


/* SYS_CON1 */
#define sysctrl_qspi_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(31))
#define sysctrl_uart2_reset()            SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(30))
#define sysctrl_usb20sie_reset()         SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(29))
#define sysctrl_dvp_reset()              SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(28))
#define sysctrl_osc_calib_reset()        SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(27))
#define sysctrl_dbg_dma_reset()          SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(26))
#define sysctrl_rf_dig_calib_reset()     SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(25))
#define sysctrl_adc_reset()              SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(24))
#define sysctrl_modem_reset()            SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(22))
#define sysctrl_mac_reset()              SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(21))
#define sysctrl_sdhc_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(20))
#define sysctrl_sdio_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(19))
#define sysctrl_usb20_phy_reset()        SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(18))
#define sysctrl_pdm_reset()              SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(17))
#define sysctrl_gmac_sys_rst()           SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(16))
#define sysctrl_sysaes_reset()           SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(15))
#define sysctrl_crc_reset()              SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(14))
#define sysctrl_iis0_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(13))
#define sysctrl_timer_reset()            SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(12))
#define sysctrl_uart1_reset()            SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(11))
#define sysctrl_uart0_reset()            SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(10))
#define sysctrl_tk_reset()               SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(8))
#define sysctrl_spi2_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(7))
#define sysctrl_spi1_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(6))
#define sysctrl_spi0_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(5))
#define sysctrl_wdt_reset()              SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, (BIT(4)|BIT(3)))
#define sysctrl_m2m_dma_reset()          SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(2))
#define sysctrl_memory_reset()           SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(1))
#define sysctrl_gpio_reset()             SYSCTRL_REG_BITS_S0S1(SYSCTRL->SYS_CON1, BIT(0))


/* SYS_CON2 */
#define sysctrl_pdm_mclk_div(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON2, 0x7F00000, n, 20) /* N > 1 */
enum pdm_clk_src {
    PDM_CLK_NONE, 
    PDM_CLK_SYSPLL, 
    PDM_CLK_USBPLL, 
    PDM_CLK_XOSC, 
};
#define sysctrl_pdm_clk_sel(pdm_clk_src)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON2, BIT(19)|BIT(18), pdm_clk_src, 18) 
#define sysctrl_xosc_loss_nmi_dis()    SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(17))
#define sysctrl_xosc_loss_nmi_en()     SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(17))
#define sysctrl_xosc_loss_swrc_dis()   SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(16))
#define sysctrl_xosc_loss_swrc_en()    SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(16))
#define sysctrl_sdio_cmd_wk_en()       SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(10))
#define sysctrl_sdio_cmd_wk_dis()      SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(10))
#define sysctrl_cpu_clk_en()           SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(9))
#define sysctrl_cpu_clk_dis()          SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(9))
#define sysctrl_gmac_dma_disable()     SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(7))
#define sysctrl_gmac_dma_enable()      SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(7))
#define sysctrl_syserr_int_dis()       SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(6))
#define sysctrl_syserr_int_en()        SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(6))
#define sysctrl_apb1_wr_optimize_dis() SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(5)) /* sfr write optimze from 3 to 2 cycle */
#define sysctrl_apb1_wr_optimize_en()  SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(5))
#define sysctrl_apb0_wr_optimize_dis() SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(4))
#define sysctrl_apb0_wr_optimize_en()  SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(4))
#define sysctrl_nmi_port_dis()         SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(3)) /* PA4 */
#define sysctrl_nmi_port_en()          SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(3))
#define sysctrl_iis_duplex_dis()       SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON2, BIT(0))
#define sysctrl_iis_duplex_en()        SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON2, BIT(0))

/* SYS_CON3 */
#define sysctrl_gmac_dbc_pll_sel(n)    SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(31), n, 31) 
#define sysctrl_dvp_dbc_pll_sel(n)     SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(30), n, 30) 
#define sysctrl_iis1_mclk_sel(n)       SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(29), n, 29) 
#define sysctrl_iis0_mclk_sel(n)       SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(28), n, 28) 
#define sysctrl_iis01_mclk_sel(n)      SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(27), n, 27) 
enum gpio_dbc_clk_src {
    GPIO_DEBUNCE_CLK_RC128K, 
    GPIO_DEBUNCE_CLK_XOSC, 
    GPIO_DEBUNCE_CLK_FCLK, 
    GPIO_DEBUNCE_CLK_RC32K,
};
#define sysctrl_gpioc_dbc_clk_sel(gpio_dbc_clk_src)   SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(25)|BIT(24), gpio_dbc_clk_src, 24) 
#define sysctrl_gpiob_dbc_clk_sel(gpio_dbc_clk_src)   SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(23)|BIT(22), gpio_dbc_clk_src, 22) 
#define sysctrl_gpioa_dbc_clk_sel(gpio_dbc_clk_src)   SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(21)|BIT(20), gpio_dbc_clk_src, 20) 
#define sysctrl_trng_clk_sel(n)                       SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON3, BIT(19), n, 19) 
#define sysctrl_err_rsp_sdio_bus_en()                 SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(18))
#define sysctrl_err_rsp_sdio_bus_dis()                SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(18))


#define sysctrl_tk_clk_en()                           SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(17))
#define sysctrl_tk_clk_dis()                          SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(17))
#define sysctrl_err_rsp_dbus_en()                     SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(16))
#define sysctrl_err_rsp_dbus_dis()                    SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(16))
#define sysctrl_err_rsp_ibus_en()                     SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(15))
#define sysctrl_err_rsp_ibus_dis()                    SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(15))
#define sysctrl_sdio_mjpeg_fifo_for_mjpeg()           SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(14))
#define sysctrl_sdio_mjpeg_fifo_for_sdio()            SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(14))
#define sysctrl_sdio_mjpeg_fifo_for_mjpeg()           SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(14))
#define sysctrl_pll_src_upll_div3()                   SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(12))
#define sysctrl_pll_src_upll_div4()                   SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(12))
#define sysctrl_pll_src_spll_div2()                   SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(11))
#define sysctrl_pll_src_spll_div3()                   SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(11))
#define sysctrl_bb_clk_en()                           SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(5))
#define sysctrl_bb_clk_dis()                          SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(5))
#define sysctrl_usb_wk_en()                           SYSCTRL_REG_SET_BITS(SYSCTRL->SYS_CON3, BIT(4))
#define sysctrl_usb_wk_dis()                          SYSCTRL_REG_CLR_BITS(SYSCTRL->SYS_CON3, BIT(4))

/* SYS_CON6 */
#define sysctrl_dbg_seg3_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON6, 0xFF000000, n, 24)
#define sysctrl_dbg_seg2_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON6, 0x00FF0000, n, 16)
#define sysctrl_dbg_seg1_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON6, 0x0000FF00, n, 8)
#define sysctrl_dbg_seg0_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON6, 0x000000FF, n, 0)

/* SYS_CON7 */
#define sysctrl_dbg_seg7_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON7, 0xFF000000, n, 24)
#define sysctrl_dbg_seg6_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON7, 0x00FF0000, n, 16)
#define sysctrl_dbg_seg5_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON7, 0x0000FF00, n, 8)
#define sysctrl_dbg_seg4_sel(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->SYS_CON7, 0x000000FF, n, 0)

/* CLK_CON0 */


/* CLK_CON1 */
#define sysctrl_rf_dac_clk_n_posedge() SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON1, BIT(31))
#define sysctrl_rf_dac_clk_n_negedge() SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON1, BIT(31))
#define sysctrl_rf_adc_clk_n_posedge() SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON1, BIT(30))
#define sysctrl_rf_adc_clk_n_negedge() SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON1, BIT(30))
#define sysctrl_rf_adc_sample_clk_n_posedge() SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON1, BIT(29))
#define sysctrl_rf_adc_sample_clk_n_negedge() SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON1, BIT(29))
 enum dvp_pll_src {
     DVP_PLL_SRC_XOSC, 
     DVP_PLL_SRC_SPLL, 
     DVP_PLL_SRC_UPLL,
 };
#define sysctrl_dvp_pll_sel(dvp_pll_src) SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, BIT(28)|BIT(27), dvp_pll_src, 27)
#define sysctrl_qspi_pll_sel(n)          SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, BIT(26), n, 26)
#define sysctrl_qspi_clk_sel_spll()               do {  SYSCTRL_REG_OPT( SYSCTRL->SYS_CON0 &= ~(BIT(3));\
                                                         SYSCTRL->SYS_CON1 &= ~(BIT(26)); );\
                                                  } while(0)
#define sysctrl_qspi_clk_sel_upll()               do {  SYSCTRL_REG_OPT( SYSCTRL->SYS_CON0 &= ~(BIT(3));\
                                                         SYSCTRL->SYS_CON1 |= (BIT(26)); );\
                                                  } while(0)
#define sysctrl_qspi_clk_sel_hxosc()              do {  SYSCTRL_REG_OPT( SYSCTRL->SYS_CON0 |= (BIT(3));\
                                                         SYSCTRL->SYS_CON1 &= ~(BIT(26)); );\
                                                  } while(0)
#define sysctrl_qspi_clk_sel_rc10m()              do {  SYSCTRL_REG_OPT( SYSCTRL->SYS_CON0 |= (BIT(3));\
                                                         SYSCTRL->SYS_CON1 |= (BIT(26)); );\
                                                  } while(0)
 enum qspi_clk_src {
     QSPI_CLK_SYSPLL, 
     QSPI_CLK_USBPLL, 
     QSPI_CLK_XOSC, 
     QSPI_CLK_RC10M,
 };
#define sysctrl_qspi_clk_src_sel(qspi_clk_src)\
do {\
    if (qspi_clk_src & BIT(1))    SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON0, BIT(3));\
    else                          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON0, BIT(3));\
    if (qspi_clk_src & BIT(0))    SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON1, BIT(26));\
    else                          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON1, BIT(26));\
} while (0)

 enum rfadc_pll_sel {
     RF_ADC_PLL_80MHZ, 
     RF_ADC_PLL_40MHZ = 2,
     RF_ADC_PLL_20MHZ = 3,
 };
#define sysctrl_adda_cnt_pr(rfadc_pll_sel)   SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, BIT(25)|BIT(24), rfadc_pll_sel, 24)
#define sysctrl_apb1_clk_div(n)              SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, 0x00FF0000, n, 16)
#define sysctrl_apb0_clk_div(n)              SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, 0x0000FF00, n, 8)
#define sysctrl_mac_clk_sel(n)               SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, 0x000000C0, n, 6)
#define sysctrl_sys_clk_div(n)               SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON1, 0x0000003F, n, 0)

/* CKL_CON2 */
#define sysctrl_dvp_clk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(31))
#define sysctrl_dvp_clk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(31))
#define sysctrl_qspi_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(30))
#define sysctrl_qspi_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(30))
#define sysctrl_tmr0_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(29))
#define sysctrl_tmr0_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(29))
#define sysctrl_test_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(28))
#define sysctrl_test_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(28))
#define sysctrl_stmr_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(27))
#define sysctrl_stmr_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(27))
#define sysctrl_tmr3_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(26))
#define sysctrl_tmr3_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(26))
#define sysctrl_rfdac_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(25))
#define sysctrl_rfdac_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(25))
#define sysctrl_rfadda_clk_open()          SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(24))
#define sysctrl_rfadad_clk_close()         SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(24))
#define sysctrl_rf_pclk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(23))
#define sysctrl_rf_pclk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(23))
#define sysctrl_modem_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(22))
#define sysctrl_modem_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(22))
#define sysctrl_mac_clk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(21))
#define sysctrl_mac_clk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(21))
#define sysctrl_sdhc_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(20))
#define sysctrl_sdhc_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(20))
#define sysctrl_sddev_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(19))
#define sysctrl_sddev_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(19))
#define sysctrl_usb20_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(18))
#define sysctrl_usb20_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(18))
#define sysctrl_pdm_clk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(17))
#define sysctrl_pdm_clk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(17))
#define sysctrl_simtmr_clk_open()          SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(16))
#define sysctrl_simtmr_clk_close()         SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(16))
#define sysctrl_sysaes_clk_open()          SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(15))
#define sysctrl_sysaes_clk_close()         SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(15))
#define sysctrl_crc_clk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(14))
#define sysctrl_crc_clk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(14))
#define sysctrl_iis0_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(13))
#define sysctrl_iis0_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(13))
#define sysctrl_tmr1_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(12))
#define sysctrl_tmr1_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(12))
#define sysctrl_uart1_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(11))
#define sysctrl_uart1_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(11))
#define sysctrl_uart0_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(10))
#define sysctrl_uart0_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(10))
#define sysctrl_tmr2_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(8))
#define sysctrl_tmr2_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(8))
#define sysctrl_spi2_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(7))
#define sysctrl_spi2_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(7))
#define sysctrl_spi1_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(6))
#define sysctrl_spi1_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(6))
#define sysctrl_spi0_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(5))
#define sysctrl_spi0_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(5))
#define sysctrl_wdt_clk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(3))
#define sysctrl_wdt_clk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(3))
#define sysctrl_ahb1_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(2))
#define sysctrl_ahb1_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(2))
#define sysctrl_ahb0_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON2, BIT(1))
#define sysctrl_ahb0_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON2, BIT(1))


/* CLK_CON3 */
#define sysctrl_dvp_mclk_div(n)            SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON3, 0xFC000000, n, 26)
#define sysctrl_cache_en(n)                SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(25))
#define sysctrl_cache_dis(n)               SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(25))
#define sysctrl_lmac_fifo_en(n)            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(24))
#define sysctrl_lmac_fifo_dis(n)           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(24))
#define sysctrl_lvd_dbc_clk_en(n)          SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(23))
#define sysctrl_lvd_dbc_clk_dis(n)         SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(23))
enum lvd_dbc_clk_src {
    LVD_DEBUNCE_CLK_RC128K, 
    LVD_DEBUNCE_CLK_XOSC, 
    LVD_DEBUNCE_CLK_FCLK, 
    LVD_DEBUNCE_CLK_RC32K,
};
#define sysctrl_lvd_dbg_clk_sel(lvd_dbc_clk_src)   SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON3, BIT(22)|BIT(21), lvd_dbc_clk_src, 21) 
#define sysctrl_sysmonitor_clk_open()      SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(20))
#define sysctrl_sysmonitor_clk_close()     SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(20))
#define sysctrl_m2mdma_clk_open()          SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(19))
#define sysctrl_m2mdma_clk_close()         SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(19))
#define sysctrl_iis1_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(18))
#define sysctrl_iis1_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(18))
#define sysctrl_gmac_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(17))
#define sysctrl_gmac_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(17))
#define sysctrl_rom_clk_open()             SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(16))
#define sysctrl_rom_clk_close()            SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(16))
#define sysctrl_apb1_clk_open()            SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(15))
#define sysctrl_apb1_clk_close()           SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(15))
#define sysctrl_mjpeg_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(14))
#define sysctrl_mjpeg_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(14))
#define sysctrl_qspi_pll_div(n)            SYSCTRL_REG_SET_VALUE(SYSCTRL->CLK_CON3, BIT(13)|BIT(12)|BIT(11), n, 11)
#define sysctrl_sram10_clk_open()          SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(10))
#define sysctrl_sram10_clk_close()         SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(10))
#define sysctrl_sram9_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(9))
#define sysctrl_sram9_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(9))
#define sysctrl_sram8_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(8))
#define sysctrl_sram8_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(8))
#define sysctrl_sram7_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(7))
#define sysctrl_sram7_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(7))
#define sysctrl_sram6_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(6))
#define sysctrl_sram6_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(6))
#define sysctrl_sram5_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(5))
#define sysctrl_sram5_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(5))
#define sysctrl_sram4_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(4))
#define sysctrl_sram4_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(4))
#define sysctrl_sram3_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(3))
#define sysctrl_sram3_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(3))
#define sysctrl_sram2_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(2))
#define sysctrl_sram2_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(2))
#define sysctrl_sram1_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(1))
#define sysctrl_sram1_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(1))
#define sysctrl_sram0_clk_open()           SYSCTRL_REG_SET_BITS(SYSCTRL->CLK_CON3, BIT(0))
#define sysctrl_sram0_clk_close()          SYSCTRL_REG_CLR_BITS(SYSCTRL->CLK_CON3, BIT(0))

/* CHIP_ID */
#define sysctrl_get_chip_dcn()             ((uint16)(SYSCTRL->CHIP_ID >> 16))

/* SRAM0_PD_CON */
#define sysctrl_sram0_pd_exit_cnt(n)       SYSCTRL_REG_SET_VALUE(SYSCTRL->SRAM0_PD_CON, 0x001FF800, n, 11)
#define sysctrl_sram0_pd_enter_cnt(n)      SYSCTRL_REG_SET_VALUE(SYSCTRL->SRAM0_PD_CON, 0x000007Fe, n, 1)
#define sysctrl_sram0_hw_pd_en()           SYSCTRL_REG_SET_BITS(SYSCTRL->SRAM0_PD_CON, BIT(0))
#define sysctrl_sram0_hw_pd_dis()          SYSCTRL_REG_CLR_BITS(SYSCTRL->SRAM0_PD_CON, BIT(0))

/* IO_MAP */

/* EFUSE_CON */
#define sysctrl_efuse_pwron_init()         SYSCTRL_REG_SET_BITS(SYSCTRL->EFUSE_CON, BIT(1)|BIT(0))

/* SYS_ERR0 */

/* SYS_ERR1 */

/* HOSC_MNT */
#define sysctrl_hosc_mnt_high_limit(n)      SYSCTRL_REG_SET_VALUE(SYSCTRL->HOSC_MNT, 0xFFFF0000, n, 16)
#define sysctrl_hosc_mnt_en()               SYSCTRL_REG_SET_BITS(SYSCTRL->HOSC_MNT, BIT(15))
#define sysctrl_hosc_mnt_dis()              SYSCTRL_REG_CLR_BITS(SYSCTRL->HOSC_MNT, BIT(15))
#define sysctrl_hosc_lost_pend_clr()        SYSCTRL_REG_SET_BITS(SYSCTRL->HOSC_MNT, BIT(14))
#define sysctrl_hosc_mnt_low_limit(n)       SYSCTRL_REG_SET_VALUE(SYSCTRL->HOSC_MNT, 0x00003FFF, n, 0)

/* WK_CTRL */
/* LP_CTRL */

/* MPE0~5 */
void sysctrl_memory_protect_config(uint32 start_addr, uint32 size, uint8 enable);
#define sysctrl_get_mem_protect_pending()   ((SYSCTRL->MPE_PND[0] | SYSCTRL->MPE_PND[1] | SYSCTRL->MPE_PND[2]))

/* MODE_REG */
#define sysctrl_get_buck_cmpfb_sta()        ((SYSCTRL->MODE_REG & BIT(31)))
#define sysctrl_get_buck_dtmod_index_sta()  ((SYSCTRL->MODE_REG & BIT(30)))
#define sysctrl_get_buck_ipeaktrimout_sta() ((SYSCTRL->MODE_REG & BIT(29)))
#define sysctrl_get_buck_irptrimout_sta()   ((SYSCTRL->MODE_REG & BIT(28)))
#define sysctrl_get_ft_mode()               ((SYSCTRL->MODE_REG & BIT(1)))
#define sysctrl_get_cp_mode()               ((SYSCTRL->MODE_REG & BIT(0)))


/* TRNG */
#define sysctrl_get_trng()                  ((SYSCTRL->TRNG))

/* DCRC_TRIM */
#define sysctrl_set_dc_trim_init_val(n)     SYSCTRL_REG_SET_VALUE(SYSCTRL->DCRC_TRIM, 0x00fc0000, n, 18)
#define sysctrl_set_dc_trim_range(n)        SYSCTRL_REG_SET_VALUE(SYSCTRL->DCRC_TRIM, 0x0003c000, n, 14)
#define sysctrl_set_dc_trim_step(n)         SYSCTRL_REG_SET_VALUE(SYSCTRL->DCRC_TRIM, 0x00003e00, n, 9)
#define sysctrl_set_dc_trim_1us_prd(n)      SYSCTRL_REG_SET_VALUE(SYSCTRL->DCRC_TRIM, 0x000001fe, n, 1)
#define sysctrl_dc_trim_en()                SYSCTRL_REG_SET_BITS(SYSCTRL->DCRC_TRIM, BIT(0))
#define sysctrl_dc_trim_dis()               SYSCTRL_REG_CLR_BITS(SYSCTRL->DCRC_TRIM, BIT(0))




/**
  * @}
  */



int32 sys_get_apbclk(void);
int32 sys_get_sysclk(void);
int32 sys_set_sysclk(uint32 system_clk);

void mcu_reset(void);
void mcu_disable_watchdog(void);

void system_enter_sleep(void);
void system_exit_sleep(void);

void sysctrl_cmu_init(void);
bool sysctrl_cmu_upll_init(void);
bool sysctrl_cmu_sysclk_set(uint32 clk, bool is_upll);
#define sysctrl_gmac_clk_sel_clk_from_io()


/**
 * @brief   system_clock_set
 * @param   clk_hz : clk wish to be set
 * @retval  OK or ERR
 * @note    
 */
int32 system_clock_set(uint32 clk_hz);
/**
* @brief   system_clock_get
* @retval  system clk_hz
* @note    this function will get the clock form sotfware temp storage
*/
uint32 system_clock_get(void);

/**
 * @brief   system_clock_get_refresh
 * @retval  system clk_hz
 * @note    this function will get the clock form hardware
 */
uint32 system_clock_get_refresh(void);

/**
 * @brief   peripheral_clock_set
 * @param   peripheral  : HG_Peripheral_Type  
 * @param   clk_hz      : Peripheral clk_hz
 * @retval  ok or err
 * @note    peripheral on same bus apb0/1 clk must be same
 */
int32 peripheral_clock_set(HG_Peripheral_Type peripheral, uint32 clk_hz);

/**
 * @brief   peripheral_clock_get
 * @param   peripheral  : HG_Peripheral_Type  
 * @retval  clk_hz
 */
uint32 peripheral_clock_get(HG_Peripheral_Type peripheral);

uint16 sysctrl_get_chip_id(void);

uint32 sysctrl_efuse_validity_get(void);

void sysctrl_efuse_validity_handle(void);

uint32 sysctrl_efuse_vddi_get(void);

uint32 sysctrl_efuse_tsensor_get(void);

uint32 sysctrl_efuse_vddi_gears_get(void);



#ifdef __cplusplus
}
#endif

#endif //__SYSCTRL_H__

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2019 HUGE-IC *****END OF FILE****/
