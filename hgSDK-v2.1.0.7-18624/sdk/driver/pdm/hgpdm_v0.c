#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "hal/pdm.h"
#include "dev/pdm/hgpdm_v0.h"
#include "hgpdm_v0_hw.h"

/**********************************************************************************/
/*                           LOW LAYER FUNCTION                                   */
/**********************************************************************************/

static int32 hgpdm_v0_switch_hal_pdm_sample_freq(enum pdm_sample_freq param) {

    switch (param) {
        case (PDM_SAMPLE_FREQ_8K):
            return 8000;
            break;
        case (PDM_SAMPLE_FREQ_16K):
            return 16000;
            break;
        case (PDM_SAMPLE_FREQ_32K):
            return 32000;
            break;
        case (PDM_SAMPLE_FREQ_48K):
            return 48000;
            break;
        default:
            return -1;
            break;
    }
}

static int32 hgpdm_v0_switch_hal_pdm_channel(enum pdm_channel param) {

    switch (param) {
        case (PDM_CHANNEL_LEFT):
            return 0;
            break;
        case (PDM_CHANNEL_RIGHT):
            return 1;
            break;
        case (PDM_CHANNEL_STEREO):
            return 2;
            break;
        default:
            return -1;
            break;
    }
}

static void hgpdm_v0_set_mclk(int32 freq) {

    uint32 temp;
    uint32 div;

    sysctrl_unlock();
    SYSCTRL->SYS_CON2 |= 1<<18; //choose pll0

#ifndef FPGA_SUPPORT
        div = (peripheral_clock_get(HG_APB0_PT_PDM)/ (freq*64*8));
#else
        float div_f;
        div_f = (96000000.0 / (freq*64*8));
        div = (uint32_t)(div_f+0.5);
#endif


    temp  = SYSCTRL->SYS_CON2;
    temp &= ~0x07F00000;
    temp |= (div - 1)<<20;            //select div
    SYSCTRL->SYS_CON2 = temp;
    sysctrl_lock();
}

static int32 hgpdm_v0_lr_channel_interchange(struct hgpdm_v0_hw *p_pdm, uint32 enable) {

    if (enable) {
        p_pdm->CON |=   LL_PDM_CON_LRSW(1);
    } else {
        p_pdm->CON &= ~ LL_PDM_CON_LRSW(1);
    }

    return RET_OK;
}

static void hgpdm_v0_enable(struct hgpdm_v0_hw *p_pdm) {

    p_pdm->CON |= LL_PDM_CON_ENABLE(1);
}

static void hgpdm_v0_disable(struct hgpdm_v0_hw *p_pdm) {

    p_pdm->CON &= ~ LL_PDM_CON_ENABLE(1);
}



/**********************************************************************************/
/*                             ATTCH FUNCTION                                     */
/**********************************************************************************/

static int32 hgpdm_v0_open(struct pdm_device *pdm, enum pdm_sample_freq freq, enum pdm_channel channel) {

    struct hgpdm_v0 *dev   = (struct hgpdm_v0 *)pdm;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;
    int32  freq_to_reg     = 0;
    int32  channel_to_reg  = 0;

    if (dev->opened) {
        return -EBUSY;
    }

    /* hal enum */
    freq_to_reg    = hgpdm_v0_switch_hal_pdm_sample_freq(freq);
    channel_to_reg = hgpdm_v0_switch_hal_pdm_channel(channel);

    if ((-1) == freq_to_reg ||
        (-1) == channel_to_reg) {
        return -EINVAL;
    }

    /* make sure the clk */
    #if 0
        if (0 != ((peripheral_clock_get(HG_APB0_PT_PDM)) % (freq_to_reg*512))) {
            return RET_ERR;
        }
    #endif

    /* pin config */
    if (pin_func(dev->dev.dev.dev_id , 1) != RET_OK) {
        return RET_ERR;
    }

    /* reg config */
    hgpdm_v0_set_mclk(freq_to_reg);

    hw->CON = LL_PDM_CON_WORKMODE(channel_to_reg) | LL_PDM_CON_DECIM(0);
    hw->STA = 3;

    dev->opened = 1;

    return RET_OK;
}

static int32 hgpdm_v0_read(struct pdm_device *pdm, void* buf, uint32 len) {

    struct hgpdm_v0 *dev = (struct hgpdm_v0 *)pdm;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;
    
    if(!buf || len == 0)
    {
        return RET_ERR;
    }
    
    hw->DMASTADR = (uint32)buf;
    hw->DMALEN   = len;

    hgpdm_v0_enable(hw);

    return RET_OK;
}

