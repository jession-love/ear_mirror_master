#ifndef _LMAC_DSLEEP_RX_H_
#define _LMAC_DSLEEP_RX_H_
#include <typesdef.h>

#ifdef __cplusplus
extern "C" {
#endif

int32 lmac_dsleep_check_beacon(uint8 *buff);
int32 lmac_dsleep_rx_beacon_hdl(uint8 *buff, uint32 frm_len);

#ifdef __cplusplus
}
#endif

#endif