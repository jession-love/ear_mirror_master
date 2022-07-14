/**
  ******************************************************************************
  * @file    pmu.h
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
  * V1.0.0  2021.01.14  First Release, move from sysctrl
  *
  ******************************************************************************
  */

#ifndef __PMU_H__
#define __PMU_H__

#include "typesdef.h"

#ifdef __cplusplus
 extern "C" {
#endif

typedef struct
{
    __IO uint32 LVD_CON;
    __IO uint32 LVD_CON1;
    __IO uint32 LVD_CON2;
         uint32 RESERVED0;
    __IO uint32 PMUCON0;
    __IO uint32 PMUCON1;
    __IO uint32 PMUCON2;
    __IO uint32 PMUCON3;
    __IO uint32 PMUCON4;
    __IO uint32 PMUCON5;
    __IO uint32 RFCON0;
    __IO uint32 RFCON1;
    __IO uint32 PMUCON6;
    __IO uint32 PMUCON7;
    __IO uint32 PMUCON8;
	__IO uint32 PMUCON9;
	__IO uint32 PMUCON10;
	__IO uint32 PMUCON11;
         uint32 RESERVED1[(0x50-0x38-16)/4];
    __IO uint32 PNDCLR;
         uint32 RESERVED10[(0x80-0x50-4)/4];
    __IO uint32 TMRCON;
    __IO uint32 TMRPR;
    __IO uint32 TMRRPT;
    __IO uint32 TMRTRIM;
    __IO uint32 TMRPR2;
    __IO uint32 TMRRTCC;
         uint32 RESERVED2[2];
    __IO uint32 WKCON;
    __IO uint32 WKCON1;
	__IO uint32 WKCON2;
         uint32 RESERVED3[1];
    __IO uint32 SECCON;
    __IO uint32 SECDAT;
    __IO uint32 SECADDR;
    __IO uint32 SECKEY;
    __IO uint32 CMPCON;
	__IO uint32 CMPCON1;
         uint32 RESERVED4[2];
    __IO uint32 TMR0CON0;
    __IO uint32 TMR1CON0;
    __IO uint32 TMR2CON0;
    __IO uint32 TMR3CON0;
} PMU_TypeDef;
#define PMU               ((PMU_TypeDef *) PMU_BASE)

#define PMU_REG_SET_BITS(reg, bits)        do { pmu_reg_write((uint32)&reg, (reg)|(bits)); } while(0)
#define PMU_REG_CLR_BITS(reg, bits)        do { pmu_reg_write((uint32)&reg, (reg) & ~(bits)); } while(0)
#define PMU_REG_SET_VALUE(reg, bit_mask, val, bit_pos)  do { pmu_reg_write((uint32)&reg, ((reg) & ~(bit_mask)) | (((val) << (bit_pos)) & (bit_mask)) ); } while(0)
#define PMU_REG_BIT_FUN(fun_name, reg, bits)\
__STATIC_INLINE void fun_name##_en(void) { PMU_REG_SET_BITS(reg, bits); }\
__STATIC_INLINE void fun_name##_dis(void) { PMU_REG_CLR_BITS(reg, bits); }


/* lvdcon */
#define pmu_vcam_oc_int_en()        PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(31))
#define pmu_vcam_oc_int_dis()       PMU_REG_SET_BITS(PMU->LVD_CON, BIT(31))
#define PMU_VCAM_OC_PENDING        (PMU->LVD_CON & BIT(30))
#define PMU_V15_OC_PENDING         (PMU->LVD_CON & BIT(29))
#define PMU_VDD_OC_PENDING         (PMU->LVD_CON & BIT(28))
#define PMU_VDD_LV_PENDING         (PMU->LVD_CON & BIT(27))
#define PMU_VCC_LV_PENDING         (PMU->LVD_CON & BIT(26))
#define PMU_VCC1_LV_PENDING        (PMU->LVD_CON & BIT(25))
#define PMU_VCC15_LV_PENDING       (PMU->LVD_CON & BIT(24))
#define pmu_vcc1_lv_int_en()        PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(23))
#define pmu_vcc1_lv_int_dis()       PMU_REG_SET_BITS(PMU->LVD_CON, BIT(23))
#define pmu_vcc15_oc_int_en()       PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(22))
#define pmu_vcc15_oc_int_dis()      PMU_REG_SET_BITS(PMU->LVD_CON, BIT(22))
#define pmu_lvd_oe_en()             PMU_REG_SET_BITS(PMU->LVD_CON, BIT(21))
#define pmu_lvd_oe_dis()            PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(21))
#define pmu_vdd_oc_int_en()         PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(20))
#define pmu_vdd_oc_int_dis()        PMU_REG_SET_BITS(PMU->LVD_CON, BIT(20))
#define pmu_vdd_lv_reset_en()       PMU_REG_SET_BITS(PMU->LVD_CON, BIT(19))
#define pmu_vdd_lv_reset_dis()      PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(19))
#define pmu_vcc_lv_reset_en()       PMU_REG_SET_BITS(PMU->LVD_CON, BIT(18))
#define pmu_vcc_lv_reset_dis()      PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(18))
#define pmu_vcc15_lv_reset_en()     PMU_REG_SET_BITS(PMU->LVD_CON, BIT(17))
#define pmu_vcc15_lv_reset_dis()    PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(17))
enum vcc15_oc_level {
    VCC15_OC_200MA = 0,
    VCC15_OC_90MA = 0x3,
    VCC15_OC_70MA,
    VCC15_OC_60MA,
    VCC15_OC_50MA,
};
#define pmu_vcc15_oc_level_set(vcc15_oc_level)      PMU_REG_SET_VALUE(PMU->LVD_CON, 0x0001c000, vcc15_oc_level, 14)

enum vdd_oc_level {
    VDD_OC_50MA,
    VDD_OC_100MA,
    VDD_OC_150MA,
    VDD_OC_200MA,
};
#define pmu_vdd_oc_level_set(vdd_oc_level)          PMU_REG_SET_VALUE(PMU->LVD_CON, 0x00003000, vdd_oc_level, 12)

enum vcc15_lv_level {
    VCC15_LV_0V86_T_0V90,
    VCC15_LV_1V00_T_1V05,
    VCC15_LV_1V14_T_1V19,
    VCC15_LV_1V29_T_1V35,

};
#define pmu_vcc15_lv_level_set(vcc15_lv_level)      PMU_REG_SET_VALUE(PMU->LVD_CON, 0x00000c00, vcc15_lv_level, 10)

enum vdd_lv_level {
    VDD_LV_0V64_T_0V68,
    VDD_LV_0V74_T_0V78,
    VDD_LV_0V84_T_0V88,
    VDD_LV_0V84_T_0V98,
};
#define pmu_vdd_lv_level_set(vdd_lv_level)          PMU_REG_SET_VALUE(PMU->LVD_CON, 0x00000300, vdd_lv_level, 8)

enum avcc_lv_level {
    AVCC_LV_1V90_T_1V99,
    AVCC_LV_2V06_T_2V16,
    AVCC_LV_2V21_T_2V31,
    AVCC_LV_2V33_T_2V44,
    AVCC_LV_2V46_T_2V58,
    AVCC_LV_2V62_T_2V74,
    AVCC_LV_2V75_T_2V87,
    AVCC_LV_2V90_T_3V04,
};
#define pmu_avcc_lv_level_set(avcc_lv_level)        PMU_REG_SET_VALUE(PMU->LVD_CON, 0x000000e0, avcc_lv_level, 5)

enum vcc1_lv_level {
    VCC1_LV_2V54_T_2V66,
    VCC1_LV_1V46_T_1V53,
};
#define pmu_vcc1_lv_level_set(vcc1_lv_level)        PMU_REG_SET_VALUE(PMU->LVD_CON, 0x00000010, vcc1_lv_level, 4)

#define pmu_vdd_lv_detect_en()                      PMU_REG_SET_BITS(PMU->LVD_CON, BIT(3))
#define pmu_vdd_lv_detect_dis()                     PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(3))
#define pmu_avcc_lv_detect_en()                     PMU_REG_SET_BITS(PMU->LVD_CON, BIT(2))
#define pmu_avcc_lv_detect_dis()                    PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(2))
#define pmu_vcc1_lv_detect_en()                     PMU_REG_SET_BITS(PMU->LVD_CON, BIT(1))
#define pmu_vcc1_lv_detect_dis()                    PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(1))
#define pmu_vcc15_lv_detect_en()                    PMU_REG_SET_BITS(PMU->LVD_CON, BIT(0))
#define pmu_vcc15_lv_detect_dis()                   PMU_REG_CLR_BITS(PMU->LVD_CON, BIT(0))

/* lvdcon1 */
#define pmu_vcc_lv_detect_sync_en()                 PMU_REG_CLR_BITS(PMU->LVD_CON1, BIT(30))
#define pmu_vcc_lv_detect_sync_dis()                PMU_REG_SET_BITS(PMU->LVD_CON1, BIT(30))
#define pmu_vdd_lv_detect_filter_en()               PMU_REG_CLR_BITS(PMU->LVD_CON1, BIT(29))
#define pmu_vdd_lv_detect_filter_dis()              PMU_REG_SET_BITS(PMU->LVD_CON1, BIT(29))
#define pmu_vcc_lv_detect_filter_en()               PMU_REG_CLR_BITS(PMU->LVD_CON1, BIT(28))
#define pmu_vcc_lv_detect_filter_dis()              PMU_REG_SET_BITS(PMU->LVD_CON1, BIT(28))
#define pmu_vdd_oc_lv_ldetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x0FE00000, level, 21)
#define pmu_vdd_oc_lv_ldetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x0FE00000, level, 21)
#define pmu_vdd_oc_lv_hdetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x01FC000, level, 14)
#define pmu_vdd_oc_lv_hdetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x01FC000, level, 14)
#define pmu_vcc_oc_lv_ldetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x00003f80, level, 7)
#define pmu_vcc_oc_lv_ldetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x00003f80, level, 7)
#define pmu_vcc_oc_lv_hdetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x0000007f, level, 0)
#define pmu_vcc_oc_lv_hdetect_filter_set(level)     PMU_REG_SET_VALUE(PMU->LVD_CON1, 0x0000007f, level, 0)

/* lvdcon2 */
#define pmu_vcc1_lv_detect_filter_en()              PMU_REG_CLR_BITS(PMU->LVD_CON2, BIT(29))
#define pmu_vcc1_lv_detect_filter_dis()             PMU_REG_SET_BITS(PMU->LVD_CON2, BIT(29))
#define pmu_vcc15_lv_detect_filter_en()             PMU_REG_CLR_BITS(PMU->LVD_CON2, BIT(28))
#define pmu_vcc15_lv_detect_filter_dis()            PMU_REG_SET_BITS(PMU->LVD_CON2, BIT(28))
#define pmu_vcc1_oc_lv_ldetect_filter_set(level)    PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x0FE00000, level, 21)
#define pmu_vcc1_oc_lv_ldetect_filter_set(level)    PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x0FE00000, level, 21)
#define pmu_vcc1_oc_lv_hdetect_filter_set(level)    PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x001FC000, level, 14)
#define pmu_vcc1_oc_lv_hdetect_filter_set(level)    PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x001FC000, level, 14)
#define pmu_vcc15_oc_lv_ldetect_filter_set(level)   PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x00003f80, level, 7)
#define pmu_vcc15_oc_lv_ldetect_filter_set(level)   PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x00003f80, level, 7)
#define pmu_vcc15_oc_lv_hdetect_filter_set(level)   PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x0000007f, level, 0)
#define pmu_vcc15_oc_lv_hdetect_filter_set(level)   PMU_REG_SET_VALUE(PMU->LVD_CON2, 0x0000007f, level, 0)

/* CORE_PMUCON0 */
#define pmu_port_wk_auto_dis_en()                   PMU_REG_SET_BITS(PMU->PMUCON0, BIT(29))
#define pmu_port_wk_auto_dis_dis()                  PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(29))
#define pmu_mclr_soft_en()                          PMU_REG_SET_BITS(PMU->PMUCON0, BIT(28))
#define pmu_mclr_soft_dis()                         PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(28))
#define pmu_led_io_mux_en()                         PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(27))
#define pmu_led_io_mux_dis()                        PMU_REG_SET_BITS(PMU->PMUCON0, BIT(27))
enum pd_tmr1_clk_sel {
    PD_TMR1_CLK_RC128K,
    PD_TMR1_CLK_XOSC,
    PD_TMR1_CLK_RC10M,
    PD_TMR1_CLK_LX32K,
};
#define pmu_pd_tmr1_clk_sel(pd_tmr1_clk_sel)        PMU_REG_SET_VALUE(PMU->PMUCON0, 0x06000000, pd_tmr1_clk_sel, 25)

#define pmu_pd_mem2_en()                            PMU_REG_SET_BITS(PMU->PMUCON0, BIT(24))
#define pmu_pd_mem2_dis()                           PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(24))
#define pmu_fls_pg_en()                             PMU_REG_SET_BITS(PMU->PMUCON0, BIT(23))
#define pmu_fls_pg_dis()                            PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(23))
#define pmu_pd_mem32k_en()                          PMU_REG_SET_BITS(PMU->PMUCON0, BIT(22))
#define pmu_pd_mem32k_dis()                         PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(22))
#define pmu_pd_ulp_en()                             PMU_REG_SET_BITS(PMU->PMUCON0, BIT(21))
#define pmu_pd_ulp_dis()                            PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(21))
#define pmu_pd_rf_latch_en()                        PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(19))
#define pmu_pd_rf_latch_dis()                       PMU_REG_SET_BITS(PMU->PMUCON0, BIT(19))
#define pmu_pd_osc_en()                             PMU_REG_SET_BITS(PMU->PMUCON0, BIT(18))
#define pmu_pd_osc_dis()                            PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(18))
#define pmu_pd_osc_hw_trim_en()                     PMU_REG_SET_BITS(PMU->PMUCON0, BIT(17))
#define pmu_pd_osc_hw_trim_dis()                    PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(17))
enum pd_tmr_clk_sel {
    PD_TMR_CLK_RC128K,
    PD_TMR_CLK_XOSC,
    PD_TMR_CLK_RC10M,
    PD_TMR_CLK_LX32K,
};
#define pmu_pd_tmr_clk_sel(pd_tmr_clk_sel)          PMU_REG_SET_VALUE(PMU->PMUCON0, 0x00018000, pd_tmr_clk_sel, 15)
#define pmu_io_clk_en()                             PMU_REG_SET_BITS(PMU->PMUCON0, BIT(14))
#define pmu_io_clk_dis()                            PMU_REG_CLR_BITS(PMU->PMUCON0, BIT(14))
enum lpvdd_vol_level {
    LPVDD_VOL_0V74,
    LPVDD_VOL_0V81,
    LPVDD_VOL_0V88,
    LPVDD_VOL_0V95,
    LPVDD_VOL_1V02,
    LPVDD_VOL_1V09,
    LPVDD_VOL_1V16,
    LPVDD_VOL_1V23,
};
#define pmu_lpvdd_vol_set(lpvdd_vol_level)          PMU_REG_SET_VALUE(PMU->PMUCON0, 0x00000700, lpvdd_vol_level, 8)


/* CORE_PMUCON1 */
/* CORE_PMUCON2 */
/* CORE_PMUCON3 */
/* CORE_PMUCON4 */
/* CORE_PMUCON5 */
enum vdd15_vol_level {
    VDD15_VOL_1V2,
    VDD15_VOL_1V3,
    VDD15_VOL_1V4,
    VDD15_VOL_1V5,
    VDD15_VOL_1V6,
    VDD15_VOL_1V7,
    VDD15_VOL_1V8,
    VDD15_VOL_1V9,
    VDD15_VOL_2V0,
    VDD15_VOL_2V1,
    VDD15_VOL_2V2,
    VDD15_VOL_2V3,
    VDD15_VOL_2V4,
    VDD15_VOL_2V5,
};
#define pmu_vdd15_vol_set(vdd15_vol_level)          PMU_REG_SET_VALUE(PMU->PMUCON5, 0xF, vdd15_vol_level, 0)


#define pmu_hirc_10mhz_en()                         PMU_REG_SET_BITS(PMU->PMUCON5, BIT(24))
#define pmu_hirc_10mhz_dis()                        PMU_REG_CLR_BITS(PMU->PMUCON5, BIT(24))


/* CORE_PMUCON7 */
#define pmu_boot_direct_run_en()                    PMU_REG_SET_BITS(PMU->PMUCON7, BIT(0))
#define pmu_boot_direct_run_dis()                   PMU_REG_CLR_BITS(PMU->PMUCON7, BIT(0))
#define pmu_set_direct_run_addr(addr)               PMU_REG_SET_VALUE(PMU->PMUCON7, 0xFF000000, ((addr >> 12) & 0xFF), 24)
#define pmu_get_boot_code_pos()                     ((PMU->PMUCON7 & BIT(3)) ? 1 : 0)
#define pmu_get_cp_fail_pending()                   ((PMU->PMUCON7 & BIT(2)) ? 1 : 0)
#define pmu_clr_cp_fail_pending()                   PMU_REG_CLR_BITS(PMU->PMUCON7, BIT(2))
#define pmu_get_deadcode_pending()                  ((PMU->PMUCON7 & BIT(1)) ? 1 : 0)
#define pmu_clr_deadcode_pending()                  PMU_REG_CLR_BITS(PMU->PMUCON7, BIT(1))


/* CORE_PMUCON8 */
#define pmu_jtag_io_en()                            PMU_REG_SET_BITS(PMU->PMUCON8, BIT(0))
#define pmu_jtag_io_dis()                           PMU_REG_CLR_BITS(PMU->PMUCON8, BIT(0))


/* CORE_PMUCON11 */
#define pmu_vcam_pg_en()                            PMU_REG_SET_BITS(PMU->PMUCON11, BIT(16))
#define pmu_vcam_pg_dis()                           PMU_REG_CLR_BITS(PMU->PMUCON11, BIT(16))
#define pmu_vcam_oc_detect_en()                     PMU_REG_SET_BITS(PMU->PMUCON11, BIT(15))
#define pmu_vcam_oc_detect_dis()                    PMU_REG_CLR_BITS(PMU->PMUCON11, BIT(15))
#define pmu_vcam_en()                               PMU_REG_SET_BITS(PMU->PMUCON11, BIT(14))
#define pmu_vcam_dis()                              PMU_REG_CLR_BITS(PMU->PMUCON11, BIT(14))
#define pmu_vcam_discharge_en()                     PMU_REG_SET_BITS(PMU->PMUCON11, BIT(13))
#define pmu_vcam_discharge_dis()                    PMU_REG_CLR_BITS(PMU->PMUCON11, BIT(13))

enum vcam_vol_level {
    VCAM_VOL_1V5,
    VCAM_VOL_1V8,
    VCAM_VOL_2V5,
    VCAM_VOL_2V8,
};
#define pmu_set_vcam_vol(vcam_vol_level)            PMU_REG_SET_VALUE(PMU->PMUCON11, 0x00000600, vcam_vol_level, 9)


/* CORE_RFCON0 */
#define pmu_xo_cs_set(xo_cs_vdd)                    PMU_REG_SET_VALUE(PMU->RFCON0, 0x03f80000, xo_cs_vdd, 19)
#define xo_set_cs_sec(xo_cs_vdd)                    pmu_xo_cs_set(xo_cs_vdd)

#define pmu_xosc_en()                               PMU_REG_SET_BITS(PMU->RFCON0, BIT(0))
#define pmu_xosc_dis()                              PMU_REG_CLR_BITS(PMU->RFCON0, BIT(0))



struct system_reset_info {
    union {
        uint32 system_reset_pending;
        struct system_reset_pending_bits {
            uint32  srp_mclr : 1,
                    srp_soft : 1,
                    srp_wdt : 1,
                    srp_lvd : 1,
                    srp_lvd_vcam_cur_ov : 1,
                    srp_lvd_vcc15_cur_ov : 1,
                    srp_lvd_vdd_cur_ov : 1,             
                    srp_lvd_vdd_vol_low : 1,
                    
                    srp_lvd_vcc_vol_low : 1,
                    srp_lvd_vcc1_vol_low : 1,
                    srp_lvd_vcc15_vol_low : 1,

                    srp_deep_sleep : 1,
                    Reserved : 20;
        } system_reset_pending_bits_s;
    };
}  __attribute__((packed));


extern struct system_reset_info sri;


void pmu_reg_write(uint32 reg_addr, uint32 data) ;

void pmu_dcdc_open(void);
void pmu_tsensor_chan_sel_sec(uint8 chan_idx);
void pmu_pd_set_anatop_en_sec(void);
int jtag_map_set(uint8 val);

void tsensor_open(void);
void tsensor_close(void);

void lvd_irq_handler(void *data);
void sys_reset_detect(void);
void sys_reset_pending_clr(void);
void sys_reset_show(void);



#ifdef __cplusplus
}
#endif

#endif //__PMU_H__



/******************* (C) COPYRIGHT 2021 HUGE-IC *****END OF FILE****/


