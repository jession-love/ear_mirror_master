#include "sys_config.h"
#include "typesdef.h"
#include "devid.h"
#include "list.h"
#include "dev.h"
#include "osal/task.h"
#include "osal/semaphore.h"
#include "lib/sdhost/sdhost.h"
#include "hal/gpio.h"
#include "osal/irq.h"
#include "osal/string.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "osal/task.h"
#include "osal/sleep.h"
#include "osal/timer.h"


struct hgsdh_hw
{
    __IO uint32 CFG;
    __IO uint32 BAUD;
    __IO uint32 CPTR;
    __IO uint32 DPTR;
    __IO uint32 DCNT;	
};


static uint8 sdhc_cmd_rsp_buf[32]__attribute__ ((aligned(4)));
void delay_us(uint32 n);

struct sdh_device *sdh_irq;

static void ll_sdhc_set_bus_clk(struct hgsdh_hw *p_sdhc, uint32 sys_clk, uint32 bus_clk)
{
    p_sdhc->BAUD = (sys_clk / 2 / bus_clk) - 1;
}

static void ll_sdhc_set_ioe(struct hgsdh_hw *p_sdhc, TYPE_LL_SDHC_IOE ioe)
{
    if (ioe == LL_SDHC_IROF) {
        p_sdhc->CFG &= ~LL_SDHC_ODAT_RISING_EDGE;
    } else {
        p_sdhc->CFG |= LL_SDHC_ODAT_RISING_EDGE;
    }
}

static void ll_sdhc_set_dat_width(struct hgsdh_hw *p_sdhc, TYPE_LL_SDHC_DAT_WIDTH width)
{
    if (width == LL_SDHC_DAT_4BIT) {
        p_sdhc->CFG |= LL_SDHC_DAT_WIDTH_4BIT;
    } else {
        p_sdhc->CFG &= ~LL_SDHC_DAT_WIDTH_4BIT;
    }
}

static void ll_sdhc_clk_suspend_ctrl(struct hgsdh_hw *p_sdhc, uint8 suspend_enable)
{
    if (suspend_enable) {
        p_sdhc->CFG &= ~LL_SDHC_CLK_OUT_EN;
    } else {
        p_sdhc->CFG |= LL_SDHC_CLK_OUT_EN;
    }
}

static void ll_sdhc_post_8clk_ctrl(struct hgsdh_hw *p_sdhc, uint8 enable)
{
    if (enable) {
        p_sdhc->CFG |= LL_SDHC_8CLK_AFTER_EN;
        //p_sdhc->CFG |= LL_SDHC_8CLK_BEFORE_EN;
    } else {
        p_sdhc->CFG &= ~LL_SDHC_8CLK_AFTER_EN;
        //p_sdhc->CFG &= ~LL_SDHC_8CLK_BEFORE_EN;       
    }
}

static void ll_sdhc_cmd_intr_enable(struct hgsdh_hw *p_sdhc, uint8 enable)
{
    if (enable) {
        p_sdhc->CFG |= LL_SDHC_CMD_INTR_EN;
    } else {
        p_sdhc->CFG &= ~LL_SDHC_CMD_INTR_EN;
    }
}

/**
 * @brief   ll_sdhc_set_dat_width
 * @param   p_sdhc  : sdhc register structure pointer
 * @retval  none
 */
static void ll_sdhc_dat_intr_enable(struct hgsdh_hw *p_sdhc, uint8 enable)
{
    if (enable) {
        p_sdhc->CFG |= LL_SDHC_DAT_INTR_EN;
    } else {
        p_sdhc->CFG &= ~LL_SDHC_DAT_INTR_EN;
    }
}

static void ll_sdhc_clr_cmd_done_pending(struct hgsdh_hw *p_sdhc)
{
    p_sdhc->CFG |= LL_SDHC_CMD_DONE_CLR;
}

static bool ll_sdhc_get_cmd_done_pending(struct hgsdh_hw *p_sdhc)
{
    return ((p_sdhc->CFG & LL_SDHC_CMD_DONE)) ? TRUE : FALSE;
}

