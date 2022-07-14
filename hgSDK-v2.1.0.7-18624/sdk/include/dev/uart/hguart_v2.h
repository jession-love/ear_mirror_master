#ifndef _HGUART_V2_H
#define _HGUART_V2_H
#include "hal/uart.h"

#ifdef __cplusplus
extern "C" {
#endif

struct hguart_v2 {
    struct uart_device   dev;
    uint32 hw;
    uart_irq_hdl         irq_hdl;
    uint32               irq_data;
    uint32               irq_num;
    uint32               opened:1, use_dma:1;
};


int32 hguart_v2_attach(uint32 dev_id, struct hguart_v2 *uart);


#ifdef __cplusplus
}
#endif
#endif
