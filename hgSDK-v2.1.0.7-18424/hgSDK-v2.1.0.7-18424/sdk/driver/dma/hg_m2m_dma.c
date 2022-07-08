#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "osal/irq.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/string.h"
#include "hal/dma.h"
#include "dev/dma/hg_m2m_dma.h"

static inline int32 hg_m2m_dma_get_free_ch(struct mem_dma_dev *dev, uint8 ch_fix)
{
    int32  ch = (ch_fix >= HG_M2M_DMA_NUM) ? (1) : ch_fix;
    uint32 flags = disable_irq();
    for (; ch>=0; ) {
        if (!(dev->busy_flag & BIT(ch))){
            dev->busy_flag |= BIT(ch);
            break;
        }
        if (ch_fix < HG_M2M_DMA_NUM) {
            ch = -1;
            break;
        } else {
            ch--;
        }
    }
    
    enable_irq(flags);
    return ch;
}

static inline void hg_m2m_dma_free_ch(struct mem_dma_dev *dev, int32 ch)
{    
    uint32 flags = disable_irq();
    dev->busy_flag &= ~ BIT(ch);
    enable_irq(flags);
}

const uint8 dma_element_size[4] = {1,2,4,8};
static int32 hg_m2m_dma_xfer(struct dma_device *dma, struct dma_xfer_data *data)
{
    int32 ch;
    uint32 val  = *((uint8 *)data->src);
    uint32 count = data->element_num * dma_element_size[data->element_per_width];
    static uint32 ch0_lock = 0;
    uint32 retry = count >> 8;
    
    struct mem_dma_dev *dev = (struct mem_dma_dev *)dma;
    struct mem_dma_hw *hw  = (struct mem_dma_hw *)dev->hw;

    if (data->element_per_width >= DMA_SLAVE_BUSWIDTH_UNDEFINED) {
        return -EBUSY;
    }
       
    /* get free channel */
    do {
        if (data->dir != DMA_XFER_DIR_M2M) {
            ch = hg_m2m_dma_get_free_ch(dev, 0);
            if (0 == ch)
                ch0_lock = 1;
        } else {
           ch = hg_m2m_dma_get_free_ch(dev, ch0_lock ? 1 : HG_M2M_DMA_NUM);
        }
        if (ch >= 0) {
            break;
        }
    } while (retry--);



    if (ch < 0) {
        //os_printf("sw：%08x, %08x, %x\r\n", data->dest, data->src, count);
        if (data->src_addr_mode == DMA_XFER_MODE_RECYCLE) {
            os_memset((void*)data->dest, val, count);
        } else {
            os_memcpy((void*)data->dest, (void*)data->src, count);
        }
        return ch;
    } 

    //sysctrl_m2m_dma_reset();
    hw->dma_ch[ch].DMA_CON  = 0x00;
    hw->dma_ch[ch].DMA_TADR = (uint32)data->dest;
    hw->dma_ch[ch].DMA_SADR = (uint32)data->src;
    hw->dma_ch[ch].DMA_DATA = val;
    hw->dma_ch[ch].DMA_DLEN = count - 1;

    if (data->src_addr_mode == DMA_XFER_MODE_RECYCLE) {
        hw->dma_ch[ch].DMA_CON  = HG_M2M_DMA_CON_DTM | HG_M2M_DMA_CON_DTE;
    } else {
        hw->dma_ch[ch].DMA_CON  = HG_M2M_DMA_CON_DTE;
    }
    while(hw->dma_ch[ch].DMA_CON & HG_M2M_DMA_CON_DTE);
    hg_m2m_dma_free_ch(dev, ch);

    return ch;
}

static int32 hg_m2m_dma_get_status(struct dma_device *dma, uint32 chn)
{
    struct mem_dma_dev *dev = (struct mem_dma_dev *)dma;
    struct mem_dma_hw *hw  = (struct mem_dma_hw *)dev->hw;
    
    if (hw->dma_ch[chn].DMA_CON & HG_M2M_DMA_CON_DTE) {
        return DMA_IN_PROGRESS;
    } else {
        return DMA_SUCCESS;
    }
}


__init int32 hg_m2m_dma_dev_attach(uint32 dev_id, struct mem_dma_dev *p_dma)
{
    p_dma->dev.pause = NULL;
    p_dma->dev.resume = NULL;
    p_dma->dev.stop = NULL;
    p_dma->dev.xfer = hg_m2m_dma_xfer;
    p_dma->dev.get_status = hg_m2m_dma_get_status;
    p_dma->busy_flag = 0;

    p_dma->hw->dma_ch[0].DMA_CON  = 0x00;
    p_dma->hw->dma_ch[0].DMA_SAIE = HG_M2M_DMA_SAIE_TCP_PENDING;
    p_dma->hw->dma_ch[1].DMA_CON  = 0x00;
    p_dma->hw->dma_ch[1].DMA_SAIE = HG_M2M_DMA_SAIE_TCP_PENDING;
    dev_register(dev_id, (struct dev_obj *)p_dma);
    return RET_OK;
}