static bool ll_sdhc_get_cmd_rsp_timeout_pending(struct hgsdh_hw *p_sdhc)
{
    return (p_sdhc->CFG & LL_SDHC_RSP_TIMEOUT) ? TRUE : FALSE;
}

static bool ll_sdhc_get_cmd_rsp_err_pending(struct hgsdh_hw *p_sdhc)
{
    return (p_sdhc->CFG & LL_SDHC_RSP_CRC_ERR) ? TRUE : FALSE;
}

static void ll_sdhc_clr_dat_done_pending(struct hgsdh_hw *p_sdhc)
{
    p_sdhc->CFG |= LL_SDHC_DAT_DONE_CLR;
}

static bool ll_sdhc_get_dat_done_pending(struct hgsdh_hw *p_sdhc)
{
    return ((p_sdhc->CFG & LL_SDHC_DAT_DONE)) ? TRUE : FALSE;
}

static bool ll_sdhc_get_rd_dat_err_pending(struct hgsdh_hw *p_sdhc)
{
    return (p_sdhc->CFG & LL_SDHC_DAT_CRC_ERR) ? TRUE : FALSE;
}

static bool ll_sdhc_get_wr_dat_err_pending(struct hgsdh_hw *p_sdhc)
{
    return ((p_sdhc->CFG & LL_SDHC_CRC_STA) != (2<<16)) ? TRUE : FALSE;
}

static void ll_sdhc_read_data_kick(struct hgsdh_hw *p_sdhc, uint32 addr, uint32 len)
{
    p_sdhc->CFG &= ~LL_SDHC_DAT_RW_MSK;
	p_sdhc->CFG &= ~LL_SDHC_CLK_OUT_EN;
    ll_sdhc_clr_dat_done_pending(p_sdhc);

    //printf("\r\nDPTR:%x\r\n",addr);
    p_sdhc->DPTR = addr;
    p_sdhc->DCNT = len;
    p_sdhc->CFG |= LL_SDHC_DAT_RCV;
}

static void ll_sdhc_write_data_kick(struct hgsdh_hw *p_sdhc, uint32 addr, uint32 len)
{
    p_sdhc->CFG &= ~LL_SDHC_DAT_RW_MSK;
    ll_sdhc_clr_dat_done_pending(p_sdhc);

    p_sdhc->DPTR = addr;
    p_sdhc->DCNT = len;
    p_sdhc->CFG |= LL_SDHC_DAT_SEND_WAIT_BUSY;
}

static bool ll_sdhc_read_data_wait(struct hgsdh_hw *p_sdhc, uint32 time_ms)
{
    do {

    } while (!ll_sdhc_get_dat_done_pending(p_sdhc));


    if (ll_sdhc_get_rd_dat_err_pending(p_sdhc)) {
        printf("dat err\r\n");
        return FALSE;
    }
    return TRUE;
}

static bool ll_sdhc_write_data_wait(struct hgsdh_hw *p_sdhc, uint32 time_ms)
{
    uint64 time = csi_kernel_get_ticks();
    do {
        if ((csi_kernel_get_ticks() - time) > time_ms) {
            return FALSE;
        }
    } while (!ll_sdhc_get_dat_done_pending(p_sdhc));

    if (ll_sdhc_get_wr_dat_err_pending(p_sdhc)) {
        return FALSE;
    }
    return TRUE;
}


void hgic_sdc_irq_update(struct sdh_device  *host, int32_t enable)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)host; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;

    if (enable) {
        printf("enable sdio irq");
    } else {
        printf("disable sdio irq");
    }
#if CMD_ISR_EN
    ll_sdhc_cmd_intr_enable(hw, enable);
#endif
#if DAT_ISR_EN  
    ll_sdhc_dat_intr_enable(hw, enable);
#endif
//  ll_sdhc_dat1_intr_enable(host->p_sdhc, enable);

}



