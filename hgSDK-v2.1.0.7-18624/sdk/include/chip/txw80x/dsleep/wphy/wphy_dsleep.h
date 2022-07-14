#ifndef _WPHY_DSLEEP_H_
#define _WPHY_DSLEEP_H_
#include <typesdef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct wphy_dsleep_cfg_type {
    union PHY_CTL_GRP ctl_grp;
    union PHY_AGC_GRP agc_grp;
    union PHY_DPD_GRP dpd_grp;
};

#define WPHY_CTL_GRP                    ((union PHY_CTL_GRP *)HG_WPHY_BGN_CTL_BASE)
#define WPHY_AGC_GRP                    ((union PHY_AGC_GRP *)HG_WPHY_BGN_AGC_BASE)
#define WPHY_DPD_GRP                    ((union PHY_DPD_GRP *)HG_WPHY_BGN_DPD_BASE)

extern struct hgwphy_bgn_cfg nphybgncfg;

void wphy_dsleep_save(void);
void wphy_dsleep_recover(void);

#ifdef __cplusplus
}
#endif

#endif

