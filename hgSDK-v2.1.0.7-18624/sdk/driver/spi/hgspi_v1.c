#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "dev/spi/hgspi_v1.h"
#include "hal/gpio.h"
#include "hgspi_v1_hw.h"

/**********************************************************************************/
/*                        SPI LOW LAYER FUNCTION                                  */
/**********************************************************************************/
static int32 hgspi_v1_switch_hal_work_mode(enum spi_work_mode work_mode) {

    switch (work_mode) {
        case (SPI_MASTER_MODE   ):
            return 0;
            break;
        case (SPI_SLAVE_MODE    ):
            return 1;
            break;
        case (SPI_SLAVE_FSM_MODE):
            return 2;
            break;
        default:
            return -1;
            break;
    }
}

static int32 hgspi_v1_switch_hal_wire_mode(enum spi_wire_mode wire_mode) {

    switch (wire_mode) {
        case (SPI_WIRE_NORMAL_MODE):
            return 0;
            break;
        case (SPI_WIRE_SINGLE_MODE):
            return 1;
            break;
        case (SPI_WIRE_DUAL_MODE  ):
            return 2;
            break;
        case (SPI_WIRE_QUAD_MODE  ):
            return 3;
            break;
        default:
            return -1;
            break;
    }
}

static int32 hgspi_v1_switch_hal_clk_mode(enum spi_wire_mode clk_mode) {

    switch (clk_mode) {
        case (SPI_CPOL_0_CPHA_0):
            return 0;
            break;
        case (SPI_CPOL_0_CPHA_1):
            return 1;
            break;
        case (SPI_CPOL_1_CPHA_0):
            return 2;
            break;
        case (SPI_CPOL_1_CPHA_1):
            return 3;
            break;
        default:
            return -1;
            break;
    }
}




static inline void hgspi_v1_disable(struct hgspi_v1_hw *p_spi)
{
    p_spi->CON1 &= ~LL_SPI_CON1_SSP_EN;
}

static inline void hgspi_v1_enable(struct hgspi_v1_hw *p_spi)
{
    p_spi->CON1 |= LL_SPI_CON1_SSP_EN;
}

static inline void hgspi_v1_set_dir_tx(struct hgspi_v1_hw *p_spi)
{
    p_spi->CON1 |= LL_SPI_CON1_TX_EN;
}

static inline void hgspi_v1_set_dir_rx(struct hgspi_v1_hw *p_spi)
{
    p_spi->CON1 &= ~LL_SPI_CON1_TX_EN;
}

/**
  * @brief  SPI module test whether spi is send over function
  * @param  p_spi       : Select the initialized SPI module pointer
  * @retval 1           : over
  *         0           : not over
  */
static inline int32 hgspi_v1_get_tx_fifo_empty(struct hgspi_v1_hw *p_spi)
{
    return ((p_spi->STA & LL_SPI_STA_BUF_EMPTY_PENDING) != 0);
}

/**
  * @brief  SPI module test whether spi is busy function
  * @param  p_spi       : Select the initialized SPI module pointer
  * @retval 1           : busy
  *         0           : not busy
  */
static inline int32 hgspi_v1_get_busy(struct hgspi_v1_hw *p_spi)
{
    return ((p_spi->STA & LL_SPI_STA_SSP_BUSY_PENDING) != 0);
}

/**
  * @brief  SPI module wait spi send over function
  * @param  p_spi       : Select the initialized SPI module pointer
  * @retval None
  */
static inline void hgspi_v1_wait_tx_done(struct hgspi_v1_hw *p_spi)
{
    while (!hgspi_v1_get_tx_fifo_empty(p_spi));
    while (hgspi_v1_get_busy(p_spi));
}

/**
  * @brief  SPI module test whether spi is ready to receive function
  * @param  p_spi       : Select the initialized SPI module pointer
  * @retval 1           : ready
  *         0           : not ready
  */
static inline int32 hgspi_v1_get_rx_ready_sta(struct hgspi_v1_hw *p_spi)
{
    return (!(p_spi->STA & LL_SPI_STA_BUF_EMPTY_PENDING));
}

/**
  * @brief  SPI module read data from spi receive fifo function
  * @param  p_spi       : Select the initialized SPI module pointer
  * @retval data received
  */
static inline int32 hgspi_v1_get_data(struct hgspi_v1_hw *p_spi)
{
    return (int32_t)p_spi->CMD_DATA;
}

static inline void hgspi_v1_set_data(struct hgspi_v1_hw *p_spi, uint32 data)
{
    p_spi->CMD_DATA = data;
}


/**
 * @brief   receive one char from spi in poll method, blocked function
 * @param[in]   p_spi : spi register structure pointer
 * @retval  data received by the spi
 */
static inline int32 hgspi_v1_poll_rcv_dat(struct hgspi_v1_hw *p_spi)
{
    hgspi_v1_set_dir_rx(p_spi);
    /** wait until spi is ready to receive */
    while (!hgspi_v1_get_rx_ready_sta(p_spi)); /* blocked */
    /** receive data */
    return hgspi_v1_get_data(p_spi);
}

/**
 * @brief   Set spi mode
 * @param   p_spi     : spi register structure pointer
 * @param   mode      : TYPE_ENUM_LL_SPI_MODE
 * @retval  None
 */
