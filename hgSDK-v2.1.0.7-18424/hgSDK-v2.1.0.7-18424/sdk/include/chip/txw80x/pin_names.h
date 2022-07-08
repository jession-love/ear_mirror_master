// Revision History
// V1.0.0  06/01/2019  First Release, copy from 4001a project
// V1.0.1  07/27/2019  change uart0 to A2/A3
// V1.0.2  02/07/2020  change PIN_SPI_CS to PIN_SPI0_CS
// V1.1.0  03/02/2020  add pa/pa-vmode pin

#ifndef _PIN_NAMES_DEF_H_
#define _PIN_NAMES_DEF_H_

//#include "sys_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/*---------------------------------------*/
/*----------UART PIN DEFINITION----------*/
/*---------------------------------------*/

/* UART0 */
#ifndef PIN_UART0_TX
#define PIN_UART0_TX PA_10
#endif

#ifndef PIN_UART0_RX
#define PIN_UART0_RX PA_11
#endif

/* UART1 */
#ifndef PIN_UART1_TX
#define PIN_UART1_TX PB_10
#endif

#ifndef PIN_UART1_RX
#define PIN_UART1_RX PC_7
#endif

/* UART4 */
#ifndef PIN_UART4_TX
#define PIN_UART4_TX PB_10
#endif

#ifndef PIN_UART4_RX
#define PIN_UART4_RX PB_11
#endif

/* UART5 */
#ifndef PIN_UART5_TX
#define PIN_UART5_TX PC_0
#endif

#ifndef PIN_UART5_RX
#define PIN_UART5_RX PC_1
#endif

/*---------------------------------------*/
/*----------IIC PIN DEFINITION-----------*/
/*---------------------------------------*/

/* IIC1 */
#ifndef PIN_IIC1_SCL
#define PIN_IIC1_SCL PA_7
#endif

#ifndef PIN_IIC1_SDA
#define PIN_IIC1_SDA PA_8
#endif

/* IIC2 */ //chengjiegai
#ifndef PIN_IIC2_SCL
#define PIN_IIC2_SCL PC_3
//#define PIN_IIC2_SCL PC_2
#endif

#ifndef PIN_IIC2_SDA
#define PIN_IIC2_SDA PC_4
//#define PIN_IIC2_SDA PC_3
#endif


/*---------------------------------------*/
/*----------IIS PIN DEFINITION-----------*/
/*---------------------------------------*/

/* IIS0 */
#ifndef PIN_IIS0_MCLK
#define PIN_IIS0_MCLK PA_8
#endif

#ifndef PIN_IIS0_BCLK
#define PIN_IIS0_BCLK PA_2
#endif

#ifndef PIN_IIS0_WCLK
#define PIN_IIS0_WCLK PA_12
#endif

#ifndef PIN_IIS0_DATA
#define PIN_IIS0_DATA PA_13
#endif

/* IIS1 */
#ifndef PIN_IIS1_MCLK
#define PIN_IIS1_MCLK PB_0
#endif

#ifndef PIN_IIS1_BCLK
#define PIN_IIS1_BCLK PB_1
#endif

#ifndef PIN_IIS1_WCLK
#define PIN_IIS1_WCLK PB_2
#endif

#ifndef PIN_IIS1_DATA
#define PIN_IIS1_DATA PB_3
#endif


/*---------------------------------------*/
/*----------PDM PIN DEFINITION-----------*/
/*---------------------------------------*/

/* PDM */
#ifndef PIN_PDM_MCLK
#define PIN_PDM_MCLK PB_0
#endif

#ifndef PIN_PDM_DATA
#define PIN_PDM_DATA PB_1
#endif


/*---------------------------------------*/
/*----------LED PIN DEFINITION-----------*/
/*---------------------------------------*/

/* LED */
#ifndef PIN_LED_SEG0
#define PIN_LED_SEG0 PA_4
#endif

#ifndef PIN_LED_SEG1
#define PIN_LED_SEG1  PA_5
#endif

#ifndef PIN_LED_SEG2
#define PIN_LED_SEG2  PA_6
#endif

#ifndef PIN_LED_SEG3
#define PIN_LED_SEG3  PA_7
#endif

#ifndef PIN_LED_SEG4
#define PIN_LED_SEG4  PA_8
#endif

#ifndef PIN_LED_SEG5
#define PIN_LED_SEG5  PA_11
#endif

#ifndef PIN_LED_SEG6
#define PIN_LED_SEG6  PA_13
#endif

#ifndef PIN_LED_SEG7
#define PIN_LED_SEG7  PA_14
#endif

#ifndef PIN_LED_SEG8
#define PIN_LED_SEG8  PA_10
#endif

#ifndef PIN_LED_SEG9
#define PIN_LED_SEG9  PA_9
#endif

#ifndef PIN_LED_SEG10 
#define PIN_LED_SEG10 PA_12
#endif

#ifndef PIN_LED_SEG11
#define PIN_LED_SEG11 PA_15
#endif

#ifndef PIN_LED_COM0
#define PIN_LED_COM0 PA_3
#endif

