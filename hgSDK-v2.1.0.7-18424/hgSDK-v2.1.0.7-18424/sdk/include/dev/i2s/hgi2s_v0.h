#ifndef _HGI2S_V0_H_
#define _HGI2S_V0_H_
#include "hal/i2s.h"

#ifdef __cplusplus
extern "C" {
#endif



/** @brief I2S operating handle structure
  * @{
  */
struct hgi2s_v0 {
    struct i2s_device   dev      ;
    uint32              hw       ;
    i2s_irq_hdl         irq_hdl  ;
    uint32              irq_data ;
    uint32              irq_num  ;
    uint32              duplex_en;
    uint32              opened   ;
};


int32 hgi2s_v0_attach(uint32 dev_id, struct hgi2s_v0 *i2s);



#ifdef __cplusplus
}
#endif

#endif /* _HGI2S_V0_H_ */