//volatile uint8 sd_8clk_open = 0;
void ll_sdhc_config(struct sdh_device *sdhost,struct hgsdh_hw * p_sdhc, TYPE_LL_SDHC_CFG *p_cfg)
{
    ll_sdhc_set_bus_clk(p_sdhc, peripheral_clock_get(HG_AHB_PT_SDMMC), p_cfg->bus_clk);
    ll_sdhc_set_ioe(p_sdhc, p_cfg->ioe);
    ll_sdhc_set_dat_width(p_sdhc, p_cfg->width);

    ll_sdhc_clk_suspend_ctrl(p_sdhc, 0);
    ll_sdhc_post_8clk_ctrl(p_sdhc, sdhost->sd_8clk_open);

    p_sdhc->CFG |= LL_SDHC_EN;
}

void ll_sdhc_close(struct hgsdh_hw * p_sdhc)
{
    p_sdhc->CFG &= ~LL_SDHC_EN;
}



volatile uint8 sd_cmd_isr_get = 0;
volatile uint8 sd_dat_isr_get = 0;

int32 sdh_cmd(struct sdh_device *sdhost,struct rt_mmcsd_cmd* cmd)
{
    uint32 rsp_type;
    uint32 cmd_tick;
    //uint32 cur_tick;
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *p_sdhc =  (struct hgsdh_hw *)sdh_hw->hw;
    sdhc_cmd_rsp_buf[0] = cmd->cmd_code | 0x40;
    //put_unaligned_be32(cmd->arg, &sdhc_cmd_rsp_buf[1]);
    sdhc_cmd_rsp_buf[1] = (cmd->arg >> 24) & 0xFF;
    sdhc_cmd_rsp_buf[2] = (cmd->arg >> 16) & 0xFF;
    sdhc_cmd_rsp_buf[3] = (cmd->arg >> 8) & 0xFF;
    sdhc_cmd_rsp_buf[4] = (cmd->arg >> 0) & 0xFF;
    p_sdhc->CPTR = (uint32)sdhc_cmd_rsp_buf;

    rsp_type = ((cmd->flags & MMC_RSP_PRESENT) ? BIT(5) : 0) |
               ((cmd->flags & MMC_RSP_136) ? BIT(6) : 0) |
               ((cmd->flags & MMC_RSP_BUSY) ? BIT(7) : 0);
    
    if (0 == rsp_type) {
        rsp_type = (0x2 << 5);
    }
    
    //ll_sdhc_post_8clk_ctrl(p_sdhc, sd_8clk_open);         
    ll_sdhc_clr_cmd_done_pending(p_sdhc);
//  printf("cmd:%d  %x\r\n",cmd->cmd_code,(p_sdhc->CFG & ~LL_SDHC_CMD_RESP_MSK) | rsp_type);
    p_sdhc->CFG  = (p_sdhc->CFG & ~LL_SDHC_CMD_RESP_MSK) | rsp_type;

    cmd_tick = 0x1fffff;
    //ll_gpio_bit_reset(GPIOB,BIT(5));
    while (!ll_sdhc_get_cmd_done_pending(p_sdhc)&&!sd_cmd_isr_get)
    {
        if (--cmd_tick == 0) {
            //ll_gpio_bit_reset(GPIOB,BIT(5));
        
            
            //sdreg(base,SDxCON1) = BIT(BIT_CPCLR);
            //sdreg(base,SDxCON0) &= ~BIT(BIT_8CKE);
            ll_sdhc_clr_cmd_done_pending(p_sdhc);
//          ll_sdhc_post_8clk_ctrl(p_sdhc,0);
//          p_sdhc->CFG &= ~(BIT(5)|BIT(6)|BIT(7)); 
            p_sdhc->CFG &= ~BIT(0);
//          printf ("CMD%d pending time out!resp:%x\r\n", cmd->cmd_code,rsp_type);
            //ll_gpio_bit_set(GPIOB,BIT(5));
            p_sdhc->CFG |= BIT(0);
            
            //ll_sdhc_post_8clk_ctrl(p_sdhc, 0);        
            return -1;
        }

            
    }

    sd_cmd_isr_get = 0;
    //ll_gpio_bit_set(GPIOB,BIT(5));
    if (ll_sdhc_get_cmd_rsp_timeout_pending(p_sdhc) ||
        ((cmd->flags & MMC_RSP_CRC) && ll_sdhc_get_cmd_rsp_err_pending(p_sdhc))) {
        printf("read timeout pending set or rsp err:%d %d\r\n",ll_sdhc_get_cmd_rsp_timeout_pending(p_sdhc),ll_sdhc_get_cmd_rsp_err_pending(p_sdhc));
        
        return -1;
    }
   
    cmd->resp[0] = (sdhc_cmd_rsp_buf[9] << 24)  | 
                   (sdhc_cmd_rsp_buf[10] << 16) |
                   (sdhc_cmd_rsp_buf[11] << 8)  |
                   (sdhc_cmd_rsp_buf[12] << 0);

    cmd->resp[1] = (sdhc_cmd_rsp_buf[13] << 24)  | 
                   (sdhc_cmd_rsp_buf[14] << 16) |
                   (sdhc_cmd_rsp_buf[15] << 8)  |
                   (sdhc_cmd_rsp_buf[16] << 0);

    cmd->resp[2] = (sdhc_cmd_rsp_buf[17] << 24)  | 
                   (sdhc_cmd_rsp_buf[18] << 16) |
                   (sdhc_cmd_rsp_buf[19] << 8)  |
                   (sdhc_cmd_rsp_buf[20] << 0);
    
    cmd->resp[3] = (sdhc_cmd_rsp_buf[21] << 24)  | 
                   (sdhc_cmd_rsp_buf[22] << 16) |
                   (sdhc_cmd_rsp_buf[23] << 8)  |
                   (sdhc_cmd_rsp_buf[24] << 0); 
#if 0
    printf("\r\n"); 
    for(cmd_tick = 0;cmd_tick < 32;cmd_tick++){
        printf("%02x",sdhc_cmd_rsp_buf[cmd_tick]);
    }                  
    printf("\r\n"); 
#endif  
//  printf("cmd->resp:%x\r\n",cmd->resp[0]);
//  ll_sdhc_post_8clk_ctrl(p_sdhc, 0);      

    return 0;    
}


