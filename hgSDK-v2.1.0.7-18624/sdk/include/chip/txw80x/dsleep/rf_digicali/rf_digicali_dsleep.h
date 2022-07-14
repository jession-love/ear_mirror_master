#ifndef _RF_DIGICALI_DSLEEP_H_
#define _RF_DIGICALI_DSLEEP_H_
#include <typesdef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct rf_digicali_cfg_type {
    uint32  TXPWRIDX;
    uint32  TXDC[4];
    uint32  TXIMB[8];
    uint32  RXPWRIDX;
    uint32  RX20MDC[4];
    uint32  RX20MIMB[8];
    uint32  RXFBDC[2];
    uint32  RXFBIMB[4];
    uint32  TXDIGPWR[4];
    uint32  DBGPATH_CFG0;
    uint32  DBGPATH_CFG1;
    uint32  SOFT_RFIF_CON;
};

#define RFDIGCAL                    ((union hgrfdigicali_reg *)RFDIGCAL_BASE)

void rf_digicali_dsleep_save(void);
void rf_digicali_dsleep_recover_tx_pwr(void);
void rf_digicali_dsleep_recover_tx_dcoc(void);
void rf_digicali_dsleep_recover_tx_imb(void);
void rf_digicali_dsleep_recover_tx_digi_gain(void);
void rf_digicali_dsleep_recover_rx_gain(void);
void rf_digicali_dsleep_recover_rx_dcoc(void);
void rf_digicali_dsleep_recover_rx_imb(void);
void rf_digicali_dsleep_recover_rx_gain_idx_src(void);
void rf_digicali_dsleep_recover_dbg_path(void);
void rf_digicali_dsleep_recover_soft_if(void);

#ifdef __cplusplus
}
#endif

#endif