static inline void hgspi_v1_set_mode(struct hgspi_v1_hw *p_spi, uint32 mode)
{
    hgspi_v1_wait_tx_done(p_spi);

    hgspi_v1_disable(p_spi);
    p_spi->CON0 = (p_spi->CON0 &~ LL_SPI_CON0_SPI_MODE(0x3)) | (mode);
    hgspi_v1_enable(p_spi);
}

/**
  * @brief  SPI module test whether spi is ready to send function
  * @param  p_spi       : Select the initialized SPI module pointer
  * @retval 1           : ready
  *         0           : not ready
  */
static inline uint32 hgspi_v1_get_tx_ready_sta(struct hgspi_v1_hw *p_spi)
{
    return (!(p_spi->STA & LL_SPI_STA_BUF_FULL_PENDING));
}

/**
 * @brief   send char by spi in poll method, blocked function
 * @param[in]   p_spi : spi register structure pointer
 * @param[in]   data        data to be sent
 */
static inline void hgspi_v1_poll_send_dat(struct hgspi_v1_hw *p_spi, uint32 data)
{
    hgspi_v1_set_dir_tx(p_spi);
    /** wait until spi is ready to send */
    while (!hgspi_v1_get_tx_ready_sta(p_spi)); /* blocked */
    /** send char */
    hgspi_v1_set_data(p_spi, data);
}

/**
  * @brief  Low layer SPI sets wire mode
  * @param  p_spi    : The structure pointer of the SPI group is selected
  * @param  wire_mode: The wire mode to be set
  * @retval None
  */
static int32 hgspi_v1_set_wire_mode(struct hgspi_v1_hw *p_spi, uint32 wire_mode)
{
    hgspi_v1_wait_tx_done(p_spi);

    hgspi_v1_disable(p_spi);
    p_spi->CON0 = (p_spi->CON0 & (~ LL_SPI_CON0_WIRE_MODE(0x3))) | LL_SPI_CON0_WIRE_MODE(wire_mode);
    hgspi_v1_enable(p_spi);
    
    return RET_OK;
}

/**
  * @brief  Low layer SPI gets wire mode
  * @param  p_spi    : The structure pointer of the SPI group (SPI0, SPI1) is selected
  * @retval Return the spi wire mode
  */
static uint32 hgspi_v1_get_wire_mode(struct hgspi_v1_hw *p_spi)
{
    return LL_SPI_CON0_WIRE_MODE_GET(p_spi->CON0);
}

static int32 hgspi_v1_set_frame_size(struct hgspi_v1_hw *p_spi, uint32 frame_size) {
    if (frame_size < 0 || frame_size > 32) {
        return -EINVAL;
    }

    hgspi_v1_disable(p_spi);
    p_spi->CON0 = (p_spi->CON0 &~ LL_SPI_CON0_FRAME_SIZE(0x3F)) | LL_SPI_CON0_FRAME_SIZE(frame_size);
    hgspi_v1_enable(p_spi);

    return RET_OK;
}

static int32 hgspi_v1_sample_delay(struct hgspi_v1_hw *p_spi, uint32 value) {
    if (value < 0 || value > 255) {
        return -EINVAL;
    }

    hgspi_v1_disable(p_spi);
    p_spi->SSP_DLY = (p_spi->SSP_DLY &~ LL_SPI_AND_IIC_SSP_DLY(0xFF)) | LL_SPI_AND_IIC_SSP_DLY(value);
    hgspi_v1_enable(p_spi);

    return RET_OK;
}

//Understand it as a host(spi slave fsm mode)
static inline int32 hgspi_v1_spi_slave_fsm_get_rx_done_sta(struct hgspi_v1_hw *p_spi) {
    return (p_spi->SLAVESTA & LL_SPI_SLAVE_STA_TX_READY_FLG) ? FALSE : TRUE;
}

//Understand it as a host(spi slave fsm mode)
static inline void hgspi_v1_spi_slave_fsm_set_rx_ready_sta(struct hgspi_v1_hw *p_spi, uint32 len) {
    p_spi->SLAVESTA &= ~ LL_SPI_SLAVE_STA_TX_LEN(0xFF);
    p_spi->SLAVESTA |=   LL_SPI_SLAVE_STA_TX_LEN(len - 1)     |
                         LL_SPI_SLAVE_STA_TX_READY_EN         | 
                         LL_SPI_SLAVE_STA_TX_READY_FLG;
}

//Understand it as a host(spi slave fsm mode)
static inline int32 hgspi_v1_spi_slave_fsm_get_tx_done_sta(struct hgspi_v1_hw *p_spi) {
    return (p_spi->SLAVESTA & LL_SPI_SLAVE_STA_RX_READY_FLG) ? FALSE : TRUE;
}

//Understand it as a host(spi slave fsm mode)
static inline void hgspi_v1_spi_slave_fsm_set_tx_ready_sta(struct hgspi_v1_hw *p_spi, uint32 len) {
    p_spi->SLAVESTA &= ~ LL_SPI_SLAVE_STA_RX_LEN(0xFF);
    p_spi->SLAVESTA |=   LL_SPI_SLAVE_STA_RX_LEN(len - 1)     |
                         LL_SPI_SLAVE_STA_RX_READY_EN         | 
                         LL_SPI_SLAVE_STA_RX_READY_FLG;
}

