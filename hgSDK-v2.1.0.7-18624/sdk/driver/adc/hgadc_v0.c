#include "typesdef.h"
#include "list.h"
#include "errno.h"
#include "dev.h"
#include "osal/string.h"
#include "osal/semaphore.h"
#include "osal/mutex.h"
#include "osal/irq.h"
#include "hal/adc.h"
#include "dev/adc/hgadc_v0.h"
#include "hgadc_v0_hw.h"


#define ADC_CHANNEL_ENABLE  1
#define ADC_CHANNEL_SUSPEND 2
#define ADC_CHANNEL_DISABLE 3

/* ADC channel type, must start at 0 & unique value & less than 32*/
/* Table */

// I/O class
#define _ADC_CHANNEL_IO_CLASS        0

//RF sensor of temperature
#define _ADC_CHANNEL_RF_TEMPERATURE  1
#define _ADC_CHANNEL_RF_VDDI         2

//Internal voltage
#define _ADC_CHANNEL_PLL_VREF        3

/**********************************************************************************/
/*                           LOW LAYER FUNCTION                                   */
/**********************************************************************************/

/* List opreation */
static int32 hgadc_v0_list_insert(adc_channel_node *head_node, adc_channel_node *new_node) {

    adc_channel_node *temp_node = head_node;

    /* find the last node */
    while (temp_node->next) {
        temp_node = temp_node->next;
    }

    temp_node->next = new_node;
    new_node->next  = NULL;

    /* channel amount */
    head_node->channel_amount++;

    printf("*** add success: ADC channel cnt = %d\n\r", head_node->channel_amount);

    return RET_OK;
}

static int32 hgadc_v0_list_delete(adc_channel_node *head_node, uint32 channel) {

    adc_channel_node *temp_node   = head_node;
    adc_channel_node *delete_node = NULL;

    /* find the node */
    while (temp_node->next) {
        if (channel == temp_node->next->data.channel) {

            delete_node = temp_node->next;
            temp_node->next = temp_node->next->next;
            os_free(delete_node);

            head_node->channel_amount--;

            printf("*** delete success: ADC channel cnt = %d\n\r", head_node->channel_amount);
            
            break;
        }
        temp_node = temp_node->next;
    }

    return RET_OK;
}

static int32 hgadc_v0_list_get_by_channel(adc_channel_node *head_node, uint32 channel, adc_channel_node **get_node) {

    adc_channel_node *temp_node = head_node;

    /* find the node */
    while (temp_node->next) {
        if (channel == temp_node->next->data.channel) {
            
            *get_node = temp_node->next;
        
            return RET_OK;
        }
        temp_node = temp_node->next;
    }

    return RET_ERR;
}

static int32 hgadc_v0_list_get_by_index(adc_channel_node *head_node, uint32 index, adc_channel_node **get_node) {

    adc_channel_node *temp_node = head_node;
    uint32 i = 0;

    /* find the node */
    for (i = 0; i < head_node->channel_amount; i++) {
        if ((i == index) && (temp_node->next)) {
            *get_node = temp_node->next;
            return RET_OK;
        }

        temp_node = temp_node->next;
    }

    return RET_ERR;

}

static int32 hgadc_v0_list_check_repetition(adc_channel_node *head_node, uint32 channel) {

    adc_channel_node *temp_node = head_node;
    uint32 i = 0;

    /* find the node which repeated */
    for (i = 0; i < head_node->channel_amount; i++) {
        if (temp_node->next) {
            if (channel == temp_node->next->data.channel) {
                return RET_ERR;
            }

        } 
        temp_node = temp_node->next;
    }

    return RET_OK;
}

static int32 hgadc_v0_list_delete_all(adc_channel_node *head_node, struct hgadc_v0 *dev) {

    adc_channel_node *temp_node = head_node;
    adc_channel_node *delete_node = NULL;

    while (temp_node->next) {
        delete_node = temp_node->next;
        /* disable adc channel */
        delete_node->data.func(dev, delete_node->data.channel, ADC_CHANNEL_DISABLE);
        temp_node->next = temp_node->next->next;
        os_free(delete_node);
        head_node->channel_amount--;

        printf("*** delete success: ADC channel cnt = %d\n\r", head_node->channel_amount);
    }

    return RET_OK;
}


