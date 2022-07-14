#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "hal/uart.h"
#include "dev/uart/hguart_v2.h"
#include "hguart_v2_hw.h"

/**********************************************************************************/
/*                           LOW LAYER FUNCTION                                   */
/**********************************************************************************/

static int32 hguart_v2_switch_hal_uart_parity(enum uart_parity param) {

    switch (param) {
        case (UART_PARITY_NONE):
            return 0;
            break;
        case (UART_PARITY_ODD):
            return 1;
            break;
        case (UART_PARITY_EVEN):
            return 2;
            break;
        default:
            return -1;
            break;
    }
}

static int32 hguart_v2_switch_hal_uart_stop_bit(enum uart_stop_bit param) {

    switch (param) {
        case (UART_STOP_BIT_1):
            return 0;
            break;
        case (UART_STOP_BIT_2):
            return 1;
            break;
        default:
            return -1;
            break;
    }
}

static int32 hguart_v2_switch_hal_uart_data_bit(enum uart_data_bit param) {

    switch (param) {
        case (UART_DATA_BIT_8):
            return 0;
            break;
        case (UART_DATA_BIT_9):
            return 1;
            break;
        default:
            return -1;
            break;
    }
}

static int32 hguart_v2_set_data_bit(struct hguart_v2_hw *p_uart, enum uart_data_bit data_bit) {

    int32 data_bit_to_reg = 0;

    data_bit_to_reg = hguart_v2_switch_hal_uart_data_bit(data_bit);

    if (((-1) == data_bit_to_reg)) {
        return RET_ERR;
    }
    
    if (data_bit_to_reg) {
        p_uart->CON |=   LL_UART_CON_BIT9_EN;
    } else {
        p_uart->CON &= ~ LL_UART_CON_BIT9_EN;
    }

    return RET_OK;
}

static int32 hguart_v2_set_parity(struct hguart_v2_hw *p_uart, enum uart_parity parity) {

    int32 parity_to_reg = 0;

    parity_to_reg = hguart_v2_switch_hal_uart_parity(parity);

    if (((-1) == parity_to_reg)) {
        return RET_ERR;
    }
    
    switch (parity_to_reg) {
        case (0):
            p_uart->CON &= ~ (LL_UART_CON_PARITY_EN | LL_UART_CON_ODD_EN);
            break;
        case (1):
            p_uart->CON |= LL_UART_CON_PARITY_EN | LL_UART_CON_ODD_EN;
            break;
        case (2):
            p_uart->CON = (p_uart->CON &~(LL_UART_CON_ODD_EN)) | LL_UART_CON_PARITY_EN;
            break;
    }
    
    return RET_OK;
}

static int32 hguart_v2_set_stop_bit(struct hguart_v2_hw *p_uart, enum uart_stop_bit stop_bit) {

    int32 stop_bit_to_reg = 0;

    stop_bit_to_reg = hguart_v2_switch_hal_uart_stop_bit(stop_bit);

    if (((-1) == stop_bit_to_reg)) {
        return RET_ERR;
    }
    
    if (stop_bit_to_reg) {
        p_uart->CON |=   LL_UART_CON_STOP_BIT(1);
    } else {
        p_uart->CON &= ~ LL_UART_CON_STOP_BIT(1);
    }

    return RET_OK;
}

static int32 hguart_v2_set_time_out(struct hguart_v2_hw *p_uart, uint32 time_bit, uint32 enable) {

    uint32 baudrate = 0;

    baudrate = peripheral_clock_get(HG_APB0_PT_UART0) / (p_uart->BAUD + 1);

    if (enable) {
        p_uart->TOCON = (p_uart->TOCON &~ LL_UART_TOCON_TO_BIT_LEN(0xFFFF)) | LL_UART_TOCON_TO_BIT_LEN(baudrate * time_bit) | LL_UART_TOCON_TO_EN;
    } else {
        p_uart->TOCON = 0;
    }
   

    return RET_OK;
}

