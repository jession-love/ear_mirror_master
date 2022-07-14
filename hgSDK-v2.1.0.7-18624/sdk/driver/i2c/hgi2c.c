#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "hal/i2c.h"
#include "dev/i2c/hgi2c.h"
#include "hgi2c_v0_hw.h"

/**********************************************************************************/
/*                        IIC LOW LAYER FUNCTION                                  */
/**********************************************************************************/

static inline void hgi2c_set_dir_tx(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    p_i2c->CON1 |= LL_IIC_CON1_TX_EN;
}

static inline void hgi2c_set_dir_rx(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    p_i2c->CON1 &= ~LL_IIC_CON1_TX_EN;
}

static inline void hgi2c_set_ack(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    p_i2c->CON0 &= ~LL_IIC_CON0_TX_NACK;
}

static inline void hgi2c_set_nack(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    p_i2c->CON0 |= LL_IIC_CON0_TX_NACK;
}

static inline int32 hgi2c_get_nack_pending(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
	return ((p_i2c->STA & LL_IIC_STA_RX_NACK_PENDING));	
}

static inline int32 hgi2c_get_busy(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    return ((p_i2c->STA & LL_IIC_STA_SSP_BUSY_PENDING));
}

static inline int32 hgi2c_get_rx_ready_sta(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    return (!(p_i2c->STA & LL_IIC_STA_BUF_EMPTY_PENDING));
}

static inline int32 hgi2c_get_tx_ready_sta(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    return (!(p_i2c->STA & LL_IIC_STA_BUF_FULL_PENDING));
}


static inline uint32 hgi2c_get_slave_address(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    return ((p_i2c->CON0 >> 2) & 0x3FF);
}


static inline void hgi2c_write_data(struct hgi2c_v0_hw *p_i2c,uint32 data)
{
	__NOP();
    ((p_i2c)->CMD_DATA = (data));
}

static inline uint32 hgi2c_get_slave_address_bitwidth(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    return p_i2c->CON0 & BIT(0);
}

static inline uint32 hgi2c_is_slave_mode(struct hgi2c_v0_hw *p_i2c)
{
	__NOP();
    return ((p_i2c->CON1 >> 2) & BIT(0));
}

void Delay_nopCnt(uint32 cnt)
{
	volatile uint32 dat;
	dat = cnt;
	while(dat-->0)
	{
		;
	}
}

static inline uint32 hgi2c_set_slave_addr(struct hgi2c_v0_hw *hw, uint32 addr) {

    if (addr > 0x3FF) {
        return RET_ERR;
    }

    hw->CON0 = (hw->CON0 &~ LL_IIC_CON0_SLAVE_ADR(0x3FF)) | LL_IIC_CON0_SLAVE_ADR(addr);

    return RET_OK;
}

void  hgi2c_master_tx(struct hgi2c_v0_hw *p_iic, uint8 data, uint8 flag)
{
    uint32 tx_data = data;
    
    if(flag & LL_IIC_START_FLAG) {
        tx_data |= LL_IIC_CMD_DATA_START_BIT_EN;
    }
	
    if(flag & LL_IIC_STOP_FLAG) {
        tx_data |= LL_IIC_CMD_DATA_STOP_BIT_EN;
    }
    
    hgi2c_set_dir_tx(p_iic);
    hgi2c_write_data(p_iic,tx_data);
    
    /* Waiting for transmission to complete */
    while(hgi2c_get_busy(p_iic));
    /* Set to input to prevent pulling SCL */
    hgi2c_set_dir_rx(p_iic);
}


static uint8 hgi2c_master_rx(struct hgi2c_v0_hw *p_iic, uint8 flag)
{
    uint32 data = 0;
    
    if(flag & LL_IIC_NACK_FLAG) {
        p_iic->CON0 |= LL_IIC_CON0_TX_NACK;
    } else {
        p_iic->CON0 &= ~LL_IIC_CON0_TX_NACK;
    }
    if(flag & LL_IIC_START_FLAG) {
        data |= LL_IIC_CMD_DATA_START_BIT_EN;
    }
    if(flag & LL_IIC_STOP_FLAG) {
        data |= LL_IIC_CMD_DATA_STOP_BIT_EN;
    }
    
    hgi2c_set_dir_rx(p_iic);
    /* Configure the number of data to receive */
    p_iic->RDMALEN   = LL_IIC_DMA_RX_LEN(1);
    /* When receiving, you need to write the flag to be sent
       by the host first.
    */
    //p_iic->CMD_DATA  = data;
    hgi2c_write_data(p_iic,data);
    /* Waiting for transmission to complete */
    while(hgi2c_get_busy(p_iic));
	//Delay_nopCnt(2000);
    return p_iic->CMD_DATA;
}