static int32 hgadc_v0_list_get_channel_amount(adc_channel_node *head_node) {

    return head_node->channel_amount;
}


static int32 hgadc_v0_switch_hal_adc_ioctl_cmd(enum adc_ioctl_cmd param) {

    switch (param) {
        default:
            return -1;
            break;
    }
}

static int32 hgadc_v0_switch_hal_adc_irq_flag(enum adc_irq_flag param) {

    switch (param) {
        case (ADC_IRQ_FLAG_SAMPLE_DONE):
            return 0;
            break;
        default:
            return -1;
            break;
    }
}


static int32 hgadc_v0_switch_param_channel(uint32 channel) {

    /* I/O class under 0x101*/
    if (channel < 0x101) {
        return _ADC_CHANNEL_IO_CLASS;
    }

    switch (channel) {
        case ADC_CHANNEL_RF_TEMPERATURE:
            return _ADC_CHANNEL_RF_TEMPERATURE;
            break;
        default :
            return RET_ERR;
            break;
    }
}

/* channel configuration */
static int32 hgadc_v0_adc_channel_io_class(struct hgadc_v0 *dev, uint32 channel, uint32 enable) {

    //BIT(26)  enable channel
    //BIT(27)  suspend channel
    //BIT(28)  disable channel

    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    int32 ret = 0;

    if (ADC_CHANNEL_ENABLE == enable) {
        /* pin config, return gpiox addr */
        ret = pin_func(dev->dev.dev.dev_id , BIT(26) | channel);
        
        if (ret == RET_ERR) {
            return RET_ERR;
        }

        /* GPIO0_EN/GPIO1_EN */
        if (BIT(25) == ret) {
            if (ADKEY1_BASE == dev->hw) {
                //GPIOA
                hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) ) | (0x1 << 8);
            } else {
                //GPIOA
                hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) ) | (0x2 << 8);
            }
        } else {
            //GPIOB / 
            hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) ) | (0x1 << 8);
        }

        /* Wait stable */
        __NOP();__NOP();__NOP();__NOP();
    } else if (ADC_CHANNEL_SUSPEND == enable) {
        /* pin config, return gpiox addr */
        ret = pin_func(dev->dev.dev.dev_id , BIT(27) | channel);

        /* disable current channel */
        hw->ADKEY_CON &= ~ (0xF << 8);
        /* clear the data */
        hw->ADKEY_DATA &= ~(0xFFF);
        
        if (ret == RET_ERR) {
            return RET_ERR;
        }
        
    } else if (ADC_CHANNEL_DISABLE == enable) {
        /* pin config, return gpiox addr */
        ret = pin_func(dev->dev.dev.dev_id , BIT(28) | channel);

        /* disable current channel */
        hw->ADKEY_CON &= ~ (0xF << 8);
        
        if (ret == RET_ERR) {
            return RET_ERR;
        }
    }


    return RET_OK;
}

static int32 hgadc_v0_adc_channel_pll_vref(struct hgadc_v0 *dev, uint32 channel, uint32 enable) {

    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    uint32 pmu_con5 = 0;

    if (ADC_CHANNEL_ENABLE == enable) {
        /* open pll_vref */
        pmu_con5 = PMU->PMUCON5;
        pmu_con5 = ( pmu_con5 &~ (0xF << 15) ) | (5 << 15);
        pmu_reg_write((uint32)&PMU->PMUCON5, pmu_con5);
        
        /* ATOUT EN */
        hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) ) | (0x8 << 8);
    } else if (ADC_CHANNEL_SUSPEND == enable) {
        /* close pll_vref */
        pmu_con5 = PMU->PMUCON5;
        pmu_con5 &= ~(0xF << 15);
        pmu_reg_write((uint32)&PMU->PMUCON5, pmu_con5);

        /* ATOUT DISABLE */
        hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) );
    } else if (ADC_CHANNEL_DISABLE == enable) {
        /* close pll_vref */
        pmu_con5 = PMU->PMUCON5;
        pmu_con5 &= ~(0xF << 15);
        pmu_reg_write((uint32)&PMU->PMUCON5, pmu_con5);
        
        /* ATOUT DISABLE */
        hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) );
    }

    return RET_OK;

}


