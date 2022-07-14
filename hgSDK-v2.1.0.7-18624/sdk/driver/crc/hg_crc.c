#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "devid.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "dev/crc/hg_crc.h"
#include "hg_crc_hw.h"

struct hgcrc_config {
    uint32 poly;
    uint32 poly_bits;
    uint32 init_val;
    uint32 xor_out;
    char   ref_in;
    char   ref_out;
};

static const struct hgcrc_config tcpip_chksum = {
    .init_val = 0x0,
    .xor_out = 0x0,
    .poly = 0x0,
    .poly_bits = 0,
    .ref_in = 1,
    .ref_out = 1,
};
static const struct hgcrc_config crc5_usb = {
    .init_val = 0x1f,
    .xor_out = 0x1f,
    .poly = 0x14,
    .poly_bits = 5,
    .ref_in = 1,
    .ref_out = 1,
};
static const struct hgcrc_config crc7_mmc = {
    .init_val = 0x0,
    .xor_out = 0x0,
    .poly = 0x48,
    .poly_bits = 7,
    .ref_in = 0,
    .ref_out = 0,
};
static const struct hgcrc_config crc8_maxim = {
    .init_val = 0x0,
    .xor_out = 0x0,
    .poly = 0x8C,
    .poly_bits = 8,
    .ref_in = 1,
    .ref_out = 1,
};
static const struct hgcrc_config crc8 = {
    .init_val = 0x0,
    .xor_out = 0x0,
    .poly = 0xE0,
    .poly_bits = 8,
    .ref_in = 0,
    .ref_out = 0,
};
static const struct hgcrc_config crc16 = {
    .init_val = 0x0,
    .xor_out = 0x0,
    .poly = 0xA001,
    .poly_bits = 16,
    .ref_in = 1,
    .ref_out = 1,
};
static const struct hgcrc_config crc16_ccitt = {
    .init_val = 0x0,
    .xor_out = 0x0,
    .poly = 0x8408,
    .poly_bits = 16,
    .ref_in = 1,
    .ref_out = 1,
};
static const struct hgcrc_config crc16_modbus = {
    .init_val = 0xFFFF,
    .xor_out = 0x0,
    .poly = 0xA001,
    .poly_bits = 16,
    .ref_in = 1,
    .ref_out = 1,
};
static const struct hgcrc_config crc32_winrar = {
    .init_val = 0xFFFFFFFF,
    .xor_out = 0xFFFFFFFF,
    .poly = 0xEDB88320,
    .poly_bits = 32,
    .ref_in = 1,
    .ref_out = 1,
};

static const struct hgcrc_config *hgcrc_cfg[CRC_TYPE_MAX] = {
    [CRC_TYPE_TCPIP_CHKSUM] = &tcpip_chksum,
    [CRC_TYPE_CRC5_USB] = &crc5_usb,
    [CRC_TYPE_CRC7_MMC] = &crc7_mmc,
    [CRC_TYPE_CRC8_MAXIM] = &crc8_maxim,
    [CRC_TYPE_CRC8] = &crc8,
    [CRC_TYPE_CRC16] = &crc16,
    [CRC_TYPE_CRC16_CCITT] = &crc16_ccitt,
    [CRC_TYPE_CRC16_MODBUS] = &crc16_modbus,
    [CRC_TYPE_CRC32_WINRAR] = &crc32_winrar,
};

static void hg_crc_irq_handler(void *data)
{
    struct hg_crc *crc = (struct hg_crc *)data;
    struct hg_crc_hw *hw = (struct hg_crc_hw *)crc->hw;

    hw->CRC_KST = LL_CRC_KST_DMA_PENDING_CLR;
    os_sema_up(&crc->done);
}

static int32 hg_crc_calc_continue(struct crc_dev *crc, struct crc_dev_req *req, uint32 *crc_value)
{
    int32 tmo = 2000;
    int32 ret = 0;
    struct hg_crc *dev = (struct hg_crc *)crc;
    struct hg_crc_hw *hw = (struct hg_crc_hw *)dev->hw;

    hw->CRC_INIT = hw->CRC_OUT ^ hw->CRC_INV;
    /* kick */
#if defined(TXW80X)
    hw->DMA_ADDR = (uint32)req->data & 0x00FFFFFF;
#else
    hw->DMA_ADDR = (uint32)req->data;
#endif
    hw->DMA_LEN  = req->len;

    ret = os_sema_down(&dev->done, tmo);
    *crc_value = hw->CRC_OUT;
    return ret > 0 ? RET_OK : RET_ERR;
}


static int32 hg_crc_hold(struct crc_dev *crc, uint16 *cookie, uint32 hold)
{
    int32 ret = RET_ERR;
    struct hg_crc *dev = (struct hg_crc *)crc;

    os_mutex_lock(&dev->lock, osWaitForever);
    if (hold) {
        if (!(dev->flags & HGCRC_FLAGS_HOLD) || (*cookie != dev->cookie)) {
            os_mutex_unlock(&dev->lock);

            if (os_mutex_lock(&dev->hold, 100) < 0) {
                return -EBUSY;
            }

            os_mutex_lock(&dev->lock, osWaitForever);
            dev->cookie = (os_jiffies()&0xffff);
            dev->flags |= HGCRC_FLAGS_HOLD;
        }
        ret = RET_OK;
    } else {
        if ((dev->flags & HGCRC_FLAGS_HOLD) && (*cookie == dev->cookie)) {
            dev->cookie = 0;
            dev->flags &= ~HGCRC_FLAGS_HOLD;
            os_mutex_unlock(&dev->hold);
            ret = RET_OK;
        }
    }
    *cookie = dev->cookie;
    os_mutex_unlock(&dev->lock);
    return ret;
}