static int32 hguart_v2_set_dma(struct hguart_v2 *dev, uint32 enable) {

    if (enable) {
        dev->use_dma = 1;
    } else {
        dev->use_dma = 0;
    }

    return RET_OK;
}

static int32 hguart_v2_set_baudrate(struct hguart_v2_hw *p_uart, uint32 baudrate) {

    p_uart->BAUD = (peripheral_clock_get(HG_APB0_PT_UART0) / baudrate - 1);

    return RET_OK;
}

static int32 hguart_v2_set_mode(struct hguart_v2_hw *p_uart, uint32 mode)
{
    switch (mode) {
        case UART_MODE_DUPLEX:
            p_uart->CON &= ~ LL_UART_CON_WORK_MODE(0x3);
            p_uart->CON |=   LL_UART_CON_WORK_MODE(0);
            break;
        case UART_MODE_SIMPLEX_TX:
            p_uart->CON &= ~ LL_UART_CON_WORK_MODE(0x3);
            p_uart->CON |=   LL_UART_CON_WORK_MODE(1);
            break;
        case UART_MODE_SIMPLEX_RX:
            p_uart->CON &= ~ LL_UART_CON_WORK_MODE(0x3);
            p_uart->CON |=   LL_UART_CON_WORK_MODE(2);
            break;
        default:
            return -ENOTSUPP;
    }
    return RET_OK;
}

/**********************************************************************************/
/*                             ATTCH FUNCTION                                     */
/**********************************************************************************/

static int32 hguart_v2_open(struct uart_device *uart, uint32 baudrate) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    if (dev->opened) {
        return -EBUSY;
    }

    if ((baudrate < 0) || ((peripheral_clock_get(HG_APB0_PT_UART0) / baudrate) > 0x0003ffff)) {
        return RET_ERR;
    }

    /* pin config */
    if (pin_func(dev->dev.dev.dev_id , 1) != RET_OK) {
        return RET_ERR;
    }

    /* reg config */
    hw->BAUD = (peripheral_clock_get(HG_APB0_PT_UART0) / baudrate) - 1;
    hw->STA  = 0xFFFFFFFF;
    hw->CON  = 0x1;      /* Default config: 8bit + 1 stop_bit + no parity + duplex mode */

    dev->opened = 1;

    return RET_OK;
}

static int32 hguart_v2_close(struct uart_device *uart) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    if (!dev->opened) {
        return RET_OK;
    }
        
    hw->CON &= ~0x1;
    irq_disable(dev->irq_num       );
    pin_func(dev->dev.dev.dev_id, 0);

    dev->opened = 0;

    return RET_OK;
}

static int32 hguart_v2_putc(struct uart_device *uart, int8 value) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    if (dev->opened && (hw->CON & LL_UART_CON_UART_EN)) {
        while (!(hw->STA & LL_UART_STA_TX_BUF_EMPTY));
        hw->DATA = value;
        return RET_OK;
    } else {
        return -EIO;
    }
}

static uint8 hguart_v2_getc(struct uart_device *uart) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    if (dev->opened && (hw->CON & LL_UART_CON_UART_EN)) {
        while (!(hw->STA & LL_UART_STA_RX_BUF_NOT_EMPTY));
    }
    return hw->DATA;
}

static int32 hguart_v2_puts(struct uart_device *uart, uint8* buf, uint32 len) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;
    int32  i = 0;

    if (dev->use_dma) {
        /* clear the tx dma done pending before tx kick */
        hw->DMASTA  = BIT(0);
        
        hw->TSTADR  = (uint32)buf;
        hw->TDMALEN = len;
        hw->DMACON |= LL_UART_DMACON_TX_DMA_EN;

        /* waiting for tx done */
        while(!(hw->DMASTA & LL_UART_DMASTA_TX_DMA_PEND));
    } else {
        for (i = 0; i < len; i++) {
            hguart_v2_putc(uart, buf[i]);
        }
    }
    return RET_OK;
}