static int32 hgadc_v0_adc_channel_rf_temperature(struct hgadc_v0 *dev, uint32 channel, uint32 enable) {

    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    uint32 mask = 0;
    uint32 i = 0;

        /* Close the interrupt to protect the RF register opreation */
        mask = disable_irq();


    if (ADC_CHANNEL_ENABLE == enable) {

        /* connect to rf_temperature channel */
        *((uint32 *)(0x40019000 + 0x18))  = ( *((uint32 *)(0x40019000 + 0x18)) & ~(0xf<<27) ) | 0x6<<27 | 0x1<<31;
        //*((uint32 *)(0x40019000 + 0x1C)) |= 0x1; 

        /* RF_TOUT */
        hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) ) | (0x4 << 8);

        /* Wait stable */
        for (i = 0; i < 20; i++) {
            __NOP();
        }
    } else if (ADC_CHANNEL_SUSPEND == enable) {
        hw->ADKEY_CON &= ~(0xF << 8);
        /* clear the data */
        hw->ADKEY_DATA &= ~(0xFFF);
    } else if (ADC_CHANNEL_DISABLE == enable) {
        hw->ADKEY_CON &= ~(0xF << 8);
    }

    /* Enable interrupt */
    enable_irq(mask);

    return RET_OK;

}


static int32 hgadc_v0_adc_channel_rf_vddi(struct hgadc_v0 *dev, uint32 channel, uint32 enable) {

    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    uint32 mask = 0;
    uint32 i = 0;

    /* Close the interrupt to protect the RF register opreation */
    mask = disable_irq();

    if (ADC_CHANNEL_ENABLE == enable) {

        //printf("ADC module info: vddi gears: %d\r\n", ((*((uint32 *)(0x40019000 + 0x1C))) & (0xF << 9) ) >> 9);

        /* connect to rf_vddi channel */
        *((uint32 *)(0x40019000 + 0x18))  = ( *((uint32 *)(0x40019000 + 0x18)) & ~(0xf<<27) ) | 0x9<<27 | 0x1<<31;
//        *((uint32 *)(0x40019000 + 0x1C)) |= 0x1;
    
        /* RF_TOUT */
        hw->ADKEY_CON = ( hw->ADKEY_CON &~ (0xF << 8) ) | (0x4 << 8);

        /* Wait stable */
        for (i = 0; i < 20; i++) {
            __NOP();
        }
    } else if (ADC_CHANNEL_SUSPEND == enable) {

        /* recovery to rf_temperature channel */
        *((uint32 *)(0x40019000 + 0x18))  = ( *((uint32 *)(0x40019000 + 0x18)) & ~(0xf<<27) ) | 0x6<<27 | 0x1<<31;
        //*((uint32 *)(0x40019000 + 0x1C)) &= ~ 0x1;

        hw->ADKEY_CON &= ~(0xF << 8);
        /* clear the data */
        hw->ADKEY_DATA &= ~(0xFFF);

        /* Wait stable */
        for (i = 0; i < 20; i++) {
            __NOP();
        }
    } else if (ADC_CHANNEL_DISABLE == enable) {

        /* recovery to rf_temperature channel */
        *((uint32 *)(0x40019000 + 0x18))  = ( *((uint32 *)(0x40019000 + 0x18)) & ~(0xf<<27) ) | 0x6<<27 | 0x1<<31;
        //*((uint32 *)(0x40019000 + 0x1C)) &= ~ 0x1;
        
        hw->ADKEY_CON &= ~(0xF << 8);

        /* Wait stable */
        for (i = 0; i < 20; i++) {
            __NOP();
        }
    }

    /* Enable interrupt */
    enable_irq(mask);

    return RET_OK;
}