/* Baudrate is SCL frequency for master mode, and is delay time in APB0_CLK uint for slave mode */
static int32 hgi2c_set_baudrate(struct i2c_device *i2c, uint32 baudrate)
{
    struct hgi2c *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;
    
    uint32 baudrate_cnt =  peripheral_clock_get(HG_APB0_PT_IIC0)/baudrate/4 - 1;
    ASSERT((baudrate_cnt >= 0) && (baudrate_cnt <= 65535));
    hw->BAUD = baudrate_cnt;
    
    return RET_OK;
}

#if (TXW80X)
static int32 hgi2c_strong_output(struct i2c_device *i2c, uint32 enable) {
    struct hgi2c *dev = (struct hgi2c*)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;

    
    if (enable) {
        hw->CON0 |=   LL_IIC_CON0_STRONG_DRV_EN;
    } else {
        hw->CON0 &= ~ LL_IIC_CON0_STRONG_DRV_EN;
    }
    
    return RET_OK;
}
#endif

#if (TXW80X)
static int32 hgi2c_sda_out_delay(struct i2c_device *i2c) {
    struct hgi2c *dev = (struct hgi2c*)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;
    uint32_t value = 0;
    
    value = hw->BAUD;

    if (value > 0xFF) {
        value = 0xFF;
    }

    hw->SSP_DLY = (hw->SSP_DLY &~(LL_SPI_AND_IIC_SSP_DLY(0xFF))) | LL_SPI_AND_IIC_SSP_DLY(value);
    
    return RET_OK;
}
#endif


static int32 hgi2c_filtering(struct i2c_device *i2c, uint32 value) {
    struct hgi2c *dev = (struct hgi2c*)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;
    
    if (value < 0 || value > 0x1F) {
        return -EINVAL;
    }

    hw->CON0 = (hw->CON0 &~(LL_IIC_CON0_FILTER_CNT(0xFF))) | LL_IIC_CON0_FILTER_CNT(value);
    
    return RET_OK;
}

/**********************************************************************************/
/*                          IIC ATTCH FUNCTION                                    */
/**********************************************************************************/

static int32 hgi2c_open(struct i2c_device *i2c, enum i2c_mode mode_sel, enum i2c_addr_mode addr_bit_sel, uint32 addr) {

    struct hgi2c *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;

    if (dev->opened) {
        return RET_OK;
    }
    
    if (pin_func(i2c->dev.dev_id, 1) != RET_OK) {
        return RET_ERR;
    }
    
    hw->STA  = 0xFFFFFFFF;
    hw->CON1 = LL_IIC_CON1_SPI_I2C_SEL(0x1);
    if(mode_sel == IIC_MODE_MASTER) {
        hw->CON1 |= LL_IIC_CON1_MODE(0x0);
    } else {
        hw->CON1 |= LL_IIC_CON1_MODE(0x1);
        if(addr_bit_sel == IIC_ADDR_10BIT) {
            hw->CON0 = LL_IIC_CON0_SLAVE_ADR_WIDTH(0x1);
        } else {
            hw->CON0 = LL_IIC_CON0_SLAVE_ADR_WIDTH(0x0);
        }
    }

    /* Open DMA tansfer done interrupt */
    hw->CON1 |= BIT(9);

    /* Address for master mode is slave device that attempted to communicate 
       or itself slave address in slave mode */
    hw->CON0 = (hw->CON0 &~ LL_IIC_CON0_SLAVE_ADR(0x3FF)) | LL_IIC_CON0_SLAVE_ADR(addr);
    hw->CON1 |= LL_IIC_CON1_SSP_EN;

    dev->opened          = 1;
    dev->flag_rx_done    = 1;
    dev->flag_tx_done    = 1;
    dev->irq_rx_done_en  = 0;
    dev->irq_tx_done_en  = 0;

    irq_enable(dev->irq_num);
    
    return RET_OK;
}

static int32 hgi2c_close(struct i2c_device *i2c)
{
    struct hgi2c *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;
    
    if (!dev->opened) {
        return RET_OK;
    }
    irq_disable(dev->irq_num);

    hw->CON1 &= ~LL_IIC_CON1_SSP_EN;
    
    pin_func(i2c->dev.dev_id, 0);
    dev->irq_hdl         = NULL;
    dev->irq_data        = 0;
    dev->opened          = 0;
    dev->flag_rx_done    = 1;
    dev->flag_tx_done    = 1;
    dev->irq_rx_done_en  = 0;
    dev->irq_tx_done_en  = 0;

    
    return RET_OK;
}


