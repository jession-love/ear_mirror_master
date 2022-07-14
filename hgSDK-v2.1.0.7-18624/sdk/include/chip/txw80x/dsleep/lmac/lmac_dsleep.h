#ifndef _LMAC_DSLEEP_H_
#define _LMAC_DSLEEP_H_
#include <typesdef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LMAC_DSLEEP_MARKER                  0x12345678
#define LMAC_DSLEEP_SET_MARKER()            (lmac_wakeup_flag = LMAC_DSLEEP_MARKER)
#define LMAC_DSLEEP_CLR_MARKER()            (lmac_wakeup_flag = 0)
#define LMAC_DSLEEP_GET_MARKER()            (lmac_wakeup_flag)

#define WLAN_WEP40_KEYLEN                   5
#define WLAN_WEP104_KEYLEN                  13
#define WLAN_CCMP_KEYLEN                    16
#define WLAN_TKIP_KEYLEN                    32
#define WLAN_BIP_KEYLEN                     16

struct lmac_dsleep_cipher_suite {
    uint32                                  cipher_suite;
    uint8                                   key_idx;
    struct lmac_bgn_ieee80211_rsna_counter  pn;
    union {
        struct {
            uint8                       key[4][WLAN_WEP104_KEYLEN];
        } wep;
        struct {
            uint8                       key[WLAN_TKIP_KEYLEN];
        } tkip;
        struct {
            uint8                       key[WLAN_CCMP_KEYLEN];
        } ccmp;
    } cipher;
};

//for BIP-CMAC-128
struct lmac_dsleep_bip_cipher_suite {
    uint32                              cipher_suite;
    uint8                               key_idx;
    uint64                              rx_pn;
    union {
        struct {
            uint8                       key[WLAN_BIP_KEYLEN];
        } aes_cmac;
    } cipher;
};


struct lmac_dsleep_beacon_info {
    uint16              interval;
    //beacon中的tim信息
    struct lmac_bgn_beacon_dtim  last_tim;
    //记录上一帧的sn，以及是否有tim
    uint16              last_sn         : 12,
                        has_tim_flag    : 1;
    //要休眠的beacon周期数目
    uint16              delta;
    //上一次beacon的时间戳
    uint64              last_timestamp;
    //预测接下来的beacon的时间戳
    uint64              forecast_timestamp;
};

struct lmac_dsleep_sta_info {
    uint8       addr[MAC_ADDR_SIZE];
    uint32      aid             : 11,
                htgf            : 1,
                short_gi_20m    : 1,
                max_ampdu_len   : 2,
                ampdu           : 1,
                amsdu           : 1,
                authent_ind     : 1;

    /* sequence number control, plus 1 for non-QoS frames */
    struct {
        uint16  sn              : 13,
                ssn_need_check  : 1;     //check rx addba ssn
    } last_seq_ctrl[IEEE80211_NUM_TIDS + 1];

    struct lmac_dsleep_cipher_suite ptksa;

    /* beacon info */
    struct lmac_dsleep_beacon_info beacon_info;
};

struct lmac_dsleep_cfg_type {
    uint16              aid;
    uint8               mac_address[MAC_ADDR_SIZE];
    uint8               channel;
    uint16              rts_threshold   : 11,
                        frm_retry_max   : 5;
    uint16              listen_interval;
    uint16              listen_beacon_cnt;      //用于辅助判断listen interval是否到来
    struct lmac_bgn_edca_para    edca[IEEE80211_AC_NUM];
    
    struct lmac_dsleep_cipher_suite     gtksa;
#ifdef LMAC_BGN_CONFIG_IEEE80211W
    struct lmac_dsleep_bip_cipher_suite igtksa;
#endif

    struct {
        //sta info
        uint32          HT_CAP;
        uint32          EDCATxTh0;
        uint32          EDCATxTh1;
        uint32          EDCATxTh2;
        uint32          EDCATxTh3;
        //timing
        uint32          RFENCTL0;
        uint32          RFENCTL1;
        uint32          RFENCTL2;
        uint32          PHYDLYCTL;
        uint32          RFPHYIFIOCFG0;
        uint32          RFPHYIFIOCFG1;
        //rx to
        uint32          LMACCTL5;
    } lmac_reg;

    struct lmac_dsleep_sta_info ap_info;
};

extern struct lmac_dsleep_cfg_type lmac_wakeup_cfg, lmac_dsleep_cfg;

extern volatile uint32 lmac_wakeup_flag;

void lmac_dsleep_stop_lmac(struct lmac_ops *ops);
void lmac_dsleep_save(struct lmac_ops *ops);
void lmac_dsleep_wakeup_reload(struct lmac_ops *ops);
void lmac_dsleep_wakeup_prepare(void);
uint16 lmac_dsleep_get_beacon_take_time(void);
void lmac_dsleep_phy_rx_start(void);
uint8 lmac_dsleep_phy_rx_is_done(void);
uint8 lmac_dsleep_phy_rx_is_err(void);
uint32 lmac_dsleep_phy_rx_len(void);
uint32 lmac_dsleep_get_tsf(void);
void lmac_dsleep_init(void);
void lmac_dsleep_wakeup_time_calc_in_ps(void);

#ifdef __cplusplus
}
#endif

#endif
