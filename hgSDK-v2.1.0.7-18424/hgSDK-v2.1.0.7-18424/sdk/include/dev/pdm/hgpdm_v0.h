#ifndef _HGPDM_V0_H_
#define _HGPDM_V0_H_
#include "hal/pdm.h"

#ifdef __cplusplus
extern "C" {
#endif


struct hgpdm_v0 {
    struct pdm_device   dev;
    uint32              hw;
    pdm_irq_hdl         irq_hdl;
    uint32              irq_data;
    uint32              irq_num;
    uint32              opened;
};


int32 hgpdm_v0_attach(uint32 dev_id, struct hgpdm_v0 *pdm);


#ifdef __cplusplus
}
#endif
#endif