static int32 hgi2c_read(struct i2c_device *i2c, int8 *addr_buf, uint32 addr_len, int8 *buf, uint32 len) {

    struct hgi2c *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;
    uint32 slave_addr      = 0;
    uint32 len_temp        = len;
    uint32 addr_len_temp   = addr_len;
    uint32 len_offset      = 0;
    int32  ret_sema        = 0;

    if(!dev->opened) {
        return RET_ERR;
    }

    /* Make sure the write done */
    if (!dev->flag_tx_done) {
        return RET_ERR;
    }

    /* make sure that a least one buf is existent */
    if ((NULL == addr_buf) && (NULL == buf)) {
        return RET_ERR;
    }

    /* make sure that a least one len is existent */
    if ((0 == addr_len) && (0 == len)) {
        return RET_ERR;
    }

    os_mutex_lock(&dev->i2c_lock, osWaitForever);

    /* Indcate IIC module is reading */
    dev->flag_rx_done = 0;

    /* IIC master mode */
    if (!hgi2c_is_slave_mode(hw)) {
        /* set IIC to send status */
        hgi2c_set_dir_tx(hw);
        
        /* send the iic slave address configured by "open" function */
        slave_addr = hgi2c_get_slave_address(hw);
        
        if(hgi2c_get_slave_address_bitwidth(hw) == IIC_ADDR_10BIT) {
            hw->CMD_DATA = LL_IIC_CMD_DATA_START_BIT_EN | LL_IIC_CMD_DATA_WRITE(slave_addr >> 8 << 1) | 0xf0;
            /* It doesn't stop here when mode is IIC master */
            while (hgi2c_get_busy(hw));
            hw->CMD_DATA = LL_IIC_CMD_DATA_WRITE(slave_addr);
        } else {
            hw->CMD_DATA = LL_IIC_CMD_DATA_START_BIT_EN | LL_IIC_CMD_DATA_WRITE(slave_addr << 1);
        }

        /* It doesn't stop here when mode is IIC master */
        while (hgi2c_get_busy(hw));

        /* send the addr, may be register address */
        while(addr_len_temp > 0) {
            hw->CMD_DATA = LL_IIC_CMD_DATA_WRITE(*addr_buf++);
            addr_len_temp--;
            /* It doesn't stop here when mode is IIC master */
            while (hgi2c_get_busy(hw));
        }

        if(hgi2c_get_slave_address_bitwidth(hw) == IIC_ADDR_10BIT) {
            // 10bit read operate only need first byte
            hw->CMD_DATA = LL_IIC_CMD_DATA_START_BIT_EN | LL_IIC_CMD_DATA_WRITE(slave_addr >> 8 << 1) | 0xf1;
        } else {
            hw->CMD_DATA = LL_IIC_CMD_DATA_START_BIT_EN | LL_IIC_CMD_DATA_WRITE(slave_addr << 1) | 0x1;
        }
        while (hgi2c_get_busy(hw));
    }

    /* set IIC to receive status */
    hgi2c_set_dir_rx(hw);

    /* data length more than 4095 byte(limited by hardware) */
    len_offset = 0;
    if (len_temp >= 4095) {
        
        do {
            /* set ACK when receive data */
            hgi2c_set_ack(hw);

            hw->RSTADR = (uint32)(buf + len_offset);
            hw->RDMALEN = 4095;
            hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */

            /* DMA interrupt to sema_up  */
            /* master use 6s for timeout */
            /* slave use wait forever    */
            if (!hgi2c_is_slave_mode(hw)) {
                ret_sema = os_sema_down(&dev->i2c_done, 6*1000);
                if (!ret_sema) {
                    printf("***IIC modoule info: M read err, 1 phase timeout!\r\n");
                    goto err_handle;
                }
            } else {
                os_sema_down(&dev->i2c_done, osWaitForever);
            }

            len_offset += 4095;
        }while ((len_temp -= 4095) >= 4095);
    }

    /* IIC master mode need to send the stop signal & NACK in last byte */
    if (!hgi2c_is_slave_mode(hw)) {
        
        /* make sure the remain (len-1) more than 0 */
        if (len_temp - 1) {
            
            /* set ACK when receive data */
            hgi2c_set_ack(hw);
                    
            hw->RSTADR = (uint32)(buf + len_offset);
            hw->RDMALEN = len_temp - 1;
            hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */
            
            /* DMA interrupt to sema_up  */
            /* master use 6s for timeout */
            ret_sema = os_sema_down(&dev->i2c_done, 6*1000);
            if (!ret_sema) {
                printf("***IIC modoule info: M read err, 2 phase timeout!\r\n");
                goto err_handle;
            }

            len_offset += (len_temp - 1);
        }

        /* set NACK when receive the last byte */
        hgi2c_set_nack(hw);
        
        //send the last byte with the stop signal
        while (hgi2c_get_busy(hw));
        hw->RDMALEN  = 1;
        hw->CMD_DATA = LL_IIC_CMD_DATA_STOP_BIT_EN | LL_IIC_CMD_DATA_WRITE(0x00);
        while (hgi2c_get_busy(hw));
        /* Read the last from hardware */
        *(buf+len_offset) = hw->CMD_DATA;
        
    } else {
        /* make sure the remain (len-1) more than 0 */
        if (len_temp - 1) {
            /* set ACK when receive data */
            hgi2c_set_ack(hw);
            
            hw->RSTADR = (uint32)(buf + len_offset);
            hw->RDMALEN = len_temp - 1;
            hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */
            
            /* DMA interrupt to sema_up */
            /* slave use wait forever   */
            os_sema_down(&dev->i2c_done, osWaitForever);
            len_offset += (len_temp - 1);
         }

        /* set NACK when receive the last byte */
        hgi2c_set_nack(hw);
        
        hw->RSTADR = (uint32)(buf + len_offset);
        hw->RDMALEN = 1;
        hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */
        
        /* DMA interrupt to sema_up */
        /* slave use wait forever   */
        os_sema_down(&dev->i2c_done, osWaitForever);
        len_offset += 1;
    }

    if (dev->irq_rx_done_en) {
        if (dev->irq_hdl) {
            dev->irq_hdl(I2C_IRQ_FLAG_RX_DONE, dev->irq_data, 0);
        }
    }

err_handle:

    os_mutex_unlock(&dev->i2c_lock);

    /* Indcate IIC module has read done */
    dev->flag_rx_done = 1;

    return RET_OK;
}