static int32 hgpdm_v0_close(struct pdm_device *pdm) {

    struct hgpdm_v0 *dev = (struct hgpdm_v0 *)pdm;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;

    if (!dev->opened) {
        return RET_OK;
    }
        
    hgpdm_v0_disable(hw            );
    irq_disable(dev->irq_num       );
    pin_func(dev->dev.dev.dev_id, 0);

    dev->opened = 0;

    return RET_OK;
}

static void hgpdm_v0_irq_handler(void *data) {

    struct hgpdm_v0 *dev = (struct hgpdm_v0 *)data;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;

    if ((hw->DMACON & LL_PDM_DMACON_HF_IE_EN(1)) && 
        (hw->STA     & LL_PDM_STA_HF_PENDING(1))) {
        hw->STA |= LL_PDM_STA_HF_PENDING(1);

        if (dev->irq_hdl) {
            dev->irq_hdl(PDM_IRQ_FLAG_DMA_HF, dev->irq_data);
        }
    }

    if ((hw->DMACON & LL_PDM_DMACON_OV_IE_EN(1)) && 
        (hw->STA     & LL_PDM_STA_OV_PENDING(1))) {
        hw->STA |= LL_PDM_STA_OV_PENDING(1);
    
        if (dev->irq_hdl) {
            dev->irq_hdl(PDM_IRQ_FLAG_DMA_OV, dev->irq_data);
        }
    }
}

static int32 hgpdm_v0_request_irq(struct pdm_device *pdm, enum pdm_irq_flag irq_flag, pdm_irq_hdl irq_hdl, uint32 data) {

    struct hgpdm_v0 *dev = (struct hgpdm_v0 *)pdm;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;

    dev->irq_hdl  = irq_hdl;
    dev->irq_data = data;

    request_irq(dev->irq_num, hgpdm_v0_irq_handler, dev);

    if (irq_flag & PDM_IRQ_FLAG_DMA_HF) {
        hw->DMACON |= LL_PDM_DMACON_HF_IE_EN(1);
    }

    if (irq_flag & PDM_IRQ_FLAG_DMA_OV) {
        hw->DMACON |= LL_PDM_DMACON_OV_IE_EN(1);
    }

    irq_enable(dev->irq_num);

    return RET_OK;
}

static int32 hgpdm_v0_release_irq(struct pdm_device *pdm, enum pdm_irq_flag irq_flag) {

    struct hgpdm_v0 *dev = (struct hgpdm_v0 *)pdm;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;

    if (irq_flag & PDM_IRQ_FLAG_DMA_HF) {
        hw->DMACON &= ~ LL_PDM_DMACON_HF_IE_EN(1);
    }

    if (irq_flag & PDM_IRQ_FLAG_DMA_OV) {
        hw->DMACON &= ~ LL_PDM_DMACON_OV_IE_EN(1);
    }

    return RET_OK;
}

static int32 hgpdm_v0_ioctl(struct pdm_device *pdm, enum pdm_ioctl_cmd ioctl_cmd, uint32 param) {

    int32 ret_val = RET_OK;
    struct hgpdm_v0 *dev = (struct hgpdm_v0 *)pdm;
    struct hgpdm_v0_hw *hw = (struct hgpdm_v0_hw *)dev->hw;

    switch (ioctl_cmd) {
        case (PDM_IOCTL_CMD_LR_CHANNEL_INTERCHANGE):
            ret_val = hgpdm_v0_lr_channel_interchange(hw, param);
            break;
        default:
            ret_val = -ENOTSUPP;
            break;
    }

    return ret_val;
}

int32 hgpdm_v0_attach(uint32 dev_id, struct hgpdm_v0 *pdm) {

    pdm->opened          = 0;
    pdm->irq_hdl         = NULL;
    pdm->irq_data        = 0;
    pdm->dev.open        = hgpdm_v0_open;
    pdm->dev.read        = hgpdm_v0_read;
    pdm->dev.close       = hgpdm_v0_close;
    pdm->dev.ioctl       = hgpdm_v0_ioctl;
    pdm->dev.request_irq = hgpdm_v0_request_irq;
    pdm->dev.release_irq = hgpdm_v0_release_irq;
    
    irq_disable(pdm->irq_num);
    dev_register(dev_id, (struct dev_obj *)pdm);
    return RET_OK;
}