#ifndef PIN_LED_COM1
#define PIN_LED_COM1 PA_2
#endif

#ifndef PIN_LED_COM2
#define PIN_LED_COM2 PA_1
#endif

#ifndef PIN_LED_COM3
#define PIN_LED_COM3 PA_0
#endif

#ifndef PIN_LED_COM4
#define PIN_LED_COM4 PC_2
#endif

#ifndef PIN_LED_COM5
#define PIN_LED_COM5 PC_3
#endif

#ifndef PIN_LED_COM6
#define PIN_LED_COM6 PC_4
#endif

#ifndef PIN_LED_COM7
#define PIN_LED_COM7 PC_5
#endif


/*---------------------------------------*/
/*---------GMAC PIN DEFINITION-----------*/
/*---------------------------------------*/

/* GMAC */

#ifndef PIN_GMAC_RMII_REF_CLKIN
#define PIN_GMAC_RMII_REF_CLKIN PB_14
#endif

#ifndef PIN_GMAC_RMII_RXD0
#define PIN_GMAC_RMII_RXD0      PB_12
#endif

#ifndef PIN_GMAC_RMII_RXD1
#define PIN_GMAC_RMII_RXD1      PB_13
#endif

#ifndef PIN_GMAC_RMII_TXD0
#define PIN_GMAC_RMII_TXD0      PB_15
#endif

#ifndef PIN_GMAC_RMII_TXD1
#define PIN_GMAC_RMII_TXD1      PC_0
#endif

#ifndef PIN_GMAC_RMII_CRS_DV
#define PIN_GMAC_RMII_CRS_DV    PC_2
#endif

#ifndef PIN_GMAC_RMII_TX_EN
#define PIN_GMAC_RMII_TX_EN     PC_1
#endif

#ifndef PIN_GMAC_RMII_MDIO
#define PIN_GMAC_RMII_MDIO      PC_4
#endif

#ifndef PIN_GMAC_RMII_MDC
#define PIN_GMAC_RMII_MDC       PC_5
#endif



/*---------------------------------------*/
/*---------SDIO PIN DEFINITION-----------*/
/*---------------------------------------*/

/* SDIO */
#ifndef PIN_SDCLK
#define PIN_SDCLK  PB_9
#endif

#ifndef PIN_SDCMD
#define PIN_SDCMD  PB_8
#endif

#ifndef PIN_SDDAT0
#define PIN_SDDAT0 PB_10
#endif

#ifndef PIN_SDDAT1
#define PIN_SDDAT1 PB_11
#endif

#ifndef PIN_SDDAT2
#define PIN_SDDAT2 PB_6
#endif

#ifndef PIN_SDDAT3
#define PIN_SDDAT3 PB_7
#endif


/*---------------------------------------*/
/*---------QSPI PIN DEFINITION-----------*/
/*---------------------------------------*/

/* QSPI */
#ifndef PIN_QSPI_CS
#define PIN_QSPI_CS  PB_4
#endif

#ifndef PIN_QSPI_CLK
#define PIN_QSPI_CLK PB_1
#endif

#ifndef PIN_QSPI_IO0
#define PIN_QSPI_IO0 PB_0
#endif

#ifndef PIN_QSPI_IO1
#define PIN_QSPI_IO1 PB_5
#endif

#ifndef PIN_QSPI_IO2
#define PIN_QSPI_IO2 PB_3
#endif

#ifndef PIN_QSPI_IO3
#define PIN_QSPI_IO3 PB_2
#endif

#ifndef PIN_QSPI_CS1
#define PIN_QSPI_CS1 PB_6
#endif


/*---------------------------------------*/
/*---------SPI PIN DEFINITION------------*/
/*---------------------------------------*/

/* SPI0 */
#ifndef PIN_SPI0_CS
#define PIN_SPI0_CS  PA_0
#endif

#ifndef PIN_SPI0_CLK
#define PIN_SPI0_CLK PA_1
#endif

#ifndef PIN_SPI0_IO0
#define PIN_SPI0_IO0 PA_2
#endif

#ifndef PIN_SPI0_IO1
#define PIN_SPI0_IO1 PA_3
#endif

#ifndef PIN_SPI0_IO2
#define PIN_SPI0_IO2 PA_4
#endif

#ifndef PIN_SPI0_IO3
#define PIN_SPI0_IO3 PA_5
#endif


/* SPI1 */
#ifndef PIN_SPI1_CS
#define PIN_SPI1_CS  PB_0
#endif

#ifndef PIN_SPI1_CLK
#define PIN_SPI1_CLK PB_1
#endif

#ifndef PIN_SPI1_IO0
#define PIN_SPI1_IO0 PB_2
#endif

#ifndef PIN_SPI1_IO1
#define PIN_SPI1_IO1 PB_3
#endif

#ifndef PIN_SPI1_IO2
#define PIN_SPI1_IO2 PB_4
#endif

#ifndef PIN_SPI1_IO3
#define PIN_SPI1_IO3 PB_5
#endif

