/**
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HGI2C_H
#define __HGI2C_H

#ifdef __cplusplus
 extern "C" {
#endif

struct hgi2c {
    struct i2c_device       dev;
    uint32                  hw;
    i2c_irq_hdl             irq_hdl;
    uint32                  irq_data;
    uint32                  irq_num;
    struct os_mutex         i2c_lock;
    struct os_semaphore     i2c_done;
    uint32                  opened;
    uint32                  irq_tx_done_en: 1,
                            irq_rx_done_en: 1,
                            flag_tx_done  : 1,
                            flag_rx_done  : 1;
}; 



int32 hgi2c_attach(uint32 dev_id, struct hgi2c *hgi2c_s);



#ifdef __cplusplus
}
#endif



#endif //__HGI2C_H

/*************************** (C) COPYRIGHT 2018 HUGE-IC ***** END OF FILE *****/