static int32 hgi2c_write(struct i2c_device *i2c, int8 *addr_buf, uint32 addr_len, int8 *buf, uint32 len) {

    struct hgi2c *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;
    uint32 slave_addr      = 0;
    uint32 len_temp        = len;
    uint32 addr_len_temp   = addr_len;
    uint32 len_offset      = 0;
    int32 ret_sema         = 0;

    if(!dev->opened) {
        return RET_ERR;
    }

    /* Make sure the read done */
    if (!dev->flag_rx_done) {
        return RET_ERR;
    }

    /* make sure that a least one buf is existent */
    if ((NULL == addr_buf) && (NULL == buf)) {
        return RET_ERR;
    }

    /* make sure that a least one len is existent */
    if ((0 == addr_len) && (0 == len)) {
        return RET_ERR;
    }

    os_mutex_lock(&dev->i2c_lock, osWaitForever);

    /* Indcate IIC module is writing */
    dev->flag_tx_done = 0;

    /* set IIC to send status */
    hgi2c_set_dir_tx(hw);


    /* IIC master mode */
    if (!hgi2c_is_slave_mode(hw)) {
        /* send the iic slave address configured by "open" function */
        slave_addr = hgi2c_get_slave_address(hw);
        
        if(hgi2c_get_slave_address_bitwidth(hw) == IIC_ADDR_10BIT) {
            hw->CMD_DATA = LL_IIC_CMD_DATA_START_BIT_EN | LL_IIC_CMD_DATA_WRITE(slave_addr >> 8 << 1) | 0xf0;
            /* It doesn't stop here when mode is IIC master */
            while (hgi2c_get_busy(hw));
            hw->CMD_DATA = LL_IIC_CMD_DATA_WRITE(slave_addr);
        } else {
            hw->CMD_DATA = LL_IIC_CMD_DATA_START_BIT_EN | LL_IIC_CMD_DATA_WRITE(slave_addr << 1);
        }

        /* It doesn't stop here when mode is IIC master */
        while (hgi2c_get_busy(hw));

        /* send the addr, may be register address */
        while(addr_len_temp > 0) {
            hw->CMD_DATA = LL_IIC_CMD_DATA_WRITE(*addr_buf++);
            addr_len_temp--;
            /* It doesn't stop here when mode is IIC master */
            while (hgi2c_get_busy(hw));
        }
    }

    /* data length more than 4095 byte(limited by hardware) */
    len_offset = 0;
    if (len_temp >= 4095) {
        do {
            hw->TSTADR = (uint32)(buf + len_offset);
            hw->TDMALEN = 4095;
            hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */

            /* DMA interrupt to sema_up  */
            /* master use 6s for timeout */
            /* slave use wait forever    */
            if (!hgi2c_is_slave_mode(hw)) {
                ret_sema = os_sema_down(&dev->i2c_done, 6*1000);
                if (!ret_sema) {
                    printf("***IIC modoule info: M send err, 1 phase timeout!\r\n");
                    goto err_handle;
                }
            } else {
                os_sema_down(&dev->i2c_done, osWaitForever);
            }
            
            len_offset += 4095;
        }while ((len_temp -= 4095) >= 4095);
    }

    /* IIC master mode need to send the stop signal in last byte */
    if (!hgi2c_is_slave_mode(hw)) {
        
        /* make sure the remain (len-1) more than 0 */
        if (len_temp - 1) {
            hw->TSTADR = (uint32)(buf + len_offset);
            hw->TDMALEN = len_temp - 1;
            hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */

            /* DMA interrupt to sema_up  */
            /* master use 6s for timeout */
            ret_sema = os_sema_down(&dev->i2c_done, 6*1000);
            if (!ret_sema) {
                printf("***IIC modoule info: M send err, 2 phase timeout!\r\n");
                goto err_handle;
            }

            len_offset += (len_temp - 1);
        }

        //send the last byte with the stop signal
        while (hgi2c_get_busy(hw));
        hw->CMD_DATA = LL_IIC_CMD_DATA_STOP_BIT_EN | LL_IIC_CMD_DATA_WRITE(*(buf+len_offset));
        while (hgi2c_get_busy(hw));
        
    } else {
        /* make sure the remain (len-1) more than 0 */
        if (len_temp) {
            hw->TSTADR = (uint32)(buf + len_offset);
            hw->TDMALEN = len_temp;
            hw->CON1 |= LL_IIC_CON1_DMA_EN; /* kick DMA */
            
            /* DMA interrupt to sema_up */
            /* slave use wait forever   */
            os_sema_down(&dev->i2c_done, osWaitForever);
            len_offset += (len_temp);
         }
    }

    if (dev->irq_tx_done_en) {
        if (dev->irq_hdl) {
            dev->irq_hdl(I2C_IRQ_FLAG_TX_DONE, dev->irq_data, 0);
        }
    }

err_handle:

    os_mutex_unlock(&dev->i2c_lock);

    /* Indcate IIC module has written done */
    dev->flag_tx_done = 1;

    return RET_OK;

}