static inline int32 hgspi_v1_spi_set_len_threshold(struct hgspi_v1 *p_spi, uint32 len_threshold) {
    p_spi->len_threshold = len_threshold;

    return RET_OK;
}


static inline void hgspi_v1_set_cs(struct hgspi_v1_hw *p_spi, uint32 value)
{
    if (value) {
        p_spi->CON0 |= LL_SPI_CON0_NSS_HIGH;
    } else {
        p_spi->CON0 &= ~LL_SPI_CON0_NSS_HIGH;
    }
}

static inline int32 hgspi_v1_complete(struct hgspi_v1 *p_spi)
{
    struct hgspi_v1 *dev = p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    /* spi tx */
    if (hw->CON1 & LL_SPI_CON1_TX_EN) {
        /* spi slave */
        if (hw->CON1 & LL_SPI_CON1_MODE(1)) {
            /* SPI slave use dma transfer, so dma done pending is 1, transfer has done */
            /* SPI slave detect cs rising, so transfer has done when cs rising */
            while ((!(hw->STA & LL_SPI_STA_NSS_POS_PENDING)) && (!(dev->spi_tx_done)));
            hw->STA =  LL_SPI_STA_NSS_POS_PENDING;
            dev->spi_tx_done  = 0;
        }
        /* spi master */
        else if (!(hw->CON1 & LL_SPI_CON1_MODE(1))) {
            while (!(dev->spi_tx_done));
            dev->spi_tx_done = 0;
        }

    } 
    /* spi rx */
    else {
        /* spi slave */
        if (hw->CON1 & LL_SPI_CON1_MODE(1)) {
            /* SPI slave use dma transfer, so dma done pending is 1, transfer has done */
            /* SPI slave detect cs rising, so transfer has done when cs rising */
            while ((!(hw->STA & LL_SPI_STA_NSS_POS_PENDING)) && (!(dev->spi_rx_done)));
            hw->STA =  LL_SPI_STA_NSS_POS_PENDING;
            dev->spi_rx_done  = 0;
        }
        /* spi master */
        else if (!(hw->CON1 & LL_SPI_CON1_MODE(1))) {
            while (!(dev->spi_rx_done));
            dev->spi_rx_done = 0;
        }
    }



    return RET_OK;
}

static int32 hgspi_v1_cs(struct hgspi_v1 *dev, uint32 cs, uint32 value)
{
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    hgspi_v1_set_cs(hw, value);

    return RET_OK;
}


static int32 hgspi_v1_set_cfg_none_cs(struct hgspi_v1 *dev, uint32 enable) {

    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if (enable) {
        hw->CON0 = (hw->CON0) &~ (LL_SPI_CON0_NSS_PIN_EN);
    } else {
        hw->CON0 |= (LL_SPI_CON0_NSS_PIN_EN);
    }

    return RET_OK;
}

static int32 hgspi_v1_read_dma_rx_cnt(struct hgspi_v1 *dev, uint32 *cnt_buf) {

    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if (!cnt_buf) {
        return RET_ERR;
    }

    /* 硬件要求：rx cnt为rxdma_cnt + buf_cnt */
    *cnt_buf = hw->RDMACNT + LL_SPI_STA_BUF_CNT(hw->STA);

    return RET_OK;
}

static int32 hgspi_v1_kick_dma_rx(struct hgspi_v1 *dev, uint32 *rx_buf, uint32 len) {

    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if ((!rx_buf) || (!len)) {
        return RET_ERR;
    }

    if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {
        hw->RDMACNT = 0;
        hw->RSTADR  = (uint32)rx_buf;
        hw->RDMALEN = len;
        hgspi_v1_spi_slave_fsm_set_rx_ready_sta(hw, len/8);
    } else {
        /* 
         *硬件要求：
         *为防止内部8bit移位器残留bit，影响后续正常接发
         *重新开关一下SPI EN，以清掉残留bit
        */

        hw->CON0 &= ~ LL_SPI_CON1_SSP_EN;
        hw->CON0 |=   LL_SPI_CON1_SSP_EN;
        
        /* 
            硬件要求：未达到DMA done的情况下，重新kick DMA
            需要先把DMA的EN关掉
        */
        hw->CON1 &= ~ LL_SPI_CON1_DMA_EN;

        /* 清掉BUF cnt */
        hw->STA = LL_SPI_STA_CLEAR_BUF_CNT;
        
        hw->RDMACNT = 0;
        hw->RSTADR  = (uint32)rx_buf;
        hw->RDMALEN = len;
        hgspi_v1_set_dir_rx(hw);
        hw->CON1 |= LL_SPI_CON1_DMA_EN;
    }

    return RET_OK;
}

static int32 hgspi_v1_kick_dma_tx(struct hgspi_v1 *dev, uint32 *tx_buf, uint32 len) {

    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if ((!tx_buf) || (!len)) {
        return RET_ERR;
    }
    
    /* 
     *硬件要求：
     *为防止内部8bit移位器残留bit，影响后续正常接发
     *重新开关一下SPI EN，以清掉残留bit
    */

    hw->CON0 &= ~ LL_SPI_CON1_SSP_EN;
    hw->CON0 |=   LL_SPI_CON1_SSP_EN;

    /* 
        硬件要求：未达到DMA done的情况下，重新kick DMA
        需要先把DMA的EN关掉
    */
    hw->CON1 &= ~ LL_SPI_CON1_DMA_EN;

    /* 清掉BUF cnt */
    hw->STA = LL_SPI_STA_CLEAR_BUF_CNT;

    hw->TDMACNT = 0;
    hw->TSTADR  = (uint32)tx_buf;
    hw->TDMALEN = len;
    hgspi_v1_set_dir_tx(hw);
    hw->CON1 |= LL_SPI_CON1_DMA_EN;

    return RET_OK;
}

