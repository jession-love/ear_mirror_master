#ifndef _HGSPI_V1_H_
#define _HGSPI_V1_H_
#include "hal/spi.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hgspi_v1 {
    struct spi_device       dev;
    uint32                  hw;
    uint32                  irq_num;
    spi_irq_hdl             irq_hdl;
    uint32                  irq_data;
    uint32                  opened;
    uint32                  len_threshold;
    struct os_mutex         os_spi_tx_lock;
    struct os_mutex         os_spi_rx_lock;
    struct os_semaphore     os_spi_tx_done;
    struct os_semaphore     os_spi_rx_done;
    uint32                  timeout;
    uint32                  spi_tx_done          :1,
                            spi_rx_done          :1,
                            spi_irq_flag_tx_done :1,
                            spi_irq_flag_rx_done :1,
                            spi_tx_async         :1,
                            spi_rx_async         :1;
};

int32 hgspi_v1_attach(uint32 dev_id, struct hgspi_v1 *p_spi);

#ifdef __cplusplus
}
#endif

#endif /* _HGSPI_V1_H_ */
