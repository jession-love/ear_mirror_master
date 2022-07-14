#ifndef _HGSDIO20_SLAVE_H_
#define _HGSDIO20_SLAVE_H_
#include "hal/sdio_slave.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hgsdio20_slave {
    struct sdio_slave dev;
    void  *hw;
    sdio_slave_irq_hdl irq_hdl;
    uint32 irq_data;
    struct os_mutex tx_lock;
    struct os_semaphore tx_done;
    uint32 irq_num;
    uint32 rst_irq_num;
    uint8 *rx_buff;
    uint32 rx_len;
    uint32 opened;
    /* usefor TX data align, compatible for host_blocksize less than this blocksize */
    uint32 block_size;
    uint16 manufacture_id;
    uint16 device_id;
    enum sdio_slave_speed speed;
    uint8  func0_cis[20];
    uint8  func1_cis[56];
    uint32 flags;
    /* Reserve the buff. When the address of WRITE_ADDR is invalid, configure
     * WRITE_ADDR to this buf.
     */
    uint32 reserve_buf[64 / 4];
};

int32 hgsdio20_slave_attach(uint32 dev_id, struct hgsdio20_slave *slave);

#ifdef __cplusplus
}
#endif

#endif /* _HGSDIO20_SLAVE_H_ */