static int32 hgspi_v1_set_timeout(struct hgspi_v1 *dev, uint32 time) {

    dev->timeout = time;

    return RET_OK;
}

static int32 hgspi_v1_set_lsb_first(struct hgspi_v1 *dev, uint32 enable) {

    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if (enable) {
        hw->CON0 |= LL_SPI_CON0_LSB_FIRST;
    } else {
        hw->CON0 &= ~LL_SPI_CON0_LSB_FIRST;
    }

    return RET_OK;
}

static int32 hgspi_v1_set_slave_sw_reserved(struct hgspi_v1 *dev, uint32 enable) {

    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if (enable) {
        hw->SLAVESTA |= LL_SPI_SLAVE_STA_SW_RESERVED;
    } else {
        hw->SLAVESTA &= ~LL_SPI_SLAVE_STA_SW_RESERVED;
    }

    return RET_OK;
}


/**********************************************************************************/
/*                          SPI ATTCH FUNCTION                                    */
/**********************************************************************************/

static int32 hgspi_v1_open(struct spi_device *p_spi, uint32 clk_freq, uint32 work_mode,uint32 wire_mode, uint32 clk_mode) {

    struct hgspi_v1 *dev   = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;
    uint32 con0_reg        = hw->CON0;
    uint32 con1_reg        = hw->CON1;
    uint32 clk_div_cnt;
    int32  work_mode_to_reg;
    int32  wire_mode_to_reg;
    int32  clk_mode_to_reg ;

    if (dev->opened) {
        return -EBUSY;
    }

    /* hal enum */
    work_mode_to_reg = hgspi_v1_switch_hal_work_mode(work_mode);
    wire_mode_to_reg = hgspi_v1_switch_hal_wire_mode(wire_mode);
    clk_mode_to_reg  = hgspi_v1_switch_hal_clk_mode(clk_mode  );
    
    if (((-1) == work_mode_to_reg) || \
        ((-1) == wire_mode_to_reg) || \
        ((-1) == clk_mode_to_reg )) {
        
        return -EINVAL;
    }

    /* pin config */
    if(pin_func(p_spi->dev.dev_id, 1) != RET_OK) {
        return RET_ERR;
    }

    hgspi_v1_disable(hw);

    con0_reg = LL_SPI_CON0_SPI_MODE(clk_mode_to_reg  )                        | //clk mode
               LL_SPI_CON0_WIRE_MODE(wire_mode_to_reg)                        | //wire mode
               ((work_mode_to_reg == 2) ? (LL_SPI_CON0_SLAVE_STATE_EN):(0UL)) | //slave fsm mode
               LL_SPI_CON0_FRAME_SIZE(8)                                      | //frame size
               LL_SPI_CON0_NSS_PIN_EN;

    con1_reg = LL_SPI_CON1_SPI_I2C_SEL(0x0)                                   |
               ((work_mode_to_reg == 2) ? (LL_SPI_CON1_MODE(1)) : LL_SPI_CON1_MODE(work_mode_to_reg));

    hw->CON0 = con0_reg  ;
    hw->CON1 = con1_reg  ;
    hw->STA  = 0xFFFFFFFF;

    /* only master mode need config clk */
    if (work_mode_to_reg == 0) {
        /* set spi clk */
        clk_div_cnt = peripheral_clock_get(HG_APB0_PT_SPI0) / 2 / clk_freq - 1;
        ASSERT((clk_div_cnt >= 0) && (clk_div_cnt <= 65535));
        hw->BAUD = clk_div_cnt;
    }

    if (!(hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN)) {
        /* open dma done interrupt for write&read func */
        hw->CON1 |= LL_SPI_CON1_DMA_IE_EN;
    }
    
    irq_enable(dev->irq_num);

    /* set CS high */
    hgspi_v1_set_cs(hw, 1);

    /* enable spi */
    hgspi_v1_enable(hw);
    
    dev->opened = 1;
    /* config default tx&rx len threshold, uint:byte */
    dev->len_threshold        = 5;
    /* other config init */
    dev->spi_irq_flag_rx_done = 0;
    dev->spi_irq_flag_tx_done = 0;
    dev->spi_tx_done          = 0;
    dev->spi_rx_done          = 0;
    dev->spi_tx_async         = 0;
    dev->spi_rx_async         = 0;
    dev->timeout              = osWaitForever;
    
    return RET_OK;
}

static int32 hgspi_v1_close(struct spi_device *p_spi) {

    struct hgspi_v1 *dev = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if (!dev->opened) {
        return RET_OK;
    }
    
    irq_disable(dev->irq_num     );
    pin_func(p_spi->dev.dev_id, 0);
    hgspi_v1_set_cs(hw, 1);
    hgspi_v1_disable(hw  );
    
    dev->opened = 0;
    
    /* Just to be sure: config again default tx&rx len threshold when close, uint:byte */
    dev->len_threshold        = 5;
    /* other setting clear */
    dev->spi_irq_flag_rx_done = 0;
    dev->spi_irq_flag_tx_done = 0;
    dev->spi_tx_done          = 0;
    dev->spi_rx_done          = 0;
    dev->spi_tx_async         = 0;
    dev->spi_rx_async         = 0;
    dev->timeout              = osWaitForever;
    
    return RET_OK;
}

