/**
 * @file uart.h
 * @author HUGE-IC Application Team
 * @brief UART HAL driver
 * @version 1.0.0
 * @date 2021-8-30
 * 
 * @copyright Copyright (c) 2021 HUGE-IC
 */
#ifndef _HAL_UART_H_
#define _HAL_UART_H_
#include "typesdef.h"
#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup Docxygenid_UART_enum UART enum
  * @ingroup  Docxygenid_UART_Driver
  * @brief    UART enumeration description.
  * @{
  */


/**
  * @brief Enumeration constant for UART mode.
  */
enum uart_mode {

    /*! Set UART to full duplex mode.
     */
    UART_MODE_DUPLEX,

    /*! Set UART to simplex_tx mode.
     */
    UART_MODE_SIMPLEX_TX,

    /*! Set UART to simplex_rx mode.
     */
    UART_MODE_SIMPLEX_RX,
};

/**
  * @brief Enumeration constant for UART parity.
  */
enum uart_parity {

    /*! Set UART to no parity mode.
     */
    UART_PARITY_NONE,

    /*! Set UART to odd parity mode.
     */
    UART_PARITY_ODD,

    /*! Set UART to even parity mode.
     */
    UART_PARITY_EVEN,
};

/**
  * @brief Enumeration constant for UART stop bit.
  */
enum uart_stop_bit {

    /*! Set UART stop bit to 1bit.
     */
    UART_STOP_BIT_1,
	
	/*! Set UART stop bit to 1.5bit.
     */
    UART_STOP_BIT_1_5,

    /*! Set UART stop bit to 2bit.
     */
    UART_STOP_BIT_2,
};

/**
  * @brief Enumeration constant for UART data bit.
  */
enum uart_data_bit {

    /*! Set UART data bit to 5bit.
     */
    UART_DATA_BIT_5,

    /*! Set UART data bit to 6bit.
     */
    UART_DATA_BIT_6,

    /*! Set UART data bit to 7bit.
     */
    UART_DATA_BIT_7,

    /*! Set UART data bit to 8bit.
     */
    UART_DATA_BIT_8,

    /*! Set UART data bit to 9bit.
     */
    UART_DATA_BIT_9,
};

/**
  * @brief Enumeration constant for UART irq flag.
  */
enum uart_irq_flag {

    /*! 
     * @brief Interrupt: send a frame of data done.
     * @note
     *        If the UART configuration data bit is 8bit, one frame of data is 8bit;\n
     *        if the data bit is 9bit, one frame of data is 9bit.
     */
    UART_IRQ_FLAG_TX_BYTE     = BIT(0),

    /*! 
     * @brief Interrupt: time is up, no data has been received yet.
     * @note
     *        This interrupt is only used for UART receive.
     */
    UART_IRQ_FLAG_TIME_OUT    = BIT(1),

    /*! 
     * @brief Interrupt: UART send done using DMA.
     * @note
     *        Before configuring this interrupt,\n
     *        you need to use @ref uart_ioctl() function to enable DMA.
     */
    UART_IRQ_FLAG_DMA_TX_DONE = BIT(2),

    /*! 
     * @brief Interrupt: UART receive done using DMA.
     * @note
     *        Before configuring this interrupt,\n
     *        you need to use @ref uart_ioctl() function to enable DMA.
     */
    UART_IRQ_FLAG_DMA_RX_DONE = BIT(3),

    /*! 
     * @brief Interrupt: UART receives a frame of data error.
     * @note
     *        - If the UART configuration data bit is 8bit, one frame of data is 8bit;\n
     *          if the data bit is 9bit, one frame of data is 9bit.
     *        - A frame of data error, usually means that the number of stop bits of \n
     *          a frame of data does not match the expected number of bits.
     */
    UART_IRQ_FLAG_FRAME_ERR   = BIT(4),

    /*! 
     * @brief Interrupt: UART receives a frame of data done.
     * @note
     *        If the UART configuration data bit is 8bit, one frame of data is 8bit;\n
     *        if the data bit is 9bit, one frame of data is 9bit.
     */
    UART_IRQ_FLAG_RX_BYTE     = BIT(5),
};

/**
  * @brief Enumeration constant for UART irq flag
  */
enum uart_ioctl_cmd {

    /*! 
     * @brief Set the baudrate of the UART.
     */
    UART_IOCTL_CMD_SET_BAUDRATE,

    /*! 
     * @brief Set the UART data bits.
     * @note
     *       reference @ref uart_data_bit.
     */
    UART_IOCTL_CMD_SET_DATA_BIT,

    /*! 
     * @brief Set the UART parity mode.
     * @note
     *       reference @ref uart_parity.
     */
    UART_IOCTL_CMD_SET_PARITY,

    /*! 
     * @brief Set the UART stop bit.
     * @note
     *       reference @ref uart_stop_bit.
     */
    UART_IOCTL_CMD_SET_STOP_BIT,

    /*! 
     * @brief Set the timeout.
     */
    UART_IOCTL_CMD_SET_TIME_OUT,

    /*! 
     * @brief Set whether the UART uses DMA to send.
     */
    UART_IOCTL_CMD_USE_DMA,
    
    /*! 
     * @brief Set UART work mode.
     * @note
     *       reference @ref uart_mode.
     */
    UART_IOCTL_CMD_SET_WORK_MODE,

    /*! 
     * @brief Query whether the UART DATA reg ready.
     */
    UART_IOCTL_CMD_DATA_RDY,
};


/** @} Docxygenid_UART_enum*/



/* User interrupt handle */
typedef int32 (*uart_irq_hdl)(uint32 irq, uint32 irq_data, uint32 param1, uint32 param2);


/** @defgroup Docxygenid_UART_structure UART structure
  * @ingroup  Docxygenid_UART_Driver
  * @brief    UART structure description
  * @note
  *       User better not modify the structure.
  * @{
  */

struct uart_device {
    struct dev_obj dev;
    int32(*open)(struct uart_device *uart, uint32 baudrate);
    int32(*close)(struct uart_device *uart);
    int32(*putc)(struct uart_device *uart, int8 value);
    uint8(*getc)(struct uart_device *uart);
    int32(*puts)(struct uart_device *uart, uint8* buf, uint32 len);
    int32(*gets)(struct uart_device *uart, uint8* buf, uint32 len);
    int32(*ioctl)(struct uart_device *uart, enum uart_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2);
    int32(*request_irq)(struct uart_device *uart, uart_irq_hdl irq_hdl, uint32 irq_flag,  uint32 irq_data);
    int32(*release_irq)(struct uart_device *uart, uint32 irq_flag);
};

/** @} Docxygenid_UART_structure*/



/** @addtogroup Docxygenid_UART_Driver
  * @{
  */


/** 
  * @brief  Open uart with baudrate and the default configuration.
  * @param  uart      : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  baudrate  : Configure the baudrate for UART.
  * @return 
  *         - RET_OK  : UART module open successfully.
  *         - RET_ERR : UART module open unsuccessfully.
  * @note
  *         The default configuration is full duplex mode, no parity mode,\n
  *         data bit is 8bit, stop bit is 1bit.If you need to change the configuration,\n
  *         you can use the @ref uart_ioctl function to make changes after initialization is complete.
  */
int32 uart_open(struct uart_device *uart, uint32 baudrate);

/** 
  * @brief  Close the UART module.
  * @param  uart : UART handle. Usually use @ref dev_get() function to get the handle.
  * @return
  *         - RET_OK  : UART module close successfully.
  *         - RET_ERR : UART module close unsuccessfully.
  * @note
  *         The UART will not be able to send and receive data normally,\n
  *         and all configurations (including interrupt-related configurations) will be invalid.
  */
int32 uart_close(struct uart_device *uart);

/** 
  * @brief  UART sends a frame of data.
  * @param  uart  : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  vaule : The data to be sent by the UART, the unit is 1 frame.
  * @return 
  *         - RET_OK  : UART module send successfully.
  *         - RET_ERR : UART module send unsuccessfully.
  * @note
  *         If the UART configuration data bit is 8bit, one frame of data is 8bit;\n
  *         if the data bit is 9bit, one frame of data is 9bit.
  */
int32 uart_putc(struct uart_device *uart, int8 value);

/** 
  * @brief  UART receives a frame of data.
  * @param  uart : UART handle. Usually use @ref dev_get() function to get the handle.
  * @return 
  *         - value : The data value.
  * @note
  *         If the UART configuration data bit is 8bit, one frame of data is 8bit;\n
  *         if the data bit is 9bit, one frame of data is 9bit.
  */
uint8 uart_getc(struct uart_device *uart);

/** 
  * @brief  The UART sends data according to the address and data number of the DATA BUFFER.
  * @param  uart : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  buf  : Start address of DATA BUFFER.
  * @param  len  : The number of data to be sent, the default unit is 8bit.
  * @return 
  *         - RET_OK  : UART module send successfully.
  *         - RET_ERR : UART module send unsuccessfully.
  * @note
  *         Send uses CPU by default. To use DMA transmission,\n
  *         it needs to be configured using the @ref uart_ioctl() function.
  */
int32 uart_puts(struct uart_device *uart, uint8* buf, uint32 len);

/** 
  * @brief  The UART receive data according to the address and data number of the DATA BUFFER.
  * @param  uart : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  buf  : Start address of DATA BUFFER.
  * @param  len  : The number of data to be sent, the default unit is 8bit.
  * @return 
  *         - RET_OK  : UART module send successfully.
  *         - RET_ERR : UART module send unsuccessfully.
  * @note
  *         Send uses CPU by default. To use DMA transmission,
  *         it needs to be configured using the @ref uart_ioctl() function.
  */
int32 uart_gets(struct uart_device *uart, uint8* buf, uint32 len);

/** 
  * @brief  Configure the UART module by ioctl_cmd.
  * @param  uart      : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  ioctl_cmd : Commands to configure the UART module, reference @ref uart_ioctl_cmd.
  * @param  param1    : Parameter 1, according to the command.
  * @param  param2    : Parameter 2, according to the command.
  * @return 
  *         - RET_OK  : UART module configure successfully.
  *         - RET_ERR : UART module configure unsuccessfully.
  * @note
  *         None.
  */
int32 uart_ioctl(struct uart_device *uart, enum uart_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2);

/** 
  * @brief  Request interrupt by irq_flag.
  * @param  uart     : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  irq_hdl  : Interrupt handle, executed after the interrupt is generated.
  * @param  irq_flag : Interrupt type, reference @ref uart_irq_flag.
  * @param  irq_data : Parameters for interrupt handler.
  * @return
  *         - RET_OK  : UART module request interrupt successfully.
  *         - RET_ERR : UART module request interrupt unsuccessfully.
  * @note
  *         Support multiple interrupts to request together.
  */
int32 uart_request_irq(struct uart_device *uart, uart_irq_hdl irq_hdl, uint32 irq_flag,  uint32 irq_data);

/** 
  * @brief  Release interrupt by irq_flag.
  * @param  uart     : UART handle. Usually use @ref dev_get() function to get the handle.
  * @param  irq_flag : Interrupt type, reference @ref uart_irq_flag.
  * @return
  *         - RET_OK  : UART module release interrupt successfully.
  *         - RET_ERR : UART module release interrupt unsuccessfully.
  * @note
  *         Support multiple interrupts to release together.
  */
int32 uart_release_irq(struct uart_device *uart, uint32 irq_flag);

/** @} Docxygenid_UART_functions*/


#ifdef __cplusplus
}
#endif
#endif