void sdhc_interrupt_handle(struct sdh_device *sdhost)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;
    if (ll_sdhc_get_dat_done_pending(hw)){
        ll_sdhc_clr_dat_done_pending(hw);
        if(sdhost->sd_opt == SD_M_R){
            if(ll_sdhc_get_rd_dat_err_pending(hw))
            {
                printf("read dat err pending\r\n");
                sdhost->data.err = DATA_DIR_READ;
                os_sema_up(&sdhost->dat_sema);
            }
            
            if(sdhost->data.blks - 1)
            {
                sdhost->data.blks = sdhost->data.blks -1;
                sdhost->data.buf = sdhost->data.buf + sdhost->data.blksize;
                ll_sdhc_read_data_kick(hw, (uint32_t)sdhost->data.buf, sdhost->data.blksize);
                
            }else
            {
                os_sema_up(&sdhost->dat_sema);
            }

        }
        else if(sdhost->sd_opt == SD_M_W)
        {
            if (ll_sdhc_get_wr_dat_err_pending(hw))
            {
                printf("write dat err pending\r\n");
                sdhost->data.err = DATA_DIR_READ;
                os_sema_up(&sdhost->dat_sema);
            }
            
            if(sdhost->data.blks - 1)
            {
                sdhost->data.blks = sdhost->data.blks -1;
                sdhost->data.buf = sdhost->data.buf + sdhost->data.blksize;
                ll_sdhc_write_data_kick(hw, (uint32_t)sdhost->data.buf, sdhost->data.blksize);
                
            }else
            {
                os_sema_up(&sdhost->dat_sema);
            }           
        }
        else
        {
            os_sema_up(&sdhost->dat_sema);   //single read/write
        }
    } 
    
}



void SDHOST_IRQHandler_action(void *sdhost)
{
    sdhc_interrupt_handle(sdhost);
}