static int32 hgspi_v1_write(struct spi_device *p_spi, const void *buf, uint32 size) {

    struct hgspi_v1 *dev = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;
    uint32 frame_size, index, data;
    uint32 num, addr;

    os_mutex_lock(&dev->os_spi_lock, osWaitForever);
    
    /* if mode is slave fsm mode */
    if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {

        //while (!hgspi_v1_spi_slave_fsm_get_tx_done_sta(hw));

        hw->TDMACNT = 0;
        /* 清掉BUF cnt，对于slave fsm模式有影响，因为不知道接收时刻，写清除buf会影响可能正在接收的过程 */
        //hw->STA = LL_SPI_STA_CLEAR_BUF_CNT;

        hw->TDMALEN = size;
        hw->TSTADR  = (uint32)buf;

        hgspi_v1_spi_slave_fsm_set_tx_ready_sta(hw, size/8);

        os_sema_down(&dev->os_spi_tx_done, dev->timeout);

        os_mutex_unlock(&dev->os_spi_lock);

        return RET_OK;
    }

    /* Clear the last cs rising pending */
    hw->STA = LL_SPI_STA_NSS_POS_PENDING;
    /* Clear the rx_dma cnt & tx_dma_cnt */
    hw->TDMACNT = 0;
    /* 清掉BUF cnt */
    hw->STA = LL_SPI_STA_CLEAR_BUF_CNT;

    /* clear the tx done pending */
    dev->spi_tx_done  = 0;
    dev->spi_tx_async = 0;

    hgspi_v1_set_dir_tx(hw);

    /* len <= 5(default) use cpu tx dev->tx_len_threshold*/
    if (size <= dev->len_threshold) {

        /* if mode == slave, use dma tx */
        if (hw->CON1 & LL_SPI_CON1_MODE(1)) {
            goto hgspi_v1_dma_tx;
        }

        addr       = (uint32)buf;
        frame_size = LL_SPI_CON0_GET_FRAME_SIZE(hw->CON0);
        index      = (frame_size > 8) + (frame_size > 16);
        num        = size >> index;

        /* Note: "spi slave mode" set cs is invaild, so it is ok */
        //hgspi_v1_set_cs(hw, 0);

        while(num--) {
            switch(index) {
                case 0:
                    data = *(uint8  *)addr;
                    break;
                case 1:
                    data = *(uint16 *)addr;
                    break;
                default:
                    data = *(uint32 *)addr;
                    break;
            }
            addr += 1<<index;

            while(!hgspi_v1_get_tx_ready_sta(hw));
            hgspi_v1_set_data(hw, data);
        }

        /* Only required for the master mode */
        if(!LL_SPI_CON1_GET_MODE(hw->CON1)) {
            hgspi_v1_wait_tx_done(hw);
        }

        /* Note: "spi slave mode" set cs is invaild, so it is ok */
        //hgspi_v1_set_cs(hw, 1);

        /* cpu tx done irq handle */
        if (dev->spi_irq_flag_tx_done) {
            if (dev->irq_hdl) {
                dev->irq_hdl(SPI_IRQ_FLAG_TX_DONE, dev->irq_data);
            }
        }
    }
    /* len > 5(default) use dma tx*/
    else {

hgspi_v1_dma_tx:

        while (hw->STA & LL_SPI_STA_SSP_BUSY_PENDING);
    
        /* Note: "spi slave mode" set cs is invaild, so it is ok */
        //hgspi_v1_set_cs(hw, 0);
        dev->spi_tx_async = 1;

        hw->TSTADR  = (uint32)buf;
        hw->TDMALEN = size;
        hw->CON1   |= LL_SPI_CON1_DMA_EN;

        #if 0
        //using semaphore for waiting tx done
        os_sema_down(&dev->os_spi_tx_done, osWaitForever);
        #else
        /* others: using semaphore to wait */
        if (1) {
            //using semaphore for waiting tx done
            os_sema_down(&dev->os_spi_tx_done, dev->timeout);
        }
        /* 0: using while to wait */
        else {
            dev->spi_tx_async = 0;
            hgspi_v1_complete(dev);
        }
        #endif
    }

    /* 防止timeout后，突然来数据，触发dma中断 */
    /* 正常接收接收完成，写它没有事 */
    hw->CON1 &= ~LL_SPI_CON1_DMA_EN;
    
    os_mutex_unlock(&dev->os_spi_lock);

    return RET_OK;
}

