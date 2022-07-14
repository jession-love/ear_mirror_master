#ifndef _HGI2C_V0_HW_H
#define _HGI2C_V0_HW_H

#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup IIC MODULE REGISTER
  * @{
  */

/***** CON0(for IIC) Register *****/
/*! IIC SCL strong output enable
 */
#define LL_IIC_CON0_STRONG_DRV_EN                       (1UL << 23)
/*! IIC received NACK signal interrupt enable
 */
#define LL_IIC_CON0_RX_NACK_IE_EN                       (1UL << 22)
/*! IIC arbitration loss interrupt enable
 */
#define LL_IIC_CON0_AL_IE_EN                            (1UL << 21)
/*! IIC received STOP signal interrupt enable
 */
#define LL_IIC_CON0_STOP_IE_EN                          (1UL << 20)
/*! IIC slave address match interrupt enable
 */
#define LL_IIC_CON0_ADR_MATCH_IE_EN                     (1UL << 19)
/*! IIC sampling filter settings
 */
#define LL_IIC_CON0_FILTER_CNT(n)                       (((n)&0x1F) << 14)
/*! IIC slave broadcast interrupt enable
 */
#define LL_IIC_CON0_BROADCAST_IE_EN                     (1UL << 13)
/*! IIC slave broadcast enable
 */
#define LL_IIC_CON0_BROADCAST_EN                        (1UL << 12)
/*! IIC slave address setting
 */
#define LL_IIC_CON0_SLAVE_ADR(n)                        (((n)&0x03FF) << 2)
/*! The IIC responds to the NACK signal after receiving the data
 */
#define LL_IIC_CON0_TX_NACK                             (1UL << 1)
/*! IIC slave address bit width
 */
#define LL_IIC_CON0_SLAVE_ADR_WIDTH(n)                  (((n)&0x01) << 0)


/***** CON1(for IIC) Register *****/
/*! IIC DMA interrupt enable
 */
#define LL_IIC_CON1_DMA_IE_EN                           (1UL << 9)
/*! IIC FIFO overflow interrupt enable
 */
#define LL_IIC_CON1_BUF_OV_IE_EN                        (1UL << 8)
/*! IIC RX FIFO not empty interrupt enable
 */
#define LL_IIC_CON1_RX_BUF_NOT_EMPTY_IE_EN              (1UL << 7)
/*! IIC TX FIFO not full interrupt enable
 */
#define LL_IIC_CON1_TX_BUF_NOT_FULL_IE_EN               (1UL << 6)
/*! IIC transfers one frame interrupt enable
 */
#define LL_IIC_CON1_SSP_IE_EN                           (1UL << 5)
/*! IIC DMA enable
 */
#define LL_IIC_CON1_DMA_EN                              (1UL << 4)
/*! The IIC is set to the TX direction
 */
#define LL_IIC_CON1_TX_EN                               (1UL << 3)
/*! IIC working mode
 */
#define LL_IIC_CON1_MODE(n)                             (((n)&0x01) << 2)
/*! SPI/IIC selection
 */
#define LL_IIC_CON1_SPI_I2C_SEL(n)                      (((n)&0x01) << 1)
/*! SPI/IIC module enable
 */
#define LL_IIC_CON1_SSP_EN                              (1UL << 0)


/***** SSP CMD DATA(for IIC) Register *****/
/*! IIC master transfers START signal
 */
#define LL_IIC_CMD_DATA_START_BIT_EN                    (1UL << 8)
/*! IIC master transfers STOP signal
 */
#define LL_IIC_CMD_DATA_STOP_BIT_EN                     (1UL << 9)
/*! Write data 
 */
#define LL_IIC_CMD_DATA_WRITE(n)                        (((n)&0xFF) << 0)
/*! Read data 
 */
#define LL_IIC_CMD_DATA_READ(n)                         (((n)>>0) & 0xFF)


/***** BAUD(for IIC) Register *****/
/*! Set baud rate(16bit)
 */
#define LL_IIC_BAUD(n)                                  (n)


/***** DMA Tx LEN(for IIC) Register *****/
/*! Set DMA Tx length(12bit)
 */
#define LL_IIC_DMA_TX_LEN(n)                            (n)


/***** DMA Tx CNT(for IIC) Register *****/
/*! The length of the byte of the received data(12bit)
 */
#define LL_IIC_DMA_TX_CNT(n)                            (n)


/***** DMA Tx STADR(for IIC) Register *****/
/*! DMA Tx start address(13bit)
 */
#define LL_IIC_DMA_TX_STADR(n)                          (n)


/***** DMA Rx LEN(for IIC) Register *****/
/*! Set DMA Rx length(12bit)
 */
#define LL_IIC_DMA_RX_LEN(n)                            (n)


/***** DMA Rx CNT(for IIC) Register *****/
/*! The length of the byte of the received data(12bit)
 */
#define LL_IIC_DMA_RX_CNT(n)                            (n)


/***** DMA Rx STADR(for IIC) Register *****/
/*! DMA Rx start address(13bit)
 */
#define LL_IIC_DMA_RX_STADR(n)                          (n)


/***** STA(for IIC) Register *****/
/*! IIC slave addressed pending
 */
#define LL_IIC_STA_SLAVE_ADDRED_PENDING                 (1UL << 27)
/*! IIC state machine
 */
#define LL_IIC_STA_STATE(n)                             (((n)>>24) & 0x07)
/*! Clear IIC FIFO
 */
#define LL_IIC_STA_CLEAR_BUF_CNT                        (1UL << 19)
/*! Get how many bytes of valid data in the FIFO
 */
#define LL_IIC_STA_BUF_CNT(n)                           (((n)>>16) & 0x07)
/*! IIC master receive data busy pending
 */
#define LL_IIC_STA_MASTER_RX_BUSY_PENDING               (1UL << 15)
/*! IIC NACK signal detected pending
 */
#define LL_IIC_STA_RX_NACK_PENDING                      (1UL << 14)
/*! IIC slave receives read request pending
 */
#define LL_IIC_STA_SLAVE_READ_PENDING                   (1UL << 13)
/*! IIC bus busy pending
 */
#define LL_IIC_STA_BUS_BUSY_PENDING                     (1UL << 12)
/*! IIC module busy pending
 */
#define LL_IIC_STA_SSP_BUSY_PENDING                     (1UL << 10)
/*! IIC master arbitration loss detected pending
 */
#define LL_IIC_STA_AL_PENDING                           (1UL << 9)
/*! IIC STOP signal detected pending
 */
#define LL_IIC_STA_STOP_PENDING                         (1UL << 8)
/*! IIC slave address matched pending
 */
#define LL_IIC_STA_ADDR_MATCH_PENDING                   (1UL << 7)
/*! IIC slave detected broadcast address pending
 */
#define LL_IIC_STA_BROADCAST_PENDING                    (1UL << 6)
/*! IIC DMA done pending
 */
#define LL_IIC_STA_DMA_PENDING                          (1UL << 4)
/*! IIC FIFO overflow pending
 */
#define LL_IIC_STA_BUF_OV_PENDING                       (1UL << 3)
/*! IIC FIFO empty pending
 */
#define LL_IIC_STA_BUF_EMPTY_PENDING                    (1UL << 2)
/*! IIC FIFO full pending
 */
#define LL_IIC_STA_BUF_FULL_PENDING                     (1UL << 1)
/*! IIC transfers one frame done pending
 */
#define LL_IIC_STA_DONE_PENDING                         (1UL << 0)

/***** SSP_DLY Register *****/
/*! Transfer delay time config
 @Note: t = APB_CLK * (n)
 */
#define LL_SPI_AND_IIC_SSP_DLY(n)                       ((n & 0xFF) << 0)



typedef enum {
    /*! IIC does not transmit signals other than ACK.
     */
    LL_IIC_NONE_FLAG    = 0,
    /*! IIC sends START signal
     */
    LL_IIC_START_FLAG   = 1,
    /*! IIC sends STOP signal
     */
    LL_IIC_STOP_FLAG    = 2,
    /*! IIC sends NACK signal
     */
    LL_IIC_NACK_FLAG    = 4,
} TYPE_ENUM_LL_IIC_FLAG;



/**
  * @brief IIC 
  */
struct hgi2c_v0_hw {
    __IO uint32_t CON0;
    __IO uint32_t CON1;
    __IO uint32_t CMD_DATA;
    __IO uint32_t BAUD;
    __IO uint32_t TDMALEN;
    __IO uint32_t RDMALEN;
    __IO uint32_t TDMACNT;
    __IO uint32_t RDMACNT;
    __IO uint32_t TSTADR;
    __IO uint32_t RSTADR;
    __IO uint32_t STA;
    __IO uint32_t SLAVESTA;
    __IO uint32_t SSP_DLY;
};


#ifdef __cplusplus
}
#endif


#endif /* _HGI2C_V0_HW_H */