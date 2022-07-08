#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "hal/dma.h"

int32 dma_pause(struct dma_device *dma, uint32 ch)
{
    if(dma && dma->pause) {
        return dma->pause(dma, ch);
    }
    return RET_ERR;
}

int32 dma_resume(struct dma_device *dma, uint32 ch)
{
    if(dma && dma->resume) {
        return dma->resume(dma, ch);
    }
    return RET_ERR;
}

void dma_memcpy(struct dma_device *dma, void *dst, const void *src, uint32 n)
{
    ASSERT(dma && dma->xfer);
    
    struct dma_xfer_data xfer_data;
    xfer_data.dest              = (uint32)dst;
    xfer_data.src               = (uint32)src;
    xfer_data.element_per_width = DMA_SLAVE_BUSWIDTH_1_BYTE;
    xfer_data.element_num       = n;
    xfer_data.dir               = ((((uint32)dst) < SRAM_BASE) || (((uint32)src) < SRAM_BASE)) ? DMA_XFER_DIR_M2D : DMA_XFER_DIR_M2M;
    xfer_data.src_addr_mode     = DMA_XFER_MODE_INCREASE;
    xfer_data.dst_addr_mode     = DMA_XFER_MODE_INCREASE;
    xfer_data.dst_id            = 0;
    xfer_data.src_id            = 0;
    xfer_data.irq_hdl           = NULL;
//    xfer_data.irq_data          = 0;

    dma->xfer(dma, &xfer_data);    
}

void dma_memset(struct dma_device *dma, void *dst, uint8 c, uint32 n)
{
    uint8 val = c;
    ASSERT(dma && dma->xfer);
    
    struct dma_xfer_data xfer_data;
    xfer_data.dest              = (uint32)dst;
    xfer_data.src               = (uint32)&val;
    xfer_data.element_per_width = DMA_SLAVE_BUSWIDTH_1_BYTE;
    xfer_data.element_num       = n;
    xfer_data.dir               = (((uint32)dst) < SRAM_BASE) ? DMA_XFER_DIR_M2D : DMA_XFER_DIR_M2M;
    xfer_data.src_addr_mode     = DMA_XFER_MODE_RECYCLE;
    xfer_data.dst_addr_mode     = DMA_XFER_MODE_INCREASE;
    xfer_data.dst_id            = 0;
    xfer_data.src_id            = 0;
    xfer_data.irq_hdl           = NULL;
//    xfer_data.irq_data          = 0;

    dma->xfer(dma, &xfer_data);
}

int32 dma_xfer(struct dma_device *dma, struct dma_xfer_data *data)
{
    if(dma && dma->xfer) {
        return dma->xfer(dma, data);
    }
    return RET_ERR;
}

int32 dma_status(struct dma_device *dma, uint32 ch)
{
    if(dma && dma->get_status) {
        return dma->get_status(dma, ch);
    }
    return RET_ERR;
}

int32 dma_stop(struct dma_device *dma, uint32 ch)
{
    if(dma && dma->stop) {
        return dma->stop(dma, ch);
    }
    return RET_ERR;
}