static int32 hgadc_v0_adc_data_verify(struct hgadc_v0 *dev, uint32 channel, uint32 *adc_data) {

    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    uint32 delay_cnt = 0, cnt = 0;
    //volatile float data_temp1 = 0.0;

    int32 data_temp = 0;

    /* Sample the pll_vref to verify the ADC data, beacause the VCC is shaking */
    hgadc_v0_adc_channel_rf_vddi(dev, _ADC_CHANNEL_RF_VDDI, ADC_CHANNEL_ENABLE);

    /* Close the interrupt */
    hw->ADKEY_CON &= ~(1 << 20);

    /* Read the div of adc clk to delay after sample done */
    delay_cnt = (hw->ADKEY_DATA >> 16);
    
    hw->ADKEY_CON |= (1 << 19);   //kick start to sample
    
    while(!(hw->ADKEY_CON & (0x1 <<31)));
    hw->ADKEY_DATA |= (1 << 12);
    
    /* Waitting for the ADC circuit ready for the next sample */
    for (cnt  = 0; cnt < delay_cnt; cnt++) {
        __NOP();
    }

    data_temp = (hw->ADKEY_DATA & 0xFFF);
    //printf("1--->%d\r\n", data_temp);
    //printf("2--->%d\r\n", *adc_data);

    if (ADC_CHANNEL_RF_TEMPERATURE == channel) {

        //printf("--Debug---->1 :%d\r\n", dev->refer_vddi);
        //printf("--Debug---->2 :%d\r\n", (*adc_data << 17) / data_temp);
        //printf("--Debug---->3 :%d\r\n", (dev->refer_tsensor << 17) / 4096);
    
        data_temp = ( ( (*adc_data << 12) / data_temp ) - (dev->refer_tsensor) ) * dev->refer_vddi;
    
        //printf("3--->%d\r\n", data_temp);
        //printf("--Debug---->4 :%d\r\n", ((data_temp * 1000) / 4) >> 25);
        data_temp = ((data_temp * 250 ) >> 20) + 25;

        //data_temp1 = (hw->ADKEY_DATA & 0xFFF);
        //data_temp1 *= 2;
        
        //printf("Debug---->1 :%.3f\r\n", (float)dev->refer_vddi / (float)128);
        //printf("Debug---->2 :%.3f\r\n", (float)(*adc_data) / data_temp1);
        //printf("Debug---->3 :%.3f\r\n", (float)(dev->refer_tsensor / 2) / (float)4096);

        //printf("vddi gears: %d\r\n", ((*((uint32 *)(0x40019000 + 0x1C))) & (0xF << 9) ) >> 9);
        //data_temp1 = ( ( (float)(*adc_data) / data_temp1) - ( (float)(dev->refer_tsensor / 2) / (float)4096) ) * (float)((float)dev->refer_vddi / (float)128);
        //printf("4--->%.3f\r\n", data_temp1);
        //data_temp1 = ((data_temp1) / (float)(0.004)) + 25;
        //printf("temp data: %d\r\n", data_temp);
        //printf("temp1 data :%f\r\n", data_temp1);

        
        *adc_data = data_temp;

   } else {
        //data_temp = ( ( (*adc_data) * 1464 ) << 15) / data_temp;

        //printf("1----> = %d\r\n", data_temp);  //A'
        //printf("2----> = %d\r\n", *adc_data);  //B'

        
        data_temp = ( ( (*adc_data) * dev->refer_vddi_adc_data) << 9) / data_temp;
        //printf("3----> = %d\r\n", data_temp);

        *adc_data = data_temp >> 9;

        if (*adc_data >= 4095) {
            *adc_data = 4095;
        }
        
        //printf("4----> = %d\r\n", *adc_data);
   }

    hgadc_v0_adc_channel_rf_vddi(dev, _ADC_CHANNEL_RF_VDDI, ADC_CHANNEL_DISABLE);

    /* Open the interrupt */
    hw->ADKEY_CON |= (1 << 20);

    return RET_OK;

}



/**********************************************************************************/
/*                             ATTCH FUNCTION                                     */
/**********************************************************************************/