static int32 hguart_v2_gets(struct uart_device *uart, uint8* buf, uint32 len) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;
    int32  i = 0;
    
    if (dev->use_dma) {
        hw->RSTADR  = (uint32)buf;
        hw->RDMALEN = len;
        hw->DMACON |= LL_UART_DMACON_RX_DMA_EN;
        return RET_OK;
    } else {
        for (i = 0; i < len; i++) {
            buf[i] = hguart_v2_getc(uart);
        }
        return i;
    }
}

static int32 hguart_v2_ioctl(struct uart_device *uart, enum uart_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2) {

    int32  ret_val = RET_OK;
    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    switch (ioctl_cmd) {
        case (UART_IOCTL_CMD_SET_BAUDRATE):
            ret_val = hguart_v2_set_baudrate(hw, param1);
            break;
        case (UART_IOCTL_CMD_SET_DATA_BIT):
            ret_val = hguart_v2_set_data_bit(hw, param1);
            break;
        case (UART_IOCTL_CMD_SET_PARITY):
            ret_val = hguart_v2_set_parity(hw, param1);
            break;
        case (UART_IOCTL_CMD_SET_STOP_BIT):
            ret_val = hguart_v2_set_stop_bit(hw, param1);
            break;
        case (UART_IOCTL_CMD_SET_TIME_OUT):
            ret_val = hguart_v2_set_time_out(hw, param1, param2);
            break;
        case (UART_IOCTL_CMD_USE_DMA):
            ret_val = hguart_v2_set_dma(dev, param1);
            break;
        case (UART_IOCTL_CMD_SET_WORK_MODE):
            ret_val = hguart_v2_set_mode(hw, param1);
            break;
        default:
            ret_val = -ENOTSUPP;
            break;
    }

    return ret_val;
}

static void hguart_v2_irq_handler(void *data) {

    struct hguart_v2    *dev = (struct hguart_v2 *)data;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    /*------Time out interrupt-----*/
    if ((hw->TOCON & LL_UART_TOCON_TO_IE_EN) &&
        (hw->STA   & LL_UART_STA_TO_PEND   )) {
        hw->STA = LL_UART_STA_TO_PEND;

        if (dev->irq_hdl) {
            dev->irq_hdl(UART_IRQ_FLAG_TIME_OUT, dev->irq_data, hw->RDMACNT, 0);
        }
    }
        
    /*------Frame erro interrupt-----*/
    if ((hw->CON & LL_UART_CON_FERR_IE_EN  ) &&
        (hw->STA & LL_UART_STA_FERR_PENDING)) {
        hw->STA = LL_UART_STA_FERR_PENDING;

        if (dev->irq_hdl) {
            dev->irq_hdl(UART_IRQ_FLAG_FRAME_ERR, dev->irq_data, 0, 0);
        }
    }

    /*------TX complete interrupt-----*/
    if ((hw->CON & LL_UART_CON_TCIE_EN   ) &&
        (hw->STA & LL_UART_STA_TC_PENDING)) {
        hw->STA = LL_UART_STA_TC_PENDING;
    
        if (dev->irq_hdl) {
            dev->irq_hdl(UART_IRQ_FLAG_TX_BYTE, dev->irq_data, 0, 0);
        }
    }

    /*------RX_DMA_INTERRUPT-----*/
    if ((hw->DMACON & LL_UART_DMACON_RX_DMA_IE_EN) &&
        (hw->DMASTA & LL_UART_DMASTA_RX_DMA_PEND )) {
        hw->DMASTA = LL_UART_DMASTA_RX_DMA_PEND;
    
        if (dev->irq_hdl) {
            dev->irq_hdl(UART_IRQ_FLAG_DMA_RX_DONE, dev->irq_data, hw->RDMACNT, 0);
        }
    }

    /*------TX_DMA_INTERRUPT-----*/
    if ((hw->DMACON & LL_UART_DMACON_TX_DMA_IE_EN) &&
        (hw->DMASTA & LL_UART_DMASTA_TX_DMA_PEND )) {
        hw->DMASTA = LL_UART_DMASTA_TX_DMA_PEND;
    
        if (dev->irq_hdl) {
            dev->irq_hdl(UART_IRQ_FLAG_DMA_TX_DONE, dev->irq_data, hw->TDMACNT, 0);
        }
    }

    /*------RX buf not empty interrupt-----*/
    if ((hw->CON & LL_UART_CON_RXBUF_NEMPTY_IE_EN) &&
        (hw->STA & LL_UART_STA_RX_BUF_NOT_EMPTY  )) {
        
        if (dev->irq_hdl) {
            dev->irq_hdl(UART_IRQ_FLAG_RX_BYTE, dev->irq_data, hw->DATA, 0);
        }
    }
}

