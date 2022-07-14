#ifndef _HGSPI_XIP_H_
#define _HGSPI_XIP_H_
#include "hal/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hgspi_xip {
    struct spi_device       dev;
	uint32                  hw;
};

struct hgcqspi {
    //struct qspi_device      dev;
    void                   *hw;
    uint32                  irq_num;
    //qspi_irq_hdl            irq_hdl;
    uint32                  irq_data;
    uint32                  opened;
    uint32                  flags;
};


int32 hgspi_xip_attach(uint32 dev_id, struct hgspi_xip *p_spi);

#ifdef __cplusplus
}
#endif

#endif /* _HGSPI_V1_H_ */