static int32 hgadc_v0_open(struct adc_device *adc) {

    struct hgadc_v0    *dev = (struct hgadc_v0 *)adc;
    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    uint32 i = 0;
    uint32 _refer_vddi    = 0;
    uint32 _refer_tsensor = 0;
    uint32 _vddi_gears    = 0;
    uint32 _rfsys_reg7    = 0;
    uint32 mask = 0;

    mask = disable_irq();

    if (dev->opened) {
        /* Enable interrupt */
        enable_irq(mask);
        return -EBUSY;
    }

    _vddi_gears     = sysctrl_efuse_vddi_gears_get();
    _refer_vddi     = sysctrl_efuse_vddi_get();
    _refer_tsensor  = sysctrl_efuse_tsensor_get();

    /* calibrate vddi to 1.18v by efuse vaule of vddi gears */
    if (_vddi_gears) {
        _rfsys_reg7 = *((uint32 *)(0x40019000 + 0x1C));
        _rfsys_reg7 = ( _rfsys_reg7 &~ (0xF << 9) ) | (_vddi_gears << 9);
        *((uint32 *)(0x40019000 + 0x1C)) = _rfsys_reg7;
    }

    if (0 == _refer_vddi) {
        printf("*** ADC module info: vddi don't calibrate!!, vddi will use default: 1.27v.\r\n");
        //(1+0.27)*256
        dev->refer_vddi = (256 + 69);

        dev->refer_vddi_adc_data = 1580;
    } else {
        //(1+vddi from efuse) * 256, "vddi from efuse" already multiply by 256
        printf("*** ADC module info: vddi calibrated value = 0x%x\r\n", _refer_vddi);
        dev->refer_vddi = 256 + _refer_vddi;

        dev->refer_vddi_adc_data = ( ( (dev->refer_vddi) * 10) / 33 ) * 4095;
        dev->refer_vddi_adc_data = dev->refer_vddi_adc_data >> 8;
        printf("*** ADC module info: vddi adc_data = %d(D)\r\n", dev->refer_vddi_adc_data);
    }

    if (0 == _refer_tsensor) {
        printf("*** ADC module info: tsensor don't calibrate!!, tsensor will use default: 4012\r\n");
        dev->refer_tsensor = 4012;
    } else {
        //efuse: ( (vptat_adc_data/vddi_adc_data) / 2 ) * 4096
        printf("*** ADC module info: tsensor calibrated value = 0x%x\r\n", _refer_tsensor);
        dev->refer_tsensor = _refer_tsensor * 2;
    }

    //ADCEN = 1; DAOUTEN = 1 & open interrupt
    hw->ADKEY_CON |= (1 << 0) | (1 << 2) | BIT(20);
    
    //software kict 
    hw->ADKEY_CON &= ~(0xF << 15);

    //clear the DATA, config baud
    hw->ADKEY_DATA = ( hw->ADKEY_DATA &~ (0xFFFF << 16) ) | (0xB3 << 16);

    /* Wait for ADC init done */
    for (i = 0; i < 200; i++) {
        __NOP();
    };

    irq_enable(dev->irq_num);

    /* init head node */
    dev->head_node.channel_amount = 0;
    dev->head_node.data.channel   = -1;
    dev->head_node.data.func      = NULL;
    dev->head_node.next           = NULL;

    dev->opened = 1;
    dev->irq_en = 0;
    printf("*** open ADC success!\n\r");

    /* Enable interrupt */
    enable_irq(mask);

    return RET_OK;
}

static int32 hgadc_v0_close(struct adc_device *adc) {

    uint32 mask = 0;
    struct hgadc_v0    *dev = (struct hgadc_v0 *)adc;
    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;

    if (!dev->opened) {
        return RET_OK;
    }

    /* Close the interrupt to protect the list opreation */
    mask = disable_irq();

    /* keep ADC open, when channel is still in use */
    if (hgadc_v0_list_get_channel_amount(&dev->head_node)) {
        
        /* Enable interrupt */
        enable_irq(mask);

        return RET_OK;
    }

    hgadc_v0_list_delete_all(&dev->head_node, dev);

    /* Enable interrupt */
    enable_irq(mask);

    irq_disable(dev->irq_num);
    dev->head_node.channel_amount = 0;
    dev->head_node.next           = NULL;
    dev->head_node.data.channel   = -1;
    dev->head_node.data.func      = NULL;
    hw->ADKEY_CON      = 0;
    hw->ADKEY_DATA     = 0;
    dev->refer_tsensor = 0;
    dev->refer_vddi    = 0;
    dev->irq_en        = 0;
    dev->opened        = 0;
    dev->refer_vddi_adc_data = 0;

    return RET_OK;
}