/* SPI5 */
#ifndef PIN_SPI5_CLK
#define PIN_SPI5_CLK PB_0
#endif

#ifndef PIN_SPI5_IO0
#define PIN_SPI5_IO0 PB_1
#endif

#ifndef PIN_SPI5_IO1
#define PIN_SPI5_IO1 PB_2
#endif

/* SPI6 */
#ifndef PIN_SPI6_CLK
#define PIN_SPI6_CLK PB_3
#endif

#ifndef PIN_SPI6_IO0
#define PIN_SPI6_IO0 PB_4
#endif

#ifndef PIN_SPI6_IO1
#define PIN_SPI6_IO1 PB_5
#endif


/*---------------------------------------*/
/*---------ADC PIN DEFINITION------------*/
/*---------------------------------------*/

/* ADC */
#ifndef PIN_ADC
#define PIN_ADC PC_2
#endif


/*---------------------------------------*/
/*---------DVP PIN DEFINITION------------*/
/*---------------------------------------*/

/* DVP */
#ifndef PIN_DVP_HSYNC
#define PIN_DVP_HSYNC  PB_7
#endif

#ifndef PIN_DVP_VSYNC
#define PIN_DVP_VSYNC  PB_6
#endif

#ifndef PIN_DVP_PCLK
#define PIN_DVP_PCLK   PB_12
#endif

#ifndef PIN_DVP_MCLK
#define PIN_DVP_MCLK   PB_9
#endif

#ifndef PIN_DVP_DATA0
#define PIN_DVP_DATA0  PB_14
#endif

#ifndef PIN_DVP_DATA1
#define PIN_DVP_DATA1  PC_0
#endif

#ifndef PIN_DVP_DATA2
#define PIN_DVP_DATA2  PC_1
#endif

#ifndef PIN_DVP_DATA3
#define PIN_DVP_DATA3  PB_15
#endif

#ifndef PIN_DVP_DATA4
#define PIN_DVP_DATA4  PB_13
#endif

#ifndef PIN_DVP_DATA5
#define PIN_DVP_DATA5  PB_11
#endif

#ifndef PIN_DVP_DATA6
#define PIN_DVP_DATA6  PB_10
#endif

#ifndef PIN_DVP_DATA7
#define PIN_DVP_DATA7  PB_8
#endif

#ifndef PIN_DVP_RESET
#define PIN_DVP_RESET  255
#endif

#ifndef PIN_DVP_PDN
#define PIN_DVP_PDN    255
#endif


/*---------------------------------------*/
/*---------SDH PIN DEFINITION------------*/
/*---------------------------------------*/

/* SDH */
#ifndef PIN_SDH_CLK
#define PIN_SDH_CLK  PA_2
#endif

#ifndef PIN_SDH_CMD
#define PIN_SDH_CMD  PA_3
#endif

#ifndef PIN_SDH_DAT0
#define PIN_SDH_DAT0 PA_1
#endif

#ifndef PIN_SDH_DAT1
#define PIN_SDH_DAT1 PA_0
#endif

#ifndef PIN_SDH_DAT2
#define PIN_SDH_DAT2 PA_5
#endif

#ifndef PIN_SDH_DAT3
#define PIN_SDH_DAT3 PA_4
#endif

/*---------------------------------------*/
/*---------TIMER PIN DEFINITION----------*/
/*---------------------------------------*/

/* NORMAL TIMER0 */
#ifndef PIN_PWM_CHANNEL_0
#define PIN_PWM_CHANNEL_0 PB_0
#endif

#ifndef PIN_CAPTURE_CHANNEL_0
#define PIN_CAPTURE_CHANNEL_0 PB_1
#endif

/* NORMAL TIMER1 */
#ifndef PIN_PWM_CHANNEL_1
#define PIN_PWM_CHANNEL_1 PB_2
#endif

#ifndef PIN_CAPTURE_CHANNEL_1
#define PIN_CAPTURE_CHANNEL_1 PB_3
#endif

/* LED_TIMER0 */
#ifndef PIN_PWM_CHANNEL_2
#define PIN_PWM_CHANNEL_2 PB_4
#endif

/* SUPTMR0 */
#ifndef PIN_PWM_CHANNEL_3
#define PIN_PWM_CHANNEL_3 PB_5
#endif



enum pin_name {

    PA_0  =  0,
    PA_1,
    PA_2,
    PA_3,
    PA_4,
    PA_5,
    PA_6,
    PA_7,
    PA_8,
    PA_9,
    PA_10,
    PA_11,
    PA_12,
    PA_13,
    PA_14,
    PA_15,
 
    PB_0,
    PB_1,
    PB_2,
    PB_3,
    PB_4,
    PB_5,
    PB_6,
    PB_7,
    PB_8,
    PB_9,
    PB_10,
    PB_11,
    PB_12,
    PB_13,
    PB_14,
    PB_15,

    PC_0,
    PC_1,
    PC_2,
    PC_3,
    PC_4,
    PC_5,
    PC_6,
    PC_7,
};

#ifdef __cplusplus
}
#endif
#endif