static int32 hguart_v2_request_irq(struct uart_device *uart, uart_irq_hdl irq_hdl, uint32 irq_flag, uint32 irq_data) {

    struct hguart_v2 *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    dev->irq_hdl  = irq_hdl;
    dev->irq_data = irq_data;
    request_irq(dev->irq_num, hguart_v2_irq_handler, dev);


    if (irq_flag & UART_IRQ_FLAG_TX_BYTE) {
        hw->CON    |= LL_UART_CON_TCIE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_TIME_OUT) {
        hw->TOCON  |= LL_UART_TOCON_TO_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_DMA_TX_DONE) {
        hw->DMACON |= LL_UART_DMACON_TX_DMA_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_DMA_RX_DONE) {
        hw->DMACON |= LL_UART_DMACON_RX_DMA_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_FRAME_ERR) {
        hw->CON    |= LL_UART_CON_FERR_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_RX_BYTE) {
        hw->CON    |= LL_UART_CON_RXBUF_NEMPTY_IE_EN;
    }
    irq_enable(dev->irq_num);

    return RET_OK;
}

static int32 hguart_v2_release_irq(struct uart_device *uart, uint32 irq_flag) {

    struct hguart_v2    *dev = (struct hguart_v2 *)uart;
    struct hguart_v2_hw *hw  = (struct hguart_v2_hw *)dev->hw;

    if (irq_flag & UART_IRQ_FLAG_TX_BYTE) {
        hw->CON    &= ~ LL_UART_CON_TCIE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_TIME_OUT) {
        hw->TOCON  &= ~ LL_UART_TOCON_TO_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_DMA_TX_DONE) {
        hw->DMACON &= ~ LL_UART_DMACON_TX_DMA_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_DMA_RX_DONE) {
        hw->DMACON &= ~ LL_UART_DMACON_RX_DMA_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_FRAME_ERR) {
        hw->CON    &= ~ LL_UART_CON_FERR_IE_EN;
    }

    if (irq_flag & UART_IRQ_FLAG_RX_BYTE) {
        hw->CON    &= ~ LL_UART_CON_RXBUF_NEMPTY_IE_EN;
    }

    return RET_OK;
}



int32 hguart_v2_attach(uint32 dev_id, struct hguart_v2 *uart) {

    uart->opened          = 0;
    uart->use_dma         = 0;
    uart->irq_hdl         = NULL;
    uart->irq_data        = 0;
    uart->dev.open        = hguart_v2_open;
    uart->dev.close       = hguart_v2_close;
    uart->dev.putc        = hguart_v2_putc;
    uart->dev.getc        = hguart_v2_getc;
    uart->dev.puts        = hguart_v2_puts;
    uart->dev.gets        = hguart_v2_gets;
    uart->dev.ioctl       = hguart_v2_ioctl;
    uart->dev.request_irq = hguart_v2_request_irq;
    uart->dev.release_irq = hguart_v2_release_irq;


    irq_disable(uart->irq_num);
    dev_register(dev_id, (struct dev_obj *)uart);
    
    return RET_OK;
}