static int32 hgadc_v0_add_channel(struct adc_device *adc, uint32 channel) {

    int32  _class = 0;
    uint32 mask = 0;
    struct hgadc_v0 *dev = (struct hgadc_v0 *)adc;
    adc_channel_node *new_node = NULL;

    if (!dev->opened) {
        return RET_ERR;
    }

    /* ADKEY1 P channel cant't support current adc sample channel, limited by hardware */
    if ((ADKEY1_BASE == dev->hw) && (PA_15 < channel) && (channel < 0x100)) {
        printf("ADKEY1 can't support the %d channel!!!\r\n", channel);
        return RET_ERR;
    }

    /* Close the interrupt to protect the list opreation */
    mask = disable_irq();

    /* Check for the channel which repeated  */
    if (RET_ERR == hgadc_v0_list_check_repetition(&dev->head_node, channel)) {
        printf("*** ADC channel repeat!!!\n\r");
        enable_irq(mask);
        return RET_OK;
    }

    _class = hgadc_v0_switch_param_channel(channel);

    if (RET_ERR == _class) {
        enable_irq(mask);
        return RET_ERR;
    }

    /* save the adkey configuration by channel */
    switch (_class) {
        case _ADC_CHANNEL_IO_CLASS:
            new_node = (adc_channel_node *)os_malloc(sizeof(adc_channel_node));
            if (!new_node) {
                enable_irq(mask);
                return RET_ERR;
            }
            
            new_node->data.func    = hgadc_v0_adc_channel_io_class;
            new_node->data.channel = channel;
            new_node->next         = NULL;
            hgadc_v0_list_insert(&dev->head_node, new_node);
            break;
        case _ADC_CHANNEL_RF_TEMPERATURE:
            new_node = (adc_channel_node *)os_malloc(sizeof(adc_channel_node));
            if (!new_node) {
                enable_irq(mask);
                return RET_ERR;
            }
            
            new_node->data.func    = hgadc_v0_adc_channel_rf_temperature;
            new_node->data.channel = channel;
            new_node->next         = NULL;
            hgadc_v0_list_insert(&dev->head_node, new_node);
            break;
    }

    /* Enable interrupt */
    enable_irq(mask);

    return RET_OK;

}

static int32 hgadc_v0_delete_channel(struct adc_device *adc, uint32 channel) {

    uint32 mask = 0;
    adc_channel_node *get_node = NULL;
    struct hgadc_v0 *dev = (struct hgadc_v0 *)adc;

    if (!dev->opened) {
        return RET_ERR;
    }

    /* Close the interrupt to protect the list opreation */
    mask = disable_irq();

    if (RET_ERR == hgadc_v0_list_get_by_channel(&dev->head_node, channel, &get_node)) {
        printf("** Delete func: No this ADC channel!!!\n\r");
        enable_irq(mask);
        return RET_ERR;
    }

    get_node->data.func(dev, channel, ADC_CHANNEL_DISABLE);

    hgadc_v0_list_delete(&dev->head_node, channel);

    /* Enable interrupt */
    enable_irq(mask);

    return RET_OK;
}