int32 sdh_write(struct sdh_device *sdhost,uint8* buf)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;
    uint32_t xfer_len = sdhost->data.blksize;   
#if DAT_ISR_EN
    sdhost->data.buf = buf;
    ll_sdhc_write_data_kick(hw, (uint32_t)buf, xfer_len);
    os_sema_down(&sdhost->dat_sema, 1000);
	if(sdhost->data.err != 0){
		return -1;
	}	
#else   
    uint32_t blocks = sdhost->data.blks;
    uint32_t size = sdhost->data.blks * sdhost->data.blksize;   
    do {
        ll_sdhc_write_data_kick(hw, (uint32_t)buf, xfer_len);
        buf += xfer_len;
        if (blocks) {
            blocks--;
        }

        if (!ll_sdhc_write_data_wait(hw, sdhost->data.timeout_ns/1000000))                             
        {
            printf("write timeout:%d\r\n",sdhost->data.timeout_ns);
            sdhost->data.err = DATA_DIR_WRITE;
            break;
        }

        if (ll_sdhc_get_wr_dat_err_pending(hw)) {                  
            printf("write dat err pending\r\n");
            sdhost->data.err = DATA_DIR_WRITE;
        }

        sd_dat_isr_get = 0;
    } while (blocks);   
#endif  
    return 0;
}

int32 sdh_read(struct sdh_device *sdhost,uint8* buf)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;
	
    uint32_t xfer_len = sdhost->data.blksize;   
    ll_sdhc_post_8clk_ctrl(hw, 0);
#if DAT_ISR_EN
    sdhost->data.buf = buf;
    ll_sdhc_read_data_kick(hw, (uint32_t)buf, xfer_len);
    os_sema_down(&sdhost->dat_sema, 1000);
	if(sdhost->data.err != 0){
		return -1;
	}	
#else
    uint32_t blocks = sdhost->data.blks;
    uint32_t size = sdhost->data.blks * sdhost->data.blksize;   
    do {
        ll_sdhc_read_data_kick(hw, (uint32_t)buf, xfer_len);
        buf += xfer_len;
        if (blocks) {
            blocks--;
        }
                      
        if (!ll_sdhc_read_data_wait(hw, sdhost->data.timeout_ns/1000000)) 
        {   
            printf("read dat timeout\r\n");
            //while(1);
            sdhost->data.err = DATA_DIR_READ;
        }


        if (ll_sdhc_get_rd_dat_err_pending(hw)) {
            printf("read dat err pending\r\n");
            sdhost->data.err = DATA_DIR_READ;
        }
        sd_dat_isr_get = 0;
    } while (blocks);
#endif  
    ll_sdhc_post_8clk_ctrl(hw,sdhost->sd_8clk_open);   

    return 0;
}



//set the sdh config
int32 sdh_open(struct sdh_device *sdhost,uint8 bus_w)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;

    if (sdhost == 0) {
        printf("mmcsd alloc host fail");
        return 0;
    }

	SYSCTRL->SYS_CON1 &= ~(BIT(20));
	delay_us(10);
	SYSCTRL->SYS_CON1 |= (BIT(20));
	
    sdh_irq = sdhost;
    /* set host default attributes */
    sdhost->sd_8clk_open = 0;
    sdhost->freq_min = 400 * 1000;
    sdhost->freq_max = 24000000;
    sdhost->valid_ocr = 0x00FFFF80;/* The voltage range supported is 1.65v-3.6v */
	if(bus_w == 4)
    	sdhost->flags = MMCSD_BUSWIDTH_4 | MMCSD_MUTBLKWRITE | MMCSD_SUP_SDIO_IRQ;
	else
    	sdhost->flags = MMCSD_MUTBLKWRITE | MMCSD_SUP_SDIO_IRQ;

    sdhost->max_seg_size = 4096;
    sdhost->max_dma_segs = 1;
    sdhost->max_blk_size = 512;
    sdhost->max_blk_count = 512;
    sdhost->sd_stop       = 0;
    sdhost->sd_opt        = SD_IDLE;
    sdhost->new_lba       = 0;
    os_sema_init(&sdhost->dat_sema, 0);

    /* link up host and sdio */
    sdhost->private_data = 0;
    sdhost->data.timeout_ns =  100000000;  /* 100ms */
    TYPE_LL_SDHC_CFG cfg;
    cfg.bus_clk = sdhost->freq_min;
    cfg.ioe = LL_SDHC_IROF;//LL_SDHC_IROF;
    if(bus_w == 4)
    	cfg.width = LL_SDHC_DAT_4BIT;
	else
		cfg.width = LL_SDHC_DAT_1BIT;
    ll_sdhc_config(sdhost,hw, &cfg);


    hgic_sdc_irq_update(sdhost, 1);
    NVIC_EnableIRQ(SDHOST_IRQn);
	request_irq(SDHOST_IRQn, SDHOST_IRQHandler_action, (void*)sdhost);	
    return 0;
    
}

