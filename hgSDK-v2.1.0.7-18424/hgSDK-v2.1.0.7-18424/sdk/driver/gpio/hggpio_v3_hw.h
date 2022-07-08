#ifndef _HGGPIO_V3_HW_H_
#define _HGGPIO_V3_HW_H_

#ifdef __cplusplus
extern "C" {
#endif


/* Switches for uncommon functions */
#define HGGPIO_V3_DIR_ATOMIC_EN       (1)
#define HGGPIO_V3_DRIVER_STRENGTH_EN  (1)
#define HGGPIO_V3_LOCK_EN             (1)
#define HGGPIO_V3_DEBUNCE_EN          (1)
#define HGGPIO_V3_TOGGLE_EN           (1)
#define HGGPIO_V3_SET_ATOMIC_EN       (1)
#define HGGPIO_V3_ANALOG_EN           (1)
#define HGGPIO_V3_INPUT_LAG_EN        (1)
#define HGGPIO_V3_ADC_ANALOG_INPUT_EN (1)
#define HGGPIO_V3_TK_ANALOG_INPUT_EN  (1)


/**
  * @breif huge-ic gpio register definition
  */
struct hggpio_v3_hw {
    __IO uint32_t MODE;
    __IO uint32_t OTYPE;
    __IO uint32_t OSPEEDL;
    __IO uint32_t OSPEEDH;
    __IO uint32_t PUPL;
    __IO uint32_t PUPH;
    __IO uint32_t PUDL;
    __IO uint32_t PUDH;
    __IO uint32_t IDAT;
    __IO uint32_t ODAT;
    __IO uint32_t BSR;
    __IO uint32_t LCK;
    __IO uint32_t AFRL;
    __IO uint32_t AFRH;
    __IO uint32_t TGL;
    __IO uint32_t IMK;
    __IO uint32_t HY;
    __IO uint32_t SRL;
    __IO uint32_t SRH;
    __IO uint32_t DEBEN;
    __IO uint32_t AIOEN;
    __IO uint32_t PND;
    __IO uint32_t PNDCLR;
    __IO uint32_t TRG0;
    __IO uint32_t TRG1;
    __IO uint32_t OTYPE1;
    __IO uint32_t ADC_AIOEN;
    __IO uint32_t TK_AIOEN;
    __IO uint32_t IEEN;
    __IO uint32_t IOFUNCOUTCON0;
    __IO uint32_t IOFUNCOUTCON1;
    __IO uint32_t IOFUNCOUTCON2;
    __IO uint32_t IOFUNCOUTCON3;
} ;


#ifdef __cplusplus
}
#endif

#endif /* _HGGPIO_V3_HW_H_ */