static int32 hg_crc_calc(struct crc_dev *crc, struct crc_dev_req *req, uint32 *crc_value, uint32 flags)
{
    int32  ret  = 0;
    uint32 hold = 0;
    uint32 cfg_reg = 0;
    uint32 cookie  = req->cookie;
    struct hg_crc    *dev = (struct hg_crc *)crc;
    struct hg_crc_hw *hw  = (struct hg_crc_hw *)dev->hw;
    const struct hgcrc_config *p_cfg;

    if (req == NULL || req->data == NULL || req->len == 0 || req->type >= CRC_TYPE_MAX) {
        return RET_ERR;
    }

#if defined(TXW4002ACK803)
    if (CRC_TYPE_TCPIP_CHKSUM == req->type) {
        return RET_ERR;
    }
#endif

    p_cfg = hgcrc_cfg[req->type];
    if (p_cfg == NULL) {
        return -ENOTSUP;
    }
    
    if(hg_crc_hold(crc, &req->cookie, 1)){
        return -EBUSY;
    }

    hold = (cookie == req->cookie);
    if (flags & CRC_DEV_FLAGS_CONTINUE_CALC) {
        return hg_crc_calc_continue(crc, req, crc_value);
    }

    sysctrl_crc_reset();
    if (0 == p_cfg->poly_bits) {
        cfg_reg = LL_CRC_CFG_INT_EN | LL_CRC_CFG_TCP_MODE_EN | LL_CRC_CFG_DMAWAIT_CLOCK(5);
    } else {
        cfg_reg = LL_CRC_CFG_INT_EN | LL_CRC_CFG_POLY_BITS(p_cfg->poly_bits) | LL_CRC_CFG_DMAWAIT_CLOCK(5);
        if (p_cfg->ref_in) {
            cfg_reg |= LL_CRC_CFG_BIT_ORDER_RIGHT;
        } else {
            cfg_reg |= LL_CRC_CFG_BIT_ORDER_LEFT;
        }
    }

    /* config */
    hw->CRC_INIT = p_cfg->init_val;
    hw->CRC_INV  = p_cfg->xor_out;
    hw->CRC_POLY = p_cfg->poly;
    hw->CRC_CFG  = cfg_reg;

    /* kick */
#if defined(TXW80X)
    hw->DMA_ADDR = (uint32)req->data & 0x00FFFFFF;
#else
    hw->DMA_ADDR = (uint32)req->data;
#endif
    hw->DMA_LEN  = req->len;

    ret = os_sema_down(&dev->done, 100);
    *crc_value = hw->CRC_OUT;

    if (!hold) {
        hg_crc_hold(crc, &req->cookie, 0);
    }
    return ret > 0 ? RET_OK : RET_ERR;
}
static int32 hg_crc_suspend(struct dev_obj *dev, int32 dsleep)
{
    int32 ret = 0;
    struct hg_crc *crc = (struct hg_crc *)dev;
    if (!dsleep) {
        ret = os_mutex_lock(&crc->hold, 2000);
        if (ret < 0) {
            return ret;
        }
        irq_disable(crc->irq_num);
        crc->flags = BIT(HGCRC_FLAGS_SUSPEND);
        /* register is holded when clock close & reopen */
        sysctrl_crc_clk_close();
    }
    return RET_OK;
}

static int32 hg_crc_resume(struct dev_obj *dev)
{
    int32 ret = 0;
    struct hg_crc *crc = (struct hg_crc *)dev;
    if (crc->flags & BIT(HGCRC_FLAGS_SUSPEND)) {
        ret = os_mutex_unlock(&crc->hold);
        if (ret < 0) {
            return ret;
        }
        sysctrl_crc_clk_open();
        crc->flags &= ~ BIT(HGCRC_FLAGS_SUSPEND);
        irq_enable(crc->irq_num);
    }
    return RET_OK;
}

__init int32 hg_crc_attach(uint32 dev_id, struct hg_crc *crc)
{
    struct hg_crc_hw *hw = (struct hg_crc_hw *)crc->hw;

    crc->dev.calc        = hg_crc_calc;
    crc->dev.hold        = hg_crc_hold;
#ifdef CONFIG_SUSPEND
    crc->dev.dev.suspend = hg_crc_suspend;
    crc->dev.dev.resume  = hg_crc_resume;
#endif
    crc->flags = 0;

    os_mutex_init(&crc->hold);
    os_mutex_init(&crc->lock);
    os_sema_init(&crc->done, 0);
    sysctrl_crc_clk_open();
    sysctrl_crc_reset();

    request_irq(crc->irq_num, hg_crc_irq_handler, crc);
    hw->CRC_CFG |= LL_CRC_CFG_INT_EN;
    irq_enable(crc->irq_num);
    dev_register(dev_id, (struct dev_obj *)crc);
    return RET_OK;
}