static int32 hgspi_v1_read(struct spi_device *p_spi, void *buf, uint32 size) {

    struct hgspi_v1 *dev = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;
    uint32 frame_size, index, data;
    uint32 num, addr;

    os_mutex_lock(&dev->os_spi_lock, osWaitForever);
    
    /* if mode is slave fsm mode */
    if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {

        while (!hgspi_v1_spi_slave_fsm_get_rx_done_sta(hw));

        hw->RDMACNT = 0;
        /* 清掉BUF cnt */
        hw->STA = LL_SPI_STA_CLEAR_BUF_CNT;

        hw->RDMALEN = size;
        hw->RSTADR  = (uint32)buf;

        hgspi_v1_spi_slave_fsm_set_rx_ready_sta(hw, size/8);

        os_mutex_unlock(&dev->os_spi_lock);
        
        return RET_OK;
    }
    
    /* Clear the last cs rising pending */
    hw->STA =  LL_SPI_STA_NSS_POS_PENDING;
    /* Clear the rx_dma cnt & tx_dma_cnt */
    hw->RDMACNT = 0;
    /* 清掉BUF cnt */
    hw->STA = LL_SPI_STA_CLEAR_BUF_CNT;

    /* clear the rx done pending */
    dev->spi_rx_done  = 0;
    dev->spi_rx_async = 0;
    gpio_set_val(PA_4, 0);

    hgspi_v1_set_dir_rx(hw);

    /* len <= 5(default) use cpu tx dev->tx_len_threshold*/
    if (size <= dev->len_threshold) {

        /* if mode == slave, use dma rx */
        if (hw->CON1 & LL_SPI_CON1_MODE(1)) {
            goto hgspi_v1_dma_rx;
        }
        
        addr       = (uint32)buf;
        frame_size = LL_SPI_CON0_GET_FRAME_SIZE(hw->CON0);
        index      = (frame_size > 8) + (frame_size > 16);
        num        = size >> index;

        /* Note: "spi slave mode" set cs is invaild, so it is ok */
        //hgspi_v1_set_cs(hw, 0);
        
        /* Only required for the master mode */
        if(!LL_SPI_CON1_GET_MODE(hw->CON1)) {
            hw->RDMALEN = LL_SPI_DMA_RX_LEN(num);
            hgspi_v1_set_data(hw, 0x00);
        }
        
        while(num--) {
            
            while(!hgspi_v1_get_rx_ready_sta(hw));
            data = hgspi_v1_get_data(hw);
        
            switch(index) {
                case 0:
                    *(uint8 *)addr = data;
                    break;
                case 1:
                    *(uint16 *)addr = data;
                    break;
                default:
                    *(uint32 *)addr = data;
                    break;
            }
            addr += 1<<index;
       }

        /* Note: "spi slave mode" set cs is invaild, so it is ok */
        //hgspi_v1_set_cs(hw, 1);

        /* cpu rx done irq handle */
        if (dev->spi_irq_flag_rx_done) {
            if (dev->irq_hdl) {
                dev->irq_hdl(SPI_IRQ_FLAG_RX_DONE, dev->irq_data);
            }
        }
    }
    /* len > 5(default) use dma tx*/
    else {

hgspi_v1_dma_rx:

        while (hw->STA & LL_SPI_STA_SSP_BUSY_PENDING);
    
        /* Note: "spi slave mode" set cs is invaild, so it is ok */
        //hgspi_v1_set_cs(hw, 0);
        dev->spi_rx_async = 1;
        
        hw->RSTADR  = (uint32)buf;
        hw->RDMALEN = size;
        hw->CON1   |= LL_SPI_CON1_DMA_EN;

        #if 0
        //using semaphore for waiting tx done
        os_sema_down(&dev->os_spi_rx_done, osWaitForever);
        #else
        /* others: using semaphore to wait */
        if (1) {
            //using semaphore for waiting tx done
            os_sema_down(&dev->os_spi_rx_done, dev->timeout);
        }
        /* 0: using while to wait */
        else {
            dev->spi_rx_async = 0;
            hgspi_v1_complete(dev);
        }
        #endif


    }

    /* 防止timeout后，突然来数据，触发dma中断 */
    /* 正常接收接收完成，写它没有事 */
    hw->CON1 &= ~LL_SPI_CON1_DMA_EN;

    os_mutex_unlock(&dev->os_spi_lock);

    return RET_OK;
}



static int32 hgspi_v1_ioctl(struct spi_device *p_spi, uint32 cmd, uint32 param1, uint32 param2)
{
    int32 ret_val = RET_OK;
    struct hgspi_v1 *dev = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    switch(cmd) {
        case (SPI_SET_FRAME_SIZE   ):
            ret_val = hgspi_v1_set_frame_size(hw, param1);
            break;
        case (SPI_SAMPLE_DELAY     ):
            ret_val = hgspi_v1_sample_delay(hw  , param1);
            break;
        case (SPI_WIRE_MODE_SET    ):
            ret_val = hgspi_v1_set_wire_mode(hw , param1);
            break;
        case (SPI_WIRE_MODE_GET    ):
            ret_val = hgspi_v1_get_wire_mode(hw);
            break;
        case (SPI_SET_LEN_THRESHOLD):
            ret_val = hgspi_v1_spi_set_len_threshold(dev, param1);
            break;
        case (SPI_SET_CS):
            ret_val = hgspi_v1_cs(dev, param1, param2);
            break;
        case (SPI_CFG_SET_NONE_CS):
            ret_val = hgspi_v1_set_cfg_none_cs(dev, param1);
            break;
        case (SPI_READ_DMA_RX_CNT):
            ret_val = hgspi_v1_read_dma_rx_cnt(dev, (uint32 *)param1);
            break;
        case (SPI_KICK_DMA_RX):
            ret_val = hgspi_v1_kick_dma_rx(dev, (uint32 *)param1, param2);
            break;
        case (SPI_KICK_DMA_TX):
            ret_val = hgspi_v1_kick_dma_tx(dev, (uint32 *)param1, param2);
            break;
        case (SPI_SET_TIMEOUT):
            ret_val = hgspi_v1_set_timeout(dev, param1);
            break;
        case (SPI_SET_LSB_FIRST):
            ret_val = hgspi_v1_set_lsb_first(dev, param1);
            break;
        case (SPI_SET_SLAVE_SW_RESERVED):
            ret_val = hgspi_v1_set_slave_sw_reserved(dev, param1);
            break;
        default:
            ret_val = -ENOTSUPP;
            break;
    }
    
    return ret_val;
}