static int32 hgi2c_ioctl(struct i2c_device *i2c, int32 cmd, uint32 param)
{
    int32  ret_val    = RET_OK;
    struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw*)( ( (struct hgi2c *)i2c )->hw );

    switch (cmd) {
            case (IIC_SET_DEVICE_ADDR):
                ret_val = hgi2c_set_slave_addr(hw, param);
                break;
            case (IIC_FILTERING):
                ret_val = hgi2c_filtering(i2c, param);
                break;
        #if (TXW80X)
            case (IIC_SDA_OUTPUT_DELAY):
                ret_val = hgi2c_sda_out_delay(i2c);
                break;
            case (IIC_STRONG_OUTPUT):
                ret_val = hgi2c_strong_output(i2c, param);
                break;
        #endif
        
       default:
            ret_val = -ENOTSUPP;
            break;
    }
    
    return ret_val;
}

/** 
  * @brief  IIC0 Interrupt handler
  * @param  None.
  * @retval None.
  */
static void hgi2c_irq_handler(void *data)
{
     struct hgi2c *dev = (struct hgi2c *)data;
     struct hgi2c_v0_hw *hw = (struct hgi2c_v0_hw *)dev->hw;

    /* DMA Done interrupt */
    if ((hw->CON1 & LL_IIC_CON1_DMA_IE_EN) && (hw->STA & LL_IIC_STA_DMA_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_IIC_STA_DMA_PENDING;
        
        os_sema_up(&dev->i2c_done);
    }

    /* I2C_IRQ_FLAG_RX_NACK */
    if ((hw->CON0 & LL_IIC_CON0_RX_NACK_IE_EN) && (hw->STA & LL_IIC_STA_RX_NACK_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_IIC_STA_RX_NACK_PENDING;
        if (dev->irq_hdl) {
            dev->irq_hdl(I2C_IRQ_FLAG_RX_NACK, dev->irq_data, 0);
        }
    }

    /* I2C_IRQ_FLAG_RX_ERROR */
    if ((hw->CON1 & LL_IIC_CON1_BUF_OV_IE_EN) && (hw->STA & LL_IIC_STA_BUF_OV_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_IIC_STA_BUF_OV_PENDING;
        if (dev->irq_hdl) {
            dev->irq_hdl(I2C_IRQ_FLAG_RX_ERROR, dev->irq_data, 0);
        }
    }

    /* I2C_IRQ_FLAG_DETECT_STOP */
    if ((hw->CON0 & LL_IIC_CON0_STOP_IE_EN) && (hw->STA & LL_IIC_STA_STOP_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_IIC_STA_STOP_PENDING;
        if (dev->irq_hdl) {
            dev->irq_hdl(I2C_IRQ_FLAG_DETECT_STOP, dev->irq_data, 0);
        }
    }
}

static int32 hgi2c_request_irq(struct i2c_device *i2c, i2c_irq_hdl irqhdl, uint32 irq_data, uint32 irq_flag)
{
    struct hgi2c       *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw  = (struct hgi2c_v0_hw *)dev->hw;
    
    if(!dev->opened) {
        return RET_ERR;
    }
    
    dev->irq_hdl = irqhdl;
    dev->irq_data = irq_data;

    if (irq_flag & I2C_IRQ_FLAG_TX_DONE) {
        dev->irq_tx_done_en = 1;
    }

    if (irq_flag & I2C_IRQ_FLAG_RX_DONE) {
        dev->irq_rx_done_en = 1;
    }

    if (irq_flag & I2C_IRQ_FLAG_DETECT_STOP) {
        hw->CON0 |= BIT(20);
    }

    if (irq_flag & I2C_IRQ_FLAG_RX_NACK) {
        hw->CON0 |= BIT(22);
    }

    if (irq_flag & I2C_IRQ_FLAG_RX_ERROR) {
        hw->CON1 |= BIT(8);
    }
    
    return RET_OK;
}

static int32 hgi2c_release_irq(struct i2c_device *i2c, uint32 irq_flag) {

    struct hgi2c       *dev = (struct hgi2c *)i2c;
    struct hgi2c_v0_hw *hw  = (struct hgi2c_v0_hw *)dev->hw;
    
    if(!dev->opened) {
        return RET_ERR;
    }

    if (irq_flag & I2C_IRQ_FLAG_TX_DONE) {
        dev->irq_tx_done_en = 0;
    }

    if (irq_flag & I2C_IRQ_FLAG_RX_DONE) {
        dev->irq_rx_done_en = 0;
    }

    if (irq_flag & I2C_IRQ_FLAG_DETECT_STOP) {
        hw->CON0 &= ~ BIT(20);
    }

    if (irq_flag & I2C_IRQ_FLAG_RX_NACK) {
        hw->CON0 &= ~ BIT(22);
    }

    if (irq_flag & I2C_IRQ_FLAG_RX_ERROR) {
        hw->CON1 &= ~ BIT(8);
    }

    return RET_OK;
}

int32 hgi2c_attach(uint32 dev_id, struct hgi2c *i2c)
{

    struct hgi2c *dev = (struct hgi2c *)i2c;

    i2c->dev.open           = hgi2c_open;
    i2c->dev.close          = hgi2c_close;
    i2c->dev.ioctl          = hgi2c_ioctl;
    i2c->dev.baudrate       = hgi2c_set_baudrate;
    i2c->dev.request_irq    = hgi2c_request_irq;
    i2c->dev.read           = hgi2c_read;
    i2c->dev.write          = hgi2c_write;
    i2c->dev.release_irq    = hgi2c_release_irq;
    i2c->irq_hdl            = NULL;
    i2c->irq_data           = 0;
    i2c->opened             = 0;
    i2c->flag_rx_done       = 1;
    i2c->flag_tx_done       = 1;
    i2c->irq_rx_done_en     = 0;
    i2c->irq_tx_done_en     = 0;
    
    os_mutex_init(&i2c->i2c_lock);
    os_sema_init(&i2c->i2c_done, 0);

    request_irq(dev->irq_num, hgi2c_irq_handler, dev);

    dev_register(dev_id, (struct dev_obj *)i2c);
    return RET_OK;
}