int32 sdh_cfg(struct sdh_device *sdhost,struct rt_mmcsd_io_cfg *io_cfg)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;

    uint32_t clk = io_cfg->clock;

    if (clk < sdhost->freq_min) {
        clk = sdhost->freq_min;
    }

    if (clk > sdhost->freq_max) {
        clk = sdhost->freq_max;
    }
    
    if (clk > 25000000) {
        ll_sdhc_set_ioe(hw, LL_SDHC_IROR);
    } else {
        ll_sdhc_set_ioe(hw, LL_SDHC_IROF);
    }
    
    printf("clk:%d width:%s%s%s power:%s%s%s\r\n",
          clk,
          io_cfg->bus_width == MMCSD_BUS_WIDTH_8 ? "8" : "",
          io_cfg->bus_width == MMCSD_BUS_WIDTH_4 ? "4" : "",
          io_cfg->bus_width == MMCSD_BUS_WIDTH_1 ? "1" : "",
          io_cfg->power_mode == MMCSD_POWER_OFF ? "OFF" : "",
          io_cfg->power_mode == MMCSD_POWER_UP ? "UP" : "",
          io_cfg->power_mode == MMCSD_POWER_ON ? "ON" : ""
         );

    if (clk) {
        ll_sdhc_set_bus_clk(hw, sys_get_apbclk()*2, clk);
    }
    
    if (io_cfg->bus_width == MMCSD_BUS_WIDTH_8) {
        printf("bus err\r\n");
    } else if (io_cfg->bus_width == MMCSD_BUS_WIDTH_4) {
        ll_sdhc_set_dat_width(hw, LL_SDHC_DAT_4BIT);
    } else {
        ll_sdhc_set_dat_width(hw, LL_SDHC_DAT_1BIT);
    }

    switch (io_cfg->power_mode)
    {
    case MMCSD_POWER_OFF:
        break;
    case MMCSD_POWER_UP:
        break;
    case MMCSD_POWER_ON:
        break;
    default:
        printf("unknown power_mode %d", io_cfg->power_mode);
        break;
    }   
    
    return 0;

}


int32 sdh_close(struct sdh_device *sdhost)
{
	struct hgsdh *sdh_hw = (struct hgsdh*)sdhost; 
	struct hgsdh_hw *hw =  (struct hgsdh_hw *)sdh_hw->hw;
    ll_sdhc_close(hw);
    os_sema_del(&sdhost->dat_sema);
    NVIC_DisableIRQ(SDHOST_IRQn);
	pin_func(sdhost->dev.dev_id,0);
    return 0;

}




void hgsdh_attach(uint32 dev_id, struct hgsdh *sdhost)
{
    sdhost->dev.open                  = sdh_open;
    sdhost->dev.close                 = sdh_close;
    //sdhost->dev.init                  = sdh_init;
    sdhost->dev.iocfg                  = sdh_cfg;
    sdhost->dev.cmd                   = sdh_cmd;
    sdhost->dev.write                  = sdh_write;
    sdhost->dev.read                  = sdh_read;   
    irq_disable(sdhost->irq_num);
    dev_register(dev_id, (struct dev_obj *)sdhost); 
}