/* interrupt handler */
static void hgspi_v1_irq_handler(void *data)
{
    struct hgspi_v1 *dev = (struct hgspi_v1 *)data;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if ((hw->CON1 & LL_SPI_CON1_DMA_IE_EN) && (hw->STA & LL_SPI_STA_DMA_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_DMA_PENDING;
        /* For spi write&read, Note: "spi slave mode" set cs is invaild */
        //hgspi_v1_set_cs(hw, 1);

        /* spi transfer done, recording for tx_sync & rx_sync */
        if (!dev->spi_rx_async) {
            dev->spi_rx_done = 1;
        }

        if (!dev->spi_tx_async) {
            dev->spi_tx_done = 1;
        }

        //spi module async tx 
        if (dev->spi_tx_async) {
            dev->spi_tx_async = 0;
            os_sema_up(&dev->os_spi_tx_done);
        }
        
        //spi module async rx 
        if (dev->spi_rx_async){
            dev->spi_rx_async = 0;
            os_sema_up(&dev->os_spi_rx_done);
        }

        if (hw->CON1 & LL_SPI_CON1_TX_EN) {
            if (dev->irq_hdl && dev->spi_irq_flag_tx_done) {
                dev->irq_hdl(SPI_IRQ_FLAG_TX_DONE, dev->irq_data);
            }
        } else {
            if (dev->irq_hdl && dev->spi_irq_flag_rx_done) {
                dev->irq_hdl(SPI_IRQ_FLAG_RX_DONE, dev->irq_data);
            }
        }
    }



    if ((hw->CON1 & LL_SPI_CON1_BUF_OV_IE_EN) && (hw->STA & LL_SPI_STA_BUF_OV_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_BUF_OV_PENDING;
        
        if (dev->irq_hdl) {
            dev->irq_hdl(SPI_IRQ_FLAG_FIFO_OVERFLOW, dev->irq_data);
        }
    }



    if ((hw->CON0 & LL_SPI_CON0_NSS_POS_IE_EN) && (hw->STA & LL_SPI_STA_NSS_POS_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_NSS_POS_PENDING;
        
        if (dev->irq_hdl) {
            dev->irq_hdl(SPI_IRQ_FLAG_CS_RISING, dev->irq_data);
        }

        /* spi salve transfer done when detect cs rising */
        if ((hw->CON1 & LL_SPI_CON1_MODE(1))) {
            if (!dev->spi_tx_async) {
                dev->spi_tx_done = 1;
            }
            
            if (!dev->spi_rx_async) {
                dev->spi_rx_done = 1;
            }
        }
    }


    //// SLAVE FSM IRQ ////
    if ((hw->CON0 & LL_SPI_CON0_SLAVE_WRDATA_IE_EN) && (hw->STA & LL_SPI_STA_SLAVE_WRDATA_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_SLAVE_WRDATA_PENDING;
        
        if (dev->irq_hdl) {
            dev->irq_hdl(SPI_IRQ_FLAG_RX_DONE, dev->irq_data);
        }
    }
    if ((hw->CON0 & LL_SPI_CON0_SLAVE_RDDATA_IE_EN) && (hw->STA & LL_SPI_STA_SLAVE_RDDATA_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_SLAVE_RDDATA_PENDING;
        os_sema_up(&dev->os_spi_tx_done);
        if (dev->irq_hdl) {
            dev->irq_hdl(SPI_IRQ_FLAG_TX_DONE, dev->irq_data);
        }
    }
    if ((hw->CON0 & LL_SPI_CON0_SLAVE_RDSTATUS_IE_EN) && (hw->STA & LL_SPI_STA_SLAVE_RDSTATUS_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_SLAVE_RDSTATUS_PENDING;
        
        if (dev->irq_hdl) {
            dev->irq_hdl(SPI_IRQ_FLAG_SLAVE_FSM_READ_STATUS, dev->irq_data);
        }
    }
    if ((hw->CON0 & LL_SPI_CON0_SLAVE_WRONGCMD_IE_EN) && (hw->STA & LL_SPI_STA_SLAVE_WRONG_CMD_PENDING)) {
        /* clear interrupt flag */
        hw->STA = LL_SPI_STA_SLAVE_WRONG_CMD_PENDING;
        
        if (dev->irq_hdl) {
            dev->irq_hdl(SPI_IRQ_FLAG_SLAVE_FSM_WRONG_CMD, dev->irq_data);
        }
    }
}

static int32 hgspi_v1_request_irq(struct spi_device *p_spi, uint32 irq_flag, spi_irq_hdl irqhdl, uint32 irq_data)
{
    struct hgspi_v1 *dev = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;
    
    dev->irq_hdl  = irqhdl;
    dev->irq_data = irq_data;
    request_irq(dev->irq_num, hgspi_v1_irq_handler, dev);

    if (irq_flag & SPI_IRQ_FLAG_TX_DONE) {
        if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {
            hw->CON0 |= LL_SPI_CON0_SLAVE_RDDATA_IE_EN;
        } else {
            dev->spi_irq_flag_tx_done = 1;
            hw->CON1 |= LL_SPI_CON1_DMA_IE_EN;
        }
    }

    if (irq_flag & SPI_IRQ_FLAG_RX_DONE) {
        if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {
            hw->CON0 |= LL_SPI_CON0_SLAVE_WRDATA_IE_EN;
        } else {
            dev->spi_irq_flag_rx_done = 1;
            hw->CON1 |= LL_SPI_CON1_DMA_IE_EN;
        }
    }

    if (irq_flag & SPI_IRQ_FLAG_CS_RISING) {
        hw->CON1 |= LL_SPI_CON0_NSS_POS_IE_EN;
    }

    if (irq_flag & SPI_IRQ_FLAG_FIFO_OVERFLOW) {
        hw->CON1 |= LL_SPI_CON1_BUF_OV_IE_EN;
    }

    if (irq_flag & SPI_IRQ_FLAG_SLAVE_FSM_READ_STATUS) {
        hw->CON0 |= LL_SPI_CON0_SLAVE_RDSTATUS_IE_EN;
    }
    
    if (irq_flag & SPI_IRQ_FLAG_SLAVE_FSM_WRONG_CMD) {
        hw->CON0 |= LL_SPI_CON0_SLAVE_WRONGCMD_IE_EN;
    }
    
    irq_enable(dev->irq_num);
    
    return RET_OK;
}

static int32 hgspi_v1_release_irq(struct spi_device *p_spi, uint32 irq_flag)
{
    struct hgspi_v1 *dev = (struct hgspi_v1 *)p_spi;
    struct hgspi_v1_hw *hw = (struct hgspi_v1_hw *)dev->hw;

    if (irq_flag & SPI_IRQ_FLAG_TX_DONE) {
        if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {
            hw->CON0 &= ~LL_SPI_CON0_SLAVE_RDDATA_IE_EN;
        } else {
            dev->spi_irq_flag_tx_done = 0;
            hw->CON1 &= ~ LL_SPI_CON1_DMA_IE_EN;
        }
    }

    if (irq_flag & SPI_IRQ_FLAG_RX_DONE) {
        if (hw->CON0 & LL_SPI_CON0_SLAVE_STATE_EN) {
            hw->CON0 &= ~LL_SPI_CON0_SLAVE_WRDATA_IE_EN;
        } else {
            dev->spi_irq_flag_rx_done = 0;
            hw->CON1 &= ~ LL_SPI_CON1_DMA_IE_EN;
        }
    }

    if (irq_flag & SPI_IRQ_FLAG_CS_RISING) {
        hw->CON1 &= ~ LL_SPI_CON0_NSS_POS_IE_EN;
    }

    if (irq_flag & SPI_IRQ_FLAG_FIFO_OVERFLOW) {
        hw->CON1 &= ~ LL_SPI_CON1_BUF_OV_IE_EN;
    }

    if (irq_flag & SPI_IRQ_FLAG_SLAVE_FSM_READ_STATUS) {
        hw->CON0 &= ~ LL_SPI_CON0_SLAVE_RDSTATUS_IE_EN;
    }
    
    if (irq_flag & SPI_IRQ_FLAG_SLAVE_FSM_WRONG_CMD) {
        hw->CON0 &= ~ LL_SPI_CON0_SLAVE_WRONGCMD_IE_EN;
    }
    return RET_OK;
}


int32 hgspi_v1_attach(uint32 dev_id, struct hgspi_v1 *p_spi)
{
    p_spi->opened               = 0;
    p_spi->irq_hdl              = NULL;
    p_spi->irq_data             = 0;
    p_spi->timeout              = osWaitForever;
    p_spi->spi_tx_done          = 0;
    p_spi->spi_tx_done          = 0;
    p_spi->spi_tx_async         = 0;
    p_spi->spi_rx_async         = 0;
    p_spi->spi_irq_flag_tx_done = 0;
    p_spi->spi_irq_flag_rx_done = 0;
    p_spi->dev.open             = hgspi_v1_open;
    p_spi->dev.close            = hgspi_v1_close;
    p_spi->dev.ioctl            = hgspi_v1_ioctl;
    p_spi->dev.read             = hgspi_v1_read;
    p_spi->dev.write            = hgspi_v1_write;
    p_spi->dev.request_irq      = hgspi_v1_request_irq;
    p_spi->dev.release_irq      = hgspi_v1_release_irq;

    os_sema_init(&p_spi->os_spi_tx_done, 0);
    os_sema_init(&p_spi->os_spi_rx_done, 0);

    //os_mutex_init(&p_spi->os_spi_tx_lock);
    //os_mutex_init(&p_spi->os_spi_rx_lock);
    os_mutex_init(&p_spi->os_spi_lock);
    
    request_irq(p_spi->irq_num, hgspi_v1_irq_handler, p_spi);
    
    dev_register(dev_id, (struct dev_obj *)p_spi);
    return RET_OK;
}
