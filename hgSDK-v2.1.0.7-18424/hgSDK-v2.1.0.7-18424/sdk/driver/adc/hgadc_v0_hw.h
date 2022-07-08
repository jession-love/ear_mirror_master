#ifndef _HGADC_V0_HW_H
#define _HGADC_V0_HW_H

#ifdef __cplusplus
    extern "C" {
#endif


/** @brief ADC register structure
  * @{
  */
struct hgadc_v0_hw {
    __IO uint32_t ADKEY_CON;
    __IO uint32_t RESERVED;
    __IO uint32_t ADKEY_DATA;
};


#ifdef __cplusplus
}
#endif

#endif /* _HGADC_V0_HW_H */