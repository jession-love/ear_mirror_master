#ifndef HG_DVP_H
#define HG_DVP_H
#include "tx_platform.h"
#include "list.h"
#include "dev.h"
#include "hal/dvp.h"

typedef enum {
	HSIE_ISR     = 0,
	VSIE_ISR,
	SSCIE_EIGHT_ISR,
	FHFIE_ISR,
	FOVIE_ISR,
	SIP_ISR,
	JPEG_DONE,
	DVP_IRQ_NUM
}DVP_IRQ_E;
 

struct hgdvp {
    struct dvp_device   dev;
    //struct hgdvp_hw     *hw;
	uint32              hw;
    dvp_irq_hdl         irq_hdl;
    uint32              irq_data;
    uint32              irq_num;
    uint32              opened:1, use_dma:1;
};


int32 hgdvp_attach(uint32 dev_id, struct hgdvp *dvp);

#endif