static int32 hgadc_v0_get_value(struct adc_device *adc, uint32 channel, uint32 *raw_data) {

    struct hgadc_v0    *dev = (struct hgadc_v0 *)adc;
    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;
    adc_channel_node *get_node = NULL;
    uint32 delay_cnt = 0, cnt = 0;

    if (!dev->opened) {
        return RET_ERR;
    }

    os_mutex_lock(&dev->adc_lock, osWaitForever);

    if (RET_ERR == hgadc_v0_list_get_by_channel(&dev->head_node, channel, &get_node)) {
        printf("** get_value func: No this ADC channel!!!\n\r");
        os_mutex_unlock(&dev->adc_lock);
        return RET_ERR;
    }

    /* Read the div of adc clk to delay after sample done */
    delay_cnt = (hw->ADKEY_DATA >> 16);

    /* config current channel */
    if (get_node->data.func) {
        get_node->data.func(dev, channel, ADC_CHANNEL_ENABLE);
    } else {
        os_mutex_unlock(&dev->adc_lock);
        return RET_ERR;
    }

    hw->ADKEY_CON |= (1 << 19);   //kick start to sample

    /* Waitting for the ADC circuit ready for the next sample */
    for (cnt  = 0; cnt < delay_cnt; cnt++) {
        __NOP();
    }
    
    os_sema_down(&dev->adc_done, osWaitForever);
    //printf("***  down!!\n\r");

    *raw_data = hw->ADKEY_DATA & 0xFFF;

    //printf("** %d channel raw_data: %d\n\r", get_node->data.channel, *raw_data);

    get_node->data.func(dev, channel, ADC_CHANNEL_SUSPEND);

    hgadc_v0_adc_data_verify(dev, channel, raw_data);


    if (dev->irq_en && dev->irq_hdl) {
        dev->irq_hdl(ADC_IRQ_FLAG_SAMPLE_DONE, get_node->data.channel, *raw_data);
    }

    os_mutex_unlock(&dev->adc_lock);

    return RET_OK;
}

static int32 hgadc_v0_ioctl(struct adc_device *adc, enum adc_ioctl_cmd ioctl_cmd, uint32 param1, uint32 param2) {

    return RET_OK;
}

static void hgadc_v0_irq_handler(void *data) {

    struct hgadc_v0    *dev = (struct hgadc_v0 *)data;
    struct hgadc_v0_hw *hw  = (struct hgadc_v0_hw *)dev->hw;

    //printf("***  interrupt!!\n\r");

    if ((hw->ADKEY_CON & (1 << 31)) && (hw->ADKEY_CON & (1 << 20))) {
        hw->ADKEY_DATA |= (1 << 12);

        os_sema_up(&dev->adc_done);
        //printf("***  up!!\n\r");
    }
}

static int32 hgadc_v0_request_irq(struct adc_device *adc, enum adc_irq_flag irq_flag, adc_irq_hdl irq_hdl, uint32 irq_data) {

    struct hgadc_v0    *dev = (struct hgadc_v0 *)adc;
    
    dev->irq_hdl  = irq_hdl;
    dev->irq_data = irq_data;

    if (irq_flag & ADC_IRQ_FLAG_SAMPLE_DONE) {
         dev->irq_en = 1;
    }

    return RET_OK;
}

static int32 hgadc_v0_release_irq(struct adc_device *adc, enum adc_irq_flag irq_flag) {

    struct hgadc_v0 *dev = (struct hgadc_v0 *)adc;

    if (irq_flag & ADC_IRQ_FLAG_SAMPLE_DONE) {
        dev->irq_en = 0;
    }

    return RET_OK;
}

int32 hgadc_v0_attach(uint32 dev_id, struct hgadc_v0 *adc) {

    struct hgadc_v0_hw *hw = (struct hgadc_v0_hw *)adc->hw;

    adc->opened             = 0;
    adc->irq_en             = 0;
    adc->refer_vddi         = 0;
    adc->refer_tsensor      = 0;
    adc->refer_vddi_adc_data= 0;
    adc->irq_hdl            = NULL;
    adc->irq_data           = 0;
    adc->dev.open           = hgadc_v0_open;
    adc->dev.close          = hgadc_v0_close;
    adc->dev.add_channel    = hgadc_v0_add_channel;
    adc->dev.delete_channel = hgadc_v0_delete_channel;
    adc->dev.get_value      = hgadc_v0_get_value;
    adc->dev.ioctl          = hgadc_v0_ioctl;
    adc->dev.request_irq    = hgadc_v0_request_irq;
    adc->dev.release_irq    = hgadc_v0_release_irq;

    os_mutex_init(&adc->adc_lock);
    os_sema_init(&adc->adc_done, 0);

    request_irq(adc->irq_num, hgadc_v0_irq_handler, adc);
    hw->ADKEY_CON |= BIT(20);
    irq_enable(adc->irq_num);
    dev_register(dev_id, (struct dev_obj *)adc);
    
    return RET_OK;
}
