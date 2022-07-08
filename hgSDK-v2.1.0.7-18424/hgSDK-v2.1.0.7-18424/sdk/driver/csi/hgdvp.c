#include "typesdef.h"
#include "errno.h"
#include "osal/irq.h"
#include "devid.h"
#include "dev/csi/hgdvp.h"
#include "osal/string.h"

struct hgdvp_hw
{
	__IO uint32 CON;
	__IO uint32 RST;
	__IO uint32 RSP;
	__IO uint32 DLT;
	__IO uint32 DHT;
	__IO uint32 STA;
	__IO uint32 INTE;
	__IO uint32 DMA_TADR0;
	__IO uint32 DMA_TADR1;
	__IO uint32 DMA_DLEN;	
};


dvp_irq_hdl dvpirq_vector_table[DVP_IRQ_NUM];
volatile uint32 dvpirq_dev_table[DVP_IRQ_NUM];


static int32 hgdvp_ioctl(struct dvp_device *p_dvp, enum dvp_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2){
    int32  ret_val = RET_OK;	
	uint32 x_s,y_s,x_e,y_e;
	uint32 *size_array;
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp;	
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	switch(ioctl_cmd){
		case DVP_IOCTL_CMD_SET_FORMAT:
			hw->CON &= ~(BIT(1)|BIT(2));
			if(param1 > 3){
				os_printf("set FORMAT mode err\r\n");
				return RET_ERR; 
			}
			hw->CON |= param1<<1;

		break;

		case DVP_IOCTL_CMD_RGB_2_YUV:
			if(param1)
				hw->CON |= BIT(9);
			else
				hw->CON &= ~BIT(9);

		break;
	
		case DVP_IOCTL_CMD_SET_SIZE:
			size_array = (uint32*)param1;
			x_s =  size_array[0];
			y_s =  size_array[1];
			x_e =  size_array[2];
			y_e =  size_array[3];
			if((x_e - x_s + 1)%16 != 0){
				os_printf("###############################warning image_w no align 16 Byte##################################################\r\n");	
			}

			if((y_e - y_s + 1)%16 != 0){
				os_printf("###############################warning image_h no align 16 Byte##################################################\r\n");	
			}	

			hw->RST	=	((x_s & 0x7ff) << 10)|(y_s & 0x3ff);
			hw->RSP	=	((x_e & 0x7ff) << 10)|(y_e & 0x3ff);							
		break;
        case DVP_IOCTL_CMD_SET_ADR_1:
			hw->DMA_TADR0 = param1;
		break;
        case DVP_IOCTL_CMD_SET_ADR_2:
			hw->DMA_TADR1 = param1;
		break;
        case DVP_IOCTL_CMD_SET_SCEN:
			if(param1)
				hw->CON |= BIT(5);
			else
				hw->CON &= ~BIT(5);
		break;
        case DVP_IOCTL_CMD_SET_HSYNC_VAILD:
			if(param1)
				hw->CON &= ~BIT(3);		
			else
				hw->CON |= BIT(3);			
		break;
        case DVP_IOCTL_CMD_SET_VSYNC_VAILD:
			if(param1)
				hw->CON &= ~BIT(4);		
			else
				hw->CON |= BIT(4);			
		break;
        case DVP_IOCTL_CMD_SET_ONE_SAMPLE:
			if(param1)
				hw->CON |= BIT(10);
			else
				hw->CON &= ~BIT(10);			
        break;
        case DVP_IOCTL_CMD_SET_DEBOUNCE:
			if(param1){
				hw->CON |= BIT(29);
				hw->CON &=~(BIT(30)|BIT(31));
				hw->CON |=(param2<<30);
			}
			else
				hw->CON &= ~BIT(29);			
		break;
        case DVP_IOCTL_CMD_SET_YCBCR_MODE:
			hw->CON &= ~(BIT(6)|BIT(7));
			if(param1 > 3){
				os_printf("set ycbcr mode err\r\n");
				return RET_ERR;
			}
			hw->CON |= param1<<6;			
		break;

        case DVP_IOCTL_CMD_DIS_UV_MODE:	
			if(param1)
				hw->CON |= BIT(8);
			else
				hw->CON &= ~BIT(8);			
		break;

        case DVP_IOCTL_CMD_SET_FRAME_RATE:
			hw->CON &= ~(BIT(12)|BIT(13));
			if(param1 > 3){
				os_printf("set FORMAT mode err\r\n");
				return RET_ERR;
			}
			hw->CON |= param1<<12;			
		break;

        case DVP_IOCTL_CMD_SET_THRESHOLD:
			hw->DLT = param1;
			hw->DHT = param2;
        break;

		case DVP_IOCTL_CMD_EX_D5_D6:
			if(param1 == 0)
				hw->INTE &= ~BIT(22);		
			else
				hw->INTE |= BIT(22);				
		break;


		case DVP_IOCTL_CMD_SET_JPEG_LEN:
			hw->CON &= ~(0x3fff << 14);
			hw->CON |= param1<<14;
		break;

		default:
            ret_val = -ENOTSUPP;
        break;
	}


	return ret_val;
}


void DVP_IRQHandler_action(void *p_dvp)
{
	uint32 sta = 0;
	uint8 loop;
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp; 
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	sta = hw->STA;
	for(loop = 0;loop < DVP_IRQ_NUM;loop++){
		if(sta&BIT(loop)){
			hw->STA = BIT(loop);
			if(dvpirq_vector_table[loop] != NULL)
				dvpirq_vector_table[loop] (loop,dvpirq_dev_table[loop],0);
		}
	}
}


void irq_dvp_enable(struct dvp_device *p_dvp,uint8 mode,uint8 irq){
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp; 
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	if(mode){
		hw->INTE |= BIT(irq);
	}else{
		hw->INTE &= ~BIT(irq);
	}
}


int32 dvpirq_register(struct dvp_device *p_dvp,uint32 irq, dvp_irq_hdl isr, uint32 dev_id){
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp; 	
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	request_irq(dvp_hw->irq_num, DVP_IRQHandler_action, p_dvp);
	
	irq_dvp_enable(p_dvp, 1, irq);
	dvpirq_vector_table[irq] = isr;
	dvpirq_dev_table[irq] = dev_id;
	hw->STA |= BIT(irq);
	os_printf("dvpirq_register:%d %x  %x\r\n",irq,(uint32)dvpirq_vector_table[irq],(uint32)isr);
	return 0;
}


int32 dvpirq_unregister(struct dvp_device *p_dvp,uint32 irq){
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp;	
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	irq_dvp_enable(p_dvp, 0, irq);
	dvpirq_vector_table[irq] = NULL;
	dvpirq_dev_table[irq] = 0;
	hw->STA |= BIT(irq);
	return 0;
}


static int32 hgdvp_open(struct dvp_device *p_dvp){
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp;	
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	hw->CON |= BIT(0);
	irq_enable(dvp_hw->irq_num);
	return 0;
}

static int32 hgdvp_close(struct dvp_device *p_dvp){
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp; 	
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	hw->CON &= ~BIT(0);
	irq_disable(dvp_hw->irq_num);
	return 0;
}

static int32 hgdvp_init(struct dvp_device *p_dvp){
//	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp; 
#ifdef FPGA_SUPPORT
	SYSCTRL->CLK_CON1 &= ~(BIT(27)|BIT(28));
	SYSCTRL->CLK_CON1 |= BIT(27);		
#else
	SYSCTRL->CLK_CON2 |= BIT(31);
	SYSCTRL->SYS_CON1 |= BIT(28);
	SYSCTRL->CLK_CON1 &= ~(BIT(27)|BIT(28));
	SYSCTRL->CLK_CON1 |= BIT(28);	
#endif
	pin_func(HG_DVP_DEVID,1);

	return 0;
}

static int32 hgdvp_set_baudrate(struct dvp_device *p_dvp,uint32 baudrate){
	struct hgdvp *dvp_hw = (struct hgdvp*)p_dvp;
	struct hgdvp_hw *hw  = (struct hgdvp_hw *)dvp_hw->hw;
	uint8 m_cfg = 0;
#if defined(FPGA_SUPPORT)
	m_cfg = 96000000/baudrate - 1;
#else
	m_cfg = (peripheral_clock_get(HG_APB0_PT_DVP))/baudrate - 1;
#endif
	
	printf("%s:clock:%d\n",__FUNCTION__,peripheral_clock_get(HG_APB0_PT_DVP));
	SYSCTRL->SYS_KEY = 0x3fac87e4;
	SYSCTRL->CLK_CON3 &= ~(0x3f<<26);		//先跑6M
	SYSCTRL->CLK_CON3 |= ((m_cfg&0x3f)<<26);		//先跑6M
	if(m_cfg&0x40){
		SYSCTRL->SYS_CON3 |= BIT(26);
	}else
	{
		SYSCTRL->SYS_CON3 &= ~BIT(26);
	}

	if(m_cfg&0x80){
		SYSCTRL->SYS_CON3 |= BIT(2);
	}else
	{
		SYSCTRL->SYS_CON3 &= ~BIT(2);
	}
	

	SYSCTRL->SYS_KEY = 0;

		
	//vsync低有效 hsync高有效 
	hw->CON = BIT(11);				//	


	return 0;
}


int32 hgdvp_attach(uint32 dev_id, struct hgdvp *dvp){
    dvp->opened          = 0;
    dvp->use_dma         = 0;
    dvp->irq_hdl                   = NULL;
    //memset(dvp->irq_hdl,0,sizeof(dvp->irq_hdl));
    dvp->irq_data                  = 0;
	//memset(dvp->irq_data,0,sizeof(dvp->irq_data));
	dvp->dev.init        = hgdvp_init;
	dvp->dev.baudrate    = hgdvp_set_baudrate;
    dvp->dev.open        = hgdvp_open;
    dvp->dev.close       = hgdvp_close;
    dvp->dev.ioctl       = hgdvp_ioctl;
    dvp->dev.request_irq = dvpirq_register;
    dvp->dev.release_irq = dvpirq_unregister;

    irq_disable(dvp->irq_num);
    dev_register(dev_id, (struct dev_obj *)dvp);	
	return 0;
}


